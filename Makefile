### Ensure devkitPro environment path variable is set

ifeq ($(strip 

![](data:image/gif;base64,R0lGODlhAQABAIAAAP///wAAACH5BAEAAAAALAAAAAABAAEAAAICRAEAOw==)(

![](data:image/gif;base64,R0lGODlhAQABAIAAAP///wAAACH5BAEAAAAALAAAAAABAAEAAAICRAEAOw==)𝐷𝐸𝑉𝐾𝐼𝑇𝑃𝑅𝑂

![](data:image/gif;base64,R0lGODlhAQABAIAAAP///wAAACH5BAEAAAAALAAAAAABAAEAAAICRAEAOw==))

![](data:image/gif;base64,R0lGODlhAQABAIAAAP///wAAACH5BAEAAAAALAAAAAABAAEAAAICRAEAOw==))

![](data:image/gif;base64,R0lGODlhAQABAIAAAP///wAAACH5BAEAAAAALAAAAAABAAEAAAICRAEAOw==),

![](data:image/gif;base64,R0lGODlhAQABAIAAAP///wAAACH5BAEAAAAALAAAAAABAAEAAAICRAEAOw==))

(error "Please set DEVKITPRO in your environment. export DEVKITPRO=/devkitpro")  
endif 

include $(DEVKITPRO)/devkitA64/base\_tools 

### \=============================================================================

### PROJECT STRUCTURAL CONFIGURATION

### \=============================================================================

TARGET := NX-LAUNCHER  
TITLE := NX-LAUNCHER  
AUTHOR := Homebrew Developer  
VERSION := 1.0.0 

### Directory layouts

BUILD := build  
SOURCES := src  
DATA := data  
INCLUDES := include 

### \=============================================================================

### COMPILER AND LINKER FLAGS (Nintendo Switch ARMv8-A Target)

### \=============================================================================

ARCH := -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE  
CFLAGS := -g -Wall -O2 -ffunction-sections (ARCH) (DEFINES)  
CFLAGS += -I$(CURDIR)/(INCLUDES) -I(DEVKITPRO)/libnx/include  
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++17 

ASFLAGS := -g $(ARCH)  
LDFLAGS = -specs=(DEVKITPRO)/libnx/switch.specs -g (ARCH) -Wl,-Map,(notdir \*.map) 

### Core framework libraries to link against

LIBS := -lnx 

### \=============================================================================

### COMPILATION RULES ENGINE

### \=============================================================================

export OUTPUT := (CURDIR)/(TARGET)  
export TOPDIR := $(CURDIR) 

export VPATH := (foreach dir,(SOURCES),(CURDIR)/(dir))  
(foreach dir,(DATA),(CURDIR)/(dir)) 

export DEPSDIR := (CURDIR)/(BUILD) 

CFILES := $(foreach dir,(SOURCES),(notdir (wildcard (dir)/*.c)))  
CPPFILES := $(foreach dir,(SOURCES),(notdir (wildcard (dir)/*.cpp)))  
SFILES := $(foreach dir,(SOURCES),(notdir (wildcard (dir)/\*.s))) 

export OFILES := $(CPPFILES:.cpp=.o) (CFILES:.c=.o) (SFILES:.s=.o)  
export INCDIRS := (foreach dir,(INCLUDES),(CURDIR)/(dir)) 

.PHONY: clean all 

all: $(BUILD)  
@$(MAKE) --no-print-directory -C (BUILD) -f (CURDIR)/Makefile 

$(BUILD):  
@mkdir -p $@ 

clean:  
@echo "Cleaning workspace artifacts..."  
@rm -fr (BUILD) (TARGET).elf (TARGET).nro (TARGET).nacp \*.map 

else 

### Internally executed rules matching objects within the secondary build step context

DEPENDS := $(OFILES:.o=.d) 

(OUTPUT).nro: (OUTPUT).elf  
(OUTPUT).elf: (OFILES) 

\-include $(DEPENDS) 

endif
