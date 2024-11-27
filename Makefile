# STM32F0xx Makefile
# #####################################
#
# Part of the uCtools project
# uctools.github.com
#
#######################################
# user configuration:
#######################################


# SOURCES: list of sources in the user application
SOURCES = MakeCanFilter.c

#######################################
# binaries
#######################################
CC = cc
#######################################

# default action: build the user application
all: MakeCanFilter

#######################################
# build the user application
#######################################

# list of user program objects
OBJECTS = $(SOURCES:.c=.o)

MakeCanFilter: $(OBJECTS)
        $(CC) -o MakeCanFilter $(OBJECTS)


%.o: %.c
	$(CC) $(CFLAGS) -Os -c -o $@ $^

# delete all user application files, keep the libraries
clean:
		-rm *.o

.PHONY: clean all cubelib
