# dune17.py

# David Adams
# July 2017

from dune17cSamples import dune17cSamples
from dune17dlacSamples import dune17dlacSamples
from dune17tscSamples import dune17tscSamples
from dune17Chip import dune17cChip
from dune17bad import badChips
from dune17bad import badFiles
from dune17sel import selectedChips
 
# Return all samples for a given chip number.
def dune17cChipSamples(chip):
  dso = []
  for ds in dune17cSamples() + dune17tscSamples():
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
def dune17cChips(skipSel=False, skipBad=True):
  chips = []
  skipChips = []
  if skipSel: skipChips += selectedChips()
  if skipBad: skipChips += badChips()
  for ds in dune17cSamples():
    chip = dune17cChip(ds)
    if chip not in chips:
      if chip not in skipChips:
        chips.append(chip)
  return chips

# Return the chips in DUNE17dla-cold
def dune17dlacChips(skipSel=False, skipBad=True):
  chips = []
  skipChips = []
  if skipSel: skipChips += selectedChips()
  if skipBad: skipChips += badChips()
  for ds in dune17dlacSamples():
    chip = dune17cChip(ds)
    if chip not in chips:
      if chip not in skipChips:
        chips.append(chip)
  return chips

# Return the chips in DUNE17ts-cold
def dune17tscChips(skipSel=False, skipBad=True):
  chips = []
  skipChips = []
  if skipSel: skipChips += selectedChips()
  if skipBad: skipChips += badChips()
  for sam in dune17tscSamples():
    chip = dune17cChip(sam)
    if chip not in chips:
      if chip not in skipChips:
        chips.append(chip)
  return chips

# Return the chips in DUNE17-cold and DUNE17ts-cold
def allChips(skipSel=False, skipBad=True):
  chips = []
  chips += dune17cChips(skipSel, skipBad)
  chips += dune17tscChips(skipSel, skipBad)
  chips.sort()
  return chips

# Return chips that appear in bad samples.
def dune17BadSampleChips(excludePdts=True, excludeCets=True):
  chips = []
  sams = []
  sams += dune17cSamples(isBad=True)
  sams += dune17tscSamples(isBad=True)
  excludeChips = []
  if excludePdts: excludeChips += dune17cChips()
  if excludeCets: excludeChips += dune17tscChips()
  for sam in sams:
    chip = dune17cChip(sam)
    if chip not in chips:
      if chip not in excludeChips:
        chips.append(chip)
  return chips

# Samples with single ramp in protoDUNE test stand.
def dune17csrSamples():
  sams = []

# Return the dataset name for a sample.
def dune17DatasetName(sam):
  if sam[:16] == "DUNE17-cold_chip": return "DUNE17-cold"
  if sam[:18] == "DUNE17ts-cold_chip": return "DUNE17ts-cold"
  return "UnknownDataset"
