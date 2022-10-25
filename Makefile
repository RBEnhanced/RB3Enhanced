# RB3Enhanced Makefile

# the source code files to compile from
SRC_DIR := source
SOURCES := $(wildcard $(SRC_DIR)/*.c)
INC_DIR := include

# output directory for final DLL/MOD files
OUTPUT := out
# output filename for final DLL/MOD files
OUTNAME := RB3Enhanced

# definitions for compilation
DEFINES := RB3E NDEBUG

# if DEBUG=1 then compile "debug" builds
ifeq ($(strip $(DEBUG)),1) 
    DEFINES += RB3EDEBUG
endif

# Wii variables
# =================
# build directory for Wii compilation
BUILD_W := build_wii
# .o object files for Wii compilation
OBJECTS_W :=$(subst $(SRC_DIR),$(BUILD_W),$(patsubst %.c,%.c.o,$(SOURCES)))
# BrainSlug library install directory
BSLUGDIR := $(DEVKITPRO)/bslug
# executable tool path
TOOLPATH_W := $(DEVKITPPC)/bin
COMPILER_W := $(TOOLPATH_W)/powerpc-eabi-gcc
LINKER_W := $(TOOLPATH_W)/powerpc-eabi-ld
# include directories
GCC_VER_W := 11.2.0
ifneq ($(strip $(DEVKITPPC)),) 
    GCC_VER_W := $(shell $(COMPILER_W) -dumpversion)
endif
INCLUDES_W := $(DEVKITPPC)/lib/gcc/powerpc-eabi/$(GCC_VER_W)/include \
				$(DEVKITPPC)/lib/gcc/powerpc-eabi/$(GCC_VER_W)/include-fixed \
				$(BSLUGDIR)/include $(INC_DIR)
# compiler flags
CFLAGS_W := -O2 -Wall -Wno-format-extra-args -x c -std=gnu99 -nostdinc -ffreestanding \
			-DGEKKO -DHW_RVL -D__wii__ -DRB3E_WII $(patsubst %,-D%,$(DEFINES)) \
			-mrvl -mcpu=750 -meabi -mhard-float -fshort-wchar -fno-common \
			-msdata=none -memb -ffunction-sections -fdata-sections \
			$(patsubst %,-I %,$(INCLUDES_W)) -iquote src
# linker flags for final compile
LFLAGS_W := --relocatable -s --gc-sections -u bslug_load -u bslug_meta \
			-T $(BSLUGDIR)/bslug.ld 
# linker flags for intermediate compile
LFLAG2_W := --relocatable -s -T $(BSLUGDIR)/bslug_elf.ld
# =================

# Xbox variables
# =================
# build directory for Xbox compilation
BUILD_X := build_xbox
# .obj object files for Xbox compilation
OBJECTS_X := $(subst $(SRC_DIR),$(BUILD_X),$(patsubst %.c,%.obj,$(SOURCES)))
# executable tool path
TOOLPATH_X := $(XEDK)/bin/win32
COMPILER_X := "$(TOOLPATH_X)/cl.exe"
LINKER_X := "$(TOOLPATH_X)/link.exe"
IMAGEXEX_X := "$(TOOLPATH_X)/imagexex.exe"
# include directories
INCLUDES_X := "$(XEDK)/include/xbox" 
# library directories
LIBDIR_X := "$(XEDK)/lib/xbox"
# library includes
LIBS_X := xapilib.lib xboxkrnl.lib xnet.lib xonline.lib
# compiler flags
CFLAGS_X := -c -Zi -nologo -W3 -WX- -Ox -Os -D _XBOX -D RB3E_XBOX $(patsubst %,-D %,$(DEFINES)) \
			-GF -Gm- -MT -GS- -Gy -fp:fast -fp:except- -Zc:wchar_t -Zc:forScope \
			-GR- -openmp- -FI"$(XEDK)/include/xbox/xbox_intellisense_platform.h" \
			-Fd"$(BUILD_X)/" -I "$(INC_DIR)"
# linker flags
LFLAGS_X := -ERRORREPORT:PROMPT -INCREMENTAL:NO -NOLOGO $(LIBS_X) \
			-MANIFESTUAC:"level='asInvoker' uiAccess='false'" -DEBUG \
			-STACK:"262144","262144" -OPT:REF -OPT:ICF -TLBID:1 -RELEASE \
			-dll -entry:"_DllMainCRTStartup" -XEX:NO
# xex generation flags
XEXFLAGS := -nologo -config:"xex.xml"
# =================

# PS3 variables
# =================
# build directory for PS3 compilation
BUILD_P := build_ps3
# .o object files for PS3 compilation
OBJECTS_P :=$(subst $(SRC_DIR),$(BUILD_P),$(patsubst %.c,%.c.o,$(SOURCES)))
# executable tool path
TOOLPATH_P := $(PS3DEV)/bin
PPUPATH_P := $(PS3DEV)/ppu
COMPILER_P := $(PPUPATH_P)/bin/powerpc64-ps3-elf-gcc
LINKER_P := $(PPUPATH_P)/bin/powerpc64-ps3-elf-ld
MAKESELF_P := $(TOOLPATH_P)/make_self_sprx
# include directories
GCC_VER_P := 7.2.0
ifneq ($(strip $(PS3DEV)),) 
    GCC_VER_P := $(shell $(COMPILER_P) -dumpversion)
endif
INCLUDES_P := $(PPUPATH_P)/lib/gcc/powerpc64-ps3-elf/$(GCC_VER_P)/include \
				$(PPUPATH_P)/lib/gcc/powerpc64-ps3-elf/$(GCC_VER_P)/include-fixed \
				$(PPUPATH_P)/include $(PPUPATH_P)/include/simdmath $(INC_DIR)
# library directories
LIBDIR_P := $(PPUPATH_P)/lib 
# library includes
LIBS_P := io sysutil rt lv2
# compiler flags
CFLAGS_P := -O2 -Wall -Wno-format-extra-args -x c -std=gnu99 \
			-DRB3E_PS3 $(patsubst %,-D%,$(DEFINES)) \
			-mcpu=cell -mhard-float -fmodulo-sched \
			-ffunction-sections -fdata-sections \
			$(patsubst %,-I %,$(INCLUDES_P)) -iquote src
# linker flags for final compile
LFLAGS_P := --relocatable
# =================

.PHONY: all
all: xbox wii

.PHONY: scripts
scripts:
	@bash scripts/version.sh

.PHONY: clean
clean:
	@rm -rf $(wildcard $(BUILD_W) $(BUILD_X) $(BUILD_P) $(OUTPUT))

# Wii compilation, creates BrainSlug .MOD file

.PHONY: wii
wii: $(OUTPUT)/$(OUTNAME).mod

$(OUTPUT)/$(OUTNAME).mod: $(BUILD_W)/output.elf
	@echo "Creating MOD..."
	@mkdir -p $(@D)
	@$(LINKER_W) $^ $(LFLAGS_W) -Map $(BUILD_W)/$(OUTNAME).map -o $@

$(BUILD_W)/output.elf: $(OBJECTS_W)
	@echo "Linking ELF..."
	@mkdir -p $(@D)
	@$(LINKER_W) $^ $(LFLAG2_W) -o $@

$(BUILD_W)/%.c.o: $(SRC_DIR)/%.c | scripts
	@echo $<
	@mkdir -p $(@D)
	@$(COMPILER_W) -c $(CFLAGS_W) $< -o $@

# Xbox compilation, creates .DLL file

.PHONY: xbox
xbox: $(OUTPUT)/$(OUTNAME).dll

$(OUTPUT)/$(OUTNAME).dll: $(BUILD_X)/$(OUTNAME).exe
	@echo "Creating XEXDLL..."
	@mkdir -p $(@D)
	@$(WINDOWS_SHIM) $(IMAGEXEX_X) $(XEXFLAGS) -out:"$@" "$^"

$(BUILD_X)/$(OUTNAME).exe: $(OBJECTS_X)
	@echo "Linking DLL..."
	@mkdir -p $(@D)
	@LIB=$(LIBDIR_X) $(WINDOWS_SHIM) $(LINKER_X) $(LFLAGS_X) -OUT:"$@" -PDB:"$(BUILD_X)/$(OUTNAME).pdb" -IMPLIB:"$(BUILD_X)/$(OUTNAME)" $^

$(BUILD_X)/%.obj: $(SRC_DIR)/%.c | scripts
	@mkdir -p $(@D)
	@INCLUDE=$(INCLUDES_X) $(WINDOWS_SHIM) $(COMPILER_X) $(CFLAGS_X) -Fo"$@" -TC $<


# PS3 compilation, creates .SPRX file

.PHONY: ps3
ps3: $(OUTPUT)/$(OUTNAME).sprx

$(OUTPUT)/$(OUTNAME).sprx: $(BUILD_P)/output.elf
	@echo "Creating SPRX...(BROKEN!)"
	@mkdir -p $(@D)
	@$(MAKESELF_P) $^ $@

$(BUILD_P)/output.elf: $(OBJECTS_P)
	@echo "Linking ELF...(BROKEN!)"
	@mkdir -p $(@D)
	@$(LINKER_P) $^ $(LFLAGS_P) -o $@

$(BUILD_P)/%.c.o: $(SRC_DIR)/%.c | scripts
	@echo $<
	@mkdir -p $(@D)
	@$(COMPILER_P) -c $(CFLAGS_P) $< -o $@
