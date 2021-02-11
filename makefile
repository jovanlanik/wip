#
# WIP
# Copyright (c) 2020 Jovan Lanik
#

# Makefile

include config.mk

ifndef NAME
$(error NAME not set)
else ifndef WINDOW_BACKEND
$(error WINDOW_BACKEND not set)
else ifndef NDEBUG
$(warning NDEBUG not explicitly set.)
NDEBUG := 0
endif

CFLAGS += -std=c11 -I . -I include -I $(NAME).d -DWIP_NAME=$(NAME)
LDLIBS = -lm -lpthread -lGL -lGLEW -lconfig -l$(WINDOW_BACKEND)

SRC = $(wildcard src/*.c $(NAME).d/*.c) backend/wip_window_$(WINDOW_BACKEND).c
OBJ = $(SRC:%.c=%.o)
GLSL = $(wildcard glsl/*.vert glsl/*.frag $(NAME).d/glsl/*.vert $(NAME).d/glsl/*.vert)
CONF = conf/$(NAME).conf

TRASH = $(wildcard include/baked/*.h) $(GLSL:%=%.h) $(OBJ)
DIRT = $(wildcard *.d/*.o backend/*.o)

ifeq '$(NDEBUG)' '1'
CFLAGS += -DNDEBUG -o2
else
CFLAGS += -Wall -pedantic -pg
LDFLAGS += -pg
TRASH += gmon.out
endif

.PHONY: all clean wipe

all: $(NAME)
clean:
	@echo Cleaning...
	@for i in $(TRASH); do [ -e $$i ] && ( rm -f $$i; echo "\t$$i" ) || true; done
wipe: clean
	@echo Wiping...
	@for i in $(DIRT); do [ -e $$i ] && ( rm -f $$i; echo "\t$$i" ) || true; done

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@
%.vert.h: %.vert
	@glslangValidator $<
	util/bake $< $<.h
%.frag.h: %.frag
	@glslangValidator $<
	util/bake $< $<.h
config.mk:
	configure

src/wip_conf.o : include/baked/config.h
src/wip_gl.o : include/baked/shaders.h
include/baked/config.h : $(CONF)
	@echo Baking $@ from $<
	util/bake $< $@
include/baked/shaders.h: $(GLSL:%=%.h)
	@cat $(GLSL:%=%.h) > $@
	@echo Baking $@ from $(GLSL)

