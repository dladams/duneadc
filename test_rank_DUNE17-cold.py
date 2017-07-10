# test_rank_DUNE17-cold.py

# David Adams
# July 2017
#
# Test python output emitted by filldbd17c.C.

from rank_DUNE17_cold import effdst
from rank_DUNE17_cold import effrank
import dune17

nerr = 0

def testerror(msg):
  global nerr
  print "Test failed: " + msg
  nerr = nerr + 1

def testequal(lhs, rhs, msg):
  if lhs != rhs:
    testerror(msg)
    print "  " + str(lhs) + " != " + str(rhs)

print "Dataset: " + effdst
testequal(effdst, "DUNE17-cold", "Dataset name")
#testequal(effdst, "badDUNE17-cold", "Dataset name 2")

dune17.setDs0710()
chips = dune17.dune17cChips()

testequal(len(chips), len(effrank), "chips and rank sizes");

for chip in chips:
  if chip not in effrank:
    testerror("Chip " + str(chip) + " is not in effrank.")

for chip in effrank:
  if chip not in chips:
    testerror("Ranked chip " + str(chip) + " is not in chip list.")

print
if nerr:
  print "Failure count: " + str(nerr)
else:
  print "All tests passed."
