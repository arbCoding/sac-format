# Don't echo all the bash commands!
MAKEFLAGS += --silent
# Use the correct shell for bash scripts
# seemed to default to /bin/sh when I use /bin/bash
SHELL := /bin/bash
# Linux or Mac
uname_s := $(shell uname -s)

ifeq ($(uname_s), Linux)
	cxx = g++
else
	cxx = g++-12 # Homebrew g++ install I have
#	cxx = clang
endif

# Param always used
param = -std=c++20 -pedantic-errors -Wall
# Debug params only if debug is true
#  Clang
#debug_param = -Weffc++ -Wextra -Wsign-conversion -Werror -Wshadow -ggdb
# G++
debug_param = -fanalyzer -Weffc++ -Wextra -Wsign-conversion -Werror -Wshadow -ggdb
# Release params only if debug is false
release_param = -O2 -DNDEBUG

# Debug version is substantially larger file size (and slower runtime)
# Debug compilation is extremely strict (warnings = errors)
debug = true
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Release version is substantially smaller file size (and faster runtime)
# Release compilation is extremely relaxed (possible bugs if using to by-pass failed debug compilation...)
#debug = false

ifeq ($(debug), true)
	params = $(param) $(debug_param)
else
	params = $(param) $(release_param)
endif

# Code base starts here
base_prefix = ./src/
# Program location here
bin_prefix = ./bin/
# Where the source code files are being stored
src_prefix = $(base_prefix)code/
# Header prefix
hdr_prefix = $(base_prefix)header/
# Implementation prefix
imp_prefix = $(base_prefix)implementation/
# Object prefix
obj_prefix = $(base_prefix)objects/

# Sources (implementations)
imp_files := $(wildcard $(imp_prefix)*.cpp)
# Objects
obj_files := $(patsubst $(imp_prefix)%.cpp, $(obj_prefix)%.o, $(imp_files))

all: sac_class_test

# Print out the variables
#$(info $$imp_files is [${imp_files}])
#$(info $$obj_files is [${obj_files}])

# By splitting into .o files I can make it so that only newly written code gets compiled
# Therefore cutting down on compilation times
# Also helps to simply the logic a little bit
# $@ is target
# $< is first prerequisite (I think same as $^ in this case)
# This is a general rule to build an object from its corresponding implementation
$(obj_prefix)%.o: $(imp_prefix)%.cpp
	@echo "Building $@"
	@echo "Build start:  $$(date)"
	@test -d $(obj_prefix) || mkdir -p $(obj_prefix)
	$(cxx) $(params) -I$(hdr_prefix) -c -o $@ $<
	@echo -e "Build finish: $$(date)\n"

# $@ is target
# $^ is all prerequisites, without duplicates, separated by spaces
sac_class_test: $(src_prefix)sac_class_test.cpp $(obj_files)
	@echo "Building $(bin_prefix)$@"
	@echo "Build start:  $$(date)"
	@test -d $(bin_prefix) || mkdir -p $(bin_prefix)
	$(cxx) $(params) -I$(hdr_prefix) -o $(bin_prefix)$@ $^
	@echo -e "Build finish: $$(date)\n"

clean:
	rm -rf $(bin_prefix) $(obj_prefix)
