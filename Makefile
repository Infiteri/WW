build_dir := $(abspath Bin)
obj_dir   := $(abspath Bin-Obj)

.PHONY: all scaffold windows_lib dev_lib clean corelib windowslib devlib

all: windows_lib dev_lib

scaffold:
	@mkdir -p $(build_dir)
	@mkdir -p $(obj_dir)

corelib:
	@make -j8 -C CoreLib all assets

windowslib:
	@make -j8 -C WindowsLib all link

devlib:
	@make -j8 -C DevLib all link

windows_lib: scaffold corelib windowslib

dev_lib: scaffold corelib devlib

clean:
	@rm -rf $(obj_dir)
