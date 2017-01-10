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

        basic_archive(archivepp::string path, secure_string password) :
            m_path(std::move(path)),
            m_password(std::move(password))
        {
        }

        virtual ~basic_archive() {}

        virtual archivepp::string const & get_path() const override final
        {
            return m_path;
        }
    protected:
        virtual secure_string const & get_password() const override final
        {
            return m_password;
        }
    private:
        archivepp::string m_path;
        secure_string m_password;
    };
}
