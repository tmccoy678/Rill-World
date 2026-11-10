CC ?= clang
CFLAGS := -std=c11 -Wall -Wextra -Werror -O2
BUILD := build
NATIVE := $(BUILD)/carrier-trial-native
INTEL := $(BUILD)/carrier-trial-x86_64

.PHONY: all native intel test clean

all: native

$(NATIVE): carrier_trial.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $@ $< -lm

native: $(NATIVE)

$(INTEL): carrier_trial.c
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -arch x86_64 -mmacosx-version-min=12.0 -o $@ $< -lm
	codesign -s - --force $@

intel: $(INTEL)

test: $(NATIVE)
	$(NATIVE) --steps 40000

clean:
	rm -f $(NATIVE) $(INTEL)
