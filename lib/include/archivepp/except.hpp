#pragma once

#include <stdexcept>

namespace archivepp
{
    class notimplemented_error : std::exception
    {
    public:
        char const * what() const noexcept
        {
            return "Function not implemented.";
        }
    protected:
    private:
    };

    class null_argument_error : std::runtime_error
    {
    public:
        null_argument_error(std::string const & parameter, std::string const & function) :
            std::runtime_error(function + std::string(" - Got unexpected null pointer in argument \'") + parameter + "\'")
        {
        }

        null_argument_error(std::string const & parameter, char const * function) :
            std::runtime_error(std::string(function) + std::string(" - Got unexpected null pointer in argument \'") + parameter + "\'")
        {
        }
    };

    class null_pointer_error : std::runtime_error
    {
    public:
         null_pointer_error(std::string const & name, std::string const & function) :
            std::runtime_error(function + std::string(" - Got unexpected null pointer \'") + name + "\'")
        {
        }

         null_pointer_error(std::string const & name, char const * function) :
            std::runtime_error(std::string(function) + std::string(" - Got unexpected null pointer \'") + name + "\'")
        {
        }
    };
}
