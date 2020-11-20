from conans import ConanFile, CMake

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

    # Alternative 2: if you want to keep MD-MDd/MT-MTd configuration
    def package_id(self):
        if self.settings.compiler == "Visual Studio":
            if "MD" in self.settings.compiler.runtime:
                self.info.settings.compiler.runtime = "MD/MDd"
            else:
                self.info.settings.compiler.runtime = "MT/MTd"

    def build(self):
        # Alternative 1: Use always default runtime (MD/MDd)
        cmake_release = CMake(self, build_type="Debug")
        cmake_release.defintions["CONAN_LINK_RUNTIME_MULTI"] = cmake_release.definitions["CONAN_LINK_RUNTIME"]
        cmake_release.definitions["CONAN_LINK_RUNTIME"] = False
        cmake_release.configure()
        cmake_release.build()

        cmake_debug = CMake(self, build_type="Release")
        cmake_debug.defintions["CONAN_LINK_RUNTIME_MULTI"] = cmake_release.definitions["CONAN_LINK_RUNTIME"]
        cmake_debug.definitions["CONAN_LINK_RUNTIME"] = False
        cmake_debug.configure()
        cmake_debug.build()