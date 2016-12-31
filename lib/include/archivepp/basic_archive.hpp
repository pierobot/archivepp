#pragma once

#include <archivepp/archive.hpp>
#include <archivepp/except.hpp>

namespace archivepp
{
    class basic_archive : public archive
    {
    public:
        basic_archive(archivepp::string path) :
            m_path(std::move(path))
        {
        }

        virtual ~basic_archive() {}

        virtual archivepp::string const & get_path() const override final
        {
            return m_path;
        }

        virtual int64_t get_number_of_entries() const
        {
            throw archivepp::notimplemented_error();
            // Just to ignore warnings
            return 0;
        }
    protected:       
    private:
        archivepp::string const m_path;
    };
}
