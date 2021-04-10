
INCLUDES=deps/include
LIBS_DIR=deps/lib

LIBS=opengl32.lib glu32.lib SDL2main.lib SDL2.lib glew32.lib

# Specify compiler
CC=cl.exe

# Specify linker
LINK=link.exe

# Build all target
all : app

# Compile and Link the object files and dependent libraries into a binary
app : objs/*obj
	$(LINK)  					-out:bin/app.exe -LIBPATH:$(LIBS_DIR) objs/*obj $(LIBS)
# 		    -SUBSYSTEM:CONSOLE

# Compile the source files into object files
objs/*obj : src/*.cpp
	$(CC) -D "WIN32" /Fo"objs/" -c -EHsc src/*.cpp -I $(INCLUDES)