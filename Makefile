#------------------------------------------------------------------------------
# Setup compiler
#------------------------------------------------------------------------------
# Use the correct shell for bash scripts
# seemed to default to /bin/sh when I use /bin/bash
SHELL := /bin/bash
# Compiler
compiler = g++-12
# Param is always used
param = -std=c++20 -pedantic-errors -Wall
# Debug params only if debug is true
# G++
debug_param = -fanalyzer -Weffc++ -Wextra -Wsign-conversion -Werror -Wshadow -ggdb
# Release params only if debug is false
release_param = -O2 -DNDEBUG
# Debug (big/slow/strict) or Release (small/fast/relaxed)
debug = true

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
# Linux or mac
uname_s := $(shell uname -s)
# Project directory structur
# Code base starts here
base_prefix = $(CURDIR)/src/
# Built programs will go here
bin_prefix = $(CURDIR)/bin/
# Test programs will go here
test_bin_prefix = $(bin_prefix)tests/
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
#------------------------------------------------------------------------------
# End directory structure
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# FFTW
#------------------------------------------------------------------------------
ifeq ($(uname_s), Linux)
	fftw_include := /usr/include/
	fftw_lib := /usr/lib/x86_64-linux-gnu/
else
	fftw_loc := /opt/homebrew/Cellar/fftw/3.3.10_1/
	fftw_include := $(fftw_loc)include/
	fftw_lib := $(fftw_loc)lib/
endif
fftw_params = -I$(fftw_include) -L$(fftw_lib) -lfftw3 -lm
#------------------------------------------------------------------------------
# End FFTW
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
all: tests

sac_tests: sac_type_test sac_io_test sac_stream_read_test sac_stream_write_test

spectral_tests: sac_stream_fftw_test sac_stream_lowpass_test

other_tests: fftw_test

tests: sac_tests other_tests spectral_tests
#------------------------------------------------------------------------------
# End program definitions
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# Compilation patterns
#------------------------------------------------------------------------------
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
	$(cxx) -c -o $@ $< $(fftw_params)
	@echo -e "Build finish: $$(date)\n"

# Manually defined for specific program
# Modules for sac_class_test
modules := sac_io sac_stream
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
basic_sac := sac_type_test sac_io_test sac_stream_read_test sac_stream_write_test sac_stream_fftw_test sac_stream_lowpass_test fftw_test
$(basic_sac): %:$(test_prefix)%.cpp $(modules)
	@echo "Building $(test_bin_prefix)$@"
	@echo "Build start:  $$(date)"
	@test -d $(test_bin_prefix) || mkdir -p $(test_bin_prefix)
	$(cxx) -o $(test_bin_prefix)$@ $< $(obj_files) $(fftw_params)
	@echo -e "Build finish $$(date)\n"
#------------------------------------------------------------------------------
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
