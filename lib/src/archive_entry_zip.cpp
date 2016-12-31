#include <archivepp/archive_entry_zip.hpp>

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

    std::string archive_entry_zip::get_contents() const
    {
        std::string contents;

        if (get_handle() == nullptr)
            throw std::runtime_error("Unable to dereference null pointer");

        uint64_t size = get_size();
        if (size != 0)
        {
            contents.resize(size);

            zip_file_t * entry_handle = ::zip_fopen_index(get_zip_handle(), get_index(), 0);
        }
        
        return contents;
    }

    zip_t * archive_entry_zip::get_zip_handle() const
    {
        return static_cast<zip_t *>(get_handle());
    }

    std::error_code archive_entry_zip::get_last_error()
    {
        if (get_zip_handle() == nullptr)
            throw std::runtime_error(std::string(__FUNCTION__) + "- Cannot dereference null zip_t pointer");

        std::error_code ec;
        zip_error_t * zerror = ::zip_get_error(get_zip_handle());
        if (zerror != nullptr)
        {
            ec = std::error_code(::zip_error_code_system(zerror), std::system_category());
        }

        return ec;
    }
}
