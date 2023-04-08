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
endif

# Param always used
param = -std=c++20 -pedantic-errors -Wall
# Debug params only if debug is true
# Default should be to avoid shadowing, so after 6.5 add the
# -Wshadow prefix to give errors on shaowing (avoid doing it!)
# debug_param = -fanalyzer -Weffc++ -Wextra -Wsign-conversion -Werror -Wshadow -ggdb
debug_param = -fanalyzer -Weffc++ -Wextra -Wsign-conversion -Werror -ggdb
# Release params only if debug is false
release_param = -O2 -DNDEBUG

debug = true

ifeq ($(debug), true)
	params = $(param) $(debug_param)
else
	params = $(param) $(release_param)
endif

# Where the source code files are being stored
src_prefix = ./src/
# Header prefix
hdr_prefix = $(src_prefix)header/
# Used to substitute an empty string ("" doesn't seem to work)
empty :=
# Get all files in source code directory ending in .cpp
cpp_files := $(wildcard $(src_prefix)*.cpp)
# For items in cpp_files, remove source directory prefix
targets := $(subst $(src_prefix), $(empty), $(cpp_files))
# Strip off the .cpp suffix so we have the raw names
final_targets := $(subst .cpp, $(empty), $(targets))

# Debugging print out of values of variables
#$(info $$cpp_files is [${cpp_files}])
#$(info $$targets is [${targets}])
#$(info $$final_targets is [${final_targets}])


# Define a function to loop through all targets and compile them
define compile_all
	for target in $(final_targets); do\
		echo Making: $$target;\
		echo $(cxx) -o $$target $(src_prefix)$$target.cpp -I$(hdr_prefix) $(params);\
		echo "";\
		$(cxx) -o $$target $(src_prefix)$$target.cpp -I$(hdr_prefix) $(params);\
	done;
endef

all:
	$(call compile_all)

clean:
	rm -r $(final_targets) *.dSYM
