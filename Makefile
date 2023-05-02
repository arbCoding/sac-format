#------------------------------------------------------------------------------
# Setup compiler
#------------------------------------------------------------------------------
# Use the correct shell for bash scripts
# seemed to default to /bin/sh when I use /bin/bash
SHELL := /bin/bash
# Compiler
# Note on g++-13
# The C++ std library included in g++-13 has problems
# In particular, <vector> gives warnings for uninitialized values
# This is not a fault with my code, this is the std library
# not being compliant with its own style guidelines
# As such, on g++-13 it will not compile with debug_params
# If this gets fixed, I'll update to g++-13
# This is true as of GCC version 13.1.0 as provided by HomeBrew
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
# Note that FFTW is optional and only used for spectral functions
# If you don't want to use the spectral functions don't worry about this
# If you do, you'll need to setup fftw_params according to your installation
# Spectral = FFT, IFFT, Filters (lowpass, highpass, bandpass)
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
# All programs
all: tests

# These only need sac_io.o
sac_fundamental_tests: sac_type_test sac_io_test

# These need sac_format.o (sac_io.o and sac_stream.o) still need to remove the FFTW requirement
sac_stream_tests: sac_stream_read_test sac_stream_write_test

# These need sac_format.o and FFTW
spectral_tests: sac_stream_fftw_test sac_stream_lowpass_test

# All tests
tests: sac_fundamental_tests sac_stream_tests spectral_tests
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

sac_spectral: $(imp_prefix)sac_spectral.cpp
	@echo "Building $@"
	@echo "Build start:  $$(date)"
	@test -d $(obj_prefix) || mkdir -p $(obj_prefix)
	$(cxx) -c -o $(obj_prefix)$@.o $< $(fftw_params)
	@echo -e "Build finish: $$(date)\n"

# Manually defined for specific program
# Modules for sac_class_test
sf_modules := sac_io sac_stream
sf_obj := $(addsuffix .o, $(addprefix $(obj_prefix), $(sf_modules)))

# link obj_files to a single object-file
sac_format: $(sf_modules)
	@echo "Building $(bin_prefix)$@"
	@echo "Build start:  $$(date)"
	ld -r -o $(obj_prefix)$@.o $(sf_obj)
	@echo -e "Build finish: $$(date)\n"

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

spectral_modules := sac_format sac_spectral
spectral_obj := $(addsuffix .o, $(addprefix $(obj_prefix), $(spectral_modules)))
spectral_sac := sac_stream_fftw_test sac_stream_lowpass_test
$(spectral_sac): %:$(test_prefix)%.cpp $(spectral_modules)
	@echo "Building $(test_bin_prefix)$@"
	@echo "Build start:  $$(date)"
	@test -d $(test_bin_prefix) || mkdir -p $(test_bin_prefix)
	$(cxx) -o $(test_bin_prefix)$@ $< $(spectral_obj) $(fftw_params)
	@echo -e "Build finish: $$(date)\n"
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
