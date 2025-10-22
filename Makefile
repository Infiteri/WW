build_dir ?= $(abspath Bin)
obj_dir ?= $(abspath Bin-Obj)

.PHONY: scaffold windows_lib dev_lib 

windows_lib: scaffold
	@make -j8 -C CoreLib
	@make -j8 -C CoreLib assets
	@make -j8 -C WindowsLib
	@make -j8 -C WindowsLib link

dev_lib: scaffold
	@make -j8 -C CoreLib
	@make -j8 -C CoreLib assets
	@make -j8 -C DevLib
	@make -j8 -C DevLib link


scaffold:
	@mkdir -p $(build_dir)
	@mkdir -p $(obj_dir)