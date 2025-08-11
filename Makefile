CC=clang

ifneq (ASAN,)
ASAN_FLAGS=-g -fsanitize=address
else
ASAN_FLAGS=
endif
# INCLUDE_FLAGS=-I$(WLROOTS_DIR)/include

CFLAGS=-Wall -Werror -Wpedantic -std=c23 $(ASAN_FLAGS) $(LIBS_FLAGS)

SRC_DIR=src
BUILD_DIR=build
LIBS_DIR=libs

OUTFILE=$(BUILD_DIR)/tvc

PKGCONF=pkg-config

WLROOTS_DIR=$(LIBS_DIR)/wlroots
WLROOTS_BRANCH=0.19
WLROOTS_LIB=$(WLROOTS_DIR)/build/libwlroots-0.19.so
WLROOTS_FLAGS=-DWLR_USE_UNSTABLE -I$(WLROOTS_DIR)/include $(shell $(PKGCONF) --cflags wlroots)

LIBS=$(WLROOTS_LIB) 
LIBS_FLAGS=$(WLROOTS_FLAGS) -lwayland-server

NPROC != nproc

.PHONY: submoddules

$(OUTFILE): $(BUILD_DIR)/main.o
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $^ -o $@


submoddules:
	git submodule update --init

$(WLROOTS_DIR): submoddules

$(WLROOTS_LIB): $(WLROOTS_DIR)
	cd $(WLROOTS_DIR)
	meson setup build/
	ninja $(NPROC) -C build/
