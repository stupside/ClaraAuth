from conans import ConanFile, CMake

class TenetAuth(ConanFile):
    name = "ClaraAuth"
    version = "0.1"
    license = "MIT"
    url = "github/xonery"

    build_requires = "jwt-cpp/0.4.0", "cpr/1.5.0", "nlohmann_json/3.8.0", "cryptopp/8.2.0@bincrafters/stable", "cppcodec/0.2"
    requires = "jwt-cpp/0.4.0", "cpr/1.5.0", "nlohmann_json/3.8.0", "cryptopp/8.2.0@bincrafters/stable", "cppcodec/0.2"

    generators = "cmake", "gcc", "txt", "cmake_find_package"
    settings = "os", "compiler", "build_type", "arch"

    options = {"shared": [True, False], "st": [True, False]}
    default_options = {"shared": False, "st": False}

    exports = "*"

    # If using OpenSSL, we need it to be active in libcurl too
    def configure(self):
        self.options["libcurl"].with_openssl = True