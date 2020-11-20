#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <map>

namespace tenet {
    class PackageResponse {
    public:
        PackageResponse() : id(0), name(""), description(""), duration(0), type(0), price(0), minQuantity(0) {}
        PackageResponse(const int id, const std::string name, const std::string description, const int duration, const int type, const int price, const int minQuantity)
            : id(id), name(name), description(description), duration(duration), type(type), price(price), minQuantity(minQuantity)
        {}
    public:
        const int id;
        const std::string name;
        const std::string description;
        const int duration;
        const int type;
        const int price;
        const int minQuantity;
    };

    class ProductResponse {
    public:
        ProductResponse() : id(0), status(0), name(""), download(""), description(""), version(""), image("") {}
        ProductResponse(const int id, const int status, const std::string name, const std::string download, const std::string description, const std::string version, const std::string image)
            : id(id), status(status), name(name), download(download), description(description), version(version), image(image)
        {}
    public:
        const int id;
        const int status;
        const std::string name;
        const std::string download;
        const std::string description;
        const std::string version;
        const std::string image;
    };

    class ApplicationUserResponse {
    public:
        ApplicationUserResponse() : id(""), userName("") {}
        ApplicationUserResponse(const std::string& id, const std::string& userName)
            : id(id), userName(userName)
        {}
    public:
        const std::string id;
        const std::string userName;
    };

    class LicenseKeyExtensionResponse {
    public:
        LicenseKeyExtensionResponse() : customer(""), notes("") {}
        LicenseKeyExtensionResponse(const std::string customer, const std::string notes)
            : customer(customer), notes(notes) {}
    public:
        const std::string customer;
        const std::string notes;
    };

    class LicenseKeyResponse {
    public:
        LicenseKeyResponse() : id(0), key("default"), hwid(""), generation(""), status(0), creation(""), activation(""), expiration(""),
            extension(tenet::LicenseKeyExtensionResponse()), package(tenet::PackageResponse()), product(tenet::ProductResponse()), owner(tenet::ApplicationUserResponse()), creator(tenet::ApplicationUserResponse())
        {}

        LicenseKeyResponse(const int id,
            const std::string key,
            const std::string hwid,
            const std::string generation,
            const int status,
            const std::string creation,
            const std::string activation,
            const std::string expiration,
            const tenet::LicenseKeyExtensionResponse extension,
            const PackageResponse package, const ProductResponse product, const ApplicationUserResponse owner, const ApplicationUserResponse creator)
            : id(id), key(key), hwid(hwid), generation(generation), status(status), creation(creation), activation(activation), expiration(expiration),
            extension(extension), package(package), product(product), owner(owner), creator(creator)
        {}
    public:
        const int id;
        const std::string key;
        const std::string hwid;
        const std::string generation;
        const int status;
        const std::string creation;
        const std::string activation;
        const std::string expiration;

        const LicenseKeyExtensionResponse extension;

        const PackageResponse package;
        const ProductResponse product;
        const ApplicationUserResponse owner;
        const ApplicationUserResponse creator;
    };

    class Auth;
    class Response {
        friend tenet::Auth;
    private:
        const bool succeed;
        const std::string message;

        std::map<std::string, std::string> variables;
    protected:
        Response(const std::string message) : succeed(false), message(message), licenseKey(nullptr), variables() {};
        Response(const bool succeed, const std::string message) : succeed(succeed), message(message), licenseKey(nullptr), variables() {};
    public:
        Response() = default;

        const bool Succeed();
        const std::string Message();

        const std::map<std::string, std::string> Variables();

        const tenet::LicenseKeyResponse* licenseKey;

        ~Response() { delete licenseKey; }
    };
}

#endif