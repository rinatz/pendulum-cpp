from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.files import load


class PendulumConan(ConanFile):
    name = "pendulum"

    license = "MIT"
    author = "IDA Kenichiro"
    url = "https://github.com/rinatz/pendulum-cpp"
    description = "Pendulum C++ is a simple wrapper around cctz inspired by Pendulum that is beautiful Python library."
    topics = ("datetime",)

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    exports_sources = (
        "pendulum/*",
        "test/*",
        "CMakeLists.txt",
        "version.h.in",
        "VERSION",
    )

    def set_version(self):
        self.version = load(self, "VERSION").strip()

    def requirements(self):
        self.requires("cctz/2.3")  # type: ignore[reportOptionalCall]
        self.requires("gtest/1.10.0", test=True)  # type: ignore[reportOptionalCall]

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
