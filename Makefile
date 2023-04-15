# Don't echo all the bash commands!
#MAKEFLAGS += --silent
# Use the correct shell for bash scripts
# seemed to default to /bin/sh when I use /bin/bash
SHELL := /bin/bash
# Linux or Mac
uname_s := $(shell uname -s)

ifeq ($(uname_s), Linux)
	compiler = g++
else
	compiler = g++-12 # Homebrew g++ install I have
#	compiler = clang
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

# Project directory structur
# Code base starts here
base_prefix = ./src/
# Built programs will go here
bin_prefix = ./bin/
# Where the source code files for programs are stored
src_prefix = $(base_prefix)code/
# Where the source code files for tests are stored
test_prefix = $(base_prefix)tests/
# Where header (interface) files are stored
hdr_prefix = $(base_prefix)header/
# Where the source code (implementation) files are stored
imp_prefix = $(base_prefix)implementation/
# Built object files will go here
obj_prefix = $(base_prefix)objects/

# Compilation command
cxx := $(compiler) $(params) -I$(hdr_prefix)

tests: sac_class_test sac_type_test sac_io_test

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
	$(cxx) -c -o $@ $<
	@echo -e "Build finish: $$(date)\n"

# Manually defined for specific program
# Modules for sac_class_test
modules := sac_io sac_class
obj_files := $(addsuffix .o, $(addprefix $(obj_prefix), $(modules)))

# Print out obj_files to check that they are set correctly
#$(info $$obj_files is [${obj_files}])

# link obj_files to a single object-file
sac_format: $(obj_files)
	@echo "Building $(bin_prefix)$@"
	@echo "Build start:  $$(date)"
	ld -r -o $(obj_prefix)$@.o $^
	@echo -e "Build finish: $$(date)\n"

# Use the single object file to simplify
modules := sac_format
obj_files := $(addsuffix .o, $(addprefix $(obj_prefix), $(modules)))
# $@ is target
# $^ is all prerequisites, without duplicates, separated by spaces
sac_type_test: $(test_prefix)sac_type_test.cpp $(modules)
	@echo "Building $(bin_prefix)$@"
	@echo "Build start:  $$(date)"
	@test -d $(bin_prefix) || mkdir -p $(bin_prefix)
	$(cxx) -o $(bin_prefix)$@ $< $(obj_files)
	@echo -e "Build finish: $$(date)\n"

sac_io_test: $(test_prefix)sac_io_test.cpp $(modules)
	@echo "Building $(bin_prefix)$@"
	@echo "Build start:  $$(date)"
	@test -d $(bin_prefix) || mkdir -p $(bin_prefix)
	$(cxx) -o $(bin_prefix)$@ $< $(obj_files)
	@echo -e "Building finish $$(date)\n"

modules := sac_format
obj_files := $(addsuffix .o, $(addprefix $(obj_prefix), $(modules)))

sac_class_test: $(test_prefix)sac_class_test.cpp $(modules)
	@echo "Building $(bin_prefix)$@"
	@echo "Build start:  $$(date)"
	@test -d $(bin_prefix) || mkdir -p $(bin_prefix)
	$(cxx) -o $(bin_prefix)$@ $< $(obj_files)
	@echo -e "Build finish: $$(date)\n"

clean:
	rm -rf $(bin_prefix) $(obj_prefix) *.dSYM