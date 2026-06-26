# Pull the standard building variables natively out of the secure container
include $(DEVKITPRO)/libnx/switch_rules

TARGET      := Launcher-NX
BUILD       := build
SOURCES     := .

APP_TITLE   := Launcher-NX
APP_AUTHOR  := Homebrew Dev
APP_VERSION := 1.0.0

include $(DEVKITPRO)/devkitA64/base_rules
