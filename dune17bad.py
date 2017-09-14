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
  #chips += [49, 64]     # Matt "Damaged" 23jul2017i. These are FE numbers--not ADC numbers.
  chips += [359, 111, 23, 284]  #  Matt missing chips 24jul2017
  chips += [83]  # Elizabeth reports bent pin 25jul2017
  chips += [75]  # Ivan Furic talk at BNL CE meeting 28jul2017
  chips += [109, 378]   # Matt missing chips 31jun2017
  chips.remove(284)   # This chip was tested on CETS on 7/31
  chips += [10003]  # Elizabeth reports this chip has bent pin and will never be used 8/1/2017
  chips += [378]    # Elizabeth reports this chip is damaged 8/2/2017
  chips += [10088, 10157, 10214, 10117, 10044, 10183]    # Matt reports these are damaged 8/24/2017
  chips.sort()
  return chips
