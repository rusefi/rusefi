# Copyright (C) 2019 Matthew Lai
# Copyright (C) 1992-2019 Kicad Developers Team
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

from jlc_kicad_tools.jlc_lib import kicad_netlist_reader
import csv
import re
import logging

LCSC_PART_NUMBER_MATCHER=re.compile('^C[0-9]+$')

def GenerateBOM(input_filename, output_filename, opts):
  net = kicad_netlist_reader.netlist(input_filename)

  try:
    f = open(output_filename, mode='w', encoding='utf-8')
  except IOError:
    logging.error("Failed to open file for writing: {}".format(output_filename))
    return False

  out = csv.writer(f, lineterminator='\n', delimiter=',', quotechar='\"',
                   quoting=csv.QUOTE_ALL)

  out.writerow(['Comment', 'Designator', 'Footprint', 'LCSC Part #'])

  grouped = net.groupComponents()

  num_groups_found = 0
  for group in grouped:
    refs = []
    lcsc_part_numbers = set()
    lcsc_part_numbers_none_found = False
    footprints = set()

    for component in group:
      refs.append(component.getRef())
      c = component
      lcsc_part_number = None

      # Get the field name for the LCSC part number.
      for field_name in c.getFieldNames():
        field_value = c.getField(field_name)

        if LCSC_PART_NUMBER_MATCHER.match(field_value):
          lcsc_part_number = field_value

      if lcsc_part_number:
        lcsc_part_numbers.add(lcsc_part_number)
      else:
        lcsc_part_numbers_none_found = True

      if c.getFootprint() != '':
        footprints.add(c.getFootprint())

    # Check part numbers for uniqueness
    if len(lcsc_part_numbers) == 0:
      if opts.warn_no_partnumber:
        logging.warning("No LCSC part number found for components {}".format(",".join(refs)))
      continue
    elif len(lcsc_part_numbers) != 1:
      logging.error("Components {components} from same group have different LCSC part numbers: {partnumbers}".format(
          components = ", ".join(refs),
          partnumbers = ", ".join(lcsc_part_numbers)))
      return False
    lcsc_part_number = list(lcsc_part_numbers)[0]

    if (not opts.assume_same_lcsc_partnumber) and (lcsc_part_numbers_none_found):
      logging.error("Components {components} from same group do not all have LCSC part number {partnumber} set. Use --assume-same-lcsc-partnumber to ignore.".format(
          components = ", ".join(refs),
          partnumber = lcsc_part_number))
      return False

    # Check footprints for uniqueness
    if (len(footprints) == 0):
      logging.error("No footprint found for components {}".format(",".join(refs)))
      return False
    if len(footprints) != 1:
      logging.error("Components {components} from same group have different foot prints: {footprints}".format(
          components = ", ".join(refs),
          footprints = ", ".join(footprints)))
      return False
    footprint = list(footprints)[0]

    # They don't seem to like ':' in footprint names.
    footprint = footprint[(footprint.find(':') + 1):]

    # Fill in the component groups common data
    out.writerow([c.getValue(), ",".join(refs), footprint, lcsc_part_number])
    num_groups_found += 1

  logging.info("{} component groups found from BOM file.".format(num_groups_found))

  return True
