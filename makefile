# WIP
# Copyright (c) 2020 Jovan Lanik

# Makefile

include mkconf.mk

MAKEFLAGS += --no-builtin-rules --no-builtin-variables

GAME ?= dungeon
BUILDDIR ?= build
WINDOW_BACKEND ?= glfw

LIBS := gl libconfig

ifeq '$(WINDOW_BACKEND)' 'glfw'
    LIBS += glfw3
    CFLAGS += -DWIP_GLFW
else
    LIBS += sdl2
    CFLAGS += -DWIP_SDL2
endif

CFLAGS += -pipe -std=c11 -DWIP_GAME=$(GAME) -DWIP_WINDOW_BACKEND=$(WINDOW_BACKEND) $(shell pkg-config --cflags $(LIBS))

CFLAGS_SRC = $(CFLAGS) -Wall -Wpedantic -I include -I $(GAME).d/include
CFLAGS_EXT = $(CFLAGS) -I include/external

LDLIBS = -ldl -lm $(shell pkg-config --libs $(LIBS))

ifndef NDEBUG
    $(warning NDEBUG not explicitly set.)
    NDEBUG := 0
endif

ifeq '$(NDEBUG)' '1'
    CFLAGS += -DNDEBUG -O2
    LDFLAGS += -Wl,-S
else
    CFLAGS += -g -pg
    LDFLAGS += -pg
endif

SRC = $(filter-out src/wip_sad.c, $(wildcard src/*.c src/external/*.c $(GAME).d/src/*.c)) \
      src/optional/wip_window_$(WINDOW_BACKEND).c
GLSL_VERT = $(wildcard glsl/*.vert $(GAME).d/glsl/*.vert)
GLSL_FRAG = $(wildcard glsl/*.frag $(GAME).d/glsl/*.frag)
RES_CONF = res/conf/$(GAME).conf
OBJ = $(addprefix $(BUILDDIR)/, \
      $(SRC:%.c=%.o) \
      $(GLSL_VERT:%.vert=%.vert.o) \
      $(GLSL_FRAG:%.frag=%.frag.o) \
      $(RES_CONF:%.conf=%.conf.o) \
      )

.PHONY: all clean

all: $(GAME)

clean:
	rm -r $(BUILDDIR)

$(GAME): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_SRC) -c $< -o $@

$(BUILDDIR)/src/external/%.o: src/external/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_EXT) -c $< -o $@

$(BUILDDIR)/%.vert.o: %.vert
	@glslangValidator $<
	@mkdir -p $(dir $@)
	util/bake $< $@

$(BUILDDIR)/%.frag.o: %.frag
	@glslangValidator $<
	@mkdir -p $(dir $@)
	util/bake $< $@

$(BUILDDIR)/%.conf.o: %.conf
	@mkdir -p $(dir $@)
	util/bake $< $@
