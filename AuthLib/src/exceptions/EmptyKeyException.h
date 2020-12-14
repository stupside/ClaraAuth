#include <iostream>
#include <exception>

struct EmptyKeyException : public std::exception
{
public:
    EmptyKeyException(const char* msg) : std::exception(msg)
    {
    }
};