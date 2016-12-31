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
}
