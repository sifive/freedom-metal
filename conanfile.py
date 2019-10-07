import os
from conans import ConanFile, AutoToolsBuildEnvironment, tools
from conans.errors import ConanInvalidConfiguration
from conans.tools import os_info, SystemPackageTool

class FreedomMetalConan(ConanFile):
    name = "freedom-metal"
    version = "v201905"
    settings = "os_build", "os", "arch_build", "arch"
    url = None # We might consider pointing on toolchain repo here
    description = """Freedom metal lib package"""
    license = "MIT"
    options = {"target_name": "ANY"}
    exports_sources=["src/*", "metal/*", "configure", "aclocal.m4",
    "depcomp", "ar-lib", "riscv__menv__metal.specs.in", "build.wake",
    "m4", "riscv__mmachine__hifive1-revb.lds", "compile", "dts", 
    "Makefile.am", "Makefile.in", "scripts", "configure",
    "configure.ac", "metal", "install-sh", "missing", "gloss/*"]

    def build(self):
        # ~ print("start build")
        if(self.settings.os_build != "Linux"):
            raise ConanInvalidConfiguration("unsupported os_build: %s" %
                                            (self.settings.os_build))
        
        # ~ self.run("""CFLAGS="$(RISCV_CFLAGS)" ./configure --host={CROSS_COMPILE} \
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
        # ~ self.run("make")

    def package(self):
        # Copy metal includes 
        # ~ self.copy("freedom-*", dst="", src="", excludes= "*.c++")
        pass

    def package_info(self):
        # ~ if(self.settings.os != "Linux"):
            # ~ raise ConanInvalidConfiguration("unsupported os: %s" %
                                            # ~ (self.settings.os))
        pass
                                            
                                            
    def requirements(self):
        if ( self.settings.arch in ("riscv32", "riscv64") ) :
            self.requires("riscv-toolchain/8.2.0@sifive/stable")
            
        self.requires("freedom-bsp/v201905@sifive/stable")
            
        
