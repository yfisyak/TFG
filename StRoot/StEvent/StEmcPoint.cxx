/***************************************************************************
 *
 * $Id: StEmcPoint.cxx,v 2.5 2001/04/05 04:00:48 ullrich Exp $
 *
 * Author: Akio Ogawa, Jan 2000
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StEmcPoint.cxx,v $
 * Revision 2.5  2001/04/05 04:00:48  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.4  2001/03/24 03:34:45  perev
 * clone() -> clone() const
 *
 * Revision 2.3  2000/07/28 19:49:27  akio
 * Change in Detector Id for Endcap SMD
 *
 * Revision 2.2  2000/05/22 19:21:53  akio
 * Bug fix, add delta into EMcPoint, wider bits for Eta in RawHit
 *
 * Revision 2.1  2000/03/23 22:24:07  akio
 * Initial version of Emc Point, and Inclusion of track pointers
 *
 *
 **************************************************************************/
#include "StEmcPoint.h"
#include <iostream.h>

static const char rcsid[] = "$Id: StEmcPoint.cxx,v 2.5 2001/04/05 04:00:48 ullrich Exp $";

ClassImp(StEmcPoint)

StEmcPoint::StEmcPoint() { /* noop */ }

StEmcPoint::StEmcPoint(const StThreeVectorF& p,
                       const StThreeVectorF& e,
                       const StThreeVectorF& s,
                       unsigned int hp, float q,
                       float energy, float csq,
                       unsigned char c)
  : StHit(p,e,hp,q,c),mEnergy(energy), mChiSquare(csq), mSize(s)
{ /* noop */ }

StEmcPoint::~StEmcPoint() {/* noop */}

float
StEmcPoint::energy()    const {return mEnergy;};

void
StEmcPoint::setEnergy(const float e) {mEnergy = e;};

float
StEmcPoint::chiSquare() const {return mChiSquare;};

void
StEmcPoint::setChiSquare(const float c) {mChiSquare = c;};

StThreeVectorF
StEmcPoint::size() const {return mSize;};

void
StEmcPoint::setSize(const StThreeVectorF& s) {mSize = s;};

int
StEmcPoint::getDetId(const StDetectorId id) const{
  if(id>=kBarrelEmcTowerId && id<=kBarrelSmdPhiStripId){
    return id-kBarrelEmcTowerId;
  }else if(id>=kEndcapEmcTowerId && id<=kEndcapSmdVStripId){
    return id-kEndcapEmcTowerId;
  }else{
    cout<<"***Error at StEmcPoint:::getDetId, Invalid  StDetectorId"<<endl;
    return 0;
  }
}

float
StEmcPoint::energyInDetector(const StDetectorId id) const{
  int i = getDetId(id);
  if(i==-1) return 0.0;
  return mEnergyInDetector[i];
}

void
StEmcPoint::setEnergyInDetector(const StDetectorId id, const float e){
  int i = getDetId(id);
  if(i>=0) mEnergyInDetector[i]=e;
}

float
StEmcPoint::sizeAtDetector(const StDetectorId id) const{
  int i = getDetId(id);
  if(i==-1) return 0.0;
  return mSizeAtDetector[i];
}

void
StEmcPoint::setSizeAtDetector(const StDetectorId id, const float s){
  int i = getDetId(id);
  if(i>=0) mSizeAtDetector[i]=s;
}

float StEmcPoint::deltaEta() const{return mDelta[0];}
float StEmcPoint::deltaPhi() const{return mDelta[1];}
float StEmcPoint::deltaU()   const{return mDelta[0];}
float StEmcPoint::deltaV()   const{return mDelta[1];}
void StEmcPoint::setDeltaEta(const float d){mDelta[0]=d;}
void StEmcPoint::setDeltaPhi(const float d){mDelta[1]=d;}
void StEmcPoint::setDeltaU(const float d){mDelta[0]=d;}
void StEmcPoint::setDeltaV(const float d){mDelta[1]=d;}

StPtrVecEmcCluster&
StEmcPoint::cluster(const StDetectorId id){
  int i = getDetId(id);
  return mCluster[i];
}
  
const StPtrVecEmcCluster&
StEmcPoint::cluster(const StDetectorId id) const{
  int i = getDetId(id);
  return mCluster[i];
}
  
void
StEmcPoint::addCluster(const StDetectorId id, const StEmcCluster* c){
  int i = getDetId(id);
  if(i>=0) mCluster[i].push_back(c);
}

StPtrVecEmcPoint&
StEmcPoint::neighbor(){
 return mNeighbors;
}

const StPtrVecEmcPoint&
StEmcPoint::neighbor()const {
 return mNeighbors;
}
  
void
StEmcPoint::addNeighbor(const StEmcPoint* p){
  mNeighbors.push_back(p);
}

int
StEmcPoint::nTracks() const {return mTracks.size();}

StPtrVecTrack&
StEmcPoint::track() {return mTracks;}

const StPtrVecTrack&
StEmcPoint::track() const {return mTracks;}

void
StEmcPoint::addTrack(StTrack* track) {mTracks.push_back(track);}

StObject* StEmcPoint::clone() const { return new StEmcPoint(*this); };



