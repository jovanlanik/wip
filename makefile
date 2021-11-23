# WIP
# Copyright (c) 2020 Jovan Lanik

# Makefile

include mkconf.mk

MAKEFLAGS += --no-builtin-rules
MAKEFLAGS += --no-builtin-variables

NAME ?= demo
BUILDDIR ?= build
WINDOW_BACKEND ?= glfw

ifeq '$(WINDOW_BACKEND)' 'glfw'
CFLAGS += -DWIP_GLFW
LDLIBS += -lglfw
NWINDOW_BACKEND=SDL2
else
CFLAGS += -DWIP_SDL2
LDLIBS += -lSDL2
NWINDOW_BACKEND=glfw
endif

SRC = $(filter-out src/wip_window_$(NWINDOW_BACKEND).c, $(wildcard src/*.c $(NAME).d/src/*.c))
OBJ = $(addprefix $(BUILDDIR)/, $(SRC:%.c=%.o))
DEPS = $(addprefix $(BUILDDIR)/, $(SRC:%.c=%.d))
GLSL = $(wildcard glsl/*.vert glsl/*.frag $(NAME).d/glsl/*.vert $(NAME).d/glsl/*.frag)
CONF = res/conf/$(NAME).conf

TRASH = $(wildcard include/baked/*.h) $(GLSL:%=%.h)

CFLAGS += -Wall -pedantic -std=c11 -I ./ -I include -I $(NAME).d/include -DWIP_NAME=$(NAME)
LDLIBS += -lm -lpthread -lGL -lGLEW -lconfig

ifndef NDEBUG
$(warning NDEBUG not explicitly set.)
NDEBUG := 0
endif

ifeq '$(NDEBUG)' '1'
CFLAGS += -DNDEBUG -O2
LDFLAGS += -Wl,-s
else
CFLAGS += -g -pg
LDFLAGS += -pg
TRASH += gmon.out
endif

.PHONY: all clean install uninstall help

all: $(NAME)
clean:
	@rm $(TRASH) | true
	@rm -r $(BUILDDIR) | true
mkconf.mk:
	./configure.sh
compile_commands.json: $(SRC) mkconf.mk
	bear -- make clean all

-include $(DEPS)

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@
$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
$(BUILDDIR)/%.d: %.c
	@echo Resolving dependencies for $<
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -MM -MG -MT $(BUILDDIR)/$*.o -MF $@ $<
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

