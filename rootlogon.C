{
  gSystem->SetBuildDir(".aclic");
  // Load and compile (if needed) the cxx files.
  ifstream fin("cxxfiles.dat");
  string name;
  while ( getline(fin, name) && !fin.eof() ) {
    if ( name.size() == 0 || name[0] == '#' ) continue;
    //cout << "  Loading " << name << endl;
    string line = ".L " + name + ".cxx+";
    gROOT->ProcessLine(line.c_str());
  }
  gROOT->ProcessLine(".L AdcHist.cxx+");   // obsolete
  gROOT->ProcessLine(".L makeplot.cxx+");   // obsolete
  gROOT->ProcessLine(".L drawChipBounds.C");
  gROOT->ProcessLine(".L drawvlimits.C");
  gROOT->ProcessLine(".L datasetAnalyze.C");
  palette(0);
  gStyle->SetTitleW(1.0);  // So title can extend full width of pad.
}
