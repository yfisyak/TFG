/***************************************************************************
 *
 * $Id: StETofHeader.h,v 2.1 2018/07/09 14:53:48 ullrich Exp $
 *
 * Author: Pengfei Lyu, April 2018
 ***************************************************************************
 *
 * Description: This class stores ETofHeader information from the DAQ stream,
 * e.g. trigger time and STAR time (reset time stamp of the bTOF clock)
 * and other event-wise information
 *
 ***************************************************************************
 *
 * $Log: StETofHeader.h,v $
 * Revision 2.1  2018/07/09 14:53:48  ullrich
 * Initial Revision.
 *
 *
 ***************************************************************************/
#ifndef STETOFHEADER_H
#define STETOFHEADER_H

#include <map>


#include <Stiostream.h>
#include "StObject.h"

class StETofHeader : public StObject {
public:

    StETofHeader();
    StETofHeader( const double&, const double&, const map< unsigned int, ULong64_t >&, const map< unsigned int, ULong64_t >& ,
                  const unsigned int&, const unsigned int&, const unsigned int&, const ULong64_t& );

    ~StETofHeader();

    double    trgGdpbFullTime()   const;
    double    trgStarFullTime()   const;

    map< unsigned int, ULong64_t > rocGdpbTs()  const;
    map< unsigned int, ULong64_t > rocStarTs()  const;

    unsigned int      starToken()         const;
    unsigned int      starDaqCmdIn()      const;
    unsigned int      starTrgCmdIn()      const;
    ULong64_t          eventStatusFlag()   const;


    void    setTrgGdpbFullTime( const double& gdpbFullTime );
    void    setTrgStarFullTime( const double& starFullTime );

    void    setRocGdpbTs( const map< unsigned int, ULong64_t >& gdpbTs );
    void    setRocStarTs( const map< unsigned int, ULong64_t >& starTs );

    void    setStarToken(    const unsigned int& token    );
    void    setStarDaqCmdIn( const unsigned int& daqCmdIn );
    void    setStarTrgCmdIn( const unsigned int& trgCmdIn );

    void    setEventStatusFlag( const ULong64_t& statusFlag );

private:
    Double_t    mTrgGdpbFullTime;
    Double_t    mTrgStarFullTime;
    
    map< UInt_t, ULong64_t > mRocGdpbTs;
    map< UInt_t, ULong64_t > mRocStarTs;
    
    UInt_t      mStarToken;
    UInt_t      mStarDaqCmdIn;
    UInt_t      mStarTrgCmdIn;
    
    ULong64_t   mEventStatusFlag;


    ClassDef( StETofHeader, 1 )
};

#endif // STETOFHEADER_H
