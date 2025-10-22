build_dir ?= ../Bin
obj_dir ?= ../Bin-Obj
src_dir ?= Source
current_dir := $(notdir $(CURDIR))

rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))
CPP_FILES := $(call rwildcard,$(src_dir)/,*.cpp)
OBJ_FILES := $(patsubst $(src_dir)/%.cpp,$(obj_dir)/$(current_dir)/$(src_dir)/%.o,$(CPP_FILES))
OBJ_DIRS := $(sort $(dir $(OBJ_FILES)))
CORE_LIB_DIR := ../Bin-Obj/CoreLib
CORE_LIB_OBJS := $(call rwildcard,$(obj_dir)/CoreLib/Source,*.o)

.PHONY: all scaffold link clean vendor

all: scaffold $(OBJ_FILES) vendor

vendor:
	@cp Vendor/GLFW/lib/glfw3.dll $(build_dir)/glfw3.dll
	@cp Vendor/ImGui/Imgui.dll $(build_dir)/Imgui.dll

scaffold:
	@mkdir -p $(build_dir)
	@mkdir -p $(build_dir)/ImGui
	@mkdir -p $(OBJ_DIRS)

$(obj_dir)/$(current_dir)/$(src_dir)/%.o: $(src_dir)/%.cpp
	@mkdir -p $(dir $@)
	g++ -c -g $< -o $@  -DUNICODE -D_UNICODE  -I../CoreLib/Source -I$(src_dir) -IVendor/ImGui -IVendor/GLFW/include -I../CoreLib/Vendor/glad/include

link:
	@g++ -o $(build_dir)/DevApp.exe $(OBJ_FILES) $(CORE_LIB_OBJS) -lgdi32 -lopengl32 -luser32 -lglfw3 -lImGui -L$(build_dir) 

clean:
	@rm -rf $(obj_dir)/$(current_dir)/$(src_dir)/*.o $(build_dir)/DevApp.exe
