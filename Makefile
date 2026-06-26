# Force paths for the cloud container
export DEVKITPRO  := /opt/devkitpro
export DEVKITARM  := /opt/devkitpro/devkitARM
export DEVKITA64  := /opt/devkitpro/devkitA64

# Set standard configuration parameters
TARGET      := Launcher-NX
BUILD       := build
SOURCES     := source

# Homebrew menu metadata properties
APP_TITLE   := Custom Boot Menu
APP_AUTHOR  := Homebrew Dev
APP_VERSION := 1.0.0

# Include standard template rules
include $(DEVKITPRO)/libnx/switch_rules

# Explicitly define the default "all" target so the compiler knows what to run
all: $(OUTPUT).nro

include $(DEVKITPRO)/devkitA64/base_rules
