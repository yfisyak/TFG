//M.L. Miller (Yale Software)
//Yale Software
//12/02

//std
#include <iostream>
#include <algorithm>
#include <time.h>
#include <map>
using std::multimap;
using std::for_each;
using std::sort;

//StJetFinder
#include "StJetEtCell.h"
#include "StJetSpliterMerger.h"
#include "StProtoJet.h"
#include "StCdfChargedJetEtCell.h"
#include "StCdfChargedConeJetFinder.h"

//careful, can't use buildGrid() call in base class constructor or we'll sef-fault because it calls
//a virtual function in the constructor while the derived class doesn't yet exist
StCdfChargedConeJetFinder::StCdfChargedConeJetFinder(const StCdfChargedConePars& pars) 
  : StConeJetFinder(pars)
{

}

StCdfChargedConeJetFinder::~StCdfChargedConeJetFinder()
{
}

StJetEtCellFactory* StCdfChargedConeJetFinder::makeCellFactory()
{
  return new StCdfChargedJetEtCellFactory;
}

void StCdfChargedConeJetFinder::findJets_sub1()
{
  doSearch();
  addToPrejets(&mWorkCell);
}

void StCdfChargedConeJetFinder::findJets_sub2()
{

}

bool StCdfChargedConeJetFinder::acceptSeed(const StJetEtCell* cell)
{
    return (cell->nTimesUsed()==0 && cell->empty()==false);
    //return (cell->empty()==false);
}

void StCdfChargedConeJetFinder::print()
{
}

bool StCdfChargedConeJetFinder::shouldNotAddToTheCell(const StJetEtCell& theCell, const StJetEtCell& otherCell) const
{
  if (otherCell.nTimesUsed()) return true;
  if (otherCell.empty()) return true;
  if (otherCell.eT() <= mPars.assocEtMin()) return true; 
  return false;
}

