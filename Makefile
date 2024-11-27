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
TARGET = MakeCanFilter

#######################################
# binaries
#######################################
CC = cc
#######################################

# default action: build the user application
all: $(TARGET)

#######################################
# build the user application
#######################################

# list of user program objects
OBJECTS = $(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)


%.o: %.c
	$(CC) -c -o $@ $^

# delete all user application files, keep the libraries
clean:
		-rm $(OBJECTS) $(TARGET)

.PHONY: clean all cubelib
