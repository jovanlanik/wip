#
# WIP
# Copyright (c) Jovan Lanik 2020
#

# Makefile

NAME = wip
CC = gcc
LDLIBS = -lm -lpthread -lGL -lGLEW -lconfig
CFLAGS = -DNDEBUG -std=c11 -Wall -pedantic -I ./ -I ./include

GLSL = $(wildcard glsl/*.vert glsl/*.frag)
SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)

WIP_WINDOW_BACKEND ?= glfw
LDLIBS += -l$(WIP_WINDOW_BACKEND)
SRC += backend/wip_window_$(WIP_WINDOW_BACKEND).c

all: $(NAME)
$(NAME): $(OBJ)
wip_gl.o : shaders.h
shaders.h: $(GLSL:%=%.h)
	@cat $(GLSL:%=%.h) > $@
	@echo making $@ from $(GLSL)
%.vert.h: %.vert
	@glslangValidator $<
	@echo making vertex shader $<
	@xxd -i $< > $<.h
%.frag.h: %.frag
	@glslangValidator $<
	@echo making fragment shader $<
	@xxd -i $< > $<.h
wip.c: shaders.h
clean:
	rm -f $(GLSL:%=%.h)
	rm -f shaders.h
	rm -f $(OBJ)

.PHONY: all clean
