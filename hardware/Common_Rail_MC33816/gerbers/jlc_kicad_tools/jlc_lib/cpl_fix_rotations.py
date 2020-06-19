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

import csv
import re
import sys
import logging

# JLC requires columns to be named a certain way.
HEADER_REPLACEMENT_TABLE={
  "Ref": "Designator",
  "PosX": "Mid X",
  "PosY": "Mid Y",
  "Rot": "Rotation",
  "Side": "Layer"
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
            row[rotation_index] = "{0:.6f}".format((float(row[rotation_index]) + correction) % 360)
            break
      writer.writerow(row)
  return True
