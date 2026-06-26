export DEVKITPRO  := /opt/devkitpro
export DEVKITA64  := /opt/devkitpro/devkitA64

include $(DEVKITPRO)/libnx/switch_rules

TARGET      := Launcher-NX
BUILD       := build
SOURCES     := .

APP_TITLE   := Launcher-NX
APP_AUTHOR  := O-Hex8
APP_VERSION := 1.0.0

include $(DEVKITPRO)/devkitA64/base_rules
