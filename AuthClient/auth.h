#ifndef AUTH_H
#define AUTH_H

#include <list>
#include <string>

#include "features.h"

namespace tenet {
	class Auth;
	class Configuration {
	public:
		class Debug {
			friend class tenet::Auth;
			friend class tenet::Configuration;
		private:
			bool		enabled;
			std::string path;
		protected:
			Debug() : enabled(false) {};
		public:
			std::string get();
			bool activated();
		};
		class Extension {
			friend class tenet::Auth;
			friend class tenet::Configuration;
		private:
			std::list<std::string> variables;
		protected:
			Extension() : variables(std::list<std::string>()) {}
		public:
			std::list<std::string> get();
		};
		class Hardware {
			friend class tenet::Auth;
			friend class tenet::Configuration;
		public:
			enum class Options {
				Sid,
				Processor,
				Computer_Name,
				Username,
				Base_Board,
				Physical_Memory
			};
			typedef std::list<tenet::Configuration::Hardware::Options> Base;
		private:
			Base options;
			std::string value;
		protected:
			Hardware() {};
		public:
			/// <summary>
			/// Returns true if the hardware id is valid.
			/// </summary>
			/// <returns></returns>
			bool is_valid();

			/// <summary>
			/// Get current hardware id.
			/// </summary>
			/// <returns></returns>
			std::string					get();
		};
		class Endpoints {
			friend class tenet::Auth;
			friend class tenet::Configuration;
		private:
			std::string					authenticate;
			std::string					stream;
		public:
			Endpoints(std::string authenticate = "", std::string stream = "")
				: authenticate(authenticate), stream(stream) {};
		};
	public:

		tenet::Configuration::Hardware	hardware;
		tenet::Configuration::Debug		debug;
		tenet::Configuration::Extension	extension;
		tenet::Configuration::Endpoints	endpoints;

		/// <summary>
		/// Init a configuration
		/// </summary>
		Configuration() : debug(), extension(), hardware(), endpoints() {};

		/// <summary>
		/// Use a custom string as the hardware id. Make sure this string is unique.
		/// </summary>
		/// <param name="value">Unique string</param>
		/// <returns></returns>
		tenet::Configuration			with_hardware(std::string value);

		/// <summary>
		/// Use an auto generated hardware id. This option might be better if you don't want to bother.
		/// </summary>
		/// <param name="options">List of options to generate the random hardware id</param>
		/// <returns></returns>
		tenet::Configuration			with_hardware(tenet::Configuration::Hardware::Base options);

		/// <summary>
		/// Enable debug logs
		/// </summary>
		/// <param name="path">Path to debug file</param>
		/// <returns></returns>
		tenet::Configuration			with_debug(std::string path);

		/// <summary>
		/// Tell the auth context to request some variables. The variables your request will be sent back only if the authentication succeeded and if they were found in the variables.
		/// </summary>
		/// <param name="names">Names of the variables you want the auth to send back if they exist</param>
		/// <returns></returns>
		tenet::Configuration			with_variables(std::list<std::string> names);

		/// <summary>
		/// Add a variable to the auth context. The variables your request will be sent back only if the authentication succeeded and if they were found in the variables.
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		tenet::Configuration			add_variable(std::string name);

		/// <summary>
		/// Configure the endpoints.
		/// </summary>
		/// <param name="endpoints"></param>
		/// <returns></returns>
		tenet::Configuration			with_endpoints(tenet::Configuration::Endpoints endpoints);

		/// <summary>
		/// Returns true if the configuration is valid
		/// </summary>
		/// <returns></returns>
		bool							is_valid();
	};

	class Auth {
	private:
		bool					initialized;

		std::string				key;
		const std::string		code;
	public:

		features::Authenticate* response;
		tenet::Configuration	configuration;

		/// <summary>
		/// Instanciate a new Auth context binded to a configuration
		/// </summary>
		/// <param name="product_code">Your product code</param>
		Auth(std::string code, tenet::Configuration configuration)
			: code(code), initialized(true), configuration(configuration), response() {};

		/// <summary>
		/// Returns true if the client can be authenticated
		/// </summary>
		/// <returns></returns>
		const bool					can_authenticate();

		/// <summary>
		/// Returns true if the client is authenticated
		/// </summary>
		/// <returns></returns>
		const bool					is_authenticated();

		/// <summary>
		/// Send an authentication request to the API.
		/// </summary>
		/// <param name="key">The key used to license your software.</param>
		/// <param name="attempts">Number of attemps in case the auth failed.</param>
		features::Authenticate		authenticate(std::string key, int attempts = 1);

		/// <summary>
		/// Stream something from your middlman server.
		/// <param name="authenticate">Auth response.</param>
		/// </summary>
		features::Stream			stream(features::Authenticate& authenticate);

		~Auth() {

		}
	};
}
#endif
