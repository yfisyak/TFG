/***************************************************************************
 *
 * $Id: StDcaGeometry.cxx,v 2.11 2013/11/13 21:35:48 fisyak Exp $
 *
 * Author: Victor Perevoztchikov, Thomas Ullrich, May 2006
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StDcaGeometry.cxx,v $
 * Revision 2.11  2013/11/13 21:35:48  fisyak
 * Suppress Warning
 *
 * Revision 2.9  2013/07/16 14:29:03  fisyak
 * Restore mass fit tracks
 *
 * Revision 2.8  2013/01/15 23:21:05  fisyak
 * improve printouts
 *
 * Revision 2.7  2012/06/01 14:19:06  fisyak
 * Fix print out
 *
 * Revision 2.6  2010/08/31 20:14:50  fisyak
 * Fix format
 *
 * Revision 2.5  2010/08/31 19:49:16  fisyak
 * adjust parameters print out
 *
 * Revision 2.4  2010/01/26 20:34:39  fisyak
 * Add print out and  conversion from DCA to x,y,z,px,py,pz
 *
 * Revision 2.3  2009/10/27 22:50:25  fisyak
 * Add set from double
 *
 * Revision 2.2  2008/03/04 01:03:36  perev
 * remove redundant mHz
 *
 * Revision 2.1  2006/05/24 17:27:43  ullrich
 * Initial Revision.
 *
 **************************************************************************/
#include "StDcaGeometry.h"
#if ROOT_VERSION_CODE < 331013
#include "TCL.h"
#else
#include "TCernLib.h"
#endif
#include "TRMatrix.h"
#include "TRSymMatrix.h"
#include "TMath.h"
#include "KFParticle/KFPTrack.h"
ClassImp(StDcaGeometry)
    
static const char rcsid[] = "$Id: StDcaGeometry.cxx,v 2.11 2013/11/13 21:35:48 fisyak Exp $";

StDcaGeometry::StDcaGeometry()
{
    memset(mBeg,0,mEnd-mBeg+1);
}

StDcaGeometry::~StDcaGeometry() {/* noop */}

StThreeVectorF StDcaGeometry::origin() const
{
    float x = -mImp*sin(mPsi);
    float y =  mImp*cos(mPsi);
    return StThreeVectorF(x,y,mZ);
}

StThreeVectorF StDcaGeometry::momentum() const
{
    float ptt = pt();
    float x   = ptt*cos(mPsi);
    float y   = ptt*sin(mPsi);
    float z   = ptt*mTan;
    return StThreeVectorF(x,y,z);
}

void StDcaGeometry::set(const float pars[6],const float errs[15])
{
    if (pars) memcpy(&mImp   ,pars,sizeof(float)*6 );
    if (errs) memcpy(&mImpImp,errs,sizeof(float)*15);
}
void StDcaGeometry::set(const double pars[6],const double errs[15])
{
  if (pars) TCL::ucopy(pars, &mImp, 6);
  if (errs) TCL::ucopy(errs, &mImpImp, 15);
}

StPhysicalHelixD StDcaGeometry::helix() const
{
    //    double curvature = fabs(mCurv);
    int  h = (mCurv>=0) ? 1:-1;
    
    double phase = mPsi-h*M_PI/2;
    
    return StPhysicalHelixD(fabs(mCurv),   // 1/cm
                            atan(mTan),    // radian
                            phase,         // radian
                            origin(),      // cm
                            h);
}

THelixTrack StDcaGeometry::thelix() const
{
    enum {kImp,kZ,kPsi,kPti,kTan};
    
    StThreeVectorD pos = origin();
    StThreeVectorD dir = momentum().unit();
    THelixTrack myHelx(&(pos.x()),&(dir.x()),mCurv);
    double errXY[6],errSZ[6];
    const float *myErr = &mImpImp;
    int jjx=0,jjz=0;
    for (int i=0,li=0;i<5; li+=++i) {
        for (int j=0;j<=i;j++)        {
	  do {// select XY part
	      if(i==kZ || i==kTan)         break;
	      if(j==kZ || j==kTan)         break;
	      errXY[jjx++]=myErr[li+j];
	  }
	  while(0);
	  do {// select SZ part
	      if(i!=kZ && i!=kTan)         break;
	      if(j!=kZ && j!=kTan)         break;
	      errSZ[jjz++]=myErr[li+j];
	  }
	  while(0);
        } }
    errXY[3]*=hz();errXY[4]*=hz();errXY[5]*=hz()*hz();
    myHelx.SetEmx(errXY,errSZ);
    return myHelx;
}
//________________________________________________________________________________
std::ostream&  operator<<(std::ostream& os, const StDcaGeometry& dca) {
  const Float_t *errMx =  dca.errMatrix();
  return os << Form("Dca: imp %7.2f +/-%7.2f,Z:%7.2f +/-%7.2f,psi:%7.2f +/-%7.2f,pT/q:%7.2f +/-%6.1f%%,TanL:%8.3f +/-%8.3f",
		    dca.impact(),    (errMx[0] >= 0)  ? TMath::Sqrt(errMx[0]) : -13,
		    dca.z(),         (errMx[2] >= 0)  ? TMath::Sqrt(errMx[2]) : -13,
		    dca.psi(),       (errMx[5] >= 0)  ? TMath::Sqrt(errMx[5]) : -13,
		    dca.charge()*dca.pt(),    (errMx[9] >= 0 && dca.pt() > 0)  ? 100*TMath::Sqrt(errMx[9])*dca.pt() : -13,
		    dca.tanDip(),    (errMx[14] >= 0) ? TMath::Sqrt(errMx[14]): -13);
}
//________________________________________________________________________________
void   StDcaGeometry::Print(Option_t *option) const {std::cout << *this << std::endl;}
//________________________________________________________________________________
void   StDcaGeometry::GetXYZ(Double_t xyzp[6], Double_t CovXyzp[21]) const {
  static const Float_t one = 1;
  Double_t sinP = TMath::Sin(mPsi);
  Double_t cosP = TMath::Cos(mPsi);
  Double_t pT   = pt();
  xyzp[0] = - mImp*sinP; // x 
  xyzp[1] =   mImp*cosP; // y
  xyzp[2] =   mZ;        // z
  xyzp[3] =   pT*cosP;   // px
  xyzp[4] =   pT*sinP;   // py
  xyzp[5] =   pT*mTan;   // pz
  Double_t dpTdPti = -pT*pT*TMath::Sign(one,mPti);
  Double_t f[30] = {
    //mImp,mZ,       mPsi,         mPti, mTan
    -sinP,  0, -mImp*cosP,            0,    0
    ,cosP,  0, -mImp*sinP,            0,    0
    ,   0,  1,	        0,            0,    0
    ,   0,  0,	 -pT*sinP, dpTdPti*cosP,    0
    ,   0,  0, 	  pT*cosP, dpTdPti*sinP,    0
    ,   0,  0,	        0, dpTdPti*mTan,   pT
  };
  TRMatrix F(6,5,f);
  TRSymMatrix C(5,errMatrix());
  TRSymMatrix Cov(F,TRArray::kAxSxAT,C);
  TCL::ucopy(Cov.GetArray(),CovXyzp,21);
}
//________________________________________________________________________________
KFParticle& StDcaGeometry::Particle(Int_t kg)  const {
  static KFParticle fParticle;
  static KFPTrack track;
  Double_t xyzp[6], CovXyzp[21];
  GetXYZ(xyzp,CovXyzp);
  Float_t xyzF[6], CovXyzF[21];
  TCL::ucopy(xyzp,xyzF,6);
  TCL::ucopy(CovXyzp,CovXyzF,21);
  track.SetParameters(xyzF);
  track.SetCovarianceMatrix(CovXyzF);
  track.SetNDF(1);
  //    track.SetChi2(GlobalTracks_mChiSqXY[k]);
  //  track.SetID(kg);
  Int_t q   = 1;
  Int_t pdg = 211;
  if (charge() < 0) {
    q = -1;
    pdg = -211;
  } 
  track.SetCharge(q);
  fParticle = KFParticle(track, pdg);
  fParticle.SetID(kg);
#if 0
  Float_t d = fParticle.GetCovariance(0) + fParticle.GetCovariance(2) + fParticle.GetCovariance(5);
  if (d <= 0 || d >= 1e6) {
    cout << "particle with broken Cov. Matrix " << *particle << endl;
  }
#endif
  return *&fParticle;
}
