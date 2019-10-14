import os
import glob
from conans import ConanFile, AutoToolsBuildEnvironment, tools
from conans.errors import ConanInvalidConfiguration
from conans.tools import os_info, SystemPackageTool

class FreedomMetalConan(ConanFile):
    name = "freedom-metal"
    version = "v201905"
    settings = "os_build", "os", "arch_build", "arch", "build_type"
    url = None # We might consider pointing on toolchain repo here
    description = """Freedom metal lib package"""
    license = "MIT"
    options = {"target_name": "ANY"}
    exports_sources=["src/*", "metal/*", "configure", "aclocal.m4",
    "depcomp", "ar-lib", "riscv__menv__metal.specs.in", "build.wake",
    "m4", "riscv__mmachine__hifive1-revb.lds", "compile", "dts", 
    "Makefile.am", "Makefile.in", "scripts", "configure",
    "configure.ac", "metal", "install-sh", "missing", "gloss/*"]
    generators = 'TXTGenerator'
        
    def _env_info_cleaner(self):
        risc_v_specific_flags = ["RISCV_ASFLAGS", "RISCV_CCASFLAGS", "RISCV_CFLAGS", "RISCV_CXXFLAGS", "RISCV_XCFLAGS", "RISCV_LDFLAGS", "RISCV_LDLIBS"]
        append_with_spaces = ["ASFLAGS", "CCASFLAGS", "XCFLAGS", "CPPFLAGS", "CFLAGS", "CXXFLAGS", "LIBS", "LDFLAGS", "LDLIBS", "CL", "_LINK_"] + risc_v_specific_flags
        cleaned_env = self.env
        
        keys_to_clean = set(append_with_spaces)
        keys_to_clean.intersection_update(set(cleaned_env.keys()))
       
        print(keys_to_clean)
        for key in keys_to_clean :
            if isinstance(cleaned_env[key], list):
                # we use string here because we want to overwrite not append
                cleaned_env[key] = " ".join(cleaned_env[key])
        return cleaned_env

    def build(self):
        if(self.settings.os_build != "Linux"):
            raise ConanInvalidConfiguration("unsupported os_build: %s" %
                                            (self.settings.os_build))
        
        with tools.environment_append(self._env_info_cleaner()):
            
            self.run("""./configure --host={CROSS_COMPILE} \
                --prefix={METAL_PREFIX} \
                --libdir={METAL_LIB_DIR} \
                --disable-maintainer-mode \
                --with-preconfigured \
                --with-machine-name={TARGET} \
                --with-machine-header={METAL_HEADER} \
                --with-machine-inline={METAL_INLINE} \
                --with-platform-header={PLATFORM_HEADER} \
                --with-machine-ldscript={METAL_LDSCRIPT} \
                --with-builtin-libgloss""".format(
                    CROSS_COMPILE = 'riscv64-unknown-elf', 
                    METAL_PREFIX = self.build_folder, 
                    METAL_LIB_DIR = os.path.join(self.build_folder, 'lib'), 
                    TARGET = self.options.target_name, 
                    METAL_HEADER= self.deps_user_info["freedom-bsp"].METAL_HEADER, 
                    METAL_INLINE = self.deps_user_info["freedom-bsp"].METAL_INLINE, 
                    PLATFORM_HEADER = self.deps_user_info["freedom-bsp"].PLATFORM_HEADER, 
                    METAL_LDSCRIPT = self.deps_user_info["freedom-bsp"].METAL_LDSCRIPT))
                 
            self.run("touch -c ./Makefile")
            self.run("make -C . install")

        os.rename(glob.glob("libriscv__mmachine_*.a")[0], "libmetal.a")
        os.rename("libriscv__menv__metal.a", "libmetal-gloss.a")

    def package(self):
        # Copy metal includes 
        self.copy("*.h", dst="include", src="include/metal")
        # copy lib
        self.copy("libmetal.a", dst="lib", src="")
        self.copy("libmetal-gloss.a", dst="lib", src="")
        if self.settings.build_type == "Debug":
            self.copy("*.c", dst="gloss", src="gloss")
            self.copy("*.c", dst="src/", src="src")
            self.copy("*.S", dst="src/", src="src")

    def package_info(self):
        if ( self.settings.arch not in ("riscv32", "riscv64") ) :
            raise ConanInvalidConfiguration("unsupported arch: %s" %
                                            (self.settings.arch))
                                            
        self.env_info.LDFLAGS += ["-L{lib_path}".format(lib_path = os.path.join(self.package_folder, "lib"))]
        self.env_info.LDLIBS += [ "-Wl,--start-group -lc -lgcc -lmetal -lmetal-gloss -Wl,--end-group"]
        
        includes_dirs = [ "-I{inc_dir}".format(inc_dir=os.path.join(self.package_folder, include_dir)) for include_dir in ['include', 'include/machine', 'include/drivers'] ]
        self.env_info.CCASFLAGS += includes_dirs
        self.env_info.CFLAGS += includes_dirs
        self.env_info.CXXFLAGS += includes_dirs
        
        self.cpp_info.includedirs = ['include', 'include/machine', 'include/drivers']  # Ordered list of include paths
        self.cpp_info.libs = ['metal', 'metal-gloss']  # The libs to link against
        self.cpp_info.libdirs = ['lib']  # Directories where libraries can be found
        
        if self.settings.build_type == "Debug":
            self.cpp_info.srcdirs = ['gloss', 'src', 'src/drivers']  # Directories where sources can be found (debugging, reusing sources)
                                            
    def requirements(self):
        if ( self.settings.arch in ("riscv32", "riscv64") ) :
            self.requires("riscv-toolchain/8.2.0@sifive/stable")
            
        self.requires("freedom-bsp/v201905@sifive/stable")
        
        # custom generator
        self.requires("TextGenSiFive/0.0.1@sifive/stable")
        self.requires("VirtualenvGenSiFive/0.0.1@sifive/stable")
