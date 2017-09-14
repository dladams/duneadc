# dune17Chip.py
#
# David Adams
# August 2017

# Return the chip number for a sample.
# Deduces the chip number from the sample name after
# If sample name is AAA:BBB, then the chip number is deduced from BBB.
# ..._chipDVVVVV -> number 10000 + VVVV
def dune17cChip(sam):
  ipos = sam.find(":")
  csam = sam[ipos+1:]
  ipos = csam.find("_chip")
  asam = csam[ipos+5:]
  chipoff = 0
  if asam[0] == "D":
    asam = asam[1:]
    chipoff = 10000
  ipos = asam.find("_")
  if ipos >= 0: schip = asam[:ipos]
  else: schip = asam
  if schip.isdigit():
    ichip = int(schip)
    if ichip < 0: return 0
    if ichip > 10000: return 0
    return ichip + chipoff
  return 0

