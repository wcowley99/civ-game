.PHONY: all valgrind clean

PROJECT_NAME ?= civ-game
PROJECT_BUILD_PATH ?= out

PROJECT_SOURCE_FILES ?= \
												src/main.c \
												src/world.c \
												src/utils.c \
												src/render.c

RAYLIB_SRC_PATH ?= deps/raylib/src
RAYLIB_INCLUDE_PATH ?= $(RAYLIB_SRC_PATH)

OBJS = $(patsubst %.c, %.o, $(PROJECT_SOURCE_FILES))

INCLUDE_PATHS ?= -Isrc -I$(RAYLIB_INCLUDE_PATH)

CFLAGS = -g

LDFLAGS = -L.
LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt

all:
	mkdir -p $(PROJECT_BUILD_PATH)
	$(MAKE) $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJS)
	$(CC) -o $(PROJECT_BUILD_PATH)/$(PROJECT_NAME)$(EXT) $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS)

valgrind: all
	valgrind ./$(PROJECT_BUILD_PATH)/$(PROJECT_NAME)$(EXT)

clean:
	rm -rf $(PROJECT_BUILD_PATH)
	rm src/*.o
