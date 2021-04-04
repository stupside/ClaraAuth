#ifndef API_H
#define API_H

#include <string>
#include <map>

#include <cpr/cpr.h>

#include "features.h"

class Api {
public:
    static const features::Response<features::Authenticate>* authenticate(const std::string& key, const std::string& hwid, const std::string& code, std::list<std::string> variables, const std::string& endpoint, int attempts);
    static const features::Response<features::Stream>& stream(const features::Authenticate& authenticate, const std::string& endpoint);
private:
    static cpr::Response post_req(std::string endpoint, cpr::Parameters parameters, cpr::Header headers, int attempts);
    static cpr::Response get_req(std::string endpoint, cpr::Parameters parameters, cpr::Header headers, int attempts);
};

#endif