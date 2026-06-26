# Ensure the compiler paths are set for the cloud container
export DEVKITPRO  := /opt/devkitpro
export DEVKITARM  := /opt/devkitpro/devkitARM
export DEVKITA64  := /opt/devkitpro/devkitA64

# Include standard template building rules from libnx
include $(DEVKITPRO)/libnx/switch_rules

# Set target application properties
TARGET      :=  SwitchLauncher
BUILD       :=  build
SOURCES     :=  source
DATA        :=  data
INCLUDES    :=  include

# Homebrew menu metadata properties
APP_TITLE   :=  Custom Boot Menu
APP_AUTHOR  :=  Homebrew Dev
APP_VERSION :=  1.0.0

# Include standard architecture rules
include $(DEVKITPRO)/devkitA64/base_rules
