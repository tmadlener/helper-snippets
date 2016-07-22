#include "vector_stuff.h"
#include "string_stuff.h"

#include "TFile.h"
#include "TObjArray.h"
#include "TTree.h"
#include "TBranch.h"
#include "TList.h"
#include "TIterator.h"
#include "TKey.h"
#include "TObject.h"

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

// compile with: g++ printRootFile.C -o printRootFile $(root-config --libs --cflags)
// run: ./printRootFile /path/to/the/root/file.root

// TODO: The really nice and more easily maintainable thing for the ouptut would be to define a facet instead of
// indenting different levels via prepending std::string consisting of whitespace only. However, I can't seem
// to get my head around facets at the moment and I have not time.

const int indentDifference = 2; /**< difference of indentation in spaces between different "levels". */
int indentLevel; /**< keep track of the current indentation globally. */

/**
 * which TTrees have we already printed?
 * TTree::AutoSave leads to duplicat TTrees when looded up like this. ROOT manages this wenn the tree is obtained
 * via TFile::Get(), however here we have to do it manually.
 */
std::vector<std::string> printedTTrees;

/** keep track of the current "working" directory. */
std::string currentDir;

/** Get all the branches stored in the TTree via TTree::GetListOfBranches(). */
const std::vector<std::string> getBranchNames(TTree* tree)
{
  std::vector<std::string> names;
  TObjArray* branchArray = tree->GetListOfBranches();
  for (int i = 0; i < branchArray->GetEntries(); ++i) {
    names.push_back( std::string(((TBranch*) branchArray->At(i))->GetName()) );
  }
  return names;
}

/** print one TTree. */
void printTTree(TTree* tree)
{
  std::cout << std::string(indentLevel, ' ') << tree->GetName() << "/" << std::endl;
  indentLevel += indentDifference;
  for (const auto& name : getBranchNames(tree)) {
    std::cout << std::string(indentLevel, ' ') << name << std::endl;
  }
  indentLevel -= indentDifference;
}

/** print recursively by going through all TDirectories that are in the file and contain a TTree. */
template<typename T>
void printRecursively(const T* fileOrDir)
{
  TIter nextKey( fileOrDir->GetListOfKeys() );
  TKey* key = nullptr;
  while (key = static_cast<TKey*>( nextKey() )) {
    TObject* obj = key->ReadObj();
    if ( obj->IsA()->InheritsFrom(TTree::Class()) ) {
      std::string treeName = static_cast<TTree*>(obj)->GetName();
      treeName = currentDir + "/" + treeName;
      // if the TTrees has already been printed. Skip it
      if (std::find(printedTTrees.begin(), printedTTrees.end(), treeName) != printedTTrees.end()) continue;

      printTTree(static_cast<TTree*>(obj));
      printedTTrees.push_back(treeName);
    } else if ( obj->IsA()->InheritsFrom(TDirectory::Class()) ) {
      std::string dirName = static_cast<TDirectory*>(obj)->GetName();
      std::cout << std::string(indentLevel, ' ') << dirName << "/" << std::endl;
      currentDir += "/" + dirName;

      indentLevel += indentDifference;
      printRecursively(static_cast<TDirectory*>(obj));
      indentLevel -= indentDifference;

      // now we move to the next directory, so we have to remove the last part of the path again
      currentDir = removeAfterLast(currentDir, "/");
    }
  }
}

/** "main" function. */
void printRootFile(const std::string& filename)
{
  indentLevel = 0;
  printedTTrees = std::vector<std::string>{};
  currentDir = "";

  TFile* file = TFile::Open(filename.c_str());
  std::cout << "contents of file \'" << filename << "\':" << std::endl;
  printRecursively(file);
}

#ifndef __CINT__
int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "please provide a filename" << std::endl;
    return 1;
  }

  printRootFile(std::string(argv[1]));
  return 0;
}
#endif
