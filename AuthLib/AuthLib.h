#ifndef AUTH_H
#define AUTH_H

#include <list>
#include <string>
#include <exception>

#include "src/modules/features.h"

namespace tenet {

	class Auth;

	class Configuration {
	public:

		class Debug {
		public:
			inline void enable(bool enabled, const std::string& path = "") { this->m_enabled = enabled; this->m_path = path; }
			inline const std::string& path() const { return this->m_path; }
			inline bool enabled() const { return this->m_enabled; }
		private:
			friend class tenet::Configuration;

			Debug() : m_enabled(false) {};

			bool m_enabled;
			std::string m_path;
		};

		class Extension {
		public:
			inline void add(const std::string& name) { this->m_variables.push_back(name); }
			inline void set(const std::list<std::string> names) { this->m_variables = names; }
			inline const std::list<std::string> get() const { return this->m_variables; }
		private:
			friend class tenet::Configuration;

			std::list<std::string> m_variables;
		};

		class Hardware {
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

			/// <summary>
			/// Get current hardware id.
			/// </summary>
			/// <returns></returns>
			const std::string& get() const { return this->m_value; };

			/// <summary>
			/// Set current hardware id from a custom value.
			/// </summary>
			/// <returns></returns>
			void set(const std::string& value);

			/// <summary>
			/// Set current hardware id from built in options.
			/// </summary>
			/// <returns></returns>
			void set(const Base& options = tenet::Configuration::Hardware::DEFAULT_OPTIONS);

			static const Base& DEFAULT_OPTIONS;
		private:
			friend class tenet::Configuration;

			Hardware() { this->set(); }

			std::string m_value;
		};

		class Endpoints {
		public:
			inline const std::string& base() const { return this->m_url; }

			inline std::string stream() const { return this->base() + tenet::Configuration::Endpoints::STREAM; }
			inline std::string auth() const { return this->base() + tenet::Configuration::Endpoints::AUTH; }

			inline void set(const std::string& url = DEFAULT_URL) { this->m_url = url; }
		private:
			friend class tenet::Configuration;

			Endpoints() : m_url(tenet::Configuration::Endpoints::DEFAULT_URL) {}

			std::string m_url;

			static const std::string& DEFAULT_URL;

			static const std::string& STREAM;
			static const std::string& AUTH;
		};

		static tenet::Configuration::Hardware& hardware() { return tenet::Configuration::get().m_hardware;}
		static tenet::Configuration::Debug& debug() { return tenet::Configuration::get().m_debug; }
		static tenet::Configuration::Extension& extension() { return tenet::Configuration::get().m_extension; }
		static tenet::Configuration::Endpoints& endpoints() { return tenet::Configuration::get().m_endpoints; }

		Configuration(tenet::Configuration& other) = delete;
		void operator=(const tenet::Configuration& configuration) = delete;

		protected:

			Configuration() : m_debug(), m_extension(), m_hardware(), m_endpoints() {}

		private:
			friend class tenet::Auth;

			static inline tenet::Configuration& get() {
				static tenet::Configuration m_configuration;

				return m_configuration;
			}

			tenet::Configuration::Hardware	m_hardware;
			tenet::Configuration::Debug		m_debug;
			tenet::Configuration::Extension	m_extension;
			tenet::Configuration::Endpoints	m_endpoints;
	};

	class Auth {
	public:

		/// <summary>
		/// Get the current Auth instance
		/// </summary>
		/// <returns></returns>
		static inline tenet::Auth& init(const std::string& code) {
			static tenet::Auth m_auth(code);

			return m_auth;
		}

		/// <summary>
		/// Get the current Auth context
		/// </summary>
		/// <returns></returns>
		inline const features::Authenticate* context() const { return this->m_context->data(); }

		/// <summary>
		/// Returns true if the client can be authenticated
		/// </summary>
		/// <returns></returns>
		const bool valid() const;

		/// <summary>
		/// Returns true if the client is authenticated
		/// </summary>
		/// <returns></returns>
		const bool authenticated() const;

		/// <summary>
		/// Send an authentication request to the API.
		/// </summary>
		/// <param name="key">The key used to license your software.</param>
		/// <param name="attempts">Number of attemps in case the auth failed.</param>
		const features::Response<features::Authenticate>& authenticate(const std::string& key, int attempts = tenet::Auth::DEFAULT_ATTEMPS);

		/// <summary>
		/// Stream something from your middlman server.
		/// </summary>
		const features::Response<features::Stream>& stream() const;

		~Auth();

		Auth(tenet::Auth& other) = delete;
		void operator=(const tenet::Auth& auth) = delete;

	protected:

		Auth(const std::string& code);
	
	private:

		static const int DEFAULT_ATTEMPS;

		const std::string m_code;
		const features::Response<features::Authenticate>* m_context;
	};
}

#endif