#include <iostream>
#include <exception>

namespace exceptions {
    struct TokenException : public std::exception
    {
    public:
        TokenException(const char* msg) : std::exception(msg)
        {
        }
    };

    struct GenericException : public std::exception
    {
    public:
        GenericException(const char* msg) : std::exception(msg)
        {
        }
    };
}