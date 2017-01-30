// dirread.cxx

#include "FileDirectory.h"

using std::string;
using std::cout;
using std::endl;
using std::map;

typedef FileDirectory::FileMap FileMap;

//**********************************************************************

FileDirectory::FileDirectory(string a_dirname) {
  open(a_dirname);
}

//**********************************************************************

void FileDirectory::open(string a_dirname) {
  const string myname = "FileDirectory::open: ";
  if ( pdir != nullptr ) closedir(pdir);
  pdir = nullptr;
  dirname = a_dirname;
  files.clear();
  if ( a_dirname.size() == 0 ) return;
  struct dirent* pent = nullptr;
  pdir = opendir(dirname.c_str());
  if ( pdir != nullptr ) {
    while ( (pent = readdir(pdir)) ) {
      string fname = pent->d_name;
      if ( fname[0] != '.' ) {
        files[fname] = pent;
      }
    }
  } else {
    cout << "Can not open directory " << dirname << endl;
  }

}

//**********************************************************************

FileDirectory::~FileDirectory() {
  if ( pdir != nullptr ) closedir(pdir);
}

//**********************************************************************

FileMap FileDirectory::find(std::string spat) const {
  FileMap outfiles;
  for ( FileMap::value_type ifil : files ) {
    string fname = ifil.first;
    if ( fname.find(spat) != string::npos ) {
      outfiles[fname] = ifil.second;
    }
  }
  return outfiles;
}

//**********************************************************************

void FileDirectory::print(ostream& out) {
  if ( dirname.size() == 0 ) {
    out << "***** No directory name. *****" << endl;
    return;
  }
  out << dirname << endl;
  if ( pdir == nullptr ) {
    out << "***** Directory not found. *****" << endl;
  } else if ( files.size() == 0 ) {
    out << "***** No files found. *****" << endl;
  } else {
    for ( FileMap::value_type ifil : files ) {
      out << "  " << ifil.first << endl;
    }
  }
}

//**********************************************************************
