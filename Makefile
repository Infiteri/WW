build_dir ?= $(abspath Bin)
obj_dir ?= $(abspath Bin-Obj)

.PHONY: scaffold windows_lib

windows_lib:
	@make -j8 -C CoreLib
	@make -j8 -C CoreLib assets
	@make -j8 -C WindowsLib
	@make -j8 -C WindowsLib link

scaffold:
	@mkdir -p $(build_dir)
	@mkdir -p $(obj_dir)