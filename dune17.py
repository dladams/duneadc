# dune17c-datasets.py

# David Adams
# July 2017

from dune17cSamples import dune17cSamples
from dune17dlacSamples import dune17dlacSamples
 
# List of available DUNE17ts-cold datasets.
def dune17tscSamples(isBad =False, isFail=False, skipSel=False, skipBad=True, isDNL=False):
  pre = "DUNE17ts-cold_chip"
  sams = []
  dnlsams = []
  badsams = []
  failsams = []
  sams.append(pre + "4")
  sams.append(pre + "5")
  dnlsams.append(pre + "17_DNL")
  dnlsams.append(pre + "21_DNL")
  dnlsams.append(pre + "31_DNL")
  dnlsams.append(pre + "32_DNL")
  dnlsams.append(pre + "33_DNL")
  dnlsams.append(pre + "34_DNL")
  dnlsams.append(pre + "35_DNL")
  dnlsams.append(pre + "36_DNL")
  dnlsams.append(pre + "37")
  dnlsams.append(pre + "38_DNL")
  dnlsams.append(pre + "47_DNL")
  dnlsams.append(pre + "68_DNL")
  dnlsams.append(pre + "76_DNL")
  dnlsams.append(pre + "77_DNL")
  dnlsams.append(pre + "88_DNL")
  dnlsams.append(pre + "89_DNL")
  dnlsams.append(pre + "91")
  sams.append(pre + "109")
  sams.append(pre + "116")
  sams.append(pre + "123")
  sams.append(pre + "126")
  sams.append(pre + "128")
  sams.append(pre + "130")
  sams.append(pre + "131")
  sams.append(pre + "134")
  sams.append(pre + "135")
  sams.append(pre + "142")
  dnlsams.append(pre + "143_DNL07_25_16")
  dnlsams.append(pre + "143_DNL07_25_18")
  sams.append(pre + "144")
  sams.append(pre + "153")
  sams.append(pre + "159")
  sams.append(pre + "160_0717")
  sams.append(pre + "160_0727")
  sams.append(pre + "161")
  sams.append(pre + "162")
  sams.append(pre + "163")
  sams.append(pre + "164")
  sams.append(pre + "165")
  sams.append(pre + "166_0717")
  sams.append(pre + "166_0724")
  sams.append(pre + "168")
  sams.append(pre + "169")
  sams.append(pre + "170")
  sams.append(pre + "171")
  sams.append(pre + "172")
  sams.append(pre + "173")
  sams.append(pre + "174")
  sams.append(pre + "175_0717")
  sams.append(pre + "175_0724")
  sams.append(pre + "176")
  sams.append(pre + "177_0717")
  sams.append(pre + "177_0724")
  sams.append(pre + "178")
  sams.append(pre + "179")
  sams.append(pre + "180")
  sams.append(pre + "181")
  sams.append(pre + "182")
  sams.append(pre + "183_0717")
  sams.append(pre + "183_0724")
  sams.append(pre + "184")
  sams.append(pre + "185_0717")
  sams.append(pre + "185_0727")
  sams.append(pre + "186_0718")
  sams.append(pre + "186_0724")
  sams.append(pre + "187")
  sams.append(pre + "188")
  sams.append(pre + "189")
  sams.append(pre + "199")
  sams.append(pre + "203")
  sams.append(pre + "211")
  sams.append(pre + "215")
  sams.append(pre + "219")
  sams.append(pre + "241")
  sams.append(pre + "243")
  sams.append(pre + "249")
  sams.append(pre + "250")
  sams.append(pre + "251")
  sams.append(pre + "252")
  sams.append(pre + "253")
  sams.append(pre + "254")
  sams.append(pre + "255")
  sams.append(pre + "256")
  sams.append(pre + "257")
  sams.append(pre + "258")
  sams.append(pre + "259_0718")
  sams.append(pre + "259_0725")
  sams.append(pre + "260")
  sams.append(pre + "261")
  sams.append(pre + "262")
  sams.append(pre + "263")
  sams.append(pre + "264")
  sams.append(pre + "265")
  sams.append(pre + "266")
  sams.append(pre + "267")
  sams.append(pre + "268")
  sams.append(pre + "269")
  sams.append(pre + "270")
  sams.append(pre + "272")
  sams.append(pre + "273")
  sams.append(pre + "274")
  sams.append(pre + "275")
  sams.append(pre + "276")
  sams.append(pre + "277")
  sams.append(pre + "278")
  sams.append(pre + "279")
  sams.append(pre + "280")
  sams.append(pre + "281")
  sams.append(pre + "282")
  sams.append(pre + "283")
  sams.append(pre + "284_0720")
  sams.append(pre + "284_0731")
  sams.append(pre + "285")
  sams.append(pre + "286")
  sams.append(pre + "287")
  sams.append(pre + "288_0720")
  #sams.append(pre + "288_0727")  # Actually chip 388 Junbin Zhang 7/28/2017
  sams.append(pre + "289")
  sams.append(pre + "290")
  sams.append(pre + "291")
  sams.append(pre + "292")
  sams.append(pre + "293")
  sams.append(pre + "295")
  sams.append(pre + "296")
  sams.append(pre + "298")
  sams.append(pre + "299")
  sams.append(pre + "300")
  sams.append(pre + "301")
  sams.append(pre + "302")
  sams.append(pre + "304")
  sams.append(pre + "305")
  sams.append(pre + "306")
  sams.append(pre + "307")
  sams.append(pre + "308")
  sams.append(pre + "309")
  sams.append(pre + "310")
  sams.append(pre + "311")
  sams.append(pre + "312")
  sams.append(pre + "313_0725")
  sams.append(pre + "313_0731")
  sams.append(pre + "314")
  sams.append(pre + "316")
  sams.append(pre + "317")
  sams.append(pre + "318")
  sams.append(pre + "319")
  sams.append(pre + "320")
  sams.append(pre + "321")
  sams.append(pre + "322")
  sams.append(pre + "323")
  sams.append(pre + "324")
  sams.append(pre + "325")
  sams.append(pre + "326")
  sams.append(pre + "327")
  sams.append(pre + "328")
  sams.append(pre + "329")
  sams.append(pre + "333")
  sams.append(pre + "335")
  sams.append(pre + "339")
  sams.append(pre + "348")
  sams.append(pre + "350")
  sams.append(pre + "355")
  sams.append(pre + "375")
  sams.append(pre + "386")
  sams.append(pre + "378")
  sams.append(pre + "388_fchip288_0727")  # Junbin Zhang 7/28/2017
  sams.append(pre + "D0001_0731")
  sams.append(pre + "D0002_0731")
  sams.append(pre + "D0003_0731")
  sams.append(pre + "D0004_0731")
  sams.append(pre + "D0005_0731")
  sams.append(pre + "D0006_0731")
  sams.append(pre + "D0007_0731")
  sams.append(pre + "D0008_0731")
  sams.append(pre + "D0009_0731")
  sams.append(pre + "D0010_0801")
  sams.append(pre + "D0011_0731")
  sams.append(pre + "D0012_0731")
  sams.append(pre + "D0013_0731")
  sams.append(pre + "D0014_0731")
  sams.append(pre + "D0015_0731")
  sams.append(pre + "D0016_0731")
  sams.append(pre + "D0017_0731")
  sams.append(pre + "D0018_0731")
  sams.append(pre + "D0019_0731")
  sams.append(pre + "D0020_0731")
  sams.append(pre + "D0021_0731")
  sams.append(pre + "D0022_0731")
  sams.append(pre + "D0023_0731")
  sams.append(pre + "D0024_0731")
  sams.append(pre + "D0025_0731")
  sams.append(pre + "D0026_0731")
  sams.append(pre + "D0027_0731")
  sams.append(pre + "D0028_0731")
  sams.append(pre + "D0029_0731")
  sams.append(pre + "D0030_0731")
  if isBad: outsams = badsams
  elif isFail: outsams = failsams
  elif isDNL: outsams = dnlsams
  else: outsams = sams + dnlsams
  if skipSel or skipBad:
    skipChips = []
    if skipSel: skipChips += selectedChips()
    if skipBad: skipChips += badChips()
    keepsams = []
    for sam in outsams:
      chip = dune17cChip(sam)
      if chip not in skipChips:
        keepsams.append(sam)
    return keepsams
  return outsams

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

# Selected chips.
def selectedChips(sel=0):
  chips = []
  selChipLists = []
  # 0. Selection 16jul2017 from Matt
  selChipLists.append([1, 2, 3, 7, 8, 9, 12, 13, 14, 15, 18, 20, 22, 25, 39, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 60, 64, 65, 67, 70, 71, 74, 78, 79, 80, 81, 82, 84, 86, 87, 90, 92, 93, 94, 95, 96, 97, 101, 105, 108, 110, 114, 115, 118, 119, 124, 127, 129, 132, 136, 138, 363, 364, 365, 366, 369, 373, 379, 380, 383, 384, 385, 387])
  # 1. Selection 23jul2017 from Matt
  selChipLists.append([316, 161, 214, 190, 225, 295, 304, 275, 201, 221, 273, 292, 176, 320, 228, 264, 357, 276, 278, 331, 189, 212, 266, 162, 147, 293, 263, 240, 277, 298, 351, 238, 257, 329, 269, 285, 261, 267, 150, 231, 325, 187, 289, 327, 163, 184, 256, 151, 282, 191, 258, 170, 317, 299, 394, 302, 148, 158, 324, 140, 204, 146, 173, 376, 274, 255, 197, 247, 287, 253, 288, 165, 301, 330, 182, 245, 217, 265, 248, 168])
  # 2. Selection 31jul2017 from Matt
  selChipLists.append([343, 193, 144, 222, 123, 172, 5, 312, 134, 341, 305, 116, 332, 178, 308, 160, 139, 338, 313, 230, 389, 279, 137, 361, 215, 19, 342, 177, 333, 211, 185, 175, 227, 188, 200, 153, 352, 131, 179, 336])
  # 3. Selection replacements 23aug2017 from Matt
  # 204->D0007
  # 376->D0018
  # 312->D0029
  # 361->D0009
  # 153->D0019
  selChipLists.append([10007, 10018, 10029, 10009, 10019])
  # 4. Selection 23aug2017 from Matt
  selChipLists.append([10004, 10192, 10161, 10034, 10180, 10137, 10199, 10221, 10163, 10002, 10186, 10206, 10176, 10120, 10213, 10162, 10037, 10222, 10225, 10174, 10032, 10204, 10215, 10042, 10014, 10130, 10022, 10041, 10205, 10017, 10202, 10210, 10195, 10184, 10236, 10069, 10047, 10123, 10207, 10159, 10039, 10211, 10104, 10033, 10185, 10239, 10148, 10197, 10235, 10113, 10111, 10156, 10152, 10179, 10138, 10234, 10212, 10135, 10160, 10103, 10200, 10171, 10124, 10021, 10028, 10151, 10125, 10196, 10105, 10025, 10182, 10081, 10209, 10020, 10016, 10023, 10166, 10074, 10241, 10057])
  chips = []
  if sel == 0:
    chips = []
    for selchips in selChipLists: chips += selchips
  elif sel <= len(selChipLists):  chips = selChipLists[sel]
  elif sel == 2: chips = sel2
  elif sel == 3: chips = sel3
  elif sel == 4: chips = sel4
  chips.sort()
  return chips

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
  chips += [10088, 10057, 10214, 10117, 10044, 10183]    # Matt reports these are damaged 8/24/2017
  chips.sort()
  return chips
