#include <archivepp/archive_zip.hpp>
#include <archivepp/archive_entry_zip.hpp>
#include <archivepp/except.hpp>

#include <cstring>
#include <stdexcept>
#include <utility>

namespace archivepp
{
    namespace zip
    {
        static std::error_code get_last_error(zip_t * zip)
        {
            zip_error_t * zerror = ::zip_get_error(zip);

            return std::error_code(::zip_error_code_zip(zerror), std::system_category());
        }

        static zip_t * open(archivepp::string const & path, std::error_code & ec)
        {
            zip_t * zip = nullptr;
            int error = ZIP_ER_OK;

            // libzip only takes a char* pointer for the path
            // This presents a problem because we use std::wstring on Windows
            // The solution to this is to encode our UTF-16 string to UTF-8 as libzip supports that
    #ifdef ARCHIVEPP_USE_WSTRING
            zip = ::zip_open(to_utf8(path.c_str()), ZIP_RDONLY, &error);
    #else
            zip = ::zip_open(path.c_str(), ZIP_RDONLY, &error);
    #endif
            if (error == ZIP_ER_OK)
            {
                ec = std::error_code();
            }
            else
            {
                ec = std::error_code(error, std::system_category());
            }

            return zip;
        }

        static std::error_code close(zip_t * zip)
        {
            return std::error_code(::zip_close(zip), std::system_category());
        }

        static int64_t get_number_of_entries(zip_t * zip)
        {
            return ::zip_get_num_entries(zip, 0);
        }

        static std::pair<uint64_t, uint64_t> get_size_from_index(zip_t * zip, int64_t index, std::error_code & ec)
        {        
            zip_stat_t zstat {};
            int result = ::zip_stat_index(zip, index, 0, &zstat);
            if (result == -1)
            {
                ec = get_last_error(zip);
                return std::make_pair(0, 0);
            }

            uint64_t original_size = 0;
            uint64_t compressed_size = 0;
            if (zstat.valid & ZIP_STAT_SIZE)
                original_size = zstat.size;
            if (zstat.valid & ZIP_STAT_COMP_SIZE)
                compressed_size = zstat.comp_size;

            ec = std::error_code();
            return std::make_pair(original_size, compressed_size);
        }

        static zip_file_t * fopen_from_index(zip_t * zip, int64_t index, secure_string const & password, std::error_code & ec)
        {
            zip_file_t * zfile = password.empty() == true ? ::zip_fopen_index(zip, index, 0) :
                                                            ::zip_fopen_index_encrypted(zip, index, 0, password.c_str());

            ec = zfile == nullptr ? get_last_error(zip) : std::error_code();
            
            return zfile;
        }

        static zip_file_t * fopen_from_name(zip_t * zip, std::string const & name, secure_string const & password, std::error_code & ec)
        {
            zip_file_t * zfile = password.empty() == true ? ::zip_fopen(zip, name.c_str(), 0) :
                                                            ::zip_fopen_encrypted(zip, name.c_str(), 0, password.c_str());

            ec = zfile == nullptr ? get_last_error(zip) : std::error_code();
            
            return zfile;
        }

        static std::error_code fclose(zip_file_t * file)
        {
            return std::error_code(::zip_fclose(file), std::system_category());
        }

        static std::string fread(zip_file_t * file, uint64_t size)
        {
            std::string contents;
            contents.resize(size);

            int result = ::zip_fread(file, &contents[0], size);
            if (result == -1)
            {
                contents.clear();
            }

            return contents;
        }

        static archivepp::string get_filename_from_index(zip_t * zip, uint64_t index, std::error_code & ec)
        {
            zip_stat_t zstat {};
            int result = ::zip_stat_index(zip, index, 0, &zstat);
            if (result == -1)
            {
                ec = get_last_error(zip);
                return archivepp::string();
            }

            archivepp::string name;

            // libzip provides a function called zip_name_locate, but doing the below will save us an allocation.
            if (zstat.valid & ZIP_STAT_NAME)
            {
                size_t length = std::strlen(&zstat.name[0]);

#ifdef ARCHIVEPP_USE_WSTRING
                name = to_utf16(std::string(&zstat.name[0], &zstat.name[length]));
 #else
                name.reserve(length);
                std::copy(&zstat.name[0], &zstat.name[length], std::back_inserter(name));
 #endif
            }

            ec = std::error_code();

            return name;
        }

        static int64_t get_index_from_filename(zip_t * zip, std::string const & name, std::error_code & ec)
        {
            int64_t result = ::zip_name_locate(zip, name.c_str(), 0);
            ec = result == -1 ? get_last_error(zip) : std::error_code();

            return result;
        }
    }

    archive_zip::archive_zip(archivepp::string path, std::error_code & ec) :
        basic_archive(std::move(path)),
        m_zip(zip::open(get_path(), ec))
    {
    }

    archive_zip::archive_zip(archivepp::string path, secure_string password, std::error_code & ec) :
        basic_archive(std::move(path), std::move(password)),
        m_zip(zip::open(get_path(), ec))
    {
    }

    archive_zip::~archive_zip()
    {
        zip::close(m_zip);
    }
        
    int64_t archive_zip::get_number_of_entries() const
    {
        if (m_zip == nullptr)
            throw archivepp::null_pointer_error("m_zip", __FUNCTION__);
        // No need to check if m_zip is null as it will return -1 in that case
        return zip::get_number_of_entries(m_zip);
    }

    std::string archive_zip::get_contents(entry_pointer const & entry, std::error_code & ec) const
    {
        if (entry == nullptr)
            throw archivepp::null_argument_error("entry", __FUNCTION__);

        return get_contents(entry->get_index(), ec);
    }

    std::string archive_zip::get_contents(entry_pointer const & entry, secure_string const & password, std::error_code & ec) const
    {
        if (entry == nullptr)
            throw archivepp::null_argument_error("entry", __FUNCTION__);

        return get_contents(entry->get_index(), password, ec);
    }

    std::string archive_zip::get_contents(uint64_t index, std::error_code & ec) const
    {
        return get_contents(index, "", ec);
    }

    std::string archive_zip::get_contents(uint64_t index, secure_string const & password, std::error_code & ec) const
    {
        std::string contents;

        // If the user supplies a password for this particular entry, then use it rather than the archive password
        secure_string realpassword = password.empty() == false ? password : get_password();
        zip_file_t * zfile = zip::fopen_from_index(m_zip, index, realpassword, ec);
        if (!ec)
        {
            auto size_pair = zip::get_size_from_index(m_zip, index, ec);
            if (!ec)
            {
                contents =  zip::fread(zfile, size_pair.first);
            }

            zip::fclose(zfile);
        }

        return contents;
    }

    std::string archive_zip::get_contents(archivepp::string const & name, std::error_code & ec) const
    {
        return get_contents(name, "", ec);
    }

    std::string archive_zip::get_contents(archivepp::string const & name, secure_string const & password, std::error_code & ec) const
    {
        std::string contents;

        // If the user supplies a password for this particular entry, then use it rather than the archive password
        secure_string realpassword = password.empty() == false ? password : get_password();

#ifdef ARCHIVEPP_USE_WSTRING
        std::string utf8name = to_utf8(name);
        zip_file_t * zfile = zip::fopen_from_name(m_zip, utf8name, realpassword, ec);
#else
        zip_file_t * zfile = zip::fopen_from_name(m_zip, name, realpassword, ec);
#endif
        if (!ec)
        {
            int64_t index = zip::get_index_from_filename(m_zip, name, ec);
            if (!ec)
            {
                auto size_pair = zip::get_size_from_index(m_zip, index, ec);
                if (!ec)
                {
                    contents = zip::fread(zfile, size_pair.first);
                }
            }

            zip::fclose(zfile);
        }

        return contents;
    }

    auto archive_zip::get_entries(filter_function filter_fn) const -> std::vector<entry_pointer>
    {
        std::vector<entry_pointer> entries;

        if (m_zip == nullptr)
            throw archivepp::null_pointer_error("m_zip", __FUNCTION__);

        for (int64_t i = 0; i < get_number_of_entries(); ++i)
        {
            std::error_code ec;
            auto size_pair = zip::get_size_from_index(m_zip, i, ec);
            if (ec)
                continue;

            archivepp::string name = zip::get_filename_from_index(m_zip, i, ec);
            if (ec)
                continue;

            entry_pointer entry_ptr(new archive_entry_zip(name, i, size_pair.first, size_pair.second, ec));
            if (!ec)
            {
                bool skip = filter_fn != nullptr ? filter_fn(entry_ptr) : false;
                if (skip == false)
                    entries.emplace_back(std::move(entry_ptr));
            }
        }

        return entries;
    }
}
