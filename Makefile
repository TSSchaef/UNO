#Makefile
BINARY=uno
CODEDIRS=src
INCDIRS=src 

CC=g++
OPT=-O0
LIBFLAGS=-lm
DEPFLAGS=-MP -MD
CFLAGS=-Wall -Werror -g $(foreach D, $(INCDIRS), -I$(D)) $(OPT) $(DEPFLAGS)

CFILES=	$(foreach D,$(CODEDIRS), $(wildcard $(D)/*.cpp))
OBJECTS= $(patsubst %.cpp, %.o, $(CFILES)) 
DEPFILES= $(patsubst %.cpp, %.d, $(CFILES)) 

all: $(BINARY)


$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(LIBFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

#TO-DO: Add second way to build the program with debugging disabled and optimiziations

.PHONY: clean

clean:
	rm -f $(OBJECTS) $(DEPFILES) $(BINARY) 

diff:
	$(info The status of the repository, and the volume of per-file changes:)
	@git status
	@git diff --stat

-include $(DEPFILES)
