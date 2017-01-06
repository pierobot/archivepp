#include <archivepp/archive_entry_zip.hpp>

#include <cstring>

namespace archivepp
{
    archive_entry_zip::archive_entry_zip(archivepp::string name,
                                         uint64_t index,
                                         uint64_t size,
                                         std::error_code & ec) :
        basic_archive_entry(std::move(name), index, size)
    {
    }

    archive_entry_zip::~archive_entry_zip()
    {
    }

//     archivepp::string archive_entry_zip::get_name() const
//     {
//         archivepp::string name;

//         if (m_zip_stat.valid & ZIP_STAT_NAME)
//         {
//             size_t length = std::strlen(&m_zip_stat.name[0]);

// #ifdef ARCHIVEPP_USE_WSTRING
//             name = to_utf16(std::string(&m_zip_stat.name[0], &m_zip_stat.name[length]));
// #else
//             name.reserve(length);
//             std::copy(&m_zip_stat.name[0], &m_zip_stat.name[length], std::back_inserter(name));
// #endif
//         }

//         return name;
//     }

    // std::string archive_entry_zip::get_contents(std::error_code & ec) const
    // {
    //     std::string contents;

    //     if (get_handle() == nullptr)
    //         throw std::runtime_error("Unable to dereference null pointer");

    //     uint64_t size = get_size();
    //     if (size != 0)
    //     {
    //         contents.resize(size);

    //         zip_file_t * entry_handle = ::zip_fopen_index(get_zip_handle(), get_index(), 0);
    //         if (entry_handle != nullptr)
    //         {
    //             if (::zip_fread(entry_handle, &contents[0], size) == -1)
    //                 ec = get_last_error();

    //             ::zip_fclose(entry_handle);
    //         }
    //         else
    //         {
    //             ec = get_last_error();
    //         }
    //     }

    //     return contents;
    // }
}
