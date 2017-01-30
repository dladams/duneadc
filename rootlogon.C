{
  gSystem->SetBuildDir(".aclic");
  gROOT->ProcessLine(".L palette.cxx+");
  gROOT->ProcessLine(".L FileDirectory.cxx+");
  gROOT->ProcessLine(".L AdcHist.cxx+");
  gROOT->ProcessLine(".L makeplot.cxx+");
  //gROOT->ProcessLine(".L responseplots.cxx+");
  palette(0);
  int chan = 0;
}
