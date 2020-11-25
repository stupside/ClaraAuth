#include "logging.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

std::string Logging::get_time() {
	return "LOG";
};

Logging::Logging(std::string file_path, std::string file_name, std::string extension) {
	if (file_path.empty())
	{
		path = getenv("APPDATA");
	}
	else {
		path = file_path;
	}

	if (file_name.empty())
		full_name = "default";

	full_name = file_name + "." + extension;

	std::replace(full_name.begin(), full_name.end(), ' ', '_');

	std::cout << "Path to debug file: " << path << full_name << std::endl;
};

void Logging::log(const std::string message) {

	std::ofstream file = std::ofstream(path+full_name);

	file << "[ " << id++ << " ] [ " << get_time() << " ] " << message << "\n";

	file.close();
}