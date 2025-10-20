build_dir ?= ../Bin
obj_dir ?= ../Bin-Obj
src_dir ?= Source
current_dir := $(notdir $(CURDIR))

rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
CPP_FILES := $(call rwildcard,$(src_dir)/,*.cpp)
OBJ_FILES := $(patsubst $(src_dir)/%.cpp,$(obj_dir)/$(current_dir)/$(src_dir)/%.o,$(CPP_FILES))
OBJ_DIRS := $(sort $(dir $(OBJ_FILES)))

.PHONY: all scaffold

all: scaffold $(OBJ_FILES)

scaffold:
	@mkdir -p $(build_dir)
	@mkdir -p $(OBJ_DIRS)

$(obj_dir)/$(current_dir)/$(src_dir)/%.o: $(src_dir)/%.cpp
	@mkdir -p $(dir $@)
	g++ -c -g $< -o $@ -IVendor/glad/include -ISource
