# dune17sel.py

# David Adams
# August 2017

# Selected chips.
def selectedChips(sel=0):
  chips = []
  selChipLists = []
  # 1. Selection 16jul2017 from Matt
  selChipLists.append([1, 2, 3, 7, 8, 9, 12, 13, 14, 15, 18, 20, 22, 25, 39, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 60, 64, 65, 67, 70, 71, 74, 78, 79, 80, 81, 82, 84, 86, 87, 90, 92, 93, 94, 95, 96, 97, 101, 105, 108, 110, 114, 115, 118, 119, 124, 127, 129, 132, 136, 138, 363, 364, 365, 366, 369, 373, 379, 380, 383, 384, 385, 387])
  # 2. Selection 23jul2017 from Matt
  selChipLists.append([316, 161, 214, 190, 225, 295, 304, 275, 201, 221, 273, 292, 176, 320, 228, 264, 357, 276, 278, 331, 189, 212, 266, 162, 147, 293, 263, 240, 277, 298, 351, 238, 257, 329, 269, 285, 261, 267, 150, 231, 325, 187, 289, 327, 163, 184, 256, 151, 282, 191, 258, 170, 317, 299, 394, 302, 148, 158, 324, 140, 204, 146, 173, 376, 274, 255, 197, 247, 287, 253, 288, 165, 301, 330, 182, 245, 217, 265, 248, 168])
  # 3. Selection 31jul2017 from Matt
  selChipLists.append([343, 193, 144, 222, 123, 172, 5, 312, 134, 341, 305, 116, 332, 178, 308, 160, 139, 338, 313, 230, 389, 279, 137, 361, 215, 19, 342, 177, 333, 211, 185, 175, 227, 188, 200, 153, 352, 131, 179, 336])
  # 4. Selection replacements 23aug2017 from Matt
  # 204->D0007
  # 376->D0018
  # 312->D0029
  # 361->D0009
  # 153->D0019
  selChipLists.append([10007, 10018, 10029, 10009, 10019])
  # 5. Selection 23aug2017 from Matt
  selChipLists.append([10004, 10192, 10161, 10034, 10180, 10137, 10199, 10221, 10163, 10002, 10186, 10206, 10176, 10120, 10213, 10162, 10037, 10222, 10225, 10174, 10032, 10204, 10215, 10042, 10014, 10130, 10022, 10041, 10205, 10017, 10202, 10210, 10195, 10184, 10236, 10069, 10047, 10123, 10207, 10159, 10039, 10211, 10104, 10033, 10185, 10239, 10148, 10197, 10235, 10113, 10111, 10156, 10152, 10179, 10138, 10234, 10212, 10135, 10160, 10103, 10200, 10171, 10124, 10021, 10028, 10151, 10125, 10196, 10105, 10025, 10182, 10081, 10209, 10020, 10016, 10023, 10166, 10074, 10241, 10057])
  # 6. Selection 30aug2017 from Matt
  selChipLists.append([133, 157, 202, 390, 381, 246, 112, 73, 141, 235])
  # 7. Selection 25sep2017 from Matt. These are for quad board testing.
  selChipLists.append([272, 280, 281, 283, 284, 286])
  # 8. Chips I see in the first 25 FEMBs.
  selChipLists.append([1, 2, 3, 5, 8, 9, 13, 14, 15, 18, 19, 20, 22, 31, 39, 41, 42, 43, 45, 46, 47, 49, 50, 51, 52, 53, 54, 55, 56, 57, 60, 62, 64, 65, 67, 70, 71, 74, 79, 80, 81, 84, 86, 87, 90, 92, 93, 94, 95, 96, 97, 101, 105, 108, 110, 112, 114, 116, 118, 119, 123, 124, 127, 129, 131, 132, 134, 136, 137, 138, 139, 140, 144, 146, 147, 148, 150, 158, 160, 161, 162, 163, 165, 168, 170, 171, 172, 173, 175, 176, 177, 178, 182, 184, 185, 187, 188, 189, 190, 191, 193, 197, 200, 201, 206, 211, 212, 214, 215, 221, 222, 225, 227, 228, 230, 231, 238, 240, 245, 246, 247, 248, 253, 255, 256, 257, 258, 263, 264, 265, 266, 267, 269, 273, 274, 275, 276, 277, 278, 279, 282, 285, 287, 288, 289, 292, 293, 294, 295, 299, 301, 302, 304, 305, 308, 313, 316, 317, 320, 324, 327, 329, 330, 331, 332, 333, 334, 336, 338, 341, 342, 343, 350, 351, 355, 357, 361, 363, 364, 366, 369, 373, 375, 379, 380, 383, 384, 385, 387, 389, 390, 10007, 10009, 10018, 10029, 10052, 10053, 10189, 10190])
  # 9. Selection 30sep2017 from Matt.
  selChipLists.append([10461, 10662, 10686, 10826, 10825, 10872, 10524, 10298, 10367, 10355, 10756, 10294, 10767, 10276, 10861, 10732, 10283, 10463, 10684, 10780, 10487, 10579, 10819, 10610, 10231, 10559, 10615, 10696, 10868, 10608, 10382, 10410, 10368, 10435, 10622, 10558, 10238, 10462, 10644, 10778, 10674, 10392, 10534, 10282, 10454, 10346, 10529, 10329, 10628, 10450, 10269, 10878, 10270, 10554, 10444, 10782, 10886, 10470, 10866, 10814, 10532, 10387, 10433, 10862, 10922, 10896, 10006, 10716, 10634, 10458, 10843, 10441, 10406, 10397, 10284, 10515, 10664, 10601, 10331, 10927, 10817, 10321, 10804, 10803, 10455, 10311, 10563, 10468, 10630, 10531, 10583, 10797, 10383, 10573, 10263, 10617, 10908, 10416, 10660, 10310, 10613, 10860, 10469, 10609, 10641, 10882, 10627, 10632, 10570, 10452, 10870, 10374, 10539, 10869, 10277, 10417, 10275, 10795, 10430, 10404, 10580, 10418, 10253, 10409, 10478, 10436, 10885, 10920, 10577, 10887, 10265, 10332, 10268, 10338, 10663, 10722, 10727, 10855, 10700, 10915, 10906, 10816, 10449, 10682, 10718, 10914, 10516, 10905, 10708, 10431, 10445, 10325, 10540, 10260, 10394, 10243, 10635, 10288, 10849, 10802])
  # 10. Selection 22oct2017 from Matt.
  selChipLists.append([11211, 10934, 11210, 11057, 11155, 11053, 10193, 11332, 10951, 10933, 11335, 10978, 11301, 11006, 10963, 11036, 11019, 11347, 11026, 11099, 11322, 10710, 11315, 10954, 10798, 11067, 11334, 11321, 10944, 11202, 10421, 11076, 10969, 11075, 11279, 11000, 10971, 11273, 11072, 11297, 11285, 10953, 11044, 11294, 10982, 11222, 10986, 11238, 11173, 11289, 11040, 11020, 11125, 11144, 11001, 11022, 11079, 11050, 11291, 11346, 10763, 11118, 10964, 11130, 11011, 11017, 11253, 10942, 11259, 11032, 11267, 10997, 11254, 10939, 11018, 11184, 11270, 11180, 11043, 10792])
  # 10. Selection 22oct2017 from Matt.
  selChipLists.append([11293, 11514, 11358, 11598, 11408, 11462, 11547, 11583, 11573, 11545, 11417, 11581, 11599, 11572, 11111, 11373, 11474, 11290, 11453, 11501, 11426, 11535, 11377, 11488, 11387, 11359, 11356, 11506, 11588, 11523, 11498, 11560, 11480, 11378, 11436, 11024, 11429, 11526, 11466, 11482, 11475, 11434, 11595, 11489, 11420, 11582, 11369, 11370, 11524, 11499, 11411, 11587, 11600, 11442, 11366, 11438, 11354, 11412, 11391, 11401, 11220, 10266, 10976, 11030, 11310, 10827, 11379, 10681, 10847, 11190, 10712, 10472, 10489, 11444, 10738, 10810, 11504, 11215, 11048, 10581])
  # Build ouput list for this request.
  chips = []
  if sel == 0:
    chips = []
    for selchips in selChipLists: chips += selchips
  elif sel <= len(selChipLists):  chips = selChipLists[sel-1]
  chips.sort()
  nodups = []
  lastchip = None
  for chip in chips:
    if chip != lastchip: nodups.append(chip)
    lastchip = chip
  return nodups
