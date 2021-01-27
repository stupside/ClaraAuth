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
		};

		class License : public Entity<int>
		{
		public:
			class Extension {
			public:

				const std::string	customer;
				const std::string	notes;
				const bool			claimed;

				Extension(
					const std::string	customer,
					const std::string	notes,
					const bool			claimed
				) : customer(customer), notes(notes), claimed(claimed) {};
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
		};
	}

	class Response {
	private:
		const bool			_succeed;
		const std::string	_message;
	public:
		const bool succeed() { return _succeed; };
		const std::string message() { return _message; };
	};

	class Stream : public Response {
	public:
		std::string decrypt(std::string secret);
		bool valid();
	};

	class Authenticate : public Response {
		typedef std::map<std::string, std::string> Variables;
	public:

		Variables* variables;
		models::License* license;

		const bool can_stream();
		const bool authenticated();

		virtual ~Authenticate() { delete variables; delete license; }
	};
}

#endif