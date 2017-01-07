#include <archivepp/archive_rar.hpp>
#include <archivepp/except.hpp>

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
            data.ArcNameW = const_cast<char*>(path.c_str());
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

        static int64_t get_number_of_entries(HANDLE handle, std::error_code & ec)
        {
            RARHeaderDataEx header {};
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

            return count;
        }

        static std::pair<uint64_t, uint64_t> get_size_from_header(RARHeaderDataEx & header)
        {
            // Initialize the size values with their upper and lower 32 bits
            int64_t unpacked_size = (static_cast<int64_t>(header.UnpSizeHigh) << 32) | header.UnpSize;
            int64_t packed_size = (static_cast<int64_t>(header.PackSizeHigh) << 32) | header.PackSize;

            return std::make_pair(unpacked_size, packed_size);
        }

        static archivepp::string get_name_from_header(RARHeaderDataEx const & header)
        {
#ifdef ARCHIVEPP_USE_WSTRING
            return archivepp::string(&header.FileNameW[0]);
#else
            return archivepp::string(&header.FileName[0]);
#endif
        }
    }

    archive_rar::archive_rar(archivepp::string path, std::error_code & ec) :
        basic_archive(std::move(path)),
        m_arc_data(),
        m_handle(unrar::open(get_path(), m_arc_data, ec))
    {
    }

    archive_rar::archive_rar(archivepp::string path, archivepp::string password, std::error_code & ec) :
        basic_archive(std::move(path), std::move(password)),
        m_arc_data(),
        m_handle(unrar::open(get_path(), m_arc_data, ec))
    {
    }

    archive_rar::~archive_rar()
    {
        unrar::close(m_handle);
    }

    int64_t archive_rar::get_number_of_entries() const
    {
        if (m_handle == nullptr)
            throw archivepp::null_pointer_error("m_handle", __FUNCTION__);

        std::error_code ec;
        return unrar::get_number_of_entries(m_handle, ec);
    }

    std::string archive_rar::get_contents(entry_pointer entry, archivepp::string const & password, std::error_code & ec) const
    {
        return std::string();
    }

    auto archive_rar::get_entries(filter_function filter_fn) const -> std::vector<entry_pointer>
    {
        std::vector<entry_pointer> entries;

        if (m_handle == nullptr)
            throw archivepp::null_pointer_error("m_handle", __FUNCTION__);

        RARHeaderDataEx header {};
        uint64_t index = 0;

        while (::RARReadHeaderEx(m_handle, &header) == ERAR_SUCCESS)
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

            ::RARProcessFile(m_handle, RAR_SKIP, nullptr, nullptr);
            ++index;
        }

        return entries;
    }
}
