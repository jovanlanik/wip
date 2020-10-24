#
# WIP
# Copyright (c) Jovan Lanik 2020
#

# Makefile

NAME = wip
CC = cc
LDLIBS = -lGL -lGLEW -lglfw -lgraphene-1.0
CFLAGS = -std=c11 -Wall -pedantic -I /usr/lib/graphene-1.0/include

GLSL = $(wildcard glsl/*.vert glsl/*.frag)
SRC = $(wildcard *.c)
DEPS = $(wildcard *.h)
OBJ = $(SRC:%.c=%.o)

all: $(NAME)
$(NAME): $(OBJ)
wip.o : shaders.h
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
