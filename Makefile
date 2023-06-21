#------------------------------------------------------------------------------
# Bugs
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# End bugs
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Use the correct shell for bash scripts
# seemed to default to /bin/sh when I use /bin/bash
SHELL := /bin/bash
# Linux or mac
uname_s := $(shell uname -s)
# Debug mode or release mode
#------------------------------------------------------------------------------
# Building the libraries
#------------------------------------------------------------------------------
# To build the libraries, execute
# `make lib`
#
# This will make two object files
# `sac_io.o` which is low-level Binary-SAC file I/O
# `sac_format.o` which is high-level access to the SacStream class
#
# e.g.
# g++ -I$(CURDIR)/src/header main.cpp sac_format.o -o your_program
#
# You can see the compilation patterns used to build the tests for examples of this
#
# Debug (big/slow/strict) or Release (small/fast/relaxed)
# The different between Debug and Release is tremendous in terms of speed
debug = true
#------------------------------------------------------------------------------
# End building the libraries
#-------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Setup compiler
#------------------------------------------------------------------------------
# Param is always used
param = -std=c++20 -pedantic-errors -Wall
# Common debug params regardless of clang++ or g++
common_debug = -Wextra -Werror -Wshadow -ggdb
# Slightly different between MacOS and Linux
ifeq ($(uname_s), Darwin)
    compiler = clang++
	debug_param = $(common_debug) -Wsign-conversion -Weffc++
else ifeq ($(uname_s), Linux)
    compiler = g++-13
	# -fanalyzer bugs out on standard library
	# -Wsign-conversion doesn't like std::size_t on Linux (warning for long unsigned int -> long long unsigned int
	# possibly changing signed, even though both unsigned)
	#debug_param = $(common_debug) -fanalyzer -Wsign-conversion -Weffc++
	debug_param = $(common_debug) -Weffc++
else
	compiler = g++
	debug_param = $(common_debug) -Weffc++
endif
# Release params only if debug is false
#release_param = -O2 -DNDEBUG
# -O3 has been determined to be the best mix of fast and stable.
release_param = -O3 -DNDEBUG
# -Ofast is a bit too loose (maybe because of fast-math)
# Using it has caused weird bugs, both here and in PsSp, so
# I will not be using it by default.
#release_param = -Ofast -DNDEBUG
ifeq ($(debug), true)
	params = $(param) $(debug_param)
else
	params = $(param) $(release_param)
endif
# Compilation command
cxx := $(compiler) $(params)
#------------------------------------------------------------------------------
# End setting up compiler
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Directory structure
#------------------------------------------------------------------------------
# Project directory structure
# Code base starts here
base_prefix = $(CURDIR)/src/
# Built programs will go here
bin_prefix = $(CURDIR)/bin/
# Test programs will go here
test_bin_prefix = $(bin_prefix)tests/
# Where the source code files for tests are stored
test_prefix = $(base_prefix)tests/
# Where header (interface) files are stored
hdr_prefix = $(base_prefix)header/
# Where the source code (implementation) files are stored
imp_prefix = $(base_prefix)implementation/
# Built object files will go here
obj_prefix = $(base_prefix)objects/
#------------------------------------------------------------------------------
# End directory structure
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Boost
#------------------------------------------------------------------------------
ifeq ($(uname_s), Darwin)
	boost_dir = /opt/homebrew/Cellar/boost/1.82.0_1/
	boost_inc = $(boost_dir)include/
	boost_lib = $(boost_dir)lib/
else ifeq ($(uname_s), Linux)
	boost_inc = /usr/include/
	boost_lib = /usr/lib/x86_64-linux-gnu/
else
	boost_inc = /mingw64/include/
	boost_list = /mingw64/lib/
endif
boost_params = -I$(boost_inc) -L$(boost_lib)
#------------------------------------------------------------------------------
# End Boost
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Include my sac headers
#------------------------------------------------------------------------------
# Compilation command with inclusion of my headers
cxx := $(cxx) -I$(hdr_prefix) -I$(boost_inc)
ifeq ($(OS), Windows_NT)
	# Fixes error of no main because windows uses WinMain
	# instead of main
	cxx += /entry:mainCRTStartu
endif
#------------------------------------------------------------------------------
# End include my sac headers
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Program definitions
#------------------------------------------------------------------------------
# All programs
all: lib tests

# Just the object-files (none of the examples/tests)
lib: sac_format

# These only need sac_io.a
sac_fundamental_tests: sac_type_test sac_io_test

# These need sac_format.a
sac_stream_tests: sac_stream_read_test sac_stream_write_test

# All tests
tests: sac_fundamental_tests sac_stream_tests
#------------------------------------------------------------------------------
# End program definitions
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Compilation patterns
#------------------------------------------------------------------------------
# By splitting into .o files I can make it so that only newly written code gets compiled
# Therefore cutting down on compilation times
# Also helps to simply the logic a little bit
sac_io: $(imp_prefix)sac_io.cpp
	@echo "Building $@"
	@echo "Build start:  $$(date)"
	@test -d $(obj_prefix) || mkdir -p $(obj_prefix)
	$(cxx) -c -o $(obj_prefix)$@.o $<
	@echo -e "Build finish:  $$(date)\n"

sac_stream: $(imp_prefix)sac_stream.cpp
	@echo "Building $@"
	@echo "Build start:  $$(date)"
	@test -d $(obj_prefix) || mkdir -p $(obj_prefix)
	$(cxx) -c -o $(obj_prefix)$@.o $<
	@echo -e "Build finish: $$(date)\n"

# Manually defined for specific program
# Modules for sac_class_test
sf_modules := sac_io sac_stream
sf_obj := $(addsuffix .o, $(addprefix $(obj_prefix), $(sf_modules)))

# link obj_files to a single object-file
sac_format: $(sf_modules)
	@echo "Building $@"
	@echo "Build start:  $$(date)"
	ld -r -o $(obj_prefix)$@.o $(sf_obj)
	@echo -e "Build finish: $$(date)\n"

# only use the low-level io because we're not using the SacStream class
basic_modules := sac_io
basic_obj := $(addsuffix .o, $(addprefix $(obj_prefix), $(basic_modules)))
basic_sac := sac_type_test sac_io_test
$(basic_sac): %:$(test_prefix)%.cpp $(basic_modules)
	@echo "Building $(test_bin_prefix)$@"
	@echo "Build start:  $$(date)"
	@test -d $(test_bin_prefix) || mkdir -p $(test_bin_prefix)
	$(cxx) -o $(test_bin_prefix)$@ $< $(basic_obj)
	@echo -e "Build finish: $$(date)\n"

# Use the single object file to simplify
stream_modules := sac_format
stream_obj := $(addsuffix .o, $(addprefix $(obj_prefix), $(stream_modules)))
# $@ is target
# $< is the first prerequisite only
# $^ is all prerequisites, without duplicates, separated by spaces
stream_sac := sac_stream_read_test sac_stream_write_test
$(stream_sac): %:$(test_prefix)%.cpp $(stream_modules)
	@echo "Building $(test_bin_prefix)$@"
	@echo "Build start:  $$(date)"
	@test -d $(test_bin_prefix) || mkdir -p $(test_bin_prefix)
	$(cxx) -o $(test_bin_prefix)$@ $< $(stream_obj)
	@echo -e "Build finish: $$(date)\n"
#-----------------------------------------------------------------------------
# End compilation patterns
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Cleanup
#------------------------------------------------------------------------------
clean:
	rm -rf $(bin_prefix) $(obj_prefix) *.dSYM *.csv
#------------------------------------------------------------------------------
# End cleanup
#------------------------------------------------------------------------------
