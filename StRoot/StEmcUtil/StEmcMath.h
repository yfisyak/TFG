//////////////////////////////////////////////////////////////////////////
//
// Author: Aleksei Pavlinov, WSU 08-mar-2001
//
// StEmcMath.h => Encapsulate math routines for special EMC case.
//
//////////////////////////////////////////////////////////////////////////
#ifndef STAR_StEmcMath
#define STAR_StEmcMath

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif
#include "StDetectorId.h" // see $STAR/include

class StMeasuredPoint;

class StEmcMath {

private:
public:
  static Bool_t etaPhi(StMeasuredPoint* point, StMeasuredPoint* vertex,
                       Double_t &eta, Double_t &phi);
  static Double_t pseudoRapidity(StMeasuredPoint* point, StMeasuredPoint* vertex=0);
  static Double_t eta(StMeasuredPoint* point, StMeasuredPoint* vertex=0) 
    {return pseudoRapidity(point,vertex);}
  static Double_t phi(StMeasuredPoint* point, StMeasuredPoint* vertex=0);

  static UInt_t       detectorId(const StDetectorId); 
  static StDetectorId detectorId(const UInt_t); 

  ClassDef(StEmcMath, 1)   // Definition of patch
};
#endif
// $Id: StEmcMath.h,v 1.3 2001/04/03 16:13:05 pavlinov Exp $
// $Log: StEmcMath.h,v $
// Revision 1.3  2001/04/03 16:13:05  pavlinov
// Added function from STAR detector Id to internal EMC Id
//
// Revision 1.2  2001/03/15 20:56:16  pavlinov
// Jose's scheme is default
//
