#ifndef AUTH_H
#define AUTH_H

#include <list>
#include <string>
#include <stdio.h>

#include "responses.h"

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

		bool success;
		std::string key;

		bool debug;
		std::string debug_path;

		std::string hwid;

		std::list<std::string> requested_variables;
		const std::string product_code;

	public:

		/// <summary>
		/// Instanciate a new Auth context
		/// </summary>
		/// <param name="product_code">Your product code</param>
		Auth(std::string product_code) : product_code(product_code), init(true), debug(false), success(false) {};

		/// <summary>
		/// Enable debug to file in case the Auth fails.
		/// </summary>
		/// <param name="path">Path to the log file. C:\exemple\exemple.log</param>
		void with_debug(std::string path = "");

		/// <summary>
		/// Use a custom string as the hardware id. You should make it an unique string.
		/// </summary>
		/// <param name="key">Unique string</param>
		void with_custom_hwid(std::string custom_hwid);

		/// <summary>
		/// Use an auto generated hardware id. This option might be better if you don't want to bother.
		/// </summary>
		/// <param name="hwid_options">List of options to generate the random hardware id</param>
		void with_hwid(std::list<HwidOption> hwid_options = std::list<HwidOption>());

		/// <summary>
		/// Tell the Auth context to request some variables. THe variables your request will be sent back only if the authentication succeeded and if they were found in the variables.
		/// </summary>
		/// <param name="variables">List of variables you want the auth to send back if the key is valid</param>
		void with_variables(std::list<std::string> variables);

		/// <summary>
		/// Send an authentication request to the API.
		/// </summary>
		/// <param name="key">The key used to license your software.</param>
		/// <param name="attempts">Number of attemps in case the auth failed.</param>
		tenet::Response process(std::string key, int attempts = 1);

		~Auth() { }
	};
}
#endif
