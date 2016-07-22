#ifndef ROOT_UTILS__TMADLENER
#define ROOT_UTILS__TMADLENER

#include "TFile.h"
#include <string>

/** get from Root file. Using dynamic cast to get throught the inheritance stuff correctly. */
template<typename T> inline
T* getFromFile(TFile* f, const std::string& name)
{
  return dynamic_cast<T*>(f->Get(name.c_str()));
}

#endif
