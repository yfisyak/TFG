/*
  root.exe MakeInnerOuterSector.C
*/
#define __TpcInnerSector__ 
class St_db_Maker;
class TTable;
St_db_Maker *dbMk = 0;
TTable *table = 0;
//________________________________________________________________________________
void MakeInnerOuterSector(const Char_t *opt = 0){
  gROOT->LoadMacro("IOSectorPar.h+");
  cout << "NP = " << NP << endl;
  SurveyPass_t Pass[2];// 0 => half sum, 1 => half diff
  Int_t kPass = NP - 1;
  Int_t NR = 0; // NR = 0 no match, NR = 1 only one set, NR = 2 two sets : half sum and half difference
  TString CS, CD;
  if (opt) {
    kPass = -1;
    for (Int_t k = 0; k < NP; k++) {
      TString Name(Passes[k].PassName);
      if (! Name.Contains(opt,TString::kIgnoreCase)) continue;
      kPass = k; 
      Pass[0] = Passes[k];
      NR = 1;
      break;
    }
    if (kPass < 0) {
      cout << "Pass for " << opt << " has not been found" << endl;
      return;
    }
  } else { //default average for two last passes
    if (kPass < 0) return;
    Int_t kP0 = kPass - 1;
    Int_t kP1 = kPass;
    if (kP0 < 0) kP0 = 0;
    Passes[kP0].Print();
    Passes[kP1].Print();
    Pass[0] = Passes[kP0]; 
    Pass[1] = Passes[kP1];
    NR = 2;
    CS = "("; CS += Passes[kP0].PassName; CS += "+"; CS += Passes[kP1].PassName; CS += ")/2";
    Pass[0].PassName = CS.Data();
    CD = "("; CD += Passes[kP0].PassName; CD += "-"; CD += Passes[kP1].PassName; CD += ")/2";
    Pass[1].PassName = CD.Data();
    for (Int_t l = 0; l < 24; l++) {
      Int_t secs;
      Double_t val, dal, err;
      Double_t *XS = &Pass[0].Data[l].x;       // 0.5*(+)
      Double_t *XD = &Pass[1].Data[l].x;       // 0.5*(-)
      Double_t *X0 = &Passes[kP0].Data[l].x; // FF
      Double_t *X1 = &Passes[kP1].Data[l].x; // RF
      for (Int_t i = 0; i < 6; i++) {
	XS[2*i]   = 0;
	XS[2*i+1] = 0;
	XD[2*i]   = 0;
	XD[2*i+1] = 0;
	if ((i < 3 && 
	     X0[2*i+1] >= 0 && X0[2*i+1] < 99 &&
	     X1[2*i+1] >= 0 && X1[2*i+1] < 99) ||
	    (i >= 3 && 
	     X0[2*i+1] >= 0 && X0[2*i+1] < 0.1 &&
	     X1[2*i+1] >= 0 && X1[2*i+1] < 0.1)
	    )  {
	  val = 0.50*(X0[2*i] + X1[2*i]);
	  dal = 0.50*(X0[2*i] - X1[2*i]);
#if 1
	  val /= 4; // 2;
	  dal /= 4; //2;
	  //	  dal = 0.25*(X0[2*i] - X1[2*i]);
	  //	  dal = 0.125*(X0[2*i] - X1[2*i]);
#endif
	  err = TMath::Sqrt(X0[2*i+1]*X0[2*i+1]+X1[2*i+1]*X1[2*i+1])/2;
	  if (err < 0.001) err = 0.001;
	  XS[2*i]   = val;
	  XS[2*i+1] = err;
	  XD[2*i]   = dal;
	  XD[2*i+1] = err;
	} else {
	}
      }
    }
  }
  cout << "Pass for " << kPass << " has been found" << endl;
  if (! NR) return;
  Pass[0].Print();
  Pass[1].Print();
  gROOT->LoadMacro("bfc.C");
  bfc(0,"mysql,tpcDb,corrX,nodefault");
  StMaker *dbMk = chain->Maker("db");
  if (! dbMk) return;
  dbMk->SetDebug(1);
  StEvtHddr *header = chain->GetEvtHddr();
  header->SetRunNumber(1);
  dbMk->SetDateTime(Pass[0].date,Pass[0].time); 
  header->SetDateTime(Pass[0].date,Pass[0].time);
  chain->MakeEvent();
  dbMk->SetDebug(2);
  dbMk->SetDateTime(Pass[0].date,Pass[0].time); 
  // Outer sector in Inner sector coordinate system
#ifndef  __TpcInnerSector__ 
  St_SurveyC                 *TpcSectorPositionBOld = StTpcOuterSectorPosition::instance();
#else /* __TpcInnerSector__ */
  St_SurveyC                 *TpcSectorPositionBOld = StTpcInnerSectorPosition::instance();
#endif /* !  __TpcInnerSector__ */
  if (! (TpcSectorPositionBOld)) return;
  Int_t NoldRows = TpcSectorPositionBOld->GetNRows();
  Int_t NoSectors = 24;
#ifndef  __TpcInnerSector__ 
  St_Survey      *TpcSectorPositionB = new St_Survey("TpcOuterSectorPositionB",NR*NoSectors);
#else /* __TpcInnerSector__ */
  St_Survey      *TpcSectorPositionB = new St_Survey("TpcInnerSectorPositionB",NR*NoSectors);
#endif /* !  __TpcInnerSector__ */
  TGeoHMatrix Flip  = StTpcDb::instance()->Flip(); cout << "Flip\t"; Flip.Print();
  TGeoHMatrix FlipI = Flip.Inverse();              cout << "FlipI\t"; FlipI.Print();
  for (Int_t r = 0; r < NR; r++) { // half sum & half diff
    for (Int_t s = 0; s < NoSectors; s++) {
      TGeoHMatrix LSold, LS, dR;
      if (r == 0 ) {
	LSold = TpcSectorPositionBOld->GetMatrix(s);   
      } else {
	if (NoldRows > 24) LSold = TpcSectorPositionBOld->GetMatrix(s+24); 
      }
      cout << "===================== Sector \t" << s+1 << endl; cout << "\tLSold\t"; LSold.Print();
      Int_t i = -1; 
      for (Int_t k = 0; k < N; k++) {
	if (TpcSectorPositionBOld->Id(s) ==Pass[r].Data[k].sector) {i = k; break;}
      }
      if (i < 0) {
	cout << "Correction for " << TpcSectorPositionBOld->Id(s) << " is not found" << endl;
      } else {
	cout << "Sector " << TpcSectorPositionBOld->Id(s) << "\ti " << i 
	     << "\talpha " << Pass[r].Data[i].alpha << "+/-" <<Pass[r].Data[i].Dalpha
	     << "\tbeta "  << Pass[r].Data[i].beta  << "+/-" <<Pass[r].Data[i].Dbeta
	     << "\tgamma " << Pass[r].Data[i].gamma << "+/-" <<Pass[r].Data[i].Dgamma
	     << "\tx " << Pass[r].Data[i].x << "+/-" <<Pass[r].Data[i].Dx
	     << "\ty " << Pass[r].Data[i].y << "+/-" <<Pass[r].Data[i].Dy
	     << "\tz " << Pass[r].Data[i].z << "+/-" <<Pass[r].Data[i].Dz << endl;
	Double_t xyz[3] = {0, 0, 0};
#if 0 /* alpha, beta rotations */
	if (Pass[r].Data[i].Dalpha >= 0) dR.RotateX(TMath::RadToDeg()*Pass[r].Data[i].alpha*1e-3);
	if (Pass[r].Data[i].Dbeta  >= 0) dR.RotateY(TMath::RadToDeg()*Pass[r].Data[i].beta *1e-3);
#endif /* no beta rotation */
	if (Pass[r].Data[i].Dgamma >= 0) dR.RotateZ(TMath::RadToDeg()*Pass[r].Data[i].gamma*1e-3);
	if (Pass[r].Data[i].Dx >= 0) xyz[0] =  1e-4*Pass[r].Data[i].x;
	if (Pass[r].Data[i].Dy >= 0) xyz[1] =  1e-4*Pass[r].Data[i].y;
	if (Pass[r].Data[i].Dz >= 0) xyz[2] =  1e-4*Pass[r].Data[i].z;
	dR.SetTranslation(xyz);           cout << "dR\t"; dR.Print();
      }
      //new:  global = Tpc2GlobalMatrix() * SupS2Tpc(sector) * StTpcSuperSectorPosition(sector) * Flip() * {StTpcInnerSectorPosition(sector)} | StTpcOuterSectorPosition(sector)}
      //      TGeoHMatrix dR1 = FlipI * dR * Flip; cout << "F^-1 dR F\t"; dR1.Print();
      TGeoHMatrix dRI = dR.Inverse(); cout << "dR^-1\t"; dRI.Print();
      //      TGeoHMatrix dRT = FlipI * dRI * Flip; cout << "F^-1 dR^-1 F\t"; dRT.Print();
      //      TGeoHMatrix dRT = FlipI * dR * Flip; cout << "F^-1 dR F\t"; dRT.Print();
      //      TGeoHMatrix dRI = dR1.Inverse(); cout << "dR^-1\t"; dRI.Print();
#ifndef __TpcInnerSector__
      LS = dR * LSold; cout << "LS_new\t"; LS.Print();
#else /* __TpcInnerSector__ */
      //      TGeoHMatrix LSoldI = LSold.Inverse();
      //      LS = LSoldI * dRI; cout << "LS_new\t"; LS.Print();
      LS = dRI * LSold; cout << "LS_new\t"; LS.Print();
#endif /* !  __TpcInnerSector__ */
      //      LS = dRT * LSold; cout << "LS_new\t"; LS.Print();
      Survey_st row; memset (&row, 0, sizeof(Survey_st));
      Double_t *rx = LS.GetRotationMatrix();
      memcpy(&row.r00, rx, 9*sizeof(Double_t));
      row.sigmaRotX =Pass[r].Data[i].Dalpha;
      row.sigmaRotY =Pass[r].Data[i].Dbeta;
      row.sigmaRotZ =Pass[r].Data[i].Dgamma;
      row.sigmaTrX  =Pass[r].Data[i].Dx;
      row.sigmaTrY  =Pass[r].Data[i].Dy;
      row.sigmaTrZ  =Pass[r].Data[i].Dz;
      row.Id = TpcSectorPositionBOld->Id(s);
      Double_t *t = LS.GetTranslation();
      memcpy(&row.t0, t, 3*sizeof(Double_t));
      TpcSectorPositionB->AddAt(&row);
    }
  }
  TString fOut =  Form("%s.%8i.%06i.C",TpcSectorPositionB->GetName(),Pass[0].date,Pass[0].time);
  ofstream out;
  cout << "Create " << fOut << endl;
  out.open(fOut.Data());
  out << "TDataSet *CreateTable() {" << endl;
  out << "  if (!gROOT->GetClass(\"St_Survey\")) return 0;" << endl;
  out << "  Survey_st row[" << NR*NoSectors << "] = {" << endl; 
  out << "    //               -gamma      beta     gamma              -alpha     -beta     alpha                 x0       y0       z0" << endl;
  Survey_st *SectorPositionBs = TpcSectorPositionB->GetTable(); 
  for (Int_t r = 0; r < NR; r++) {
    for (Int_t i = 0; i < NoSectors; i++, SectorPositionBs++) { 
      out << "    {" << Form("%2i",TpcSectorPositionBOld->Id(i)); 
      Double_t *rx = &(SectorPositionBs->r00);
      for (Int_t j =  0; j <  9; j++) out << Form(",%9.6f",rx[j]);
      for (Int_t j =  9; j < 12; j++) out << Form(",%8.4f",rx[j]);
      for (Int_t j = 12; j < 18; j++) out << Form(",%5.2f",TMath::Min(99.99,rx[j]));
      out << ",\"" <<Pass[r].PassName << "\"}";
      if (24*r + i != NR*NoSectors - 1) out << ",";
      out << endl;
    } 
  }
  out << "  };" << endl;
  out << "  St_Survey *tableSet = new St_Survey(\"" << TpcSectorPositionB->GetName() << "\"," << NR*NoSectors << ");" << endl; 
  out << "  for (Int_t i = 0; i < " << NR*NoSectors << "; i++) tableSet->AddAt(&row[i].Id, i);" << endl; 
  out << "  return (TDataSet *)tableSet;" << endl;
  out << "}" << endl;
  out.close(); 
}

