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

CFLAGS += -Wall -pedantic -std=c11 -I . -I include -I $(NAME).d -DWIP_NAME=$(NAME)
LDLIBS += -lm -lpthread -lGL -lGLEW -lconfig -l$(WINDOW_BACKEND)

SRC = $(wildcard src/*.c $(NAME).d/*.c) backend/wip_window_$(WINDOW_BACKEND).c
OBJ = $(SRC:%.c=%.o)
GLSL = $(wildcard glsl/*.vert glsl/*.frag $(NAME).d/glsl/*.vert $(NAME).d/glsl/*.vert)
CONF = conf/$(NAME).conf

TRASH = $(wildcard include/baked/*.h) $(GLSL:%=%.h) $(OBJ)
DIRT = $(wildcard *.d/*.o backend/*.o) $(patsubst %.d, %, $(wildcard *.d)) config.mk

ifeq '$(NDEBUG)' '1'
CFLAGS += -DNDEBUG -O2
LDFLAGS += -Wl,-s
else
CFLAGS += -g -pg
LDFLAGS += -pg
TRASH += gmon.out
endif

.PHONY: all clean distclean install uninstall help

all: $(NAME)
clean: ## Clean objects and baked files for NAME
	@rm $(TRASH) | true
distclean: clean ##Clean all files including config
	@rm $(DIRT) | true
install: all ## Install NAME to DESTDIR
	install $(NAME) $(DESTDIR)$(bindir)/$(NAME)
uninstall: ## Uninstall NAME from DESTDIR
	rm -f $(DESTDIR)$(bindir)/$(NAME)
help: ## Show commented targets
	@grep -hE '^\S[^:]*:[^#]*##.+' $(MAKEFILE_LIST) \
		| sed 's/:[^#]*##\s*/#/' \
		| sort -t '#' -k 1 \
		| column -tN TARGET,COMMENT  -s '#'

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@
%.vert.h: %.vert
	@glslangValidator $<
	util/bake $< $<.h
%.frag.h: %.frag
	@glslangValidator $<
	util/bake $< $<.h

src/wip_conf.o: include/baked/config.h
src/wip_gl.o: include/baked/shaders.h
include/baked/config.h: $(CONF)
	@echo Baking $@ from $<
	util/bake $< $@
include/baked/shaders.h: $(GLSL:%=%.h)
	@cat $(GLSL:%=%.h) > $@
	@echo Baking $@ from $(GLSL)

