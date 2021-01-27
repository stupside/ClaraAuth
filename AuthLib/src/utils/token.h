#include <string>

#include <nlohmann/json.hpp>
using nlohmann::json;

namespace token {
	std::string verify(std::string token, std::string code);
	std::string generate(json object, std::string code);
}