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

    # If using OpenSSL, we need it to be active in libcurl too
    def configure(self):
        self.options["libcurl"].with_ssl = "openssl"
        if self.settings.compiler == "Visual Studio": del self.settings.compiler.runtime

#Alternative 2: if you want to keep MD-MDd/MT-MTd configuration
    def package_id(self):
        if self.settings.compiler == "Visual Studio":
            if "MD" in self.settings.compiler.runtime:
                self.info.settings.compiler.runtime = "MD/MDd"
            else:
                self.info.settings.compiler.runtime = "MT/MTd"

    def build(self):
        cmake = CMake(self)
        if cmake.is_multi_configuration:
            # Alternative 2:
            cmd_args = cmake.command_line.replace("CONAN_LINK_RUNTIME", "CONAN_LINK_RUNTIME_MULTI")
            cmd = 'cmake "%s/hello" %s' % (self.source_folder, cmd_args)
            self.run(cmd)
            self.run("cmake --build . --config Debug")
            self.run("cmake --build . --config Release")
        else:
            for config in ("Debug", "Release"):
                self.output.info("Building %s" % config)
                self.run('cmake "%s/hello" %s -DCMAKE_BUILD_TYPE=%s'
                        % (self.source_folder, cmake.command_line, config))
                self.run("cmake --build .")
                shutil.rmtree("CMakeFiles")
                os.remove("CMakeCache.txt")