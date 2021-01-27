#ifndef API_H
#define API_H

#include <string>
#include <map>

#include <cpr/cpr.h>

#include "features.h"

class Api {
public:
    static features::Authenticate   authenticate(std::string key, std::string hwid, std::string code, std::list<std::string> variables, std::string endpoint, int attempts = 1);
    static features::Stream         stream(features::Authenticate authenticate, std::string endpoint, int attempts = 1);
private:
    static cpr::Response            post_req(std::string endpoint, cpr::Parameters parameters, cpr::Header headers, int attempts = 1);
    static cpr::Response            get_req(std::string endpoint, cpr::Parameters parameters, cpr::Header headers, int attempts = 1);
};

#endif