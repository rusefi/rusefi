#!/usr/bin/python

# Copyright 2012 by Wayne C. Gramlich
#
# ki2gm.py is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# ki2gm.py is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# The complete text of the license can be found at:
#
#   http://www.gnu.org/licenses/.

# This program takes a KiCAD .drl file and converts it to
# a .drl file that can be read by the gerbmerge program.
# The gerbmege program can be found at:
#
#   http://ruggedcircuits.com/gerbmerge/

import sys

def main():
    """ ki2gm: This program takes a KiCAD .drl file an converts it
        to a .drl file that can be used by gerbmerge. """

    args = sys.argv
    if len(args) < 3:
	print "Usage: ki2gm in.drl out.drl"
    else:
	# Do argument checking:
	in_file_name = args[1]
	out_file_name = args[2]
	assert in_file_name != out_file_name, \
	  "Input file '{0}' must be different from output file name {1}". \
	  format(in_file_name, out_file_name)
	assert in_file_name.endswith(".drl"), \
	  "Input file '{0}' must end with .drl suffix".format(in_file_name)
	assert out_file_name.endswith(".drl"), \
	  "Output file '{0}' must end with .drl suffix".format(out_file_name)

	# Read in {in_file_name}:
	in_file = open(in_file_name, "r")
	lines = in_file.readlines()
	in_file.close()

	# Create {mappings} table:
	mappings = {}
	mappings["G90\n"] = ";G90\n"
	mappings["G05\n"] = ";G05\n"
	mappings["FMAT,2\n"] = ";FMAT,2\n"
	mappings["T0\n"] = ";T0\n"

	# Output the modified .drl file:
	out_file = open(out_file_name, "w")
	for line in lines:
	    # Remap any {line} that needs to be changed:
	    if line in mappings:
		line = mappings[line]
            out_file.write(line)
	out_file.close()

main()
