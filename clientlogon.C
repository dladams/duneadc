{
cout << 100 << endl;
  const char* ctopdir = gSystem->Getenv("DUNEADCDIR");
  if ( ctopdir == nullptr ) {
    cout << "DUNEADCDIR is not defined" << endl;
  } else {
    string topdir = ctopdir;
    cout << "Duneadc installation dir: " << topdir << endl;
    // Load the cxx files.
    string cxxfile = topdir + "/cxxfiles.dat";
    ifstream fin(cxxfile.c_str());
    string name;
    while ( getline(fin, name) && !fin.eof() ) {
      if ( name.size() == 0 || name[0] == '#' ) continue;
      cout << "  Loading " << name << endl;
      string libpath = topdir + "/.aclic" + topdir + "/" + name + "_cxx";
      cout << libpath << endl;
      gSystem->Load(libpath.c_str());
    }
  }
}
