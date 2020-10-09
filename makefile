#
# WIP
# Copyright (c) Jovan Lanik 2020
#

# Makefile

NAME = wip
CC = gcc
LDLIBS = -lGL -lGLEW -lglfw -lgraphene-1.0
CFLAGS = -std=c11 -I /usr/lib/graphene-1.0/include

GLSL = *.glsl
SRC = *.c
OBJ = wip.o funcs.o callback.o

all: $(GLSL) $(NAME)
$(NAME): $(OBJ)
$(GLSL): clean_shaders
	xxd -i $@ >> shaders.h
clean: clean_shaders clean_obj
	rm -f $(NAME)
clean_shaders:
	rm -f shaders.h
clean_obj:
	rm -f $(OBJ)

.PHONY: all clean clean_shaders clean_obj
