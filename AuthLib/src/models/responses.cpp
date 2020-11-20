#include "responses.h"

const bool tenet::Response::Succeed()
{
    return succeed;
}

const std::string tenet::Response::Message()
{
    return message;
}

const std::map<std::string, std::string> tenet::Response::Variables()
{
    return this->variables;
}
