/***************************************************************************
 *
 * $Id: StRichMCHit.h,v 2.3 2001/04/05 04:00:40 ullrich Exp $
 *
 * Author: Brian Lasiuk, May 2000
 ***************************************************************************
 *
 * Description: Definition of the persistent MC Hit object
 *
 ***************************************************************************
 *
 * $Log: StRichMCHit.h,v $
 * Revision 2.3  2001/04/05 04:00:40  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.2  2001/03/24 03:34:55  perev
 * clone() -> clone() const
 *
 * Revision 2.1  2000/05/22 21:44:32  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StRichMCHit_hh
#define StRichMCHit_hh

#include "StRichHit.h"
#include "StRichMCInfo.h"

class StRichMCHit : public StRichHit {
public:
    StRichMCHit();
    StRichMCHit(const StThreeVectorF& xg, const StThreeVectorF& dx);
    StRichMCHit(const StThreeVectorF& xg, const StThreeVectorF& dx,
              unsigned int hp, float q, float maxAdc, unsigned char tc);
    StRichMCHit(const StThreeVectorF& xg, const StThreeVectorF& dx,
                unsigned int hp, float q, float maxAdc, unsigned char tc,
                StRichMCInfo& info);
    
    ~StRichMCHit();
    
    //StRichMCHit(const StRichMCHit&){}
    //StRichMCHit& operator=(const StRichMCHit&){}

    void setMCInfo(const StRichMCInfo&);
    const StRichMCInfo& getMCInfo() const;

protected:
    StObject* clone() const;
    
    StRichMCInfo  mInfo;
    
    ClassDef(StRichMCHit,1)
};

inline const StRichMCInfo& StRichMCHit::getMCInfo() const { return mInfo; }
inline void StRichMCHit::setMCInfo(const StRichMCInfo& info) { mInfo = info;}
#endif
