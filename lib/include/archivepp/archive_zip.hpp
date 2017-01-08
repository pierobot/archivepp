#pragma once

#include <archivepp/basic_archive.hpp>

#include <system_error>

#include <zip.h>

namespace archivepp
{
    /**
     * Class for reading from zip archives.
     */
    class archive_zip final : public basic_archive
    {
    public:
        /**
         * Constructs a zip archive object from a path.
         *
         * @param path The path of the archive.
         * @param ec An object that will hold any errors.
         */
        archive_zip(archivepp::string path, std::error_code & ec);

        /**
         * Constructs a zip archive object from a path and password.
         *
         * @param path The path of the archive.
         * @param password The password of the archive.
         * @param ec An object that will hold any errors.
         */
        archive_zip(archivepp::string path, archivepp::string password, std::error_code & ec);

        virtual ~archive_zip();

        virtual int64_t get_number_of_entries() const override;

        virtual std::string get_contents(entry_pointer const & entry, std::error_code & ec) const override;
        virtual std::string get_contents(entry_pointer const & entry, archivepp::string const & password, std::error_code & ec) const override;
        virtual std::string get_contents(uint64_t index, std::error_code & ec) const override;
        virtual std::string get_contents(uint64_t index, archivepp::string const & password, std::error_code & ec) const override;
        virtual std::string get_contents(archivepp::string const & name, std::error_code & ec) const override;
        virtual std::string get_contents(archivepp::string const & name, archivepp::string const & password, std::error_code & ec) const override;

        virtual std::vector<entry_pointer> get_entries(filter_function filter_fn = nullptr) const override;
    protected:
    private:
        zip_t * m_zip;
    };
}
