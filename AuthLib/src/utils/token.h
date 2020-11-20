#include <string>

#include <nlohmann/json.hpp>
using nlohmann::json;

class Token {
public:
	static std::string verify(std::string token, std::string code);
	static std::string generate(json object, std::string code);
	
private:
	static std::string signature(std::string code);
};