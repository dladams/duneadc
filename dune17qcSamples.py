# dune17qcSamples.py

# David Adams
# August 2017

from dune17bad import badChips
from dune17sel import selectedChips
from dune17Chip import dune17cChip

def dune17qcSamples(isBad =False, isFail=False, skipSel=False, skipBad=True):
  pre = "DUNE17q-cold_chip"
  # List the bad samples here.
  badsams = []
  badsams.append(pre + "D0362_0828T1558")  # 1 Mhz
  badsams.append(pre + "D0363_0828T1558")  # 1 Mhz
  badsams.append(pre + "D0365_0828T1638")  # 1 Mhz
  badsams.append(pre + "D0366_0828T1638")  # 1 Mhz
  badsams.append(pre + "D0365_0828T1752")  # 1 Mhz
  badsams.append(pre + "D0364_0828T1752")  # 1 Mhz
  badsams.append(pre + "D0352_0828T1354")  # 1 Mhz
  badsams.append(pre + "D0357_0828T1354")  # 1 Mhz
  badsams.append(pre + "D0358_0828T1841")  # 1 Mhz
  badsams.append(pre + "D0361_0828T1841")  # 1 Mhz
  badsams.append(pre + "D0359_0828T1509")  # 1 Mhz
  badsams.append(pre + "D0351_0828T1509")  # 1 Mhz
  badsams.append(pre + "D0359_0828T1714")  # 1 Mhz
  badsams.append(pre + "D0360_0828T1714")  # 1 Mhz
  badsams.append(pre + "D0352_0828T1345")  # 1 Mhz
  badsams.append(pre + "D0357_0828T1345")  # 1 Mhz
  failsams = []
  # Fetch the list of all samples from the text file.
  ifile = open("DUNE17q-cold-all.txt", "r")
  samsraw = ifile.read().splitlines()
  sams = []
  for sam in samsraw:
    if sam not in badsams: sams.append(sam)
  # Construct the output list.
  if isBad: outsams = badsams
  elif isFail: outsams = failsams
  else: outsams = sams
  if skipSel or skipBad:
    skipChips = []
    if skipSel: skipChips += selectedChips()
    if skipBad: skipChips += badChips()
    keepsams = []
    for sam in outsams:
      chip = dune17cChip(sam)
      #print "Chip " + str(chip)
      if chip not in skipChips:
        keepsams.append(sam)
    return keepsams
  return outsams
