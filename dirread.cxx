// dirread.cxx

#include <string>
#include <iostream>
#include <map>
#include <sys/types.h>
#include <dirent.h>

using std::string;
using std::cout;
using std::endl;
using std::map;

typedef std::map<string, dirent*> FileMap;

int main (void) {

  string dirname = "./";
  FileMap files;
  struct dirent* pent = nullptr;
  DIR* pdir = opendir (dirname.c_str());
  if ( pdir != nullptr ) {
    cout << dirname << endl;
    while ( (pent = readdir(pdir)) ) {
      string fname = pent->d_name;
      files[fname] = pent;
    }
    for ( FileMap::value_type ifil : files ) {
      string fname = ifil.first;
      cout << "  " << fname << endl;
    }
    closedir(pdir);
  } else {
    cout << "Couldn't open the directory " << dirname << endl;
  }

  return 0;
}
