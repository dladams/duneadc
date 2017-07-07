# dune17c-datasets.py

# David Adams
# July 2017

# List of available DUNE17 datasets.

def dune17cDatasets():
  pre = "DUNE17-cold_chip"
  dss = []
  dss.append(pre + "2_0705")
  dss.append(pre + "3_0702T16")
  for chip in range(6,12): dss.append(pre + str(chip))
  dss.append(pre + "12_0705T1142")
  for chip in range(13,17): dss.append(pre + str(chip))
  for chip in range(18,21): dss.append(pre + str(chip))
  dss.append(pre + "21_0626")
  dss.append(pre + "21_0706")
  dss.append(pre + "22_0627")
  dss.append(pre + "22_0706")
  dss.append(pre + "23_0627")
  dss.append(pre + "23_0706")
  dss.append(pre + "24")
  dss.append(pre + "25_0627")
  dss.append(pre + "25_0706")
  dss.append(pre + "27")
  dss.append(pre + "28")
  dss.append(pre + "29_0630")
  dss.append(pre + "29_0701")
  dss.append(pre + "29_0706")
  dss.append(pre + "30_0703")
  for chip in range(39,61): dss.append(pre + str(chip))
  dss.append(pre + "61_0626")
  dss.append(pre + "61_0628")
  dss.append(pre + "62")
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
  for chip in range(84,91): dss.append(pre + str(chip))
  dss.append(pre + "92_0630T13")
  dss.append(pre + "92_0630T17")
  for chip in range(93,98): dss.append(pre + str(chip))
  for chip in range(99,102): dss.append(pre + str(chip))
  dss.append(pre + "110")
  return dss  

# Datasets with single rampl in protoDUNE test stand.
def dune17csrDatasets():
  dss = []
