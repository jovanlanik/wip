#
# WIP
# Copyright (c) 2020 Jovan Lanik
#

# Makefile

NAME = wip
CC = gcc
LDLIBS = -lm -lpthread -lGL -lGLEW -lconfig
CFLAGS = -std=c11 -Wall -pedantic -I ./ -I ./include

SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)
GLSL = $(wildcard glsl/*.vert glsl/*.frag)
CONF = wip.conf

WIP_WINDOW_BACKEND ?= glfw
LDLIBS += -l$(WIP_WINDOW_BACKEND)
SRC += backend/wip_window_$(WIP_WINDOW_BACKEND).c

all: $(NAME)
$(NAME): $(OBJ)
wip_conf.o : include/baked/config.h
wip_gl.o : include/baked/shaders.h
include/baked/config.h : $(CONF)
	@echo making $@ from $(CONF)
	@util/bake $(CONF) $@
include/baked/shaders.h: $(GLSL:%=%.h)
	@cat $(GLSL:%=%.h) > $@
	@echo making $@ from $(GLSL)
%.vert.h: %.vert
	@glslangValidator $<
	@util/bake $< $<.h
%.frag.h: %.frag
	@glslangValidator $<
	@util/bake $< $<.h
clean:
	rm -f $(wildcard include/baked/*)
	rm -f $(GLSL:%=%.h)
	rm -f $(OBJ)

.PHONY: all clean
