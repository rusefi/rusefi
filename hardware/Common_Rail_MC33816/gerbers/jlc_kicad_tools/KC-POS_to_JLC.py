#!/usr/bin/env python3
# Copyright (C) 2019 Matthew Lai
#
# This file is part of JLC Kicad Tools.
#
# JLC Kicad Tools is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# JLC Kicad Tools is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with JLC Kicad Tools.  If not, see <https://www.gnu.org/licenses/>.

import os
import re
import sys
import argparse
import logging
import errno

import csv
import re
import sys
import logging

# JLC requires columns to be named a certain way.
HEADER_REPLACEMENT_TABLE={
  "Ref": "Designator",
  "Val": "Val",
  "Package": "PackageReference",
  "PosX": "Mid X",
  "PosY": "Mid Y",
  "Rot": "Rotation",
  "Side": "Layer",
}

ROW_REPLACEMENT_TABLE={
  "TopLayer": "Top",
  "BottomLayer": "Bottom",
}

def ReadDB(filename):
  db = {}
  with open(filename) as csvfile:
    reader = csv.reader(csvfile, delimiter=',')
    for row in reader:
      if row[0] == "Footprint pattern":
        continue
      else:
        db[re.compile(row[0])] = int(row[1])
  logging.info("Read {} rules from {}".format(len(db), filename))
  return db

def FixRotations(input_filename, output_filename, db):
  with open(input_filename) as csvfile:
    reader = csv.reader(csvfile, delimiter=',')
    writer = csv.writer(open(output_filename, 'w', newline=''), delimiter=',')
    package_index = None
    rotation_index = None
    for row in reader:
      if not package_index:
        # This is the first row. Find "Package" and "Rot" column indices.
        for i in range(len(row)):
          if row[i] == "Package":
            package_index = i
          elif row[i] == "Rot":
            rotation_index = i
        if package_index is None:
          logging.warning("Failed to find 'Package' column in the csv file")
          return False
        if rotation_index is None:
          logging.warning("Failed to find 'Rot' column in the csv file")
          return False
        # Replace column names with labels JLC wants.
        for i in range(len(row)):
          if row[i] in HEADER_REPLACEMENT_TABLE:
            row[i] = HEADER_REPLACEMENT_TABLE[row[i]]
      else:
        for pattern, correction in db.items():
          if pattern.match(row[package_index]):
            logging.info("Footprint {} matched {}. Applying {} deg correction"
                .format(row[package_index], pattern.pattern, correction))
            row[rotation_index] = "{0:.0f}".format((float(row[rotation_index]) + correction) % 360)
            break
        for i in range(len(row)):
          if row[i] in ROW_REPLACEMENT_TABLE:
            row[i] = ROW_REPLACEMENT_TABLE[row[i]]
      del row[package_index]
      writer.writerow(row)
  return True

DEFAULT_DB_PATH="cpl_KC-to-JLC-rotations_db.csv"

def main():
	parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter, description='Generates BOM and CPL in CSV fashion to be used in JLCPCB Assembly Service', prog='generate_jlc_files')
	parser.add_argument('project_dir', metavar='INPUT_DIRECTORY', type=os.path.abspath, help='Directory of KiCad project. Name should match KiCad project name.')
	parser.add_argument('-d', '--database', metavar='DATABASE', type=str, help='Filename of database', default=os.path.join(os.path.dirname(__file__), DEFAULT_DB_PATH))
	verbosity = parser.add_argument_group('verbosity arguments')
	verbosity.add_argument('-v', '--verbose', help='Increases log verbosity for each occurrence', dest='verbose_count', action="count", default=0)
	verbosity.add_argument('--warn-no-lcsc-partnumber', help='Enable warning output if lcsc part number is not found', dest='warn_no_partnumber', action='store_true')
	parser.add_argument('--assume-same-lcsc-partnumber', help='Assume same lcsc partnumber for all components of a group', action='store_true', dest='assume_same_lcsc_partnumber')
	parser.add_argument('-o', '--output', metavar='OUTPUT_DIRECTORY', dest='output_dir', type=os.path.abspath, help='Output directory. Default: INPUT_DIRECTORY')

	if (len(sys.argv) == 1):
		parser.print_help()
		sys.exit()

	# Parse arguments
	opts = parser.parse_args(sys.argv[1:])

	# Default log level is WARNING
	logging.basicConfig(format="%(message)s", level=max(logging.WARNING - opts.verbose_count * 10, logging.NOTSET))

	if not os.path.isdir(opts.project_dir):
		logging.error("Failed to open project directory: {}".format(opts.project_dir))
		return errno.ENOENT

	# Set default output directory
	if opts.output_dir == None:
		opts.output_dir = opts.project_dir

	if not os.path.isdir(opts.output_dir):
		logging.info("Creating output directory {}".format(opts.output_dir))
		os.mkdir(opts.output_dir)

	project_name = os.path.basename(opts.project_dir)
	logging.debug("Project name is '%s'.", project_name)
	cpl_filename = project_name + "-top-pos.csv"
	cpl_path = None

	for dir_name, subdir_list, file_list in os.walk(opts.project_dir):
		for file_name in file_list:
			if file_name == cpl_filename:
				cpl_path = os.path.join(dir_name, file_name)

	if cpl_path is None:
		logging.error((
			"Failed to find CPL file: {} in {} (and sub-directories). "
			"Run 'File -> Fabrication Outputs -> Footprint Position (.pos) File' in Pcbnew. "
			"Settings: 'CSV', 'mm', 'single file for board'.").format(cpl_filename, opts.project_dir))
		return errno.ENOENT

	logging.info("CPL file found at: {}".format(cpl_path))

	cpl_output_path = os.path.join(opts.output_dir, project_name + "_JLC_CPL.csv")

	db = ReadDB(opts.database)
	if FixRotations(cpl_path, cpl_output_path, db):
		logging.info("JLC CPL file written to: {}".format(cpl_output_path))
	else:
		return errno.EINVAL

	return 0

if __name__ == '__main__':
	sys.exit(main())
	