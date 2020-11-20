#include <iostream>
#include <exception>

struct GenericException : public std::exception
{
public:
    GenericException(const char* msg) : std::exception(msg)
    {
    }
};