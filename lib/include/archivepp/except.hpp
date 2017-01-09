#pragma once

#include <stdexcept>

namespace archivepp
{
    class exception : std::exception
    {
    public:
        exception() : m_message() {}
        exception(std::string const & message) : m_message(message) {}
        
        char const * what() const noexcept { return m_message.c_str(); }
    protected:
    private:
        std::string const m_message;
    };

    class null_argument_error : archivepp::exception
    {
    public:
        null_argument_error(std::string const & parameter, char const * function) :
            archivepp::exception(std::string(function) + std::string(" - Got unexpected null pointer in argument \'") + parameter + "\'")
        {
        }
    };

    class null_pointer_error : archivepp::exception
    {
    public:
         null_pointer_error(std::string const & name, char const * function) :
            archivepp::exception(std::string(function) + std::string(" - Got unexpected null pointer \'") + name + "\'")
        {
        }
    };
}
