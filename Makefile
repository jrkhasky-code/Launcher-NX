# Pull the standard building rules natively from the safe host container environment
include $(DEVKITPRO)/libnx/switch_rules

TARGET      := Launcher-NX
BUILD       := build
SOURCES     := source

APP_TITLE   := Launcher-NX
APP_AUTHOR  := Homebrew Dev
APP_VERSION := 1.0.0

include $(DEVKITPRO)/devkitA64/base_rules
