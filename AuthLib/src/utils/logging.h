#include <string>


class Logging {
private:
	std::string full_name;
	std::string path;
	int id = 0;
	std::string get_time();
public:

	Logging() = default;

	Logging(std::string file_path);

	void log(const std::string message);

	~Logging() {  }
};