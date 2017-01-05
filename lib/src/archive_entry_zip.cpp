#include <archivepp/archive_entry_zip.hpp>

#include <cstring>

namespace archivepp
{
    archive_entry_zip::archive_entry_zip(zip_t * handle,
                                         uint64_t index,
                                         std::error_code & ec) :
        basic_archive_entry(handle, index)
    {
        int error = ::zip_stat_index(handle, index, 0, &m_zip_stat);
        
        ec = error == -1 ? get_last_error() : std::error_code();
    }

    archive_entry_zip::~archive_entry_zip()
    {
    }

    uint64_t archive_entry_zip::get_size() const
    {
        return m_zip_stat.valid & ZIP_STAT_SIZE ? m_zip_stat.size : 0;
    }

    archivepp::string archive_entry_zip::get_name() const
    {
        archivepp::string name;

        if (m_zip_stat.valid & ZIP_STAT_NAME)
        {
            size_t length = std::strlen(&m_zip_stat.name[0]);

#ifdef ARCHIVEPP_USE_WSTRING
            name = to_utf16(std::string(&m_zip_stat.name[0], &m_zip_stat.name[length]));
#else
            name.reserve(length);
            std::copy(&m_zip_stat.name[0], &m_zip_stat.name[length], std::back_inserter(name));
#endif
        }

        return name;
    }

    std::string archive_entry_zip::get_contents(std::error_code & ec) const
    {
        std::string contents;

        if (get_handle() == nullptr)
            throw std::runtime_error("Unable to dereference null pointer");

        uint64_t size = get_size();
        if (size != 0)
        {
            contents.resize(size);

            zip_file_t * entry_handle = ::zip_fopen_index(get_zip_handle(), get_index(), 0);
            if (entry_handle != nullptr)
            {
                ::zip_fread(entry_handle, &contents[0], size);
                ::zip_fclose(entry_handle);
            }
            else
            {
                ec = get_last_error();
            }
        }

        return contents;
    }

    zip_t * archive_entry_zip::get_zip_handle() const
    {
        return static_cast<zip_t *>(get_handle());
    }

    std::error_code archive_entry_zip::get_last_error() const
    {
        if (get_zip_handle() == nullptr)
            throw std::runtime_error(std::string(__FUNCTION__) + "- Cannot dereference null zip_t pointer");

        std::error_code ec;
        zip_error_t * zerror = ::zip_get_error(get_zip_handle());
        if (zerror != nullptr)
        {
            ec = std::error_code(::zip_error_code_zip(zerror), std::system_category());
        }

        return ec;
    }
}
