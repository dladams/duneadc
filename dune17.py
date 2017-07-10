# dune17c-datasets.py

# David Adams
# July 2017

# List of available DUNE17 datasets.

ds0710 = False

def setDs0710():
  global ds0710
  ds0710 = True

def dune17cDatasets():
  pre = "DUNE17-cold_chip"
  global ds0710  # For 7/10 processing
  dss = []
  dss.append(pre + "2_0705")
  dss.append(pre + "3_0702T16")
  for chip in range(6,12): dss.append(pre + str(chip))
  dss.append(pre + "12_0705T1142")
  for chip in range(13,17): dss.append(pre + str(chip))
  dss.append(pre + "18")
  if ds0710: dss.append(pre + "19")
  dss.append(pre + "20")
  if ds0710: dss.append(pre + "21")
  else:      dss.append(pre + "21_0626")
  dss.append(pre + "21_0706")
  dss.append(pre + "22_0627")
  dss.append(pre + "22_0706")
  if ds0710: dss.append(pre + "23")
  else:      dss.append(pre + "23_0627")
  dss.append(pre + "23_0706")
  if ds0710: dss.append(pre + "24")
  else:      dss.append(pre + "24_0627")
  if ds0710: dss.append(pre + "25")
  else:      dss.append(pre + "25_0627")
  if not ds0710: dss.append(pre + "25_0706")
  dss.append(pre + "26")
  dss.append(pre + "27_0707")
  dss.append(pre + "29_0630")
  dss.append(pre + "29_0701")
  dss.append(pre + "29_0706")
  dss.append(pre + "30_0703")
  for chip in range(39,61): dss.append(pre + str(chip))
  dss.append(pre + "61_0626")
  dss.append(pre + "61_0628")
  dss.append(pre + "63_0628T17")
  dss.append(pre + "63_0628T21")
  dss.append(pre + "63_0628T22")
  dss.append(pre + "64")
  dss.append(pre + "65_0629")       # Four 0628 datasets are bad
  dss.append(pre + "66")
  dss.append(pre + "67_0629T10")
  dss.append(pre + "67_0629T14")
  dss.append(pre + "69")
  dss.append(pre + "70")
  dss.append(pre + "71_0628T12")
  dss.append(pre + "72")
  dss.append(pre + "74")
  # 75 0628 two tests look bad. 0629 ADC looks bad.
  for chip in range(78,83): dss.append(pre + str(chip))
  dss.append(pre + "84")
  dss.append(pre + "86")
  dss.append(pre + "87")
  dss.append(pre + "90")
  dss.append(pre + "92_0630T13")
  dss.append(pre + "92_0630T17")
  for chip in range(93,107): dss.append(pre + str(chip))
  dss.append(pre + "108")
  dss.append(pre + "110")
  dss.append(pre + "112")
  return dss  
 
# Return the chip number for a dataset.
def dune17cChip(ds):
  ads = ds[16:]
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
def dune17cChips():
  chips = []
  for ds in dune17cDatasets():
    chip = dune17cChip(ds)
    if chip not in chips: chips.append(chip)
  return chips

# Datasets with single ramp in protoDUNE test stand.
def dune17csrDatasets():
  dss = []
