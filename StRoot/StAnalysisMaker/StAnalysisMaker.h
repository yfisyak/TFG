// $Id: StAnalysisMaker.h,v 1.6 1999/07/30 22:56:02 kathy Exp $
//
// $Log: StAnalysisMaker.h,v $
// Revision 1.6  1999/07/30 22:56:02  kathy
// added new method and input param qaflag so that if turned on, a log file will be printed out with QA information
//
// Revision 1.5  1999/07/15 13:56:41  perev
// cleanup
//
// Revision 1.4  1999/06/25 19:20:40  fisyak
// Merge StRootEvent and StEvent
//
// Revision 1.3  1999/06/24 21:56:47  wenaus
// Version minimally changed from standard StAnalysisMaker
//
// Revision 1.3  1999/03/30 15:33:43  wenaus
// eliminate obsolete branch methods
//
// Revision 1.2  1999/02/12 02:00:27  wenaus
// Incorporate tag loading example
//
// Revision 1.1  1999/02/05 17:54:56  wenaus
// initial commit
//

 * Revision for new StEvent
 *

///////////////////////////////////////////////////////////////////////////////
//
// StAnalysisMaker
//
// Description: 
//  Sample maker to access and analyze StEvent
//
// Environment:
//  Software developed for the STAR Detector at Brookhaven National Laboratory
//
// Author List: 
//  Torre Wenaus, BNL
//
// History:
//
///////////////////////////////////////////////////////////////////////////////
 **************************************************************************/
#include "tables/HighPtTag.h"
#define StAnalysisMaker_HH
#include "StMaker.h"
#include "HighPtTag.h"

class StEvent;

    virtual const char *GetCVS() const
  Bool_t drawinit;
  Char_t collectionName[256];

  // Maker generates a tag
  HighPtTag_st* theTag; //!
  ofstream ofile;  // QA output file
  Int_t nEvtProc;   // Number of events requested (QA)

protected:

public:

  StAnalysisMaker(const Char_t *name="analysis");
  StAnalysisMaker(const char *,const Int_t,
                  const Char_t *name="analysis");  // overloaded for QA
  virtual ~StAnalysisMaker();
  virtual void Clear(Option_t *option="");
  virtual Int_t Init();
  virtual Int_t  Make();
  virtual Int_t  Finish();

  // fake copy constructor to fool CINT!  Necessary for QA.
  StAnalysisMaker(const StAnalysisMaker &);

  // Tag accessor
  HighPtTag_st* tag() {return theTag;};

  virtual const char *GetCVS() const
  {static const char cvs[]="Tag $Name:  $ $Id: StAnalysisMaker.h,v 1.6 1999/07/30 22:56:02 kathy Exp $ built "__DATE__" "__TIME__ ; return cvs;}

  ClassDef(StAnalysisMaker, 1)
    Int_t nevents;

    
    ClassDef(StAnalysisMaker,1)
};
#endif
