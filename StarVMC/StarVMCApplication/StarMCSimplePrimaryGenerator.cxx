// $Id: StarMCSimplePrimaryGenerator.cxx,v 1.1.1.1 2008/12/10 20:45:52 fisyak Exp $
#include "StarMCSimplePrimaryGenerator.h"
#include "TF1.h"
#include "TGeant3.h"
#include "TEnv.h"
#include "TRandom.h"
#include "StDetectorDbMaker/St_vertexSeedC.h"
ClassImp(StarMCSimplePrimaryGenerator);
Double_t StarMCSimplePrimaryGenerator::fTemperature = 1; // GeV/c
//_____________________________________________________________________________
StarMCSimplePrimaryGenerator::StarMCSimplePrimaryGenerator(Int_t    nprim,   Int_t    Id, 
							   Double_t pT_min , Double_t pT_max,
							   Double_t Eta_min, Double_t Eta_max, 
							   Double_t Phi_min, Double_t Phi_max, 
							   Double_t Z_min,   Double_t Z_max, 
							   const Char_t *option): StarMCPrimaryGenerator() {
  PreSet(); 
  SetGenerator(nprim, Id, pT_min, pT_max, Eta_min, Eta_max, 
	       Phi_min, Phi_max,  Z_min, Z_max, option);
}
//_____________________________________________________________________________
void StarMCSimplePrimaryGenerator::PreSet() {
  fStarStack = 0;
  fIsRandom = false;
  fNofPrimaries = 0; fId = 0;
  fpT_min = 0; fpT_max = 0; fEta_min = 0; fEta_max = 0; fPhi_min = 0; fPhi_max = 0; fZ_min = 0; fZ_max = 0;
  fOption = "";
  fOrigin = TVector3(0,0,0);
  fGun = kFALSE;
  fGunpX = fGunpY = fGunpZ = fGunX = fGunY = fGunZ = 0;
  fGunId = 0;
}
//_____________________________________________________________________________
void StarMCSimplePrimaryGenerator::SetGenerator(Int_t nprim, Int_t Id, 
						Double_t pT_min,Double_t pT_max,
						Double_t Eta_min, Double_t Eta_max, 
						Double_t Phi_min, Double_t Phi_max, 
						Double_t Z_min, Double_t Z_max, const Char_t *option) {
  fGun = kFALSE;
  fNofPrimaries = nprim; 
  fpT_min = pT_min; 
  fpT_max = pT_max; 
  fEta_min = Eta_min; 
  fEta_max = Eta_max; 
  fPhi_min = Phi_min; 
  fPhi_max = Phi_max; 
  fZ_min = Z_min; 
  fZ_max = Z_max; 
  fOption = option; 
  if (fOption.Contains("G",TString::kIgnoreCase)) {
    fId = ((TGeant3* ) TVirtualMC::GetMC())->PDGFromId(Id);
  } else {
    fId = Id;
  }
  cout << "Generate " << fNofPrimaries << " primary tracks of type " << fId << " in " << endl;
  if (! fOption.Contains("BL",TString::kIgnoreCase)) {
    cout << fpT_min << " <  pT < " << fpT_max << endl;
  } else {
    cout << fpT_min << " <  log10(beta*gamma) < " << fpT_max << endl;
  }
  if (fOption.Contains("mtsq",TString::kIgnoreCase)) {
    fTemperature = 0.457;
    cout << "Use dN/dmT^2 = exp(-mT/T) pT generation with T = " << Temperature() << " GeV/c" << endl;
  } else if (fOption.Contains("mt",TString::kIgnoreCase)) {
    cout << "Use dN/dmT = exp(-mT/T) pT generation with T = " << Temperature() << " GeV/c" << endl;
  }
  cout << fEta_min  << " < eta < " << fEta_max  << endl;
  cout << fPhi_min<< " < phi < " << fPhi_max<< endl;
  cout << fZ_min  << " < zVer< " << fZ_max  << endl;
  fgInstance = this;
}
//_____________________________________________________________________________
void StarMCSimplePrimaryGenerator::GeneratePrimary() {     
  // Add one primary particle to the user stack (derived from TVirtualMCStack).
  // Track ID (filled by stack)
  static TF1 *dNdpT = 0; 
  Int_t ntr;
  // Option: to be tracked
  Int_t toBeDone = 1; 
  // Particle type
  Int_t pdg  = fId;
  if (fGun) pdg = fGunId;
  Double_t mass      = TDatabasePDG::Instance()->GetParticle(pdg)->Mass();
  // Polarization
  Double_t polx = 0.; 
  Double_t poly = 0.; 
  Double_t polz = 0.; 
  Double_t px = 0, py = 0, pz = 0;
  Double_t vx = 0, vy = 0, vz = 0;
  Double_t tof = 0.;
  if (fGun) {
    px = fGunpX; py = fGunpY; pz = fGunpZ; vx = fGunX; vy = fGunY; vz = fGunZ; 
  } else {
    // Position
    vx  = fOrigin.X(); 
    vy  = fOrigin.Y(); 
    vz =  fOrigin.Z(); 
    // Energy (in GeV)
    Double_t eta       = fEta_min + (fEta_max - fEta_min)*gRandom->Rndm();
    Double_t phi       = fPhi_min + (fPhi_max - fPhi_min)*gRandom->Rndm();
    Double_t pT        = 0;
    if (fOption.Contains("BL",TString::kIgnoreCase)) {
      Double_t p = -1;
      Double_t bgL10   = fpT_min + (fpT_max - fpT_min)*gRandom->Rndm();
      Double_t bg      = TMath::Power(10.,bgL10);
      p       = mass*bg;
      pT               = p/TMath::CosH(eta);
    } else if (fOption.Contains("mtsq",TString::kIgnoreCase)) {
      if (! dNdpT) {
	dNdpT = new TF1("dNdpT","x*TMath::Exp(-TMath::Sqrt(x*x+[0]*[0])/[1])", fpT_min,fpT_max);
	dNdpT->SetParameters(mass,Temperature());
      }
      pT = dNdpT->GetRandom();
    } else if (fOption.Contains("mt",TString::kIgnoreCase)) {
      while (pT < fpT_min || pT > fpT_max) {
	Double_t mT = mass -Temperature()*TMath::Log(gRandom->Rndm());
	Double_t pT2 = mT*mT - mass*mass;
	pT  = TMath::Sqrt(pT2);
      }
    } else {
      pT               = fpT_min + (fpT_max - fpT_min)*gRandom->Rndm();
    }
    // Particle momentum
    px = pT*TMath::Cos(phi); 
    py = pT*TMath::Sin(phi);
    pz = pT*TMath::SinH(eta);
  }
  // Double_t kinEnergy = 0.050;  
  Double_t e  = TMath::Sqrt(mass*mass + px*px +py*py + pz*pz);
  // Add particle to stack 
  fStarStack->PushTrack(toBeDone, -1, pdg, px, py, pz, e, vx, vy, vz, tof, polx, poly, polz, 
			kPPrimary, ntr, 1., 2);
}
//_____________________________________________________________________________
void StarMCSimplePrimaryGenerator::GeneratePrimaries(const TVector3& origin) {    
  // Fill the user stack (derived from TVirtualMCStack) with primary particles.
  // ---
  Double_t sigmaX = gEnv->GetValue("FixedSigmaX", 0.00176);
  Double_t sigmaY = gEnv->GetValue("FixedSigmaY", 0.00176);
  Double_t sigmaZ = gEnv->GetValue("FixedSigmaZ", 0.00176);
  TVector3 dR(gRandom->Gaus(0, sigmaX), gRandom->Gaus(0, sigmaY), gRandom->Gaus(0, sigmaZ));
  fOrigin = origin + dR;
  for (Int_t i=0; i<fNofPrimaries; i++) GeneratePrimary();  
  fStarStack->SetNprimaries(fNofPrimaries);
}
//_____________________________________________________________________________
void StarMCSimplePrimaryGenerator::GeneratePrimaries() {
  if (! fSetVertex) {
    if (fUseBeamLine) {
      St_vertexSeedC* vSeed = St_vertexSeedC::instance();
      if (vSeed) {
	Double_t x0   = vSeed->x0()  ;// Double_t err_x0   = vSeed->err_x0();
	Double_t y0   = vSeed->y0()  ;// Double_t err_y0   = vSeed->err_y0();
	//	Double_t z0   = 0            ;// Double_t err_z0   = 60; 
	Double_t dxdz = vSeed->dxdz();
	Double_t dydz = vSeed->dydz(); 
	Double_t z    = fZ_min + (fZ_max-fZ_min)*gRandom->Rndm();
	SetOrigin(x0 + dxdz*z, y0 + dydz*z, z);
      } else {
	cout << "Warning : Requested Beam Line, but there is no beam line" << endl;
      }
    } else {
      fOrigin.SetZ(fZ_min + (fZ_max-fZ_min)*gRandom->Rndm());
    }
  }
  GeneratePrimaries(fOrigin);
}
//________________________________________________________________________________
void StarMCSimplePrimaryGenerator::SetGun(Int_t Id, 
					 Double_t px, Double_t py, Double_t pz,
					 Double_t x, Double_t y, Double_t z, const Char_t *option) {
  fGun = kTRUE;
  fGunpX = px; fGunpY = py; fGunpZ = pz; fGunX = x; fGunY = y; fGunZ = z; 
  fOption = option;
  if (fOption.Contains("G",TString::kIgnoreCase)) {
    fGunId = ((TGeant3* ) TVirtualMC::GetMC())->PDGFromId(Id);
  } else {
    fGunId = Id;
  }
  cout << "StarMCSimplePrimaryGenerator::SetGun\tid = " << fGunId 
       << "\tpxyz = (" << fGunpX << "," << fGunpY << "," << fGunpZ
       << ")\txyz = (" << fGunX << "," << fGunY << "," << fGunZ << ")" << endl;
}
