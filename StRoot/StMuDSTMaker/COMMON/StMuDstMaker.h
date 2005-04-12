/***************************************************************************
 *
 * $Id: StMuDstMaker.h,v 1.40 2005/04/12 21:56:29 mvl Exp $
 * Author: Frank Laue, BNL, laue@bnl.gov
 ***************************************************************************/
#ifndef StMuDstMaker_hh
#define StMuDstMaker_hh

#include <string>

#ifndef ST_NO_NAMESPACES
using namespace std;
#endif

#include "StMaker.h"
#include "StChain.h"
#include "StIOInterFace.h"
#include "St_DataSetIter.h"

#include "StMuArrays.h"

#include "StMuFilter.h"
#include "StMuL3Filter.h"


class StMuEvent;
class StMuDst;
class StMuCut;

class StEvent;
class StTrackNode;
class StTrack;
class StRichSpectra;
class StDetectorState;
class StL3AlgorithmInfo;

class StuProbabilityPidAlgorithm;

class StIOMaker;
class StTreeMaker;

/// strangeness group stuff
class StStrangeEvMuDst;
class StStrangeMuDstMaker;
class StV0MuDst;
class StV0Mc;
class StXiMuDst;
class StXiMc;
class StKinkMuDst;
class StKinkMc;
class StStrangeAssoc;
class StStrangeCuts;

/// emc stuff
#include "StMuEmcCollection.h"
class StMuEmcUtil;

/// PMD by Supriya Das
#include "StMuPmdCollection.h"
class StMuPmdUtil;

// tof stuff
class StMuTofHit;
class StMuTofHitCollection;
class StTofCollection;
class StTofDataCollection;
// run 5 - dongx
class StTofRawDataCollection;
class StMuTofUtil;

// ezTree stuff
class StMuEzTree;

class TFile;
class TTree;
class TChain;
class TClonesArray;

/**
   \class StMuDstMaker

   Class to create and read STAR's common micro dst (StMuDst)

   This class is a true maker in the STAR sense. It inherits from "StMaker" and implements
   the functions "int Init()", "void Clear()", int Make()", and "int Finish()" in order to
   run as part of an "StChain". Please refer to the STAR Computing Web pages in case you do
   not know what "StMaker" and "StChain" mean.

*/
class StMuDstMaker : public StIOInterFace {
 public:
    /// Default constructor
    StMuDstMaker(const char* name="MuDst");
    /// Constructor
    StMuDstMaker(int mode, int nameMode, const char* dirName="./", const char* fileName="", const char* filter=".", int maxfiles=10,
		 const char* name="MuDst" );
    ~StMuDstMaker();

  virtual int Init();
  virtual void Clear(Option_t *option="");
  virtual int  Make();
  virtual int  Finish();
          void printArrays();
          void SetStatus(const char *arrType,int status);
	    /// Set the track filter used for all tracks (except the L3 tracks) when creating muDsts from StEvent and writing to disk.
  void setTrackFilter(StMuCut* c);
  StMuFilter* trackFilter();
  /// Set the track filter used for L3 tracks when creating muDsts from StEvent and writing to disk.
  void setL3TrackFilter(StMuCut* c);
  StMuL3Filter* l3TrackFilter();
  /// Set the file from where the PID probability tables should be read.
  /**
      Set the file from where the PID probability tables should be read.
      These tables might change from production version to production version.
      It is the reposibility of who ever creates muDsts to make sure the right tables
      are used. So far, Aihong was providing these files.
      Thanks, Aihong.
      Note that this method hard code the file name if the argument is not give.
      The default is currently

          getenv("STAR") << "/StarDb/dEdxModel/" << PIDtable

      where PIDtable=PIDTableP01gl.root . This will later change with possibly
      a database approach.

  */
  void setProbabilityPidFile(const char* file=NULL);
  /// Returns pointer to the StMuDst object, the transient class that holds all the TClonesArrays and has access functions to the tracks, v0s, etc.
  /// Returns null pointer if no StMuDst available.
  StMuDst* muDst();
  /// In read mode, returns pointer to the chain of .MuDst.root files that where selected.
  TChain* chain();
  /// Returns pointer to the current TTree, the top level io structure that holds the event, track, v0, etc. information in branches of that tree.
  TTree* tree();

  /// Sets the split level for the file and all branches. Please refer to the ROOT manual (http://root.cern.ch) for more information.
  void setSplit(int=99);
  /// Sets the buffer size for all branches.
  void setBufferSize(int=65536*4);
  /// Sets the compression level for the file and all branches. 0 means no compression, 9 is the higher compression level.
  void setCompression(int comp=9);

  StMuEmcUtil* muEmcUtil() { return mEmcUtil; } ///< return pointer to StMuEmcUtil;
  StMuPmdUtil* muPmdUtil() { return mPmdUtil; } ///< return pointer to StMuPmdUtil;

  virtual const char *GetCVS() const {  ///< Returns version tag.

    static const char cvs[]="Tag $Name:  $ $Id: StMuDstMaker.h,v 1.40 2005/04/12 21:56:29 mvl Exp $ built "__DATE__" "__TIME__ ;
    return cvs;
  }


protected:
  /// routine to set up connection between mEmcCollection and Emc arrays
  void connectEmcCollection();
  void connectPmdCollection();
  enum ioMode {ioRead, ioWrite};
  /** Specifies the way the output file name is contructed when creating muDsts.
      ioFix = use filename specified in when calling the constructor, right in the
      same output file for all input files.
      ioIOMaker = create one output file per input file, derive output filename
      from current input file of the StIOMaker.
      ioTreeMaker = create one output file per input file, derive output filename
      from current input file of the StTreeMaker.
  */
  enum ioNameMode {ioFix=0, ioIOMaker, ioTreeMaker};


  StEvent* mStEvent;
  StMuDst* mStMuDst;
  StStrangeMuDstMaker* mStStrangeMuDstMaker;
  StIOMaker* mIOMaker;
  StTreeMaker* mTreeMaker;
  StMuEmcUtil* mEmcUtil;
  StMuPmdUtil* mPmdUtil;
  StMuTofUtil* mTofUtil;
  StMuEzTree* mEzTree; 

  int mIoMode;
  int mIoNameMode;
  string mDirName;
  string mFileName;
  string mFilter;
  int mMaxFiles;

  unsigned int mTrackType;
  bool mReadTracks;
  bool mReadV0s;
  bool mReadXis;
  bool mReadKinks;
  bool mFinish;

  StMuCut* mTrackFilter;
  StMuCut* mL3TrackFilter;

  TFile* mCurrentFile;
  string mCurrentFileName;

  TChain* mChain;
  TTree* mTTree;

  int mEventCounter;
  int mSplit;
  int mCompression;
  int mBufferSize;

  StuProbabilityPidAlgorithm* mProbabilityPidAlgorithm;


#define saveDelete(t) { delete t; t=0;}

  //! protected:

virtual void openWrite(string fileName);
virtual   void write();
virtual   void closeWrite();

  void streamerOff();

virtual   int openRead();
virtual   void read();
void setBranchAddresses();
virtual   void closeRead();

  void setBranchAddresses(TChain*);

  void assignArrays();
  void clearArrays();
  void zeroArrays();
  void createArrays();
  TClonesArray* clonesArray(TClonesArray*& p, const char* type, int size, int& counter);

  void fill();
  void fillTrees(StEvent* ev, StMuCut* cut=0);
  void fillEvent(StEvent* ev, StMuCut* cut=0);
  void fillPmd(StEvent* ev);
  void fillEmc(StEvent* ev);
  void fillStrange(StStrangeMuDstMaker*);
  void fillL3Tracks(StEvent* ev, StMuCut* cut=0);
  void fillTracks(StEvent* ev, StMuCut* cut=0);
  void fillDetectorStates(StEvent* ev);
  void fillL3AlgorithmInfo(StEvent* ev);
  void fillTof(StEvent* ev);
  void fillEzt(StEvent* ev);

  void fillHddr();

  template <class T, class U> int addType(TClonesArray* tcaTo  , U &u, T *t);
  template <class T>          int addType(TClonesArray* tcaFrom, TClonesArray* &tcaTo ,T *t);
  template <class T>          int addType(TClonesArray* tcaTo  , T &t);
  void addTrackNode(const StEvent* ev, const StTrackNode* node, StMuCut* cut, TClonesArray* gTCA=0, TClonesArray* pTCA=0, TClonesArray* oTCA=0, bool l3=false);
  int  addTrack(TClonesArray* tca, const StEvent* event, const StTrack* track, StMuCut* cut, int index2Global, bool l3=false);
/*   int addType(TClonesArray* tcaTo , StMuEmcCollection t); */

  StRichSpectra* richSpectra(const StTrack* track);

  void setStEvent(StEvent*);
  StEvent* stEvent();
  void setStStrangeMuDstMaker(StStrangeMuDstMaker*);
  StStrangeMuDstMaker* stStrangeMuDstMaker();

  unsigned int trackType();
  bool readTracks();
  bool readV0s();
  bool readXis();
  bool readKinks();
  void setTrackType(unsigned int);
  void setReadTracks(bool);
  void setReadV0s(bool);
  void setReadXis(bool);
  void setReadKinks(bool);

  string basename(string);
  string dirname(string);
  string buildFileName(string dir, string fileName, string extention);

  friend class StMuDst;
  friend class StMuDstFilterMaker;

  // Beware that this was added to counteract contiguous memory.
  // See implementation for caveat
  TClonesArray*  mAArrays         [__NALLARRAYS__];
  TClonesArray** mArrays;       //[__NARRAYS__       ];
  TClonesArray** mStrangeArrays;//[__NSTRANGEARRAYS__];
  TClonesArray** mEmcArrays;    //[__NEMCARRAYS__    ];
  TClonesArray** mPmdArrays;    //[__NPMDARRAYS__    ];
  TClonesArray** mTofArrays;    //[__NTOFARRAYS__    ];
  TClonesArray** mEztArrays;    //[__NEZTARRAYS__    ];
  char           mStatusArrays    [__NALLARRAYS__    ];
  TClonesArray*  mEmcCollectionArray; // Needed to hold old format
  StMuEmcCollection *mEmcCollection;
  TClonesArray*  mPmdCollectionArray; // Needed to hold old format
  StMuPmdCollection *mPmdCollection;

  ClassDef(StMuDstMaker, 0)
};

inline StMuDst* StMuDstMaker::muDst() { return mStMuDst;}
inline TChain* StMuDstMaker::chain() { return mChain; }
inline TTree* StMuDstMaker::tree() { return mTTree; }
inline void StMuDstMaker::setTrackFilter(StMuCut* c) { mTrackFilter=c;}
inline void StMuDstMaker::setL3TrackFilter(StMuCut* c) { mL3TrackFilter=c;}
inline StMuFilter* StMuDstMaker::trackFilter() { return (StMuFilter*)mTrackFilter;}
inline StMuL3Filter* StMuDstMaker::l3TrackFilter() { return (StMuL3Filter*)mL3TrackFilter;}
inline void StMuDstMaker::setStStrangeMuDstMaker(StStrangeMuDstMaker* s) {mStStrangeMuDstMaker=s;}
inline StStrangeMuDstMaker* StMuDstMaker::stStrangeMuDstMaker() {return mStStrangeMuDstMaker;}
inline void StMuDstMaker::setTrackType(unsigned int t) {mTrackType=t;}
inline unsigned int StMuDstMaker::trackType() {return mTrackType;}

inline bool StMuDstMaker::readTracks() { return mReadTracks;}
inline bool StMuDstMaker::readV0s() { return mReadV0s;}
inline bool StMuDstMaker::readXis() { return mReadXis;}
inline bool StMuDstMaker::readKinks() { return mReadKinks;}
inline void StMuDstMaker::setReadTracks(bool b) { mReadTracks=b;}
inline void StMuDstMaker::setReadV0s(bool b) { mReadV0s=b;}
inline void StMuDstMaker::setReadXis(bool b) { mReadXis=b;}
inline void StMuDstMaker::setReadKinks(bool b) { mReadKinks=b;}

inline void StMuDstMaker::setSplit(int split) { mSplit = split;}
inline void StMuDstMaker::setCompression(int comp) { mCompression = comp;}
inline void StMuDstMaker::setBufferSize(int buf) { mBufferSize = buf; }


#endif

/***************************************************************************
 *
 * $Log: StMuDstMaker.h,v $
 * Revision 1.40  2005/04/12 21:56:29  mvl
 * Changes by Xin Dong for year-5 TOF data format: extra TClonesArray and routines to fill it from StEvent (StTofRawData).
 *
 * Revision 1.39  2004/10/28 00:11:33  mvl
 * Added stuff to support ezTree mode of MuDstMaker.
 * This is a special mode for fast-online processing of fast-detector data.
 *
 * Revision 1.38  2004/10/19 01:42:29  mvl
 * Changes for splitting Emc and Pmd collections. Emc clusters and points dropped
 *
 * Revision 1.37  2004/09/18 20:35:09  jeromel
 * Little bit baffled by what CVS did around here
 * http://www.star.bnl.gov/cgi-bin/cvsweb.cgi/StRoot/StMuDSTMaker/COMMON/StMuDstMaker.h.diff?r1=1.35&r2=1.36
 * but re-comitting without the mess up (seems like source and h were merged)
 *
 * Revision 1.35  2004/09/18 01:28:18  jeromel
 * *** empty log message ***
 *
 * Revision 1.34  2004/05/04 00:09:17  perev
 *
 * //  Selecting SetBranchStatus for particular MuDst branches
 * //  Special names:
 * //  MuEventAll - all branches related to StMuEvent
 * //  StrangeAll - all branches related to StrangeMuDst
 * //  EmcAll     - all branches related to Emc
 * //  PmdAll     - all branches related to Pmd
 * //  TofAll     - all branches related to Tof
 * //  By default all branches of MuDst are read. If user wants to read only some of
 * //  them, then:
 * //  SetStatus("*",0)           // all branches off
 * //  SetStatus("MuEventAll",1)  // all standard MuEvent branches ON
 * //  SetStatus("StrangeAll",1)  // all standard Strange branches ON
 * //  SetStatus("EmcAll"    ,1)  // all standard Emc     branches ON
 * //  SetStatus("PmdAll"    ,1)  // all standard Pmd     branches ON
 * //  SetStatus("TofAll"    ,1)  // all standard Tof     branches ON
 * //
 * //  SetStatus("XiAssoc"    ,1) // Strange branch "XiAssoc" is ON
 * //  Names of branches look StMuArrays::arrayTypes[]
 * //  It allows to speed up reading MuDst significantly
 *
 * Revision 1.33  2004/04/26 00:13:28  perev
 * Cleanup+simplification
 *
 * Revision 1.32  2004/04/20 18:42:40  perev
 * remove redundant arrays
 *
 * Revision 1.31  2004/04/15 00:26:14  perev
 * fillHddr() added to fill time stamp ...
 *
 * Revision 1.30  2004/04/09 22:03:47  subhasis
 * after tof createevent fix by Xin
 *
 * Revision 1.29  2004/04/09 03:36:15  jeromel
 * Removed TOF support entirely for now as we need a working version ... Will
 * revisit later.
 *
 * Revision 1.28  2004/04/02 03:24:54  jeromel
 * Changes implements PMD and TOF.  TOF is clearly incomplete.
 *
 * Revision 1.27  2004/02/17 04:56:36  jeromel
 * Extended help, added crs support, restored __GNUC__ for PRETTY_FUNCTION(checked once
 * more and yes, it is ONLY defined in GCC and so is __FUCTION__),  use of a consistent
 * internal __PRETTYF__, return NULL if no case selected (+message) and protected against
 * NULL mChain.
 *
 * Revision 1.26  2003/11/09 01:02:59  perev
 * more sofisticated clear() to fix leaks
 *
 * Revision 1.25  2003/10/30 20:08:13  perev
 * Check of quality added
 *
 * Revision 1.24  2003/10/27 23:54:33  perev
 * weird template bug fized and templates simplified
 *
 * Revision 1.23  2003/10/20 19:50:13  perev
 * workaround added for TClonesArray::Delete + some cleanup of MuEmc
 *
 * Revision 1.22  2003/09/19 01:45:18  jeromel
 * A few problems hopefully fixed i.e. one constructor lacked zeroing
 * emcArrays were not  zeroed, mStMuDst not zeroed.
 * For maintainability zeroArrays() added.
 *
 * Revision 1.21  2003/09/02 17:58:44  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 1.20  2003/04/15 18:48:36  laue
 * Minor changes to be able to filter MuDst.root files and an example
 * how to do this. The StMuDstFilterMaker is just an example, it has to be
 * customized (spoilers, chrome weels, etc.) by the user.
 *
 * Revision 1.19  2003/02/19 15:38:10  jeromel
 * Modifications made to account for the new location of the PIDTable file.
 * The setProbabilityPidFile() method has been modified to take care of a default
 * file loading if unspecified. Messages will be displayed appropriatly.
 * Macros mdoofied to not call the method (leave it handled through the default
 * file).
 *
 * Revision 1.18  2003/01/29 03:04:57  laue
 * !!DIRTY FIX FOR StMuEmcCollection
 * !! Was memor leaking. Leak fixed, but slow and dirty.
 * !! Propose to change the structure as soon as possible.
 *
 * Revision 1.17  2003/01/23 21:59:50  laue
 * Modification to compile on Solaris.
 *
 * Revision 1.16  2003/01/09 18:59:45  laue
 * initial check in of new EMC classes and the changes required
 *
 * Revision 1.15  2002/11/08 14:18:59  laue
 * saveDelete(<T>) added, sets pointer=null after delete
 *
 * Revision 1.14  2002/09/11 21:02:41  laue
 * added cut on track encoded method for ITTF
 *
 * Revision 1.13  2002/08/20 19:55:49  laue
 * Doxygen comments added
 *
 * Revision 1.12  2002/05/20 17:23:31  laue
 * StStrangeCuts added
 *
 * Revision 1.11  2002/05/04 23:56:30  laue
 * some documentation added
 *
 * Revision 1.10  2002/04/26 20:57:31  jeromel
 * Added GetCVS()
 *
 * Revision 1.9  2002/04/11 14:19:30  laue
 * - update for RH 7.2
 * - decrease default arrays sizes
 * - add data base readerfor number of events in a file
 *
 * Revision 1.8  2002/04/01 22:42:30  laue
 * improved chain filter options
 *
 * Revision 1.7  2002/03/28 05:10:34  laue
 * update for running in the production
 *
 * Revision 1.6  2002/03/27 03:47:27  laue
 * better filter options
 *
 * Revision 1.5  2002/03/27 00:50:11  laue
 * bux fix from earlier check in
 *
 * Revision 1.4  2002/03/26 19:33:15  laue
 * minor updates
 *
 * Revision 1.3  2002/03/20 16:04:11  laue
 * minor changes, mostly added access functions
 *
 * Revision 1.2  2002/03/08 20:04:31  laue
 * change from two trees to 1 tree per file
 *
 *
 **************************************************************************/
