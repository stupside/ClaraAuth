#ifndef MODULES_H
#define MODULES_H

#include <map>
#include <string>

class Api;

namespace features {

	namespace models {

		template<class T>
		class Entity {
		private:
			friend class Api;

			const T id;
		protected:
			Entity(const T id) : id(id) {}
		public:
			inline const T getId() const { return id; }
		};

		class User : public Entity<std::string> {
		public:
			User(const std::string& id, const std::string& name) : Entity(id), m_name(name) {}
			inline const std::string& name() const { return this->m_name; }
		private:
			const std::string	m_name;
			friend class Api;
		};

		class Product : public Entity<int> {
		public:

			enum class Status {
				Unedetected = 0,
				Uncertain = 1,
				Detected = 2,
				Maintenance = 3
			};

			Product(
				const int			id,
				const Status		status,
				const std::string& name,
				const std::string& download,
				const std::string& description,
				const std::string& version,
				const std::string& image
			) : Entity(id), m_status(status), m_name(name), m_download(download), m_description(description), m_version(version), m_image(image) {}

			inline const Status& status() const { return this->m_status; }
			inline const std::string& name() const { return this->m_name; }
			inline const std::string& download() const { return this->m_download; }
			inline const std::string& description() const { return this->m_description; }
			inline const std::string& version() const { return this->m_version; }
			inline const std::string& image() const { return this->m_image; }

		private:

			const Status		m_status;
			const std::string	m_name;
			const std::string	m_download;
			const std::string	m_description;
			const std::string	m_version;
			const std::string	m_image;

			friend class Api;
		};

		class Package : public Entity<int>
		{
		public:

			enum class Type {
				Hour = 0,
				Day = 1,
				Month = 2,
				Year = 3,
				Lifetime = 4
			};

			Package(
				const int			id,
				const std::string& name,
				const std::string& description,
				const int			duration,
				const Type			type,
				const float			price
			) : Entity(id), m_name(name), m_description(description), m_duration(duration), m_type(type), m_price(price) {}

			inline const std::string& name() const { return this->m_name; }
			inline const std::string& description() const { return this->m_description; }
			inline const int duration() const { return this->m_duration; }
			inline const Type& type() const { return this->m_type; }
			inline const float price() const { return this->m_price; }

		protected:
		private:

			const std::string	m_name;
			const std::string	m_description;
			const int			m_duration;
			const Type			m_type;
			const float			m_price;

			friend class Api;
		};

		class License : public Entity<int>
		{
		public:
			class Extension {
				friend class License;
			public:

				Extension(
					const std::string	customer,
					const std::string	notes,
					const bool			claimed
				) : m_customer(customer), m_notes(notes), m_claimed(claimed) {}

				inline const std::string& customer() const { return this->m_customer; }
				inline const std::string& notes() const { return this->m_notes; }
				inline const bool claimed() const { return this->m_claimed; }

			private:

				const std::string	m_customer;
				const std::string	m_notes;
				const bool			m_claimed;

				friend class Api;
			};

		public:

			enum class Status {
				Active = 0,
				Inactive = 1,
				Paused = 2,
				Expired = 3
			};

			License(
				const int			id,
				const std::string& key,
				const std::string& hwid,
				const Status		status,
				const std::string& creation,
				const std::string& activation,
				const std::string& expiration,
				const Extension		extension,
				const Product		product,
				const Package		package,
				const User			owner,
				const User			creator
			) : Entity(id), m_key(key), m_hwid(hwid), m_status(status), m_creation(creation), m_activation(activation), m_expiration(expiration), m_product(product), m_package(package), m_owner(owner), m_creator(creator), m_extension(extension) {}

			inline const Product& product() const { return this->m_product; }
			inline const Package& package() const { return this->m_package; }
			inline const User& owned() const { return this->m_owner; }
			inline const User& creator() const { return this->m_creator; }
			inline const Extension& extension() const { return this->m_extension; }

			inline const std::string& key() const { return this->m_key; }
			inline const std::string& hardware() const { return this->m_hwid; }
			inline const Status status() const { return this->m_status; }
			inline const std::string& creation() const { return this->m_creation; }
			inline const std::string& activation() const { return this->m_activation; }
			inline const std::string& expiration() const { return this->m_expiration; }

		private:
			const Product		m_product;
			const Package		m_package;
			const User			m_owner;
			const User			m_creator;
			const Extension		m_extension;

			const std::string	m_key;
			const std::string	m_hwid;
			const Status		m_status;
			const std::string	m_creation;
			const std::string	m_activation;
			const std::string	m_expiration;

			friend class Api;
		};
	};

	template<class T>
	class Response {
	private:
		const bool			m_succeed;
		const std::string	m_message;
		const T* m_data;
	public:

		Response() : m_succeed(false), m_data(nullptr) {}
		Response(const std::string& message) : m_succeed(false), m_message(message), m_data(nullptr) {}
		Response(bool succeed, const std::string& message) : m_succeed(succeed), m_message(message), m_data(nullptr) {}

		Response(const T* data) : m_data(data), m_succeed(true) {}
		inline const T* data() const { return this->m_data; }

		inline const std::string& message() const { return this->m_message; }
		inline bool succeed() const { return this->m_succeed; }

		~Response() {
			if (this->m_data)
				delete this->m_data;
		}
	};



	class Stream {
	public:
		const std::string& descrypte(const std::string& secret) const;
		bool valid() const;

		inline const std::string& iv() const { return this->m_iv; }
		inline const std::string& hash() const { return this->m_hash; }
		inline const std::string& encrypted() const { return this->m_encrypted; }

		explicit Stream(const std::string& encrypted, const std::string& iv, const std::string& hash) : m_encrypted(encrypted), m_iv(iv), m_hash(hash) {}

	private:
		friend class Api;

		const std::string m_encrypted;
		const std::string m_iv;
		const std::string m_hash;
	};

	class Authenticate {
	public:

		const bool canStream() const;

		inline const std::map<std::string, std::string>& variable() const { return this->m_variables; }
		inline const models::License& license() const { return this->m_license; }

		explicit Authenticate(const models::License& license, const std::map<std::string, std::string>& variables, const std::string& ist) : m_license(license), m_variables() {}

	private:
		friend class Api;

		inline void addVariable(const std::string& name, const std::string& value) { this->m_variables.emplace(name, value); }

		std::map<std::string, std::string> m_variables;
		const models::License m_license;

		const std::string m_ist;
	};
}

#endif