// define macro to switch between c++11 and before
#define __CPP11_SUP (__cplusplus == 201103L)

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

/** get the UNIQUE names of all trees in the file.
 * TTree::AutoSave leads to duplicate trees when looked up like this. ROOT manages this when the tree is obtained
 * via TFile::Get(), however here we have to do it manually. This is done by employing std::unique.
 */
const std::vector<std::string> getTreeNames(TFile* file)
{
  std::vector<std::string> names;
  TIter nextKey( file->GetListOfKeys() );
#if __CPP11_SUP
  TKey* key = nullptr;
#else
  TKey* key = NULL;
#endif
  while ( key = (TKey*) nextKey() ) {
    TObject* obj = key->ReadObj();
    if ( obj->IsA()->InheritsFrom(TTree::Class()) ) {
      names.push_back( std::string( ((TTree*) obj)->GetName() ) );
    }
  }

  std::sort(names.begin(), names.end());
  std::vector<std::string>::iterator nEnd = std::unique(names.begin(), names.end()); // no auto key-word pre c++11
  names.resize(std::distance(names.begin(), nEnd));
  return names;
}

const std::vector<TTree*> getTreePointers(TFile* file)
{
  std::vector<TTree*> trees;
#if __CPP11_SUP
  for (const auto& name : getTreeNames(file)) {
#else
  const std::vector<std::string> treeNames = getTreeNames(file);
  for (size_t i = 0; i < treeNames.size(); ++i) {
    const std::string& name = treeNames[i];
#endif
    trees.push_back( (TTree*) file->Get(name.c_str()) );
  }

  return trees;
}

const std::vector<std::string> getBranchNames(TTree* tree)
{
  std::vector<std::string> names;
  TObjArray* branchArray = tree->GetListOfBranches();
  for (int i = 0; i < branchArray->GetEntries(); ++i) {
    names.push_back( std::string(((TBranch*) branchArray->At(i))->GetName()) );
  }
  return names;
}

/** "main" function. */
void printRootFile(const std::string& filename)
{
  TFile* file = TFile::Open(filename.c_str());
#if __CPP11_SUP
  for (const auto tree : getTreePointers(file)) {
    std::cout << "TTree: " << tree->GetName() << std::endl;
    for (const auto& name : getBranchNames(tree)) {
#else
  std::vector<TTree*> trees = getTreePointers(file);
  for (size_t iTree = 0; iTree < trees.size(); ++iTree) {
    std::cout << "TTree: " << trees[iTree]->GetName() << std::endl;
    const std::vector<std::string> branchNames = getBranchNames(trees[iTree]);
    for (size_t iBranch = 0; iBranch < branchNames.size(); ++iBranch) {
      const std::string& name = branchNames[iBranch];
#endif
      std::cout << name << std::endl;
    }
  }
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
