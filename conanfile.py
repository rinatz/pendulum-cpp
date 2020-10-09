from conans import ConanFile


class PendulumConan(ConanFile):
    name = "pendulum"
    version = "0.8.0"
    license = "MIT"
    author = "IDA Kenichiro"
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
        self.copy("*.h", src="include", dst="include")
        self.copy("*.a", src="lib", dst="lib")

    def package(self):
        self.copy("*.h", src="pendulum", dst="include/pendulum")
        self.copy("*.hpp", src="pendulum", dst="include/pendulum")
