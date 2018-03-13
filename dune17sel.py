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
  # 10. Selection 19nov2017 from Matt.
  selChipLists.append([11293, 11514, 11358, 11598, 11408, 11462, 11547, 11583, 11573, 11545, 11417, 11581, 11599, 11572, 11111, 11373, 11474, 11290, 11453, 11501, 11426, 11535, 11377, 11488, 11387, 11359, 11356, 11506, 11588, 11523, 11498, 11560, 11480, 11378, 11436, 11024, 11429, 11526, 11466, 11482, 11475, 11434, 11595, 11489, 11420, 11582, 11369, 11370, 11524, 11499, 11411, 11587, 11600, 11442, 11366, 11438, 11354, 11412, 11391, 11401, 11220, 10266, 10976, 11030, 11310, 10827, 11379, 10681, 10847, 11190, 10712, 10472, 10489, 11444, 10738, 10810, 11504, 11215, 11048, 10581])
  # 11. Selection 22nov2017 from Matt (replacement chip).
  selChipLists.append([11446])
  # 12. Selection 10dec2017 from Matt.
  selChipLists.append([11342, 11780, 11755, 11733, 11803, 11637, 11691, 11800, 11610, 11816, 11814, 11611, 11612, 11842, 11738, 11836, 11707, 11697, 11812, 11759, 11711, 11808, 11615, 11306, 11822, 10395, 11705, 11748, 11650, 11495, 11643, 11770, 11651, 11811, 11690, 11609, 11777, 11341, 11616, 11728, 11773, 10689, 11692, 11752, 11714, 11744, 11454, 11641, 11608, 11802, 11778, 11757, 11245, 11775, 11793, 11699, 11642, 11701, 11820, 11739, 11807, 11635, 11747, 11654, 11740, 11621, 11681, 11632, 11676, 11762, 11782, 11630, 11826, 11662, 11620, 11751, 11703, 11634, 11735, 11694, 11661, 11790, 11763, 11783, 11727, 11798, 11712, 11400, 11742, 11633, 11682, 11704, 10972, 11658, 11829, 11716, 11718, 11825, 11631, 11397, 10811, 10734, 11073, 11656, 10892, 10319, 11389, 10341, 11490, 10501, 10254, 10926, 11217, 11502, 11484, 10988, 11470, 10422, 11028, 11487])
  # 13. Selection 17jan2018 from Matt.
  selChipLists.append([11866, 11862, 12020, 11904, 11955, 12061, 11970, 11843, 11995, 12082, 11957, 12019, 11997, 12151, 12031, 11993, 12110, 12100, 12043, 12054, 11981, 11845, 11859, 12088, 12077, 12058, 11854, 12015, 11994, 11909, 11987, 11897, 12132, 12142, 11869, 12147, 12128, 11915, 11998, 11983, 12060, 12072, 11985, 11924, 11989, 12134, 12064, 12117])
  # 14. Selected as replacement 17jan2018 from Matt.
  selChipLists.append([11857, 11850, 11959, 11930, 11919, 11879, 11968, 11948, 11865, 11902])
  # 15. Selection 23jan2018 from Matt.
  selChipLists.append([12247, 12196, 12208, 12204, 12243, 12244, 12181, 12092, 12219, 12163, 12119, 12008, 11947, 12225, 12201, 12074, 12094, 12230, 12044, 12199, 12098, 12079, 12187, 12104, 12216, 12197, 12189, 12083, 12173, 12097, 12041, 11882, 12218, 12096, 12010, 11935, 12140, 12158, 12211, 12188, 12175, 12039, 12037, 12202, 12171, 12067, 12177, 11991])
  # 16. Selected as replacement 23jan2018 from Matt.
  selChipLists.append([11942])
  # 17. Selection 03feb2018 from Matt.
  selChipLists.append([12278, 12499, 12284, 12354, 12473, 12514, 12453, 12306, 12513, 12536, 12392, 12550, 12487, 12528, 12566, 12292, 12373, 12449, 12253, 12343, 12483, 12420, 12488, 12494, 12024, 12501, 12342, 12095, 12365, 12299, 12409, 12332, 12541, 12341, 12404, 12331, 12316, 12493, 12461, 12481, 12329, 12382, 12446, 12476, 12349, 12377, 12531, 12280, 12127, 12500, 12258, 12522, 12526, 12311, 12520, 12394, 12347, 12406, 12470, 12506, 12414, 12374, 12356, 12320, 12410, 12561, 12457, 12309, 12564, 12357, 12303, 12504, 12327, 12263, 12289, 12348, 12407, 12421, 12338, 12468, 12312, 12519, 12259, 12298, 12388, 12400, 12326, 12273, 12318, 12300, 12361, 12510, 12497, 12478, 12402, 12456])
  # 18. Selected as replacements 05feb2018 from Matt.
  selChipLists.append([11884, 11846])
  # 19. Selection 11feb2018 from Matt.
  selChipLists.append([12656, 12622, 12734, 12625, 12702, 12676, 12649, 12645, 12632, 12623, 12684, 12714, 12617, 12693, 12766, 12659, 12629, 12652, 12590, 12540, 12708, 12636, 12703, 12750, 12660, 12640, 12579, 12699, 12709, 12756, 12679, 12665, 12637, 12732, 12701, 12692, 12753, 12600, 12618, 12591, 12544, 12694, 12655, 12642, 12746, 12715, 12641, 12761, 12673, 12768, 12752, 12764, 12585, 12744, 12607, 12666, 12668, 12605, 12759, 12670, 12635, 12235, 12245, 12345])
  # 20. Selected as replacement 11feb2018 from Matt.
  selChipLists.append([12282])
  # 21. Selected as replacements 14feb2018 from Matt.
  selChipLists.append([11899, 12680])
  # 22. Selection 16feb2018 from Matt.
  selChipLists.append([12878, 12826, 12863, 12783, 12840, 12785, 12790, 12839, 12948, 12895, 12866, 12905, 12791, 12823, 12885, 12859, 12931, 12784, 12812, 12795, 12779, 12927, 12767, 12822, 12911, 12782, 12864, 12890, 12920, 12828, 12817, 12916, 12842, 12851, 12774, 12789, 12943, 12892, 12872, 12889, 12815, 12850, 12800, 12879, 12820, 12942, 12875, 12946])
  # 23. Selected as replacement 16feb2018 from Matt.
  selChipLists.append([12871])
  # 24. Selected as replacement 24feb2018 from Matt.
  selChipLists.append([12680, 12345, 13044, 13045, 13046, 11856, 11941, 11852, 11896, 12269, 11894, 12314])
  # 25. Selection 09mar2018 from Matt.
  selChipLists.append([13004, 13093, 13174, 13129, 12983, 13264, 12849, 13189, 13134, 13267, 13100, 13201, 12959, 13202, 13231, 13065, 13024, 13048, 13037, 13149, 13064, 13234, 13150, 13058, 12974, 13007, 13029, 13263, 13070, 13218, 13170, 13054, 13261, 13010, 13187, 12968, 13162, 13237, 13156, 13177, 13138, 13074, 13225, 13198, 13052, 13043, 13176, 13115, 13088, 13122, 13020, 13075, 12993, 13079, 13247, 13191, 13042, 12977, 13109, 13013, 13092, 13060, 12991, 13106, 12984, 13182, 13221, 13166, 13083, 12954, 13117, 13095, 13040, 13087, 12963, 13203, 13209, 13047, 13194, 13155])
  # 26. Selected as replacement 24feb2018 from Matt.
  selChipLists.append([13108])
  # 27. Selected as replacement 12mar2018 from Matt.
  selChipLists.append([13279, 13285, 13298, 13300])
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
