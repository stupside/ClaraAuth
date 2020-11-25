#include "logging.h"

#include <fstream>
#include <iostream>

std::string Logging::get_time() {
	return "LOG";
};

Logging::Logging(std::string file_path) {
	if (file_path.empty())
	{
		path = getenv("APPDATA");
		path = path.append("/Tenet_Auth/auth.log");
	}
	else {
		path = file_path;
	}

	std::cout << "Path to debug file: " << path << std::endl;
};

void Logging::log(const std::string message) {
	std::ofstream file;
	file.open(path, std::ios_base::app);

	if (file.fail())
		return;

	file << "[" << id++ << "][" << get_time() << "] " << message << "\n";

	file.close();
}