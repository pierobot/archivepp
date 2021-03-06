#pragma once

#include <archivepp/basic_archive.hpp>
#include <archivepp/archive_entry_rar.hpp>

#include <system_error>

namespace archivepp
{
    class archive_rar final : public basic_archive
    {
    public:
        archive_rar(archivepp::string path, std::error_code & ec);
        archive_rar(archivepp::string path, secure_string password, std::error_code & ec);

        archive_rar(archive_rar const&) = delete;

        virtual ~archive_rar();

        archive_rar & operator=(archive_rar const&) = delete;

        virtual int64_t get_number_of_entries() const override;

        virtual std::string get_contents(entry_pointer const & entry, std::error_code & ec) const override;
        virtual std::string get_contents(entry_pointer const & entry, secure_string const & password, std::error_code & ec) const override;
        virtual std::string get_contents(uint64_t index, std::error_code & ec) const override;
        virtual std::string get_contents(uint64_t index, secure_string const & password, std::error_code & ec) const override;
        virtual std::string get_contents(archivepp::string const & name, std::error_code & ec) const override;
        virtual std::string get_contents(archivepp::string const & name, secure_string const & password, std::error_code & ec) const override;

        virtual std::vector<entry_pointer> get_entries(filter_function filter_fn = nullptr) const override;
    protected:
    private:
    };
}
