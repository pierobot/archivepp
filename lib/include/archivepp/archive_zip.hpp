#pragma once

#include <archivepp/basic_archive.hpp>

#include <system_error>

#include <zip.h>

namespace archivepp
{
    class archive_zip final : public basic_archive
    {
    public:
        archive_zip(archivepp::string path, std::error_code & ec);

        virtual ~archive_zip();

        virtual int64_t get_number_of_entries() const override;
    protected:
    private:
        zip_t * m_zip;
    };
}