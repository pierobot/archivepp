#include <archivepp/archive_rar.hpp>
#include <archivepp/except.hpp>

#include <cmath>
#include <cstring>

namespace archivepp
{
    namespace unrar
    {
        static std::error_code get_last_error(RAROpenArchiveDataEx & data)
        {
            return std::error_code(data.OpenResult, std::system_category());
        }

        static HANDLE open(archivepp::string const & path, RAROpenArchiveDataEx & data, std::error_code & ec)
        {
            data.OpenMode = RAR_OM_EXTRACT;
#ifdef ARCHIVEPP_USE_WSTRING
            data.ArcNameW = const_cast<wchar_t*>(path.c_str());
#else
            data.ArcName = const_cast<char*>(path.c_str());
#endif
            HANDLE handle = ::RAROpenArchiveEx(&data);
            ec = handle == nullptr ? get_last_error(data) : std::error_code();

            return handle;
        }

        static std::error_code close(HANDLE handle)
        {
            return std::error_code(::RARCloseArchive(handle), std::system_category());
        }

        static int64_t get_number_of_entries(archivepp::string const & path, std::error_code & ec)
        {
            RAROpenArchiveDataEx data {};
            RARHeaderDataEx header {};
            
            HANDLE handle = unrar::open(path, data, ec);
            if (handle == nullptr)
                throw archivepp::null_pointer_error("handle", __FUNCTION__);
            
            int64_t count = 0;
            ec = std::error_code(::RARReadHeaderEx(handle, &header), std::system_category());
            while (!ec)
            {
                ::RARProcessFile(handle, RAR_SKIP, NULL, NULL);
                ++count;

                ec = std::error_code(::RARReadHeaderEx(handle, &header), std::system_category());
            }

            // Expected error
            if (ec.value() == ERAR_END_ARCHIVE)
                ec = std::error_code();

            unrar::close(handle);

            return count;
        }

        static std::pair<uint64_t, uint64_t> get_size_from_header(RARHeaderDataEx const & header)
        {
            // Initialize the size values with their upper and lower 32 bits
            int64_t unpacked_size = (static_cast<int64_t>(header.UnpSizeHigh) << 32) | header.UnpSize;
            int64_t packed_size = (static_cast<int64_t>(header.PackSizeHigh) << 32) | header.PackSize;

            return std::make_pair(unpacked_size, packed_size);
        }

        static archivepp::string get_name_from_header(RARHeaderDataEx const & header)
        {
#ifdef ARCHIVEPP_USE_WSTRING
            archivepp::string name(&header.FileNameW[0]);
            return archivepp::normalize_path(name);
#else
            archivepp::string name(&header.FileName[0]);
            return archivepp::normalize_path(name);
#endif
        }

        struct cb_context
        {
            std::string buffer;
            secure_string password;

            cb_context() : buffer(), password() {}
            ~cb_context() {}
        };

        /**
        *  Callback that the unrar library calls whenever an event occurs during extraction.
        *
        *  @param msg The event.
        *  @param context A pointer to custom data.
        *  @param data A chunk of unpacked data.
        *  @param size The size of the unpacked chunk.
        */
        static int CALLBACK callback(UINT msg, LPARAM context, LPARAM data, LPARAM size)
        {
            int result = 0;
            cb_context * context_ptr = reinterpret_cast<cb_context*>(context);
            // Ensure it's a valid pointer
            if (context_ptr == nullptr)
                return -1;

            switch (msg)
            {
            case UCM_PROCESSDATA:
            {
                // A chunk of data has been unpacked and is ready for further processing
                char const * const unpacked_data = reinterpret_cast<char *>(data);
                // Reserve (size) + (size / 2).
                // This takes into account that there might be more than one chunk
                context_ptr->buffer.reserve(context_ptr->buffer.size() + (std::ceil(static_cast<float>(size) * 1.5f)));
                // Copy the chunk of unpacked data to our buffer
                // Use std::back_inserter to ensure that the unpacked data is always appended to our buffer
                std::copy(&unpacked_data[0], &unpacked_data[size], std::back_inserter(context_ptr->buffer));

                result = 1;
                break;
            }

#ifdef ARCHIVEPP_USE_WSTRING
            case UCM_NEEDPASSWORDW:
            {
                // Ensure there's a password for the context
                if (context_ptr->password.empty() != true)
                {
                    // Copy the password to data
                    // No need to check if data is a valid pointer
                    // It is allocated on the stack in libunrar's function CmdExtract::ExtrDllGetPassword()
                    auto length = context_ptr->password.length() > size ? size : context_ptr->password.length();
                    std::wcsncpy(reinterpret_cast<wchar_t*>(data), context_ptr->password.c_str(), length);

                    result = 1;
                }

                break;
            }
#else
            case UCM_NEEDPASSWORD:
            {
                //Ensure there's a password for the context
                if (context_ptr->password.empty() != true)
                {
                    // Copy the password to data
                    // No need to check if data is a valid pointer
                    // It is allocated on the stack in libunrar's function CmdExtract::ExtrDllGetPassword()
                    auto length = static_cast<LPARAM>(context_ptr->password.length()) > size ? size : context_ptr->password.length();
                    std::strncpy(reinterpret_cast<char*>(data), context_ptr->password.c_str(), length);

                    result = 1;
                }

                break;
            }
#endif
            default:
                break;
            }

            return result;
        }

        std::string fread(archivepp::string const & path, archivepp::string const & name, secure_string const & password, std::error_code & ec)
        {
            RAROpenArchiveDataEx data {};
            RARHeaderDataEx header {};
            HANDLE handle = unrar::open(path, data, ec);
            if (handle == nullptr)
                return std::string();

            std::unique_ptr<unrar::cb_context> context_ptr(new unrar::cb_context());
            if (context_ptr == nullptr)
                throw archivepp::null_pointer_error("context_ptr", __FUNCTION__);
            
            context_ptr->password = password;

            ::RARSetCallback(handle, unrar::callback, reinterpret_cast<LPARAM>(context_ptr.get()));

            ec = std::error_code(::RARReadHeaderEx(handle, &header), std::system_category());
            while (ec.value() == ERAR_SUCCESS)
            {
                archivepp::string current_name = get_name_from_header(header);
                if (current_name == name)
                {
                    ec = std::error_code(::RARProcessFile(handle, RAR_TEST, nullptr, nullptr), std::system_category());
                    break;
                }

                ::RARProcessFile(handle, RAR_SKIP, nullptr, nullptr);
                ec = std::error_code(::RARReadHeaderEx(handle, &header), std::system_category());
            }

            unrar::close(handle);

            return std::string(std::move(context_ptr->buffer));
        }

        std::string fread_index(archivepp::string const & path, uint64_t index, secure_string const & password, std::error_code & ec)
        {
            RAROpenArchiveDataEx data {};
            RARHeaderDataEx header {};
            HANDLE handle = unrar::open(path, data, ec);
            if (handle == nullptr)
                return std::string();

            std::unique_ptr<unrar::cb_context> context_ptr(new unrar::cb_context());
            if (context_ptr == nullptr)
                throw archivepp::null_pointer_error("context_ptr", __FUNCTION__);
            
            context_ptr->password = password;

            ::RARSetCallback(handle, unrar::callback, reinterpret_cast<LPARAM>(context_ptr.get()));

            ec = std::error_code(::RARReadHeaderEx(handle, &header), std::system_category());
            uint64_t current_index = 0;
            while (ec.value() == ERAR_SUCCESS)
            {
                if (current_index == index)
                {
                    ec = std::error_code(::RARProcessFile(handle, RAR_TEST, nullptr, nullptr), std::system_category());
                    break;
                }

                ++current_index;

                ::RARProcessFile(handle, RAR_SKIP, nullptr, nullptr);
                ec = std::error_code(::RARReadHeaderEx(handle, &header), std::system_category());
            }

            unrar::close(handle);

            return std::string(std::move(context_ptr->buffer));
        }
    }

    archive_rar::archive_rar(archivepp::string path, std::error_code & ec) :
        basic_archive(std::move(normalize_path(path)))
    {
        RAROpenArchiveDataEx data {};
        HANDLE handle = unrar::open(get_path(), data, ec);
        unrar::close(handle);
    }

    archive_rar::archive_rar(archivepp::string path, secure_string password, std::error_code & ec) :
        basic_archive(std::move(normalize_path(path)), std::move(password))
    {
        RAROpenArchiveDataEx data {};
        HANDLE handle = unrar::open(get_path(), data, ec);
        unrar::close(handle);
    }

    archive_rar::~archive_rar()
    {
    }

    int64_t archive_rar::get_number_of_entries() const
    {
        std::error_code ec;
        return unrar::get_number_of_entries(get_path(), ec);
    }

    std::string archive_rar::get_contents(entry_pointer const & entry, std::error_code & ec) const
    {
        return get_contents(entry, ARCHIVEPP_STR(""), ec);
    }

    std::string archive_rar::get_contents(entry_pointer const & entry, secure_string const & password, std::error_code & ec) const
    {
        if (entry == nullptr)
            throw archivepp::null_argument_error("entry", __FUNCTION__);

        return get_contents(entry->get_index(), password, ec);
    }

    std::string archive_rar::get_contents(uint64_t index, std::error_code & ec) const
    {
        return get_contents(index, ARCHIVEPP_STR(""), ec);
    }

    std::string archive_rar::get_contents(uint64_t index, secure_string const & password, std::error_code & ec) const
    {
        secure_string real_password = get_password().empty() == true ? password : get_password();
        return unrar::fread_index(get_path(), index, real_password, ec);
    }

    std::string archive_rar::get_contents(archivepp::string const & name, std::error_code & ec) const
    {
        return get_contents(name, ARCHIVEPP_STR(""), ec);
    }

    std::string archive_rar::get_contents(archivepp::string const & name, secure_string const & password, std::error_code & ec) const
    {
        secure_string real_password = get_password().empty() == true ? password : get_password();
        archivepp::string normalized_name = name;
        normalized_name = normalize_path(normalized_name);

        return unrar::fread(get_path(), normalized_name, real_password, ec);
    }

    auto archive_rar::get_entries(filter_function filter_fn) const -> std::vector<entry_pointer>
    {
        std::vector<entry_pointer> entries;

        RAROpenArchiveDataEx data {};
        std::error_code ec;
        HANDLE handle = unrar::open(get_path(), data, ec);
        if (handle == nullptr)
            throw archivepp::null_pointer_error("handle", __FUNCTION__);

        RARHeaderDataEx header {};
        uint64_t index = 0;

        while (::RARReadHeaderEx(handle, &header) == ERAR_SUCCESS)
        {
            std::error_code ec;
            archivepp::string name = unrar::get_name_from_header(header);
            auto size_pair = unrar::get_size_from_header(header);
            entry_pointer entry_ptr(new archive_entry_rar(name, index, size_pair.first, size_pair.second, ec));
            
            if(!ec)
            {
                bool skip = filter_fn != nullptr ? filter_fn(entry_ptr) : false;
                if (skip == false)
                    entries.emplace_back(std::move(entry_ptr));
            }

            ::RARProcessFile(handle, RAR_SKIP, nullptr, nullptr);
            ++index;
        }

        unrar::close(handle);

        return entries;
    }
}
