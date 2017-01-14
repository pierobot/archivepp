#include <archivepp/archive.hpp>

#include <archivepp/archive_zip.hpp>
#include <archivepp/archive_rar.hpp>

#include <archivepp/extensions.hpp>

namespace archivepp
{
     auto archive::open(archivepp::string path, std::error_code & ec) -> archive_pointer
     {
        archivepp::string extension = get_extension(path);
        archive_pointer archive_ptr(nullptr);

        if (zip::is_whitelisted(extension) == true)
            archive_ptr = std::move(archive_pointer(new archive_zip(std::move(path), ec)));
        else if (rar::is_whitelisted(extension) == true)
            archive_ptr = std::move(archive_pointer(new archive_rar(std::move(path), ec)));
        
        return archive_ptr;
     }

     auto archive::open(archivepp::string path, archivepp::secure_string password, std::error_code & ec) -> archive_pointer
     {
        archivepp::string extension = get_extension(path);
        archive_pointer archive_ptr(nullptr);

        if (zip::is_whitelisted(extension) == true)
            archive_ptr = std::move(archive_pointer(new archive_zip(std::move(path), std::move(password), ec)));
        else if (rar::is_whitelisted(extension) == true)
            archive_ptr = std::move(archive_pointer(new archive_rar(std::move(path), std::move(password), ec)));
        
        return archive_ptr;
     }
}
