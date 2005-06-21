// $Id: StiSsdDetectorGroup.cxx,v 1.6 2005/06/21 15:31:47 lmartin Exp $
// 
// $Log: StiSsdDetectorGroup.cxx,v $
// Revision 1.6  2005/06/21 15:31:47  lmartin
// CVS tags added
//
#include "StiSsd/StiSsdDetectorGroup.h"
#include "StiSsd/StiSsdHitLoader.h"
#include "StiSsd/StiSsdDetectorBuilder.h"
#include "Sti/StiDedxCalculator.h"
#include "Sti/StiElossCalculator.h"
#include "StEvent.h"
#include "StMcEvent.hh"
#include <stdexcept>

StiSsdDetectorGroup::StiSsdDetectorGroup(bool active, const string & inputFile)
  : StiDetectorGroup<StEvent,StMcEvent>("SSD",
			      active?new StiSsdHitLoader():0,
			      new StiSsdDetectorBuilder(active,inputFile),0,0)

{}

StiSsdDetectorGroup::~StiSsdDetectorGroup()
{}


