#pragma once

#include <archivepp/archive_entry.hpp>
#include <archivepp/string.hpp>

#include <cstdint>
#include <functional>
#include <memory>
#include <system_error>
#include <vector>

namespace archivepp
{
    class archive
    {
    public:
        /**
         * A smart pointer that contains the interface to an archive.
         */
        typedef std::unique_ptr<archive> archive_pointer;

        /**
         * A smart pointer that contains the interface to an archive entry.
         */
        typedef std::unique_ptr<archive_entry> entry_pointer;

        /**
         * A unary function that determines whether an archive entry is excluded or not.
         */
        typedef std::function<bool(entry_pointer const & entry_ptr)> filter_function;

        virtual ~archive() {}

        /**
         * Gets the path of the archive.
         */
        virtual archivepp::string const & get_path() const = 0;
        
        /**
         * Gets the total number of entries in the archive.
         */
        virtual int64_t get_number_of_entries() const = 0;

        /**
         * Gets the contents of an entry in the archive.
         *
         * @param entry A smart pointer to an archive entry.
         * @param ec An object that will hold any errors.
         */
        virtual std::string get_contents(entry_pointer const & entry, std::error_code & ec) const = 0;

                /**
         * Gets the contents of an entry in the archive.
         *
         * @param entry A smart pointer to an archive entry.
         * @param password The password for the entry.
         * @param ec An object that will hold any errors.
         */
        virtual std::string get_contents(entry_pointer const & entry, archivepp::secure_string const & password, std::error_code & ec) const = 0;

        /**
         * Gets the contents of an entry in the archive.
         *
         * @param index The index of an archive entry.
         * @param ec An object that will hold any errors.
         */
        virtual std::string get_contents(uint64_t index, std::error_code & ec) const = 0;

        /**
         * Gets the contents of an entry in the archive.
         *
         * @param index The index of an archive entry.
         * @param password The password for the entry.
         * @param ec An object that will hold any errors.
         */
        virtual std::string get_contents(uint64_t index, archivepp::secure_string const & password, std::error_code & ec) const = 0;

        /**
         * Gets the contents of an entry in the archive.
         *
         * @param name The name of an archive entry.
         * @param ec An object that will hold any errors.
         */
        virtual std::string get_contents(archivepp::string const & name, std::error_code & ec) const = 0;

        /**
         * Gets the contents of an entry in the archive.
         *
         * @param name The name of an archive entry.
         * @param password The password for the entry.
         * @param ec An object that will hold any errors.
         */
        virtual std::string get_contents(archivepp::string const & name, archivepp::secure_string const & password, std::error_code & ec) const = 0;

        /**
         * Gets a vector of entry pointers from the archive.
         *
         * @param filter_fn The unary function to be called to determine whether to exclude an entry or not.
         */
        virtual std::vector<entry_pointer> get_entries(filter_function filter_fn) const = 0;

        static archive_pointer open(archivepp::string path, std::error_code & ec);

        static archive_pointer open(archivepp::string path, archivepp::secure_string password, std::error_code & ec);
    protected:
        /**
         * Gets the password of the archive.
         */
        virtual archivepp::secure_string const & get_password() const = 0;
    private:
    };
}
