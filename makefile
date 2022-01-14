# WIP
# Copyright (c) 2020 Jovan Lanik

# Makefile

include mkconf.mk

MAKEFLAGS += --no-builtin-rules --no-builtin-variables

NAME ?= demo
BUILDDIR ?= build
WINDOW_BACKEND ?= glfw

LIBS := gl glew libconfig

ifeq '$(WINDOW_BACKEND)' 'glfw'
    LIBS += glfw3
else
    LIBS += sdl2
endif

SRC = $(wildcard src/*.c $(NAME).d/src/*.c) src/opt/wip_window_$(WINDOW_BACKEND).c
OBJ = $(addprefix $(BUILDDIR)/, $(SRC:%.c=%.o))
GLSL = $(wildcard glsl/*.vert glsl/*.frag $(NAME).d/glsl/*.vert $(NAME).d/glsl/*.frag)
CONF = res/conf/$(NAME).conf
TRASH = $(wildcard include/baked/*.h) $(GLSL:%=%.h)

CFLAGS += -pipe -Wall -pedantic -std=c11
CFLAGS += -I ./ -I include -I $(NAME).d/include
CFLAGS += -DWIP_NAME=$(NAME) -DWIP_WINDOW_BACKEND=$(WINDOW_BACKEND)
CGLAGS += `pkg-config --cflags $(LIBS)`

LDFLAGS += -lm `pkg-config --libs $(LIBS)`

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
    TRASH += gmon.out
endif

.PHONY: all clean

all: $(NAME)
clean:
	@rm $(TRASH) | true
	@rm -r $(BUILDDIR) | true
$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@
$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
%.vert.h: %.vert
	@glslangValidator $<
	util/bake $< $<.h
%.frag.h: %.frag
	@glslangValidator $<
	util/bake $< $<.h
include/baked/$(NAME)_config.h: $(CONF)
	@echo Baking $@ from $<
	util/bake $< $@
include/baked/shaders.h: $(GLSL:%=%.h)
	@cat $(GLSL:%=%.h) > $@
	@echo Baking $@ from $(GLSL)
$(NAME).d/src/wip_game.c: include/baked/$(NAME)_config.h include/baked/shaders.h
