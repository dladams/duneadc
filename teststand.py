# teststand.py

# David Adams
# July 2017

# Info about ADC datasets from the test stand.

# March set of 77 chips.
def samplesMarch17(isBad =False):
  pre = "201703a_D"
  sams = []
  for chip in range(1, 80):
    if chip == 2: continue
    if chip == 6: continue
    ssam = pre;
    if chip < 10: ssam += "0"
    ssam += str(chip)
    sams.append(ssam)
  return sams

