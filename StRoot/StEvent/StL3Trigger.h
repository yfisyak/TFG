/***************************************************************************
 *
 * $Id: StL3Trigger.h,v 2.2 2001/04/05 04:00:38 ullrich Exp $
 *
 * Author: Thomas Ullrich, Apr 2000
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StL3Trigger.h,v $
 * Revision 2.2  2001/04/05 04:00:38  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.1  2000/03/29 16:53:11  ullrich
 * Initial Revision
 *
 **************************************************************************/
#ifndef StL3Trigger_hh
#define StL3Trigger_hh
#include "StContainers.h"
#include "TArrayL.h"

class StTrackNode;
class StTpcHitCollection;
class StTrackDetectorInfo;
class StPrimaryVertex;

class StL3Trigger : public StObject {
public:
    StL3Trigger();
    ~StL3Trigger();

    StTpcHitCollection*                 tpcHitCollection();
    const StTpcHitCollection*           tpcHitCollection() const;

    StSPtrVecTrackDetectorInfo&         trackDetectorInfo();
    const StSPtrVecTrackDetectorInfo&   trackDetectorInfo() const;
    
    StSPtrVecTrackNode&                 trackNodes();
    const StSPtrVecTrackNode&           trackNodes() const;

    unsigned int                        numberOfPrimaryVertices() const;
    StPrimaryVertex*                    primaryVertex(unsigned int = 0);
    const StPrimaryVertex*              primaryVertex(unsigned int = 0) const;

    void setTpcHitCollection(StTpcHitCollection*);
    void addPrimaryVertex(StPrimaryVertex*);
    
protected:
    StTpcHitCollection*          mL3TpcHits;
    StSPtrVecTrackDetectorInfo   mL3TrackDetectorInfo;
    StSPtrVecTrackNode           mL3TrackNodes;
    StSPtrVecPrimaryVertex       mL3PrimaryVertices;
    TArrayL                      mTriggerWords;

private:
    StL3Trigger(const StL3Trigger&);
    StL3Trigger& operator=(const StL3Trigger&);

    ClassDef(StL3Trigger,1)
};
#endif
