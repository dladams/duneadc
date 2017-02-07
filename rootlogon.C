{
  gSystem->SetBuildDir(".aclic");
  gROOT->ProcessLine(".L palette.cxx+");
  gROOT->ProcessLine(".L FileDirectory.cxx+");
  gROOT->ProcessLine(".L AdcChannelCalibration.cxx+");
  gROOT->ProcessLine(".L AdcCalibrationTree.cxx+");
  gROOT->ProcessLine(".L AdcSampleReader.cxx+");
  gROOT->ProcessLine(".L AdcSampleAnalyzer.cxx+");
  gROOT->ProcessLine(".L AdcHist.cxx+");
  gROOT->ProcessLine(".L makeplot.cxx+");
  palette(0);
  int chan = 0;
}
