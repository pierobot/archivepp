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
        archive_rar(archivepp::string path, archivepp::string password, std::error_code & ec);

        virtual ~archive_rar();

        virtual int64_t get_number_of_entries() const override;
        virtual std::string get_contents(entry_pointer entry, archivepp::string const & password, std::error_code & ec) const override;
        virtual std::vector<entry_pointer> get_entries(filter_function filter_fn = nullptr) const override;
    protected:
    private:
        RAROpenArchiveDataEx m_arc_data;
        HANDLE m_handle;
    };
}
