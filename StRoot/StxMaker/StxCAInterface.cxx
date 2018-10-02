#include "StxCAInterface.h"

#include "TPCCATracker/AliHLTTPCCAGBHit.h"
#include "TPCCATracker/AliHLTTPCCAGBTrack.h"
#include "TPCCATracker/AliHLTTPCCAParam.h"
// need for hits data
#include "StMaker.h"
#include "StEvent/StEvent.h"
#include "StEvent/StGlobalTrack.h"
#include "StEvent/StTrackDetectorInfo.h"
#include "StEvent/StTrackNode.h"
#include "StTpcHitCollection.h"
#include "StTpcSectorHitCollection.h"
#include "StTpcPadrowHitCollection.h"
#include "StTpcHit.h"
#include "StTpcDb/StTpcDb.h"
#include "StDbUtilities/StTpcCoordinateTransform.hh"
#include "StDbUtilities/StTpcLocalSectorCoordinate.hh"
  // for MCdata
#include "tables/St_g2t_track_Table.h" 
#include "tables/St_g2t_tpc_hit_Table.h"
#include "TDatabasePDG.h"
  //to obtain error coefficients
#include "StDetectorDbMaker/StiTpcInnerHitErrorCalculator.h"
#include "StDetectorDbMaker/StiTpcOuterHitErrorCalculator.h"
#include "StDetectorDbMaker/StiTPCHitErrorCalculator.h"
  //to get Magnetic Field
#include "StarMagField/StarMagField.h"
#include "TStopwatch.h"
#include <vector>
#include <algorithm>
using std::vector;
StxCAInterface *StxCAInterface::fgStxCAInterface = 0;
//________________________________________________________________________________
StxCAInterface &StxCAInterface::Instance() {
  if (! fgStxCAInterface) fgStxCAInterface = new StxCAInterface(); 
  return *fgStxCAInterface;
}
//________________________________________________________________________________
StxCAInterface::StxCAInterface() : StTPCCAInterface() {
}
//________________________________________________________________________________
void StxCAInterface::MakeHits() {
  StEvent   *pEvent = dynamic_cast<StEvent*>( StMaker::GetTopChain()->GetInputDS("StEvent") );
  if (! pEvent) return;
  StTpcHitCollection* TpcHitCollection = pEvent->tpcHitCollection();
  if (! TpcHitCollection) { cout << "No TPC Hit Collection" << endl; return;}
  UInt_t numberOfSectors = TpcHitCollection->numberOfSectors();
  for (UInt_t i = 0; i< numberOfSectors; i++) {
    StTpcSectorHitCollection* sectorCollection = TpcHitCollection->sector(i);
    if (sectorCollection) {
      Int_t numberOfPadrows = sectorCollection->numberOfPadrows();
#if 1 /* Sti Convention */
      Int_t sector = i + 1;
      Double_t beta = (sector <= 12) ? (60 - 30*(sector - 1)) : (120 + 30 *(sector - 13));
      Double_t cb   = TMath::Cos(TMath::DegToRad()*beta);
      Double_t sb   = TMath::Sin(TMath::DegToRad()*beta);
#else
      AliHLTTPCCAParam &SlicePar = fCaParam[i];
      Double_t cb   = SlicePar.CosAlpha();
      Double_t sb   = SlicePar.SinAlpha();
#endif
      for (int j = 0; j< numberOfPadrows; j++) {
	StTpcPadrowHitCollection *rowCollection = sectorCollection->padrow(j);
	if (rowCollection) {
	  StSPtrVecTpcHit &hits = rowCollection->hits();
	  Long64_t NoHits = hits.size();
	  for (Long64_t k = 0; k < NoHits; k++) {
	    const StTpcHit *tpcHit = static_cast<const StTpcHit *> (hits[k]);
	    if ( ! tpcHit) continue;
	    //	    Int_t Id = fCaHits.size();
	    Int_t Id = fSeedHits.size();
	    StThreeVectorD glob(tpcHit->position());
	    //  StTpcCoordinateTransform tran(gStTpcDb);
	    //  StTpcLocalSectorCoordinate loc;
	    //	tran(glob,loc,tpcHit->sector(),tpcHit->padrow());
	    
	    // obtain seed Hit
	    SeedHit_t hitc;
	    hitc.padrow = tpcHit->padrow();
	    hitc.status=0;
	    hitc.taken=0;
	    hitc.track_key=tpcHit->idTruth();
	    hitc.hit  = tpcHit;
	    hitc.Id = Id;
	    fSeedHits.push_back(hitc);
	    //yf      if (  hit->timesUsed()) 	continue;//VP
	    // convert to CA Hit
	    AliHLTTPCCAGBHit caHit;
// 	    caHit.SetX( loc.position().x() );
// 	    //	    caHit.SetX( loc.position() ); // take position of the row
// 	    caHit.SetY( - loc.position().y() );
// 	    caHit.SetZ( - loc.position().z() );
	    Double_t xL =  cb*glob.x() + sb*glob.y();
	    Double_t yL = -sb*glob.x() + cb*glob.y();
	    Double_t zL =                  glob.z();
            caHit.SetX(   xL);
	    caHit.SetY( - yL);
	    caHit.SetZ( - zL);
	    // caHit.SetErrX(   );
	    caHit.SetErrY( 0.12 );// TODO: read parameters from somewhere 
	    caHit.SetErrZ( 0.16 );
	    caHit.SetISlice( tpcHit->sector() - 1 );
	    caHit.SetIRow( tpcHit->padrow()-1 );
	    caHit.SetID( Id );
	    //	    caHit.SetID( tpcHit->id() );
	    fIdTruth.push_back( tpcHit->idTruth() );
	    fCaHits.push_back(caHit);
	  }
	}
      }
    }
  }
} // void StxCAInterface::MakeHits()
//________________________________________________________________________________
void StxCAInterface::ConvertPars(const AliHLTTPCCATrackParam& caPar, double _alpha, StxNodePars& nodePars, StxNodeErrs& nodeErrs)
{
  // set jacobian integral coef 
  //                      y   z eta ptin tanl  
  static Double_t JI[5] {-1, -1, -1,   1, -1};
  // get parameters
  nodePars.x()    =         caPar.GetX();
  nodePars.y()    = JI[0] * caPar.GetY();
  nodePars.z()    = JI[1] * caPar.GetZ();
  nodePars.eta()  = JI[2] * asin(caPar.GetSinPhi()); // (signed curvature)*(local Xc of helix axis - X current point on track)
  nodePars.ptin() = JI[3] * caPar.GetQPt();          // signed invert pt [sign = sign(-qB)]
  nodePars.tanl() = JI[4] * caPar.GetDzDs();         // tangent of the track momentum dip angle
  // set jacobian integral coef
  Double_t J[5]; 
  J[0] = JI[0];                     // y
  J[1] = JI[1];                     // z
  J[2] = JI[2]/cos(nodePars.eta()); // eta
  J[3] = JI[3];                     // ptin
  J[4] = JI[4];                     // tanl
  
    // get cov matrises
  const float *caCov = caPar.GetCov();
//   double nodeCov[15];
//   for (int i1 = 0, i = 0; i1 < 5; i1++){
//     for (int i2 = 0; i2 <= i1; i2++, i++){
//       nodeCov[i] = J[i1]*J[i2]*caCov[i];
//     }
//   }
  // if ( (caCov[0] <= 0) || (caCov[2] <= 0) || (caCov[5] <= 0) || (caCov[9] <= 0) || (caCov[14] <= 0))
  //   cout << "Warrning: Bad CA Cov Matrix." << endl;
  // if ( (nodeCov[0] <= 0) || (nodeCov[2] <= 0) || (nodeCov[5] <= 0) || (nodeCov[9] <= 0) || (nodeCov[14] <= 0))
  //   cout << "Warrning: Bad Node Cov Matrix." << endl;

  double *A = nodeErrs.G();
/*  for (int i1 = 0, i = 0; i1 < 5; i1++){
    for (int i2 = 0; i2 <= i1; i2++, i++){
      A[i+i1+2] = caCov[i];
    }
  }*/
  
  nodeErrs._cYY = caCov[ 0];
  nodeErrs._cZY = caCov[ 1]*J[0]*J[1];
  nodeErrs._cZZ = caCov[ 2]*J[0]*J[1];
  nodeErrs._cEY = caCov[ 3]*J[0]*J[2];
  nodeErrs._cEZ = caCov[ 4]*J[1]*J[2];
  nodeErrs._cEE = caCov[ 5]*J[2]*J[2];
  nodeErrs._cTY = caCov[ 6]*J[0]*J[4];
  nodeErrs._cTZ = caCov[ 7]*J[1]*J[4];
  nodeErrs._cTE = caCov[ 8]*J[2]*J[4];    
  nodeErrs._cTT = caCov[ 9]*J[4]*J[4];
  nodeErrs._cPY = caCov[10]*J[0]*J[3];
  nodeErrs._cPZ = caCov[11]*J[1]*J[3];
  nodeErrs._cPE = caCov[12]*J[2]*J[3];
  nodeErrs._cTP = caCov[13]*J[4]*J[3];
  nodeErrs._cPP = caCov[14]*J[3]*J[3];
#if 1  
  A[0] = 1; // don't use parameter X
  A[1] = 0;
  A[3] = 0;
  A[6] = 0;
  A[10] = 0;
  A[15] = 0;
#endif
}
//________________________________________________________________________________
void StxCAInterface::MakeSeeds() {
#if 0
  const int NRecoTracks = fTracker->NTracks();
  for ( int iTr = 0; iTr < NRecoTracks; iTr++ ) {
      // get seed
    const AliHLTTPCCAGBTrack tr = fTracker->Track( iTr );
    Seedx_t seed;

    const int NHits = tr.NHits();
    for ( int iHit = NHits-1; iHit >= 0; iHit-- ){ 
      const int index = fTracker->TrackHit( tr.FirstHitRef() + iHit );
      const int hId   = fTracker->Hit( index ).ID();
      seed.vhit.push_back(fSeedHits[hId].hit);
    }
    seed.total_hits = seed.vhit.size();
    ConvertPars( tr.InnerParam(), tr.Alpha(), seed.firstNodePars, seed.firstNodeErrs );
    ConvertPars( tr.OuterParam(), tr.Alpha(), seed.lastNodePars,  seed.lastNodeErrs );

    fSeeds.push_back(seed);
  }
#endif
} // void StxCAInterface::MakeSeeds()
#if 0
//________________________________________________________________________________
void StxCAInterface::MakeSettings()
{
  
  const int NSlices = 24; //TODO initialize from StRoot
  for ( int iSlice = 0; iSlice < NSlices; iSlice++ ) {
    AliHLTTPCCAParam SlicePar;
    //    memset(&SlicePar, 0, sizeof(AliHLTTPCCAParam));

    Int_t sector = iSlice+1;
    const int NoOfInnerRows = St_tpcPadConfigC::instance()->innerPadRows(sector);
    const int NRows = St_tpcPadConfigC::instance()->padRows(sector);
    SlicePar.SetISlice( iSlice );
    SlicePar.SetNRows ( NRows ); 
    SlicePar.SetNInnerRows ( NoOfInnerRows ); 
    Double_t beta = 0;
    if (sector > 12) beta = (24-sector)*2.*TMath::Pi()/12.;
    else             beta =     sector *2.*TMath::Pi()/12.;
    Double_t alpha = TMath::Pi()/2 -  beta;
    if (alpha < - TMath::Pi()) alpha += 2*TMath::Pi();
    if (alpha >   TMath::Pi()) alpha -= 2*TMath::Pi();
    SlicePar.SetAlpha  (alpha);
    SlicePar.SetDAlpha  ( 30*TMath::DegToRad() );                        //TODO initialize from StRoot
    SlicePar.SetCosAlpha ( TMath::Cos(SlicePar.Alpha()) );
    SlicePar.SetSinAlpha ( TMath::Sin(SlicePar.Alpha()) );
    SlicePar.SetAngleMin ( SlicePar.Alpha() - 0.5*SlicePar.DAlpha() );
    SlicePar.SetAngleMax ( SlicePar.Alpha() + 0.5*SlicePar.DAlpha() );
    SlicePar.SetRMin     (  51. );                                        //TODO initialize from StRoot
    SlicePar.SetRMax     ( 194. );                                        //TODO initialize from StRoot
    SlicePar.SetErrX     (   0. );                                        //TODO initialize from StRoot
    SlicePar.SetErrY     (   0.12 ); // 0.06  for Inner                        //TODO initialize from StRoot
    SlicePar.SetErrZ     (   0.16 ); // 0.12  for Inner                NodePar->fitPars()        //TODO initialize from StRoot
      //   SlicePar.SetPadPitch (   0.675 );// 0.335 -"-
    float x[3]={0,0,0},b[3];
    StarMagField::Instance()->BField(x,b);
    SlicePar.SetBz       ( - b[2] );   // change sign because change z
    if (sector <= 12) {
      SlicePar.SetZMin     (   0. );                                        //TODO initialize from StRoot
      SlicePar.SetZMax     ( 210. );                                        //TODO initialize from StRoot
    } else {
      SlicePar.SetZMin     (-210. );                                        //TODO initialize from StRoot
      SlicePar.SetZMax     (   0. );                                        //TODO initialize from StRoot
    }
    for( int iR = 0; iR < NRows; iR++){
      SlicePar.SetRowX(iR, St_tpcPadConfigC::instance()->radialDistanceAtRow(sector,iR+1));
    }

    Double_t *coeffInner = 0;
    if (St_tpcPadConfigC::instance()->iTPC(sector)) {
      coeffInner = StiTPCHitErrorCalculator::instance()->coeff();
    } else {
      coeffInner = StiTpcInnerHitErrorCalculator::instance()->coeff();
    }
    for(int iCoef=0; iCoef<6; iCoef++)
    {
      SlicePar.SetParamS0Par(0, 0, iCoef, (float)coeffInner[iCoef] );
    }  
  
    SlicePar.SetParamS0Par(0, 0, 6, 0.0f );
    
    Double_t *coeffOuter =StiTpcOuterHitErrorCalculator::instance()->coeff();
    for(int iCoef=0; iCoef<6; iCoef++)
    {
      SlicePar.SetParamS0Par(0, 1, iCoef, (float)coeffOuter[iCoef] );
    }
    SlicePar.SetParamS0Par(0, 1, 6, 0.0f );
    SlicePar.SetParamS0Par(0, 2, 0, 0.0f );
    SlicePar.SetParamS0Par(0, 2, 1, 0.0f );
    SlicePar.SetParamS0Par(0, 2, 2, 0.0f );
    SlicePar.SetParamS0Par(0, 2, 3, 0.0f );
    SlicePar.SetParamS0Par(0, 2, 4, 0.0f );
    SlicePar.SetParamS0Par(0, 2, 5, 0.0f );
    SlicePar.SetParamS0Par(0, 2, 6, 0.0f );
    SlicePar.SetParamS0Par(1, 0, 0, 0.0f );
    SlicePar.SetParamS0Par(1, 0, 1, 0.0f );
    SlicePar.SetParamS0Par(1, 0, 2, 0.0f );
    SlicePar.SetParamS0Par(1, 0, 3, 0.0f );
    SlicePar.SetParamS0Par(1, 0, 4, 0.0f );
    SlicePar.SetParamS0Par(1, 0, 5, 0.0f );
    SlicePar.SetParamS0Par(1, 0, 6, 0.0f );
    SlicePar.SetParamS0Par(1, 1, 0, 0.0f );
    SlicePar.SetParamS0Par(1, 1, 1, 0.0f );
    SlicePar.SetParamS0Par(1, 1, 2, 0.0f );
    SlicePar.SetParamS0Par(1, 1, 3, 0.0f );
    SlicePar.SetParamS0Par(1, 1, 4, 0.0f );
    SlicePar.SetParamS0Par(1, 1, 5, 0.0f );
    SlicePar.SetParamS0Par(1, 1, 6, 0.0f );
    SlicePar.SetParamS0Par(1, 2, 0, 0.0f );
    SlicePar.SetParamS0Par(1, 2, 1, 0.0f );
    SlicePar.SetParamS0Par(1, 2, 2, 0.0f );
    SlicePar.SetParamS0Par(1, 2, 3, 0.0f );
    SlicePar.SetParamS0Par(1, 2, 4, 0.0f );
    SlicePar.SetParamS0Par(1, 2, 5, 0.0f );
    SlicePar.SetParamS0Par(1, 2, 6, 0.0f );
    
    fCaParam.push_back(SlicePar);
  } // for iSlice
} // void StxCAInterface::MakeSettings()
#endif
