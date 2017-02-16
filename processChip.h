// processChip.h

// Script that creates the plots and fills calib tree for one
// or more channel in a chip.
//   ssam: chip sample name, e.g. 201701_14
//   icha1: First channel to process
//   ncha: # channels to process
//   savecalib: If true, the calibration tree is updated for these channels
//   vmin, vmax, nv: if nv>0 and vmax > vmin, then

#ifndef processChip_H
#define processChip_H

#include <string>

void processChip(std::string ssam,
                 unsigned int icha1 =0,
                 unsigned int ncha =0,
                 bool savecalib =false,
                 float vmin = 0.0,
                 float vmax = 0.0,
                 float nv = 0);

#endif
