#ifndef TOKEN_H
#define TOKEN_H

#include <string>

#include <nlohmann/json.hpp>
using nlohmann::json;

namespace token {
	std::string verify(std::string& token, const std::string& code);
	std::string generate(const json& object, const std::string& code);
}
#endif