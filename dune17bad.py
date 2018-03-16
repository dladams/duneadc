# dune17bad.py

# David Adams
# August 2017

# Files declared bad.
def badFiles():
  files = []
  files.append("adcTestData_20170803T191545_chipd0013_adcClock0_adcOffset-1_sampleRate2000000_functype3_freq4.000_offset0.700_amplitude1.000.root")    # mislabeled chip
  files.append("adcTestData_20170807T162003_chipD0060_adcClock0_adcOffset-1_sampleRate2000000_functype3_freq4.000_offset0.700_amplitude1.000.root")  # warm test.
  files.append("adcDavidAdamsOnlyData_20170807T160827_chipD0060_adcClock0_adcOffset-1_sampleRate2000000_functype3_freq4.000_offset0.700_amplitude1.000_try1.root")   # warm test
  return files

# Chips declared bad.
def badChips():
  chips = []
  #chips += [133, 167, 256]     # Matt "BAD" 23jul2017 These are FE numbers--not ADC numbers.
  #chips += [49, 64]     # Matt "Damaged" 23jul2017. These are FE numbers--not ADC numbers.
  chips += [359, 111, 23, 284]  #  Matt missing chips 24jul2017
  chips += [83]  # Elizabeth reports bent pin 25jul2017
  chips += [75]  # Ivan Furic talk at BNL CE meeting 28jul2017
  chips += [109, 378]   # Matt missing chips 31jun2017
  chips.remove(284)   # This chip was tested on CETS on 7/31
  chips += [10003]  # Elizabeth reports this chip has bent pin and will never be used 8/1/2017
  chips += [378]    # Elizabeth reports this chip is damaged 8/2/2017
  chips += [10088, 10157, 10214, 10117, 10044, 10183]    # Matt reports these are damaged 8/24/2017
  chips += [10578, 10267, 10428, 10636]    # Matt reports these are damaged 9/30/2017
  chips += [10363, 10326]    # Matt reports these are missing 9/30/2017
  chips += [10012, 10030, 10044, 10055, 10067, 10076, 10088, 10099, 10106, 10117, 10157, 10183, 10214, 10261, 10267, 10287, 10300, 10305, 10322, 10353, 10365, 10393, 10428, 10432, 10456, 10481, 10506, 10517, 10546, 10578, 10606, 10636, 10688, 10706, 10707, 10711, 10720, 10733, 10748, 10753, 10809, 10822, 10893, 10921, 10923, 10956, 10962]  # Matt reports these are bad 03oct2017
  chips += [10961, 11237, 11277, 10938, 10871, 11039, 11172]  # Matt reports these are bad 22oct2017
  chips += [11460, 11418, 11597, 11383, 11491, 11540, 11592]  # Matt reports these are bad 19nov2017
  chips += [11603, 11567, 11163, 10991, 10789, 10566, 10426, 10252, 10090]  # Matt: Chips with good pins sent to IA state 11/30/17
  chips += [11592, 11383, 11172, 10921, 10636, 10517, 10305, 10257, 10117]  # Matt: Chips with good pins sent to IA state 11/30/17
  chips += [11521, 11696, 11302, 11646]  # Matt: Chips with bad pins 10dec2017
  chips += [10359]                       # Matt: Missing 10dec2018
  chips += [12133, 11900]                # Matt: Bad pin 17jan2018
  #chips += [12095, 12024, 12127]         # Matt: Bad bulk resolution 23jan2018
  chips += [12210, 12138, 12048]         # Matt: Bent pin 23jan2018
  chips += [12162]                       # Matt: Bad SPI readout 23jan2018
  chips += [12336]                       # Matt: Bad pin 25jan2018
  chips += [12469]                       # Matt: Bad pin 31jan2018
  chips += [12285, 12398, 12355, 12350, 12549]   # Matt: Bad input pin 03feb18
  chips += [12425, 12360, 12430, 12539]          # Matt: Brokent input pin 03feb18
  chips += [12556]          # Matt, David: Gives inconsistent cold test results. Drop. 05feb18
  chips += [12588, 12737]   # Matt: Bad input pin 11feb2018.
  chips += [12834, 12909]   # Matt: SPI errors 15feb2018.
  chips += [12610, 12484]   # Matt: Cold setup errors 16feb2018.
  chips += [12770, 12712]   # Matt: Bent pin 16feb2018.
  chips += [13097, 12953, 13240]   # Matt: Bad input pin 09mar2018.
  chips += [13439, 13440, 13441, 13442, 13444, 13446, 13447, 13449, 13451, 13452]   # Matt: Bad input pins 16mar2018.
  chips.sort()
  lastchip = None
  nodups = []
  for chip in chips:
    if chip != lastchip: nodups.append(chip)
    lastchip = chip
  return nodups
