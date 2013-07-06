# Terminus Makefile

EXE      = prototipo

CFLAGS   = -g `sdl-config --cflags`
CXXFLAGS = -Wall -Wextra -g `sdl-config --cflags`
LDFLAGS  = `sdl-config --libs` \
           -lSDL_image -lSDL_ttf -lSDL_mixer -lSDL_gfx -lm

INCLUDE = -I"/usr/include/SDL"

########## Do NOT edit this
CC  = gcc
CXX = g++

# All C file sources of the projects
CFILES   = $(shell find src -maxdepth 1 -type f -name '*.c')
COBJECTS = $(CFILES:.c=.o)

# All C++ file sources of the projects
CXXFILES   = $(shell find src -maxdepth 1 -type f -name '*.cpp')
CXXOBJECTS = $(CXXFILES:.cpp=.o)

SOURCES = $(CFILES)   $(CXXFILES)
OBJECTS = $(COBJECTS) $(CXXOBJECTS)

LIBS_OBJECTS = src/lib/SDL_Config/SDL_config.o

ifdef V
MUTE =
VTAG = -v
else
MUTE = @
endif
##########

###################################################################
# Don't try to understand this

# BUILD is initially undefined
ifndef BUILD

# max equals 256 x's
sixteen := x x x x x x x x x x x x x x x x
MAX := $(foreach x,$(sixteen),$(sixteen))

# T estimates how many targets we are building by replacing BUILD with
# a special string
T := $(shell $(MAKE) -nrRf $(firstword $(MAKEFILE_LIST)) $(MAKECMDGOALS) \
            BUILD="COUNTTHIS" | grep -c "COUNTTHIS")

# N is the number of pending targets in base 1, well in fact, base x
# :-)
N := $(wordlist 1,$T,$(MAX))

# auto-decrementing counter that returns the number of pending targets
# in base 10
counter = $(words $N)$(eval N := $(wordlist 2,$(words $N),$N))

# BUILD is now defined to show the progress, this also avoids
# redefining T in loop
BUILD = @echo $(counter) of $(T)
endif
################################################################################

all: dirs $(EXE)
	# Build successful!

$(EXE): $(OBJECTS) $(LIBS_OBJECTS)
	# Linking...
	$(MUTE)$(CXX) $(OBJECTS) $(LIBS_OBJECTS) -o $(EXE) $(LDFLAGS)

# This needed to be added manually
src/lib/SDL_Config/SDL_config.o: src/lib/SDL_Config/SDL_config.c
	# Compiling $<...
	$(MUTE)$(CXX) $< -c -o $@ $(INCLUDE) -fexpensive-optimizations -O3

src/%.o: src/%.cpp
	# Compiling $<...
	$(MUTE)$(CXX) $(CXXFLAGS) $(INCLUDE) $< -c -o $@
	$(BUILD)

src/%.o: src/%.c
	# Compiling $<...
	$(MUTE)$(CC) $(CFLAGS) $(INCLUDE) $< -c -o $@

run: all
	$(MUTE)./$(EXE)

clean:
	# Cleaning...
	-$(MUTE)rm -f $(EXE) $(OBJECTS)

libclean: clean
	# Cleaning libs...
	-$(MUTE)rm -f $(LIBS_OBJECTS)

dirs:
	$(MUTE)mkdir -p img src

# Creates a zip to send to moodle
zip: libclean
	cd .. && zip -r $(MATRICULA)_tr0$(NUMERO_DO_TRABALHO).zip trabalho$(NUMERO_DO_TRABALHO)

# Generates full documentation for the project
dox:
	$(MUTE)doxygen Doxyfile
	# Documentation generated!

doxclean:
	$(MUTE)rm -rf doc/html
	# Documentation cleaned!

