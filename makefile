#
# WIP
# Copyright (c) Jovan Lanik 2020
#

# Makefile

NAME = wip
CC = gcc
LDLIBS = -lGL -lGLEW -lglfw -lgraphene-1.0
CFLAGS = -std=c11 -Wall -pedantic -I /usr/lib/graphene-1.0/include

GLSL = *.glsl
SRC = *.c
OBJ = wip.o wip_fn.o wip_gl.o wip_ply.o wip_glfw.o callback.o

all: $(GLSL) $(NAME)
$(NAME): $(OBJ)
$(GLSL): clean_shaders
	@printf 'shaders.h: from file $@ '
	@sed 1,5d $@ | sed 1q | tee -a shaders.h | sed 's/\/\//adding/g'
	@mv $@ _$@
	@grep -v '^$$' _$@ | grep -v '^//' > $@
	@xxd -i $@ >> shaders.h
	@echo >> shaders.h
	@rm -f $@
	@mv _$@ $@
clean: clean_shaders clean_obj
	rm -f $(NAME)
clean_shaders:
	rm -f shaders.h
clean_obj:
	rm -f $(OBJ)

.PHONY: all clean clean_shaders clean_obj
