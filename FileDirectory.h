// FileDirectory.h

// David Adams
// December 2016
//
// Finds the (not hidden) files in a directory.

#ifndef FileDirectory_H
#define FileDirectory_H

#include <string>
#include <iostream>
#include <map>
#include <sys/types.h>
#include <dirent.h>

class FileDirectory {
public:

  typedef std::map<std::string, dirent*> FileMap;

  std::string dirname;
  DIR* pdir = nullptr;
  FileMap files;

  FileDirectory(std::string a_dirname ="");
  ~FileDirectory();
  void open(std::string a_dirname);
  void print(std::ostream& out =std::cout);

  // Find files with the indicated string in their names.
  FileMap find(std::string spat) const;

};

#endif
