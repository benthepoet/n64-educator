# Shared fragment for n64-educator lessons.
# Include after setting ROMNAME and (optionally) ROM_TITLE.
#
# Optional flags in the lesson Makefile:
#   USE_NG_MATH := 1   — link common/src/ng_math.c
#   USE_T3D     := 1   — Tiny3D headers + libt3d
#
# Optional assets:
#   assets/*.png  → filesystem/*.sprite
#   assets/*.glb  → filesystem/*.t3dm  (requires USE_T3D / gltf_to_t3d)

ifeq ($(N64_INST),)
  $(error N64_INST is not set. See docs/guide/setup.md)
endif

ifeq ($(wildcard $(N64_INST)/include/n64.mk),)
  $(error $(N64_INST)/include/n64.mk not found. Is N64_INST correct?)
endif

BUILD_DIR ?= build
SOURCE_DIR ?= .

include $(N64_INST)/include/n64.mk

# Tiny3D only when requested (do not auto-link just because T3D_INST is in the env).
ifeq ($(USE_T3D),1)
  ifneq ($(wildcard $(N64_INST)/include/t3d.mk),)
    include $(N64_INST)/include/t3d.mk
  else ifneq ($(T3D_INST),)
    include $(T3D_INST)/t3d.mk
  else
    $(error USE_T3D=1 but Tiny3D not found. Install Tiny3D or set T3D_INST.)
  endif
  # Examples use newer C; keep portable.
  N64_CFLAGS += -std=gnu17
endif

NG_ROOT ?= $(abspath ../..)
N64_CFLAGS += -I$(NG_ROOT)/common/include
N64_CXXFLAGS += -I$(NG_ROOT)/common/include

C_FILES := $(shell find src -name '*.c' 2>/dev/null)
OBJS := $(addprefix $(BUILD_DIR)/,$(C_FILES:.c=.o))

ifeq ($(USE_NG_MATH),1)
  OBJS += $(BUILD_DIR)/ng_math.o
endif

ASSETS_PNG := $(wildcard assets/*.png)
ASSETS_GLB := $(wildcard assets/*.glb)
ASSETS_T3DM_PRE := $(wildcard assets/*.t3dm)
ASSETS_SDATA := $(wildcard assets/*.sdata)
ASSETS_WAV := $(wildcard assets/*.wav)
ASSETS_XM := $(wildcard assets/*.xm)
ASSETS_SPRITES := $(addprefix filesystem/,$(notdir $(ASSETS_PNG:%.png=%.sprite)))
# Prefer prebuilt .t3dm in assets/; else convert from .glb
ASSETS_T3DM_FROM_GLB := $(addprefix filesystem/,$(notdir $(ASSETS_GLB:%.glb=%.t3dm)))
ASSETS_T3DM_FROM_PRE := $(addprefix filesystem/,$(notdir $(ASSETS_T3DM_PRE)))
ASSETS_T3DM := $(ASSETS_T3DM_FROM_PRE) $(filter-out $(ASSETS_T3DM_FROM_PRE),$(ASSETS_T3DM_FROM_GLB))
ASSETS_SDATA_OUT := $(addprefix filesystem/,$(notdir $(ASSETS_SDATA)))
ASSETS_WAV64 := $(addprefix filesystem/,$(notdir $(ASSETS_WAV:%.wav=%.wav64)))
ASSETS_XM64 := $(addprefix filesystem/,$(notdir $(ASSETS_XM:%.xm=%.xm64)))
# strip: empty wildcards must not become a " " that looks non-empty to ifneq
DFS_INPUTS := $(strip $(ASSETS_SPRITES) $(ASSETS_T3DM) $(ASSETS_SDATA_OUT) $(ASSETS_WAV64) $(ASSETS_XM64))
MKSPRITE_FLAGS ?=
# Optional inject step for vanilla Blender exports (no Fast64 addon)
GLTF_INJECT ?= $(NG_ROOT)/scripts/gltf_inject_f3d.py

.DEFAULT_GOAL := all
all: $(ROMNAME).z64
.PHONY: all

$(BUILD_DIR)/$(ROMNAME).elf: $(OBJS)

$(ROMNAME).z64: N64_ROM_TITLE = $(or $(ROM_TITLE),"$(ROMNAME)")

ifneq ($(ASSETS_PNG),)
filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) $(MKSPRITE_FLAGS) -o filesystem "$<"
endif

ifneq ($(ASSETS_T3DM_PRE),)
filesystem/%.t3dm: assets/%.t3dm
	@mkdir -p $(dir $@)
	@echo "    [T3D-COPY] $@"
	@cp "$<" $@
endif

ifneq ($(ASSETS_SDATA),)
filesystem/%.sdata: assets/%.sdata
	@mkdir -p $(dir $@)
	@echo "    [SDATA] $@"
	@cp "$<" $@
endif

ifneq ($(ASSETS_GLB),)
filesystem/%.t3dm: assets/%.glb
	@mkdir -p $(dir $@) $(BUILD_DIR)
	@echo "    [T3D-MODEL] $@"
	@python3 $(GLTF_INJECT) "$<" "$(BUILD_DIR)/$(basename $(notdir $<))_f3d.glb"
	@$(T3D_GLTF_TO_3D) "$(BUILD_DIR)/$(basename $(notdir $<))_f3d.glb" $@
	@$(N64_BINDIR)/mkasset -c 2 -o filesystem $@
endif

ifneq ($(ASSETS_WAV),)
filesystem/%.wav64: assets/%.wav
	@mkdir -p $(dir $@)
	@echo "    [AUDIO] $@"
	@$(N64_AUDIOCONV) --wav-compress 3 -o filesystem "$<"
endif

ifneq ($(ASSETS_XM),)
filesystem/%.xm64: assets/%.xm
	@mkdir -p $(dir $@)
	@echo "    [XM] $@"
	@$(N64_AUDIOCONV) -o filesystem "$<"
endif

ifneq ($(DFS_INPUTS),)
$(BUILD_DIR)/$(ROMNAME).dfs: $(DFS_INPUTS)
$(ROMNAME).z64: $(BUILD_DIR)/$(ROMNAME).dfs
endif

ifeq ($(USE_NG_MATH),1)
$(BUILD_DIR)/ng_math.o: $(NG_ROOT)/common/src/ng_math.c
	@mkdir -p $(dir $@)
	@echo "    [CC] $<"
	$(N64_CC) -c $(N64_CFLAGS) -I$(NG_ROOT)/common/include -o $@ $<
endif

clean:
	$(RM) -r $(BUILD_DIR) $(ROMNAME).z64 filesystem
.PHONY: clean

ifneq ($(wildcard $(BUILD_DIR)),)
  -include $(shell find $(BUILD_DIR) -name '*.d' 2>/dev/null)
endif
