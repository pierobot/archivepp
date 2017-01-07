#pragma once

#include <archivepp/archive.hpp>
#include <archivepp/except.hpp>

namespace archivepp
{
    class basic_archive : public archive
    {
    public:
        basic_archive(archivepp::string path) :
            m_path(std::move(path)),
            m_password()
        {
        }

        basic_archive(archivepp::string path, archivepp::string password) :
            m_path(std::move(path)),
            m_password(std::move(password))
        {
        }

        virtual ~basic_archive() {}

        virtual archivepp::string const & get_path() const override final
        {
            return m_path;
        }

        virtual archivepp::string const & get_password() const override final
        {
            return m_password;
        }
    protected:       
    private:
        archivepp::string m_path;
        archivepp::string m_password;
    };
}
