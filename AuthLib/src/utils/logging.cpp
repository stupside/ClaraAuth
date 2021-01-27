#include "logging.h"

#include <fstream>
#include <iostream>

std::string Logging::get_time(std::string type) {
	time_t now = time(0);
	struct tm  tstruct;
	char  buf[80];
	tstruct = *localtime(&now);

	if (type == "now")
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	else if (type == "date")
		strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);

	return std::string(buf);
};

void Logging::log(std::string path, const std::string message) {
	if (path.empty())
	{
		path = getenv("APPDATA");
		path = path.append("/tenet/");
	}

	std::ofstream file(path.append("tenet_"+get_time("date")+".log").c_str(), std::ios_base::out | std::ios_base::app);
	if (file.fail())
		return;

	file << get_time("now") << '\t' << message << '\n';
	file.close();
}