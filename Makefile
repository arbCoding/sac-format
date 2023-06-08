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
debug = false
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
else
    compiler = g++-12
		debug_param = $(common_debug) -fanalyzer -Wsign-conversion -Weffc++
endif
#------------------------------------------------------------------------------
# Note on g++-13
#------------------------------------------------------------------------------
# Info:
# The C++ std library included in g++-13 has problems
# In particular, <vector> gives warnings for uninitialized values
#
# This is not a fault with my code, this is the std library
# not being compliant with its own style guidelines.
#
# As such, on g++-13 it will not compile with debug_params
# I have 12.2.0 
#
# If this gets fixed, I'll update to g++-13
# This is true as of GCC version 13.1.0 as provided by HomeBrew
# I believe this is related to the following gcc issues on bugzilla
# * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=107138
# * https://gcc.gnu.org/bugzilla/show_bug.cgi?id=107919
#
# Summary:
# It looks like these errors/warnings were always "happening", but the compiler
# would suppress them because they were from the standard library.
# This was considered non-ideal, as people wanted to see errors/warnings regardless
# of source. End result is errors/warnings that are not terribly useful to me
# (useful = actionable) as that would involve modifying the standard library.
#
# Solution:
# Stick to gcc12 (on HomeBrew use `brew install gcc@12`) for now
# I have 12.2.0 installed
#
# Alternative solution:
# It does compile (and work) on g++-13 if you turn debug = false (release version)
# This is because release version uses a much looser ruleset for warnings/erros
# If you're just using this, not contributing to this, then you're likely already
# working with the release version anyway (faster/smaller programs, faster compilation)
# So this is possibly a non-issue for you.
#------------------------------------------------------------------------------
# End note on g++-13
#------------------------------------------------------------------------------
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
# Include my sac headers
#------------------------------------------------------------------------------
# Compilation command with inclusion of my headers
cxx := $(cxx) -I$(hdr_prefix)
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
