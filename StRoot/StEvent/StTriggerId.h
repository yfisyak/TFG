/***************************************************************************
 *
 * $Id: StTriggerId.h,v 2.8 2011/02/02 20:26:30 ullrich Exp $
 *
 * Author: Thomas Ullrich, January 2003
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTriggerId.h,v $
 * Revision 2.8  2011/02/02 20:26:30  ullrich
 * Switched data member type of mask from ULong64_t to ULong64_t
 *
 * Revision 2.7  2011/02/02 20:20:10  ullrich
 * Extend to 64 bit (Jamie)
 *
 * Revision 2.6  2006/05/04 19:07:02  ullrich
 * Extended mMaxTriggerIds to 42.
 *
 * Revision 2.5  2004/10/11 23:00:20  ullrich
 * Add copy constructor and assign. op., implement ostream op., define to enum.
 *
 * Revision 2.4  2003/09/02 17:58:06  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 2.3  2003/02/18 22:19:58  jeromel
 * Skip mIdx
 *
 * Revision 2.2  2003/02/18 21:34:46  jeromel
 * Changed vector to arrays
 *
 * Revision 2.1  2003/01/30 18:14:15  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#ifndef StTriggerId_hh
#define StTriggerId_hh
#include "StObject.h"
#include <Stiostream.h>
#include <vector>

class StTriggerId : public StObject {
public:
    StTriggerId();
    StTriggerId(const StTriggerId &id);
    const StTriggerId &operator=(const StTriggerId &id);
    ~StTriggerId();
	
    ULong64_t mask() const;
    
    UInt_t    maxTriggerIds() const;
    Bool_t    isTrigger(UInt_t id) const;
    
    UInt_t    version(UInt_t id) const;
    UInt_t    nameVersion(UInt_t id) const;
    UInt_t    thresholdVersion(UInt_t id) const;
    UInt_t    prescaleVersion(UInt_t id) const;    

    UInt_t    triggerId(const Int_t idx) const;    
    vector<UInt_t> triggerIds() const;  


    void setMask(ULong64_t);
    void addTrigger(UInt_t, UInt_t,
	          UInt_t, UInt_t, UInt_t);

    friend ostream& operator<<(ostream&, const StTriggerId&);
    
private:
    UInt_t index(UInt_t) const;
    
protected:
    enum {mMaxTriggerIds = 64};

private:
    UInt_t       mIdx;    //!
    ULong64_t    mMask;
    UInt_t       mId[mMaxTriggerIds];
    UInt_t       mVersion[mMaxTriggerIds];
    UInt_t       mNameVersion[mMaxTriggerIds];
    UInt_t       mThresholdVersion[mMaxTriggerIds];
    UInt_t       mPrescaleVersion[mMaxTriggerIds];
    
    ClassDef(StTriggerId,5)
};

inline UInt_t
StTriggerId::maxTriggerIds() const {return  mMaxTriggerIds;}

inline UInt_t
StTriggerId::triggerId(const Int_t idx) const
{
    if (idx>=0 && idx<mMaxTriggerIds)
        return mId[idx];
    else
        return 0;
}

#endif
