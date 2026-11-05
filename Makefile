ARCH ?= x86_64
MACOS_MIN ?= 12.0
CC ?= clang
BUILD_DIR := build
RILL := $(BUILD_DIR)/rill
APP := $(BUILD_DIR)/RillWorld.app
APP_BIN := $(APP)/Contents/MacOS/rill-world

.PHONY: all verify

all: $(RILL) $(APP_BIN)

$(RILL): agar_medium.c
	mkdir -p $(BUILD_DIR)
	$(CC) -std=c11 -Wall -Wextra -Werror -O2 -arch $(ARCH) \
		-mmacosx-version-min=$(MACOS_MIN) -o $@ $<
	codesign -s - --force $@

$(APP_BIN): rill_world.m RillWorld-Info.plist
	mkdir -p $(APP)/Contents/MacOS
	$(CC) -fobjc-arc -Wall -Wextra -Werror -O2 -arch $(ARCH) \
		-mmacosx-version-min=$(MACOS_MIN) \
		-framework AppKit -framework AVFoundation -framework CoreBluetooth \
		-framework CoreMedia -framework CoreVideo -framework AudioToolbox \
		-framework IOBluetooth -o $@ rill_world.m
	cp RillWorld-Info.plist $(APP)/Contents/Info.plist
	codesign -s - --force --deep $(APP)

verify: all
	codesign --verify --strict --verbose=2 $(RILL)
	codesign --verify --deep --strict --verbose=2 $(APP)
	plutil -lint RillWorld-Info.plist local.agar.rill.world.plist \
		local.agar.rill.plist
