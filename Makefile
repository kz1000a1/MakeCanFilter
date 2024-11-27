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

# use the periphlib core library, plus generic ones (libc, libm, libnosys)
LIBS = -lstm32cube -lc -lm -lnosys
LDFLAGS = -T $(LD_SCRIPT) -L $(CUBELIB_BUILD_DIR) -static $(LIBS) $(USER_LDFLAGS)

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex $(BUILD_DIR)/$(TARGET).elf $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $(BUILD_DIR)/$(TARGET).elf $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) $(USB_OBJECTS) $(CUBELIB)
	$(CC) -o $@ $(CFLAGS) $(OBJECTS) $(USB_OBJECTS) \
		$(LDFLAGS) -Xlinker \
		-Map=$(BUILD_DIR)/$(TARGET).map
	$(SIZE) $@

%.o: %.c
	$(CC) $(CFLAGS) -Os -c -o $@ $^

# delete all user application files, keep the libraries
clean:
		-rm *.o

.PHONY: clean all cubelib
