// $Id: StAssociator.C,v 1.38 2007/10/01 13:54:03 jeromel Exp $
// $Log: StAssociator.C,v $
// Revision 1.38  2007/10/01 13:54:03  jeromel
// Old issue for Ming Shao
//
// Revision 1.37  2006/08/15 21:43:01  jeromel
// Fix rhic -> rhic.bnl.gov
//
// Revision 1.36  2005/07/07 21:19:18  calderon
// Load EEmcUtil.
//
// Revision 1.35  2004/03/26 23:29:02  calderon
// Added switch to control the id or distance association.
//
// Revision 1.34  2003/03/19 17:16:19  calderon
// remove loading of StarRoot (it's loaded by default).
//
// Revision 1.33  2002/09/22 18:27:11  calderon
// Revert to loading St_Tables instead of all tables individually to avoid
// chasing dependencies every time someone else requires another table.
//
// Revision 1.32  2002/04/24 18:25:21  calderon
// added gSystem->Load("libtpc_Tables");
//       ioMaker->SetBranch("eventBranch",0,"r");
//
// Revision 1.31  2002/04/05 02:34:22  calderon
// Added
//   gSystem->Load("StDetectorDbMaker");
// The hijing files produced now have the event.root branch, in this case,
// the StEventMaker should NOT be used.  So this is now the default, StEventMaker
// is neither loaded nor instantiated.
//
// Revision 1.30  2001/10/23 18:58:50  hardtke
// Load StTpcDb library
//
// Revision 1.29  2001/06/15 19:33:13  jeromel
// StarRoot
//
// Revision 1.28  2001/05/30 22:48:13  calderon
// don't load St_emc_Maker, not needed and caused problems
// with optimized libraries.
//
// Revision 1.27  2001/04/27 18:44:10  calderon
// added comment on usage of L3Trigger switch
//
// Revision 1.26  2001/04/25 21:16:57  jeromel
// Added libgeometry_Tables missing for some StEmcUtil calls.
//
// Revision 1.25  2001/04/09 18:39:42  jeromel
// Just checking (could not commit before). No changes.
//
// Revision 1.21  2000/06/08 20:09:22  calderon
// load St_emc_Maker to work with new Emc Hit classes
//
// Revision 1.20  2000/05/11 22:14:27  calderon
// Go back to getting the histograms from the TList.
//
// Revision 1.19  2000/05/11 16:20:33  calderon
// histograms have to be obtained directly from maker again.
// Examples of using the loading of hits in StMcEventMaker (commented out by
// default).
//
// Revision 1.18  2000/04/20 17:02:42  calderon
// Modified macros to continue looping when status = 3
// Pick up maker with name "StMcAnalysisMaker" instead of "McAnalysis"
// in StAssociator.C
//
// Revision 1.17  2000/04/13 22:01:41  calderon
// proper table and branch activating as per Kathy
//
// Revision 1.16  2000/04/13 21:46:21  kathy
// remove loading of libtpc_Tables since l3Track table is now dst_track type from global
//
// Revision 1.15  2000/04/12 17:39:02  kathy
// change to only load table libraries needed: lib*_Tables instead of all tables: St_Tables
//
// Revision 1.14  2000/01/19 21:00:40  kathy
// update macros to use standard default xdf files in /afs/rhic.bnl.gov/star/data/samples
//
// Revision 1.13  2000/01/19 19:38:12  calderon
// activate runco branch
//
// Revision 1.12  2000/01/12 20:23:01  calderon
// Changed default file to the one produced weekly by Lidia in
// /star/rcf/test/dev/tfs_Linux/Tue/year_2a/hc_standard/
//
// Revision 1.11  1999/12/14 18:18:01  calderon
// using new StMcEvent, StEvent & StAssociationMaker
//
// Revision 1.10  1999/11/03 22:39:35  calderon
// Changed default file.  Previous one was removed.
//
// Revision 1.9  1999/10/01 13:59:18  calderon
// Opened up default Local X cut to 5 mm as a result of
// studies on merged tracks.
// Changed default Hit resolution histogram to plot from global z vs global x
// to the more significant global z vs local x which are the variables
// used in the association criterion.
// The global z vs global x still belongs to StMcAnalysisMaker and is built
// using the multimap, the global z vs local x is made by default in
// StAssociationMaker without any requirement of association to serve
// as a diagnostic.
// Changed size of default canvas
//
// Revision 1.8  1999/09/10 19:11:55  calderon
// Write the Ntuple in StMcAnalysisMaker into a file.
// This way it can be accessed after the macro finishes,
// otherwise it gets deleted.
//
// Revision 1.7  1999/09/10 00:02:24  calderon
// Made the following changes:
// -load StUtilities
// -add line to print number of processed events
// -create canvas here
// -no longer draw # of Pings histogram
//
// Revision 1.6  1999/07/29 15:08:36  calderon
// Include Mom. Resolution example (Histograms & Ntuple)
//
// Revision 1.5  1999/07/28 21:29:34  calderon
// Modified event loop: use 'if' and 'goto' to avoid using 'for'
//
// Revision 1.4  1999/07/28 20:27:45  calderon
// Version with SL99f libraries
//
// Revision 1.3  1999/07/23 14:35:41  calderon
// Updated names of default files and of packages
//
// Revision 1.2  1999/07/23 10:53:48  kathy
// put in header info in Manuel's macros
//
//////////////////////////////////////////////////////////////////////
// owner: Manuel Calderon de la Barca Sanchez
//
// what it does: reads .geant.root file, 
//               runs a chain of 4 makers: 
//                 StEventReaderMaker, StMcEventMaker,StAssociationMaker,
//                 StMcAnalysisMaker
//
// note: for more info on StMcEvent and StAssociationMaker, do a 
//      cvs checkout and say "make" in the doc/tex directory - you'll
//      get a ps file with user guide and reference manual.
//////////////////////////////////////////////////////////////////////

class StChain;
StChain *chain=0;
TCanvas *myCanvas = 0;
void StAssociator(Int_t nevents=100, 
		  const Char_t *MainFile="/star/rcf/test/dev/trs_sl302.ittf/Mon/year_2005/cucu200_minbias/rcf1216_05_200evts.geant.root") {
  // Dynamically link needed shared libs
  gROOT->LoadMacro("bfc.C");
  TString Chain("in,detDb,StEvent,gen_T,sim_T,readall,nodefault,mcana,minimcmk,useInTracker");
  bfc(-1,Chain.Data(),MainFile,0,"mcAnaPlots.root");
  StAssociationMaker* associator    = chain->GetMakerInheritsFrom("StAssociationMaker");
  StMcAnalysisMaker*  examples      = chain->GetMakerInheritsFrom("StMcAnalysisMaker");
  // If you need to use L3 TRIGGER uncomment the line:
  // associator->useL3Trigger();
  //associator->SetDebug();
  // For tracks created with the Sti package (ITTF), uncomment the next line:
  //associator->useInTracker();
  // Switch to use the distance or id association.
  associator->useDistanceAssoc();
  //associator->useIdAssoc();
  // Note: useDistanceAssoc and useIdAssoc are mutually exclusive
  // and they set and unset the same flag.
  // The flag will be set by the call done at the end.
  //associator->doPrintMemoryInfo = kTRUE;

  // Define the cuts for the Associations

  StMcParameterDB* parameterDB = StMcParameterDB::instance();  
  // TPC
  parameterDB->setXCutTpc(.6); // 6 mm
  parameterDB->setYCutTpc(.6); // 6 mm
  parameterDB->setZCutTpc(.6); // 6 mm
  parameterDB->setReqCommonHitsTpc(3); // Require 3 hits in common for tracks to be associated
  // FTPC
  parameterDB->setRCutFtpc(.3); // 3 mm
  parameterDB->setPhiCutFtpc(5*(3.1415927/180.0)); // 5 degrees
  parameterDB->setReqCommonHitsFtpc(3); // Require 3 hits in common for tracks to be associated
  // SVT
  parameterDB->setXCutSvt(.08); // 800 um
  parameterDB->setYCutSvt(.08); // 800 um
  parameterDB->setZCutSvt(.08); // 800 um
  parameterDB->setReqCommonHitsSvt(1); // Require 1 hits in common for tracks to be associated
    
    
  // now execute the chain member functions

  chain->PrintInfo();
  if (nevents < 0) return;
  Int_t initStat = chain->Init(); // This should call the Init() method in ALL makers
  if (initStat) chain->Fatal(initStat, "during Init()");
  if (nevents <= 0) return;
  chain->EventLoop(nevents);
  //________________________________________________________________________________
  TFile *tfile = chain->GetTFile();
  if (tfile) {
    if (! myCanvas)   myCanvas = new TCanvas("mAssociationCanvas", "Histograms",200,10,600,600);
    else              myCanvas->Clear();
    myCanvas->Divide(2,2);
    myCanvas->cd(1);
    gPad->SetLogy(0);
    TNtuple *TrackNtuple = (TNtuple *) tfile->Get("TrackNtuple");
    if (TrackNtuple) TrackNtuple->Draw("(p-prec)/p:commTpcHits","prec!=0&&abs((p-prec)/p)<0.1","colz");
    TH2F* hitRes  = (TH2F *) tfile->Get("hitRes");
    TH1F* momRes  = (TH1F *) tfile->Get("momRes");
    TH2F* coordRc = (TH2F *) tfile->Get("coordRc");
    TH2F* coordMc = (TH2F *) tfile->Get("coordMc");
    if (hitRes) {
      myCanvas->cd(2);
      gPad->SetLogy(0);
      hitRes->Draw("colz");
    }
    if (momRes) {
      myCanvas->cd(3);
      gPad->SetLogy(0);
      momRes->Draw();
    }
    if (coordRc) {
      myCanvas->cd(4);
      gPad->SetLogy(0);
      coordRc->SetMarkerStyle(20);
      coordRc->Draw();
    }
    if (coordMc) {
      myCanvas->cd(4);
      gPad->SetLogy(0);
      coordMc->SetMarkerColor(2);
      coordMc->SetMarkerStyle(20);
      coordMc->Draw("same");
    }
  }
}

