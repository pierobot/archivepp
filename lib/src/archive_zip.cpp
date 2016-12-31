#include <archivepp/archive_zip.hpp>

namespace archivepp
{
    archive_zip::archive_zip(archivepp::string path, std::error_code & ec) :
        basic_archive(std::move(path))
    {
        int error = ZIP_ER_OK;

        // libzip only takes a char* pointer for the path
        // This presents a problem because we use std::wstring on Windows
        // The solution to this is to encode our UTF-16 string to UTF-8 as libzip supports that
#ifdef ARCHIVEPP_USE_WSTRING
        m_zip = ::zip_open(to_utf8(get_path().c_str()), ZIP_RDONLY, &error);
#else
        m_zip = ::zip_open(get_path().c_str(), ZIP_RDONLY, &error);
#endif
        if (error == ZIP_ER_OK)
        {
            ec = std::error_code();
        }
        else
        {
            ec = std::error_code(error, std::generic_category());
        }
    }

    archive_zip::~archive_zip()
    {
        if (m_zip != nullptr)
            ::zip_close(m_zip);
    }
        
    int64_t archive_zip::get_number_of_entries() const
    {
        // No need to check if m_zip is null as it will return -1 in that case
        return ::zip_get_num_entries(m_zip, 0);
    }
}