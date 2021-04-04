#ifndef LOGGING_H
#define LOGGING_H

#include <string>

class Logging {
public:
	static void log(std::string path, const std::string message);
private:
	static std::string get_time(std::string type);
};
#endif