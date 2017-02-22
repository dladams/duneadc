{
  gSystem->SetBuildDir(".aclic");
  gROOT->ProcessLine(".L palette.cxx+");
  gROOT->ProcessLine(".L EffNBit.cxx+");
  gROOT->ProcessLine(".L histNSigma.cxx+");
  gROOT->ProcessLine(".L drawNSigma.cxx+");
  gROOT->ProcessLine(".L FileDirectory.cxx+");
  gROOT->ProcessLine(".L AdcChannelCalibration.cxx+");
  gROOT->ProcessLine(".L AdcCalibrationTree.cxx+");
  gROOT->ProcessLine(".L AdcSampleReader.cxx+");
  gROOT->ProcessLine(".L AdcVoltageResponse.cxx+");
  gROOT->ProcessLine(".L AdcVoltagePerformance.cxx+");
  gROOT->ProcessLine(".L AdcPerformanceTree.cxx+");
  gROOT->ProcessLine(".L AdcSampleAnalyzer.cxx+");
  gROOT->ProcessLine(".L AdcChipAnalyzer.cxx+");
  gROOT->ProcessLine(".L AdcChipStatus.cxx+");
  //gROOT->ProcessLine(".L processChip.cxx+");
  gROOT->ProcessLine(".L AdcHist.cxx+");   // obsolete
  gROOT->ProcessLine(".L makeplot.cxx+");   // obsolete
  gROOT->ProcessLine(".L drawChipBounds.C");
  gROOT->ProcessLine(".L drawvlimits.C");
  palette(0);
  int chan = 0;
}
