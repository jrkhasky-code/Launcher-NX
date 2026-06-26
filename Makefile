# Force explicit paths for the cloud container
export DEVKITPRO  := /opt/devkitpro
export DEVKITARM  := /opt/devkitpro/devkitARM
export DEVKITA64  := /opt/devkitpro/devkitA64

# Safely include the switch building rules 
include $(DEVKITPRO)/libnx/switch_rules

# Set configuration parameters
TARGET      := SwitchLauncher
BUILD       := build
SOURCES     := source

# Homebrew menu metadata properties
APP_TITLE   := Custom Boot Menu
APP_AUTHOR  := Homebrew Dev
APP_VERSION := 1.0.0

# Safely include compiling engine rules
include $(DEVKITPRO)/devkitA64/base_rules
