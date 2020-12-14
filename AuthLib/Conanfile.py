from conans import ConanFile, CMake

#conan install . --install-folder .\conan\

class TenetAuth(ConanFile):
    name = "ClaraAuth"
    version = "0.1"
    license = "MIT"
    url = "github/xonery"
     
    build_requires = "nlohmann_json/3.9.1", "jwt-cpp/0.4.0", "cpr/1.5.0", "cryptopp/8.2.0@bincrafters/stable", "cppcodec/0.2", "libcurl/7.73.0", "openssl/1.1.1g"
    requires = "nlohmann_json/3.9.1", "jwt-cpp/0.4.0", "cpr/1.5.0", "cryptopp/8.2.0@bincrafters/stable", "cppcodec/0.2", "libcurl/7.73.0", "openssl/1.1.1g",

    generators = "cmake", "gcc", "txt", "cmake_find_package"
    settings = {"os", "compiler", "build_type", "arch"}

    options = {"shared": [True, False], "st": [True, False]}
    default_options = {"shared": False, "st": False}

    exports = "*"

    def configure(self):
        self.options["libcurl"].with_ssl = "openssl"
        if self.settings.compiler == "Visual Studio":
            del self.settings.compiler.runtime

    def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()