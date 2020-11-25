#ifndef AUTH_H
#define AUTH_H

#include <list>
#include <string>
#include <stdio.h>

#include "src/models/responses.h"

namespace tenet {

	enum class HwidOption {
		Computer_Name,
		Username,
		Base_Board,
		Physical_Memory
	};

	class Auth {
	private:

		bool init;

		bool debug;
		std::string debug_path;

		std::string hwid;

		std::list<std::string> requested_variables;
		const std::string product_code;

	public:

		Auth(std::string product_code) : product_code(product_code), init(true), debug(false) {};

		void with_debug(std::string path = "");

		void with_custom_hwid(std::string custom_hwid);
		void with_hwid(std::list<HwidOption> hwid_options = std::list<HwidOption>());

		void with_variables(std::list<std::string> variables);

		tenet::Response process(std::string key, int attempts = 1);

		~Auth() { }
	};
}
#endif
