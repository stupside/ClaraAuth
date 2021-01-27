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
			Entity(T id) : id(id) {};
		public:
			T get_id() { return id; };
		private:
		};

		class User : public Entity<std::string> {
		public:

			const std::string	name;

			User(
				const std::string id,
				const std::string name
			) : Entity(id), name(name) {};
		protected:
		private:
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

			const Status		status;
			const std::string	name;
			const std::string	download;
			const std::string	description;
			const std::string	version;
			const std::string	image;

			Product(
				const int			id,
				const Status		status, 
				const std::string	name, 
				const std::string	download, 
				const std::string	description, 
				const std::string	version, 
				const std::string	image
			) : Entity(id), status(status), name(name), download(download), description(description), version(version), image(image) {};
		protected:
		private:
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

			const std::string	name;
			const std::string	description;
			const int			duration;
			const Type			type;
			const float			price;

			Package(
				const int			id,
				const std::string	name,
				const std::string	description,
				const int			duration,
				const Type			type,
				const float			price
			) : Entity(id), name(name), description(description), duration(duration), type(type), price(price) {};
		protected:
		private:
			friend class Api;
		};

		class License : public Entity<int>
		{
		public:
			class Extension {
				friend class License;
			public:

				const std::string	customer;
				const std::string	notes;
				const bool			claimed;

				Extension(
					const std::string	customer,
					const std::string	notes,
					const bool			claimed
				) : customer(customer), notes(notes), claimed(claimed) {};
			protected:
			private:
				friend class Api;
			};

		public:
			enum class Status {
				Active = 0,
				Inactive = 1,
				Paused = 2,
				Expired = 3
			};

			const Product		product;
			const Package		package;
			const User			owner;
			const User			creator;
			const Extension		extension;

			const std::string	key;
			const std::string	hwid;
			const Status		status;
			const std::string	creation;
			const std::string	activation;
			const std::string	expiration;

			License(
				const int			id,
				const std::string	key,
				const std::string	hwid,
				const Status		status,
				const std::string	creation,
				const std::string	activation,
				const std::string	expiration,
				const Extension		extension,
				const Product		product,
				const Package		package,
				const User			owner,
				const User			creator
			) : Entity(id), key(key), hwid(hwid), status(status), creation(creation), activation(activation), expiration(expiration), product(product), package(package), owner(owner), creator(creator), extension(extension) {};
		protected:
		private:
			friend class Api;
		};
	}

	class Response {
	private:
		const bool			_succeed;
		const std::string	_message;
	protected:
		explicit Response() : _succeed(false) {};
		explicit Response(const std::string message) : _succeed(false), _message(message) {};
		explicit Response(bool succeed, const std::string message) : _succeed(succeed), _message(message) {};
	public:
		bool succeed();
		std::string message();
	};

	class Stream : public Response {
	public:
		std::string decrypt(std::string secret);
		bool valid();
	protected:
		Stream(std::string message) : Response(false, message) {};
		Stream(bool succeed, std::string message) : Response(succeed, message) {};
		Stream(std::string encrypted, std::string iv, std::string hash)
			: Response(true, "Stream succeed"), encrypted(encrypted), iv(iv), hash(hash) {};
	private:
		std::string encrypted;
		std::string iv;
		std::string hash;

		friend class Api;
	};

	class Authenticate : public Response {
		typedef std::map<std::string, std::string> Variables;
	public:

		Variables*			variables;
		models::License*	license;

		const bool can_stream();
		const bool authenticated();

		virtual ~Authenticate() {

		}
	protected:
		Authenticate(std::string message) : Response(false, message), license(nullptr), variables(nullptr) {};
		Authenticate(bool succeed, std::string message) : Response(succeed, message), license(nullptr), variables(nullptr) {};
		Authenticate(models::License* license) 
			: Response(true, "Authentication succeed"), license(license), variables(nullptr) {};

		
	private:
		std::string ist;
		friend class Api;
	};
}

#endif