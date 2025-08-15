CC=clang

ifneq ($(ASAN),)
ASAN_FLAGS=-g -fsanitize=address
else
ASAN_FLAGS=
endif
# INCLUDE_FLAGS=-I$(WLROOTS_DIR)/include

CFLAGS=-Wall -Werror -Wpedantic -std=c23 $(ASAN_FLAGS) $(INCLUDE_FLAGS) $(LIBS_FLAGS)

SRC_DIR=src
BUILD_DIR=build
INCLUDE_DIR=include
LIBS_DIR=libs

OUTFILE=$(BUILD_DIR)/tvc

PKGCONF=pkg-config

WLROOTS_DIR=$(LIBS_DIR)/wlroots
WLROOTS_BRANCH=0.19
WLROOTS_LIB=$(WLROOTS_DIR)/build/libwlroots-0.19.so
WLROOTS_FLAGS=-DWLR_USE_UNSTABLE -I$(WLROOTS_DIR)/include $(shell $(PKGCONF) --cflags wlroots-$(WLROOTS_BRANCH))
WLROOTS_PKGCONFIG_DIR=/usr/lib64/pkgconfig

INCLUDE_FLAGS=-I$(INCLUDE_DIR)

LIBS=$(WLROOTS_LIB) -lwayland-server
LIBS_FLAGS=$(WLROOTS_FLAGS) 

NPROC != nproc

.PHONY: submoddules tvc deps wlroots

tvc: $(OUTFILE)

submoddules:
	git submodule update --init

deps: wlroots

depclean:
	cd $(WLROOTS_DIR) && rm -rf build/ 

$(WLROOTS_DIR): submoddules

$(WLROOTS_LIB): $(WLROOTS_DIR)
	cd $(WLROOTS_DIR) && meson setup build/ && ninja -j$(NPROC) -C build/

wlroots: $(WLROOTS_LIB)


$(OUTFILE): $(BUILD_DIR)/main.o
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $^ -o $@
