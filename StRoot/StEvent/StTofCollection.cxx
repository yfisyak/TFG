/***************************************************************************
 *
 * $Id: StTofCollection.cxx,v 2.2 2001/04/24 18:20:13 ullrich Exp $
 *
 * Author: Thomas Ullrich, Dec 2000
 ***************************************************************************
 *
 * Description:
 *
 * Persistent data which is written into StEvent
 * directly from the reco chain. All ToF stuff goes here
 * except the StTofPidTraits and the StTofSoftwareMonitor.
 *
 ***************************************************************************
 *
 * $Log: StTofCollection.cxx,v $
 * Revision 2.2  2001/04/24 18:20:13  ullrich
 * Added hits and slats to collection.
 *
 * Revision 2.1  2000/12/08 03:52:42  ullrich
 * Initial Revision
 *
 **************************************************************************/
#include "StTofCollection.h"

static const char rcsid[] = "$Id: StTofCollection.cxx,v 2.2 2001/04/24 18:20:13 ullrich Exp $";

ClassImp(StTofCollection)
    
StTofCollection::StTofCollection() { /* noop */ }

StTofCollection::~StTofCollection() { /* noop */ }

const StSPtrVecTofSlat&
StTofCollection::tofSlats() const
{
    return mTofSlats;
}

StSPtrVecTofSlat&
StTofCollection::tofSlats()
{
    return mTofSlats;
}

const StSPtrVecTofHit&
StTofCollection::tofHits() const
{
    return mTofHits;
}

StSPtrVecTofHit&
StTofCollection::tofHits()
{
    return mTofHits;
}

void
StTofCollection::addSlat(const StTofSlat* aSlat)  
{
    if (aSlat) mTofSlats.push_back(aSlat);
}

void
StTofCollection::addHit(const StTofHit* aHit)
{
    if (aHit) mTofHits.push_back(aHit);
}

bool
StTofCollection::slatsPresent() const
{
    return mTofSlats.size();
}


bool
StTofCollection::hitsPresent() const
{
    return mTofHits.size();
}
