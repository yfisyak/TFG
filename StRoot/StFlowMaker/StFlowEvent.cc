//////////////////////////////////////////////////////////////////////
//
// $Id: StFlowEvent.cc,v 1.2 1999/11/24 18:17:13 posk Exp $
//
// Author: Raimond Snellings and Art Poskanzer
//////////////////////////////////////////////////////////////////////
//
// Description: A subset of StEvent 
//
//////////////////////////////////////////////////////////////////////
//
// $Log: StFlowEvent.cc,v $
// Revision 1.2  1999/11/24 18:17:13  posk
// Put the methods which act on the data in with the data in StFlowEvent.
//
// Revision 1.1  1999/11/11 23:08:54  posk
// Rearrangement of files.
//
// Revision 1.1  1999/11/04 19:02:04  snelling
// First check in of StFlowMaker. It contains the common code from
// StFlowTagMaker and StFlowAnalysisMaker.
//
//////////////////////////////////////////////////////////////////////

#include <iostream.h>
#include <stdlib.h>
#include <math.h>
//#include "StChain.h"
#include "StFlowEvent.hh"
#include "PhysicalConstants.h"
#include "SystemOfUnits.h"
#include "TVector2.h"
#include "StFlowTrackCollection.hh"
#define PR(x) cout << "##### FlowEvent: " << (#x) << " = " << (x) << endl;

//-----------------------------------------------------------

StFlowEvent::StFlowEvent() {
  pTrackCollection = new StFlowTrackCollection;
}

//-----------------------------------------------------------

StFlowEvent::~StFlowEvent() {
  StFlowTrackIterator iter;
  for (iter= pTrackCollection->begin(); iter!= pTrackCollection->end(); iter++){
    delete *iter;
  }
  delete pTrackCollection;
}

//-------------------------------------------------------------

Int_t StFlowEvent::checkInput(Int_t harN, Int_t selN, Int_t subN) const {
  
  if (harN < 0 || harN >= nHars) {
    cout << "### Harmonic " << harN << " not valid" << endl;
    return kFALSE;
  }
  if (selN < 0 || selN >= nSels) {
    cout << "### Selection " << selN << " not valid" << endl;
    return kFALSE;
  }
  if (subN < 0 || subN > nSubs) {
    cout << "### Subevent " << subN << " not valid" << endl;
    return kFALSE;
  }

  return kTRUE;
}

//-------------------------------------------------------------

///void StFlowEvent::SetPhiWeight(const PhiWgt_t* &pPhiWgt) {
void StFlowEvent::SetPhiWeight(const Double_t* pPhiWgt) {
  for (int k = 0; k < nSels; k++) {
    for (int j = 0; j < nHars; j++) {
      for (int n = 0; n < nPhiBins; n++) {
	mPhiWgt[k][j][n] = *(pPhiWgt + n + nPhiBins*(j + nHars*k));
      }
    }
  }
//   PR(mPhiWgt[0][0][0]);
//   PR(mPhiWgt[1][0][30]);
//   PR(mPhiWgt[1][3][59]);
}

//-------------------------------------------------------------

Double_t StFlowEvent::PhiWeight(Float_t mPhi, Int_t selN, Int_t harN) const {
  if (!checkInput(harN, selN, 0)) return 0.;

  if (mPhi < 0.) mPhi += twopi;
  Int_t n = (mPhi/twopi)*nPhiBins;
  //float phiWgt = mPhiWgt[selN][harN][n];  

  return mPhiWgt[selN][harN][n];
}

//-------------------------------------------------------------

UInt_t StFlowEvent::Mult(Int_t harN, Int_t selN, Int_t subN) {
  UInt_t mMult = 0;
  if (!checkInput(harN, selN, subN)) return mMult;

  StFlowTrackIterator itr;
  for (itr = TrackCollection()->begin(); itr != TrackCollection()->end(); itr++) {
    StFlowTrack* pFlowTrack = *itr;
    if (pFlowTrack->Select(harN, selN, subN))  mMult++;
  }
  
  return mMult;
}

//-------------------------------------------------------------

Float_t StFlowEvent::MeanPt(Int_t harN, Int_t selN, Int_t subN) {
  Float_t mSumPt = 0.;
  UInt_t  mMult  = 0;
  if (!checkInput(harN, selN, subN)) return 0.;

  StFlowTrackIterator itr;
  for (itr = TrackCollection()->begin(); itr != TrackCollection()->end(); itr++) {
    StFlowTrack* pFlowTrack = *itr;
    if (pFlowTrack->Select(harN, selN, subN)) {
      mSumPt += pFlowTrack->Pt();
      mMult++;
    }
  }
  return (mMult) ? mSumPt/(float)mMult : 0.;
}

//-------------------------------------------------------------

TVector2 StFlowEvent::Q(Int_t harN, Int_t selN, Int_t subN) { 
  TVector2 mQ;
  mQ.Set(0., 0.);
  if (!checkInput(harN, selN, subN)) return mQ;
  Double_t mQx=0., mQy=0.;
  float order = (float)(harN + 1);

  StFlowTrackIterator itr;
  for (itr = TrackCollection()->begin(); itr != TrackCollection()->end(); itr++) {
    StFlowTrack* pFlowTrack = *itr;
    if (pFlowTrack->Select(harN, selN, subN)) {
      Float_t mPhi = pFlowTrack->Phi();
      Double_t phiWgt = PhiWeight(mPhi, selN, harN);
      if (pFlowTrack->Eta() < 0 && (harN+1) % 2 == 1) phiWgt *= -1.;
      mQx += phiWgt * cos(mPhi * order);
      mQy += phiWgt * sin(mPhi * order);
    }
  }
  mQ.Set(mQx, mQy);

  return mQ;
}

//-------------------------------------------------------------

Float_t StFlowEvent::Psi(Int_t harN, Int_t selN, Int_t subN) {
  Float_t mPsi = 0.;
  if (!checkInput(harN, selN, subN)) return mPsi;
  float order = (float)(harN + 1);

  TVector2 mQ = Q(harN, selN, subN);
  mPsi= mQ.Phi() / order;
  if (mPsi < 0.) {mPsi += twopi / order;}
  
  return mPsi;
}

//-------------------------------------------------------------

Float_t StFlowEvent::q(Int_t harN, Int_t selN, Int_t subN) { 
  Float_t mq = 0.;
  if (!checkInput(harN, selN, subN)) return mq;

  TVector2 mQ  = Q(harN, selN, subN);
  UInt_t mMult = Mult(harN, selN, subN);
  
  return (mMult) ? mQ.Mod() / sqrt(mMult) : 0.;
}

//-------------------------------------------------------------
