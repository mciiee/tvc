CC=clang

ifneq (ASAN,)
ASAN_FLAGS=-g -fsanitize=address
else
ASAN_FLAGS=
endif
CFLAGS=-Wall -Werror -Wpedantic -std=c23 $(ASAN_FLAGS)

SRC_DIR=src
BUILD_DIR=build
LIBS_DIR=libs

WLROOTS_DIR=$(LIBS_DIR)/wlroots
WLROOTS_BRANCH=0.19
WLROOTS_LIB=$(WLROOTS_DIR)/build/libwlroots-0.19.so

NPROC != nproc

.PHONY: submoddules

submoddules:
	git submodule update --init

$(WLROOTS_DIR): submoddules

$(WLROOTS_LIB): $(WLROOTS_DIR)
	cd $(WLROOTS_DIR)
	meson setup build/
	ninja $(NPROC) -C build/
