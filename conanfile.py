from conans import ConanFile


class PendulumConan(ConanFile):
    name = "pendulum"
    version = "0.1.0"
    license = "MIT"
    author = "Ida Kenichiro"
    url = "https://github.com/rinatz/pendulum-cpp"
    description = "Datetime library for C++."
    topics = ("datetime")
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    requires = (
        "cctz/2.3",
    )

    build_requires = (
        "gtest/1.10.0",
    )

    def imports(self):
        self.copy("*.h", dst="libs/include", src="include")
        self.copy("*.a", dst="libs/lib", src="lib")

    def package(self):
        self.copy("*.h", dst="include/pendulum", src="pendulum")
        self.copy("*.hpp", dst="include/pendulum", src="pendulum")
