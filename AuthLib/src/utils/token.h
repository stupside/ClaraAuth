#include <string>

#include <nlohmann/json.hpp>
using nlohmann::json;

namespace token {
	const std::string& verify(std::string& token, const std::string& code);
	const std::string& generate(const json& object, const std::string& code);
}