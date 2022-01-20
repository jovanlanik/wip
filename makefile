# WIP
# Copyright (c) 2020 Jovan Lanik

# Makefile

include mkconf.mk

MAKEFLAGS += --no-builtin-rules --no-builtin-variables

NAME ?= demo
BUILDDIR ?= build
WINDOW_BACKEND ?= glfw

LIBS := gl libconfig

ifeq '$(WINDOW_BACKEND)' 'glfw'
    LIBS += glfw3
else
    LIBS += sdl2
endif

SRC = $(wildcard src/*.c src/external/*.c $(NAME).d/src/*.c) src/optional/wip_window_$(WINDOW_BACKEND).c
GLSL_VERT = $(wildcard glsl/*.vert $(NAME).d/glsl/*.vert)
GLSL_FRAG = $(wildcard glsl/*.frag $(NAME).d/glsl/*.frag)
RES_CONF = res/conf/$(NAME).conf
OBJ = $(addprefix $(BUILDDIR)/, \
      $(SRC:%.c=%.o) \
      $(GLSL_VERT:%.vert=%.vert.o) \
      $(GLSL_FRAG:%.frag=%.frag.o) \
      $(RES_CONF:%.conf=%.conf.o) \
      )

CFLAGS += -pipe -std=c11 -DWIP_NAME=$(NAME) -DWIP_WINDOW_BACKEND=$(WINDOW_BACKEND) $(shell pkg-config --cflags $(LIBS))

CFLAGS_SRC = $(CFLAGS) -Wall -Wpedantic -I ./ -I include -I $(NAME).d/include
CFLAGS_EXT = $(CFLAGS) -I include/external

LDFLAGS += -ldl -lm $(shell pkg-config --libs $(LIBS))

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

.PHONY: all clean

all: $(NAME)
clean:
	rm -r $(BUILDDIR)
$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@
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
