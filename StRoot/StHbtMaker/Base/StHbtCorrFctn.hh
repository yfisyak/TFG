/***************************************************************************
 *
 * $Id: StHbtCorrFctn.hh,v 1.6 2000/03/23 22:43:27 laue Exp $
 *
 * Author: Mike Lisa, Ohio State, lisa@mps.ohio-state.edu
 ***************************************************************************
 *
 * Description: part of STAR HBT Framework: StHbtMaker package
 *    base class for a STAR correlation function.  Users should inherit 
 *    from this and must implement constructor, destructor, Report(),
 *    AddMixedPair(), AddRealPair(), Finish()
 *
 ***************************************************************************
 *
 * $Log: StHbtCorrFctn.hh,v $
 * Revision 1.6  2000/03/23 22:43:27  laue
 * Clone() function implemented in cuts.
 *
 * Revision 1.5  2000/03/16 01:54:36  laue
 * Copy constructor added to all the cut base classes and to the
 * corrfctn base class
 *
 * Revision 1.4  2000/02/13 17:13:09  laue
 * EventBegin() and EventEnd() functions implemented
 *
 * Revision 1.3  1999/12/03 22:24:33  lisa
 * (1) make Cuts and CorrFctns point back to parent Analysis (as well as other way). (2) Accommodate new PidTraits mechanism
 *
 * Revision 1.2  1999/07/06 22:33:18  lisa
 * Adjusted all to work in pro and new - dev itself is broken
 *
 * Revision 1.1.1.1  1999/06/29 16:02:56  lisa
 * Installation of StHbtMaker
 *
 **************************************************************************/

#ifndef StHbtCorrFctn_hh
#define StHbtCorrFctn_hh

#include "StHbtMaker/Infrastructure/StHbtTypes.hh"
#include "StHbtMaker/Infrastructure/StHbtPair.hh"
#include "StHbtMaker/Infrastructure/StHbtEvent.hh"

class StHbtCorrFctn{

public:
  StHbtCorrFctn(){/* no-op */};
  StHbtCorrFctn(const StHbtCorrFctn& );
  virtual ~StHbtCorrFctn(){/* no-op */};

  virtual StHbtString Report() = 0;

  virtual void AddRealPair(const StHbtPair*) = 0;
  virtual void AddMixedPair(const StHbtPair*) = 0;

  virtual void EventBegin(const StHbtEvent*) { /* no-op */ }
  virtual void EventEnd(const StHbtEvent*) { /* no-op */ }
  virtual void Finish() = 0;

  virtual StHbtCorrFctn* Clone() { return 0;}

  // the following allows "back-pointing" from the CorrFctn to the "parent" Analysis
  friend class StHbtAnalysis;
  StHbtAnalysis* HbtAnalysis(){return myAnalysis;};

protected:
  StHbtAnalysis* myAnalysis;

private:

};

inline StHbtCorrFctn::StHbtCorrFctn(const StHbtCorrFctn& c) { myAnalysis =0; }

#endif
