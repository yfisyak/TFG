/***************************************************************************
 *
 * $Id: StTrsUnpacker.hh,v 1.1 1999/02/04 18:38:03 lasiuk Exp $
 *
 * Author: bl prelim
 ***************************************************************************
 *
 * Description: Access to the digital information via the abstract
 *              interface
 ***************************************************************************
 *
 * $Log: StTrsUnpacker.hh,v $
 * Revision 1.1  1999/02/04 18:38:03  lasiuk
 * Initial Revision
 *
 **************************************************************************/
#ifndef ST_TRS_UNPACKER_HH
#define ST_TRS_UNPACKER_HH

#include <vector>

// From SCL
#include "StTpcUnpacker.hh"
#include "StSequence.hh"

// From TRS
#include "StTrsRawDataEvent.hh"
#include "StTrsDigitalSector.hh"

class StTrsUnpacker : public StTpcUnpacker {
public:
    StTrsUnpacker();
    ~StTrsUnpacker();
    
    //StTrsUnpacker(const StTrsUnpacker&);
    //StTrsUnpacker& operator=(const StTrsUnpacker&);

    // access functions
    int  getSector(int which, StTpcRawDataEvent* eventData);
    int  getSequences(int padRow, int npad, int *nSeq, StSequence** seq);
    int  getPadList(int padRow, unsigned char **padList);
    void clear();
    
private:
    StTrsDigitalSector* mSector;
    StSequence*         mSequence;
};
#endif
