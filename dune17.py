# dune17c-datasets.py

# David Adams
# July 2017

# List of available DUNE17 datasets.
def dune17cDatasets(isBad =False, isFail=False, skipSel=False):
  pre = "DUNE17-cold_chip"
  dss = []
  baddss = []
  faildss = []
  baddss.append(pre + "1")
  baddss.append(pre + "2_0702")          # waveform very small
  dss.append(pre + "2_0705")
  baddss.append(pre + "3_0702T15")       # square wave
  dss.append(pre + "3_0702T16")
  dss.append(pre + "6")
  dss.append(pre + "7")
  dss.append(pre + "8_0704")
  baddss.append(pre + "8_0713")   # Bad waveforms.
  dss.append(pre + "9")
  dss.append(pre + "10")
  dss.append(pre + "11")
  baddss.append(pre + "12_0701T16")      # square wave
  baddss.append(pre + "12_0701T17")      # square wave
  baddss.append(pre + "12_0705T1141")    # waveform is missing
  dss.append(pre + "12_0705T1142")
  for chip in range(13,17): dss.append(pre + str(chip))
  dss.append(pre + "18")
  baddss.append(pre + "19")
  dss.append(pre + "20")
  dss.append(pre + "21_0626")
  dss.append(pre + "21_0706")
  dss.append(pre + "22_0627")
  dss.append(pre + "22_0706")
  dss.append(pre + "23_0627")
  dss.append(pre + "23_0706")
  dss.append(pre + "24_0627")
  baddss.append(pre + "24_0707")
  dss.append(pre + "25_0627")
  baddss.append(pre + "25_0706")
  dss.append(pre + "26")
  baddss.append(pre + "27_0628")
  dss.append(pre + "27_0707")
  baddss.append(pre + "28")
  dss.append(pre + "29_0630")
  dss.append(pre + "29_0701")
  dss.append(pre + "29_0706")
  baddss.append(pre + "30_0630")    # square wave
  dss.append(pre + "30_0703")
  for chip in range(39,61): dss.append(pre + str(chip))
  dss.append(pre + "61_0626")
  dss.append(pre + "61_0628")
  baddss.append(pre + "62")
  dss.append(pre + "63_0628T17")
  dss.append(pre + "63_0628T21")
  dss.append(pre + "63_0628T22")
  dss.append(pre + "64")
  baddss.append(pre + "65_0628T193")    # waveform near zero
  baddss.append(pre + "65_0628T194")    # waveform near zero
  baddss.append(pre + "65_0628T195")    # waveform near zero
  baddss.append(pre + "65_0628T200")    # waveform near zero
  dss.append(pre + "65_0629")
  dss.append(pre + "66")
  dss.append(pre + "67_0629T10")
  dss.append(pre + "67_0629T14")
  dss.append(pre + "69")
  dss.append(pre + "70")
  dss.append(pre + "71_0628T12")
  dss.append(pre + "71_0628T17")
  dss.append(pre + "72")    # waveforms a bit ugly
  dss.append(pre + "74")
  baddss.append(pre + "75_0628T2220")  # bad test?
  baddss.append(pre + "75_0628T2228")  # bad test?
  baddss.append(pre + "75_0629")       # bad waveform?
  for chip in range(78,83): dss.append(pre + str(chip))
  dss.append(pre + "84")
  baddss.append(pre + "85")
  dss.append(pre + "86")
  dss.append(pre + "87")
  baddss.append(pre + "88")
  baddss.append(pre + "89")
  dss.append(pre + "90")
  dss.append(pre + "92_0630T13")
  dss.append(pre + "92_0630T17")
  for chip in range(93,107): dss.append(pre + str(chip))
  dss.append(pre + "108")
  baddss.append(pre + "109")
  dss.append(pre + "110")
  dss.append(pre + "111")
  dss.append(pre + "112")
  dss.append(pre + "113")
  dss.append(pre + "114_0710")
  baddss.append(pre + "114_0711")
  dss.append(pre + "115")
  baddss.append(pre + "116")
  baddss.append(pre + "117")
  dss.append(pre + "118")
  dss.append(pre + "119")
  dss.append(pre + "120")
  baddss.append(pre + "121_0711T16")   # waveform is very small
  baddss.append(pre + "121_0711T18")   # No wave
  dss.append(pre + "121_0713")
  dss.append(pre + "122")
  baddss.append(pre + "123")
  dss.append(pre + "124")
  dss.append(pre + "125")
  baddss.append(pre + "126")
  dss.append(pre + "127")
  baddss.append(pre + "128")
  dss.append(pre + "129")
  baddss.append(pre + "130")   # waveforms near zero
  baddss.append(pre + "131")
  dss.append(pre + "132")
  dss.append(pre + "133_0711")
  dss.append(pre + "133_0712")
  baddss.append(pre + "135")   # Bad waveforms
  dss.append(pre + "136")
  dss.append(pre + "138")
  baddss.append(pre + "144")  # Distorted waveforms
  dss.append(pre + "194")
  baddss.append(pre + "199")   # Bad waveforms
  dss.append(pre + "201")
  dss.append(pre + "205")
  dss.append(pre + "206_0717T16")
  dss.append(pre + "206_0717T20")
  dss.append(pre + "207")
  baddss.append(pre + "211")  # Some waveforms are bad
  dss.append(pre + "212")
  dss.append(pre + "213")
  faildss.append(pre + "349")  # crashes in extremum finding for channel 15
  dss.append(pre + "362")
  dss.append(pre + "363_0712T13")
  baddss.append(pre + "363_0712T15")   # distorted waveforms
  dss.append(pre + "364")
  dss.append(pre + "365")
  baddss.append(pre + "366_0712T1810")   # waveform empty
  dss.append(pre + "366_0712T1812")
  dss.append(pre + "368")
  dss.append(pre + "369")
  dss.append(pre + "370")
  dss.append(pre + "371")
  dss.append(pre + "372")
  dss.append(pre + "373")
  dss.append(pre + "374")
  dss.append(pre + "376")
  baddss.append(pre + "378")   # Bad waveforms
  dss.append(pre + "379")
  dss.append(pre + "380")
  faildss.append(pre + "382")   # Voltage calibration fails
  dss.append(pre + "383")
  dss.append(pre + "384")
  dss.append(pre + "385")
  dss.append(pre + "386")
  baddss.append(pre + "389")   # Bad waveforms
  dss.append(pre + "387")
  dss.append(pre + "390")
  dss.append(pre + "392")
  dss.append(pre + "393")
  baddss.append(pre + "391")
  dss.append(pre + "394_0715")
  dss.append(pre + "394_0717")
  baddss.append(pre + "-999999999")
  if isBad: outsams = baddss
  elif isFail: outsams = faildss
  else: outsams = dss  
  if skipSel:
    skipChips = selChips()
    keepsams = []
    for sam in outsams:
      chip = dune17cChip(sam)
      print "Chip " + str(chip)
      if chip not in skipChips:
        keepsams.append(sam)
    return keepsams
  return outsams
 
# List of available DUNE17ts-cold datasets.
def dune17tscDatasets(isBad =False, isFail=False, skipSel=False):
  pre = "DUNE17ts-cold_chip"
  dss = []
  baddss = []
  faildss = []
  dss.append(pre + "160")
  dss.append(pre + "161")
  dss.append(pre + "162")
  dss.append(pre + "163")
  dss.append(pre + "164")
  dss.append(pre + "165")
  dss.append(pre + "166")
  dss.append(pre + "168")
  dss.append(pre + "169")
  dss.append(pre + "170")
  dss.append(pre + "171")
  dss.append(pre + "172")
  dss.append(pre + "173")
  dss.append(pre + "174")
  dss.append(pre + "175")
  dss.append(pre + "176")
  dss.append(pre + "177")
  dss.append(pre + "178")
  dss.append(pre + "179")
  dss.append(pre + "180")
  dss.append(pre + "181")
  dss.append(pre + "182")
  dss.append(pre + "183")
  dss.append(pre + "184")
  dss.append(pre + "185")
  if isBad: outsams = baddss
  elif isFail: outsams = faildss
  else: outsams = dss  
  if skipSel:
    skipChips = selChips()
    keepsams = []
    for sam in outsams:
      chip = dune17cChip(sam)
      if chip not in skipChips:
        keepsams.append(sam)
    return keepsams
  return outsams

# Return the chip number for a dataset.
def dune17cChip(ds):
  ipos = ds.find("_chip")
  ads = ds[ipos+5:]
  ipos = ads.find("_")
  if ipos >= 0: schip = ads[:ipos]
  else: schip = ads
  return int(schip)
 
# Return all datasets for a given chip number.
def dune17cChipDatasets(chip):
  dso = []
  for ds in dune17cDatasets():
    if dune17cChip(ds) == chip:
      dso.append(ds)
  return dso

# Return the suffix for a dataset.
def dune17cSuffix(ds):
  ads = ds[16:]
  ipos = ads.find("_")
  if ipos >= 0: suf = ads[ipos+1:]
  else: suf = ""
  return suf

# Return the chips in DUNE17-cold
def dune17cChips(skipSel=False):
  chips = []
  skipChips = []
  if skipSel: skipChips = selChips()
  for ds in dune17cDatasets():
    chip = dune17cChip(ds)
    if chip not in chips:
      if chip not in skipChips:
        chips.append(chip)
  return chips

# Return the chips in DUNE17ts-cold
def dune17tscChips(skipSel=False):
  chips = []
  skipChips = []
  if skipSel: skipChips = selChips()
  for ds in dune17tscDatasets():
    chip = dune17cChip(ds)
    if chip not in chips:
      if chip not in skipChips:
        chips.append(chip)
  return chips

# Datasets with single ramp in protoDUNE test stand.
def dune17csrDatasets():
  dss = []

# Selected chips.
selApa1 = [1, 2, 3, 7, 8, 9, 12, 13, 14, 15, 18, 20, 22, 25, 39, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 60, 64, 65, 67, 70, 71, 74, 78, 79, 80, 81, 82, 84, 86, 87, 90, 92, 93, 94, 95, 96, 97, 101, 105, 108, 110, 114, 115, 118, 119, 124, 127, 129, 132, 136, 138, 363, 364, 365, 366, 369, 373, 379, 380, 383, 384, 385, 387]
def selChips():
  return selApa1
