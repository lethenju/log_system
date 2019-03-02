# Makefile for Log_system server, source and example
# MIT License
# Julien LE THENO

all: setup test 

SHELL:=/bin/bash
MAKE=make

BUILD_DIR=build
OBJECTS_DIR=$(BUILD_DIR)/obj
EXE_DIR=$(BUILD_DIR)/exe
SRC_DIR=src
EXAMPLES_DIR=examples


F1_EXISTS=$(shell [ -e $(BUILD_DIR) ] && echo Y || echo N )

.PHONY: all test termlib clean

### EXAMPLES TARGETS

# TEST EXAMPLE
test: clean_obj log_system.o $(EXAMPLES_DIR)/main.c 
	gcc -g $(EXAMPLES_DIR)/main.c $(OBJECTS_DIR)/* -o $(EXE_DIR)/test_exe -lpthread -lm
### END EXAMPLE TARGETS

### SERVER TARGET

server: clean_obj log_system_server
	@echo "SERVER BUILD DONE"

log_system_server: log_system_server.o
	gcc -o $(EXE_DIR)/server_exe $(OBJECTS_DIR)/* 
### END SERVER TARGET


### LIB TARGET


log_system.o: ini.o $(SRC_DIR)/log_system.c  $(SRC_DIR)/log_system.h 
	gcc -g -c $(SRC_DIR)/log_system.c -o  $(OBJECTS_DIR)/log_system.o


ini.o: $(SRC_DIR)/inih/ini.c $(SRC_DIR)/inih/ini.h
	gcc -g -c $(SRC_DIR)/inih/ini.c -o $(OBJECTS_DIR)/ini.o


log_system_server.o: $(SRC_DIR)/log_system_server.c $(SRC_DIR)/log_system.h 
	gcc -g -c $(SRC_DIR)/log_system_server.c -o  $(OBJECTS_DIR)/log_system_server.o

### END LIB TARGET

setup:
ifeq ($(F1_EXISTS),N)
	@echo "Creating build directory"
	mkdir $(BUILD_DIR)
	mkdir $(EXE_DIR)
	mkdir $(OBJECTS_DIR)
endif

clean: clean_obj clean_exe
	@echo "CLEANED"

clean_obj:
	rm -f $(OBJECTS_DIR)/*

clean_exe:
	rm -f $(EXE_DIR)/*


rebuild: clean all
