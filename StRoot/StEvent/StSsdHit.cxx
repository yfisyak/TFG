/***************************************************************************
 *
 * $Id: StSsdHit.cxx,v 2.8 2001/04/05 04:00:55 ullrich Exp $
 *
 * Author: Thomas Ullrich, Jan 1999
 *         Lilian Martin, Dec 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StSsdHit.cxx,v $
 * Revision 2.8  2001/04/05 04:00:55  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.7  2001/03/24 03:34:58  perev
 * clone() -> clone() const
 *
 * Revision 2.6  2000/06/01 21:39:00  ullrich
 * Added member mFlag and access member flag() and setFlag().
 *
 * Revision 2.5  2000/01/05 16:05:37  ullrich
 * Updated for actual use in StEvent. Unpacking changed.
 *
 * Revision 2.4  1999/11/09 19:35:15  ullrich
 * Memory now allocated using StMemoryPool via overloaded new/delete
 *
 * Revision 2.3  1999/11/04 21:40:52  ullrich
 * Added missing default constructor
 *
 * Revision 2.2  1999/10/28 22:26:36  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.1  1999/10/13 19:45:11  ullrich
 * Initial Revision
 *
 **************************************************************************/
#include "StSsdHit.h"
#include "StTrack.h"
#include "tables/St_dst_point_Table.h"

static const char rcsid[] = "$Id: StSsdHit.cxx,v 2.8 2001/04/05 04:00:55 ullrich Exp $";

StMemoryPool StSsdHit::mPool(sizeof(StSsdHit));

ClassImp(StSsdHit)

StSsdHit::StSsdHit() { /* noop */ }

StSsdHit::StSsdHit(const StThreeVectorF& p,
                   const StThreeVectorF& e,
                   unsigned int hw, float q, unsigned char c)
    : StHit(p, e, hw, q, c)
{ /* noop */ }

StSsdHit::StSsdHit(const dst_point_st& pt)
{
    //
    // Unpack charge and status flag
    //
    const unsigned int iflag = pt.charge/(1L<<16);
    const unsigned int ssdq  = pt.charge - iflag*(1L<<16);
    mCharge = float(ssdq)/(1<<21);
    mFlag = static_cast<unsigned char>(iflag);

    //
    // Unpack position in xyz
    //
    const float maxRange   = 40;
    const float mapFactor  = 16000;
    unsigned int ssdy11 = pt.position[0]/(1L<<20);
    unsigned int ssdz   = pt.position[1]/(1L<<10);
    unsigned int ssdx   = pt.position[0] - (1L<<20)*ssdy11;
    unsigned int ssdy10 = pt.position[1] - (1L<<10)*ssdz;
    unsigned int ssdy   = ssdy11 + (1L<<10)*ssdy10;
    mPosition.setX(float(ssdx)/mapFactor - maxRange);
    mPosition.setY(float(ssdy)/mapFactor - maxRange);
    mPosition.setZ(float(ssdz)/mapFactor - maxRange);
    
    //
    // Unpack error on position in xyz
    //
    ssdy11 = pt.pos_err[0]/(1L<<20);
    ssdz   = pt.pos_err[1]/(1L<<10);
    ssdx   = pt.pos_err[0] - (1L<<20)*ssdy11;
    ssdy10 = pt.pos_err[1] - (1L<<10)*ssdz;
    ssdy   = ssdy11 + (1L<<10)*ssdy10;
    mPositionError.setX(float(ssdx)/(1L<<26));
    mPositionError.setY(float(ssdy)/(1L<<26));
    mPositionError.setZ(float(ssdz)/(1L<<26));

    //
    // The hardware position stays at it is
    //
    mHardwarePosition = pt.hw_position;
}

StSsdHit::~StSsdHit() {/* noop */}

StObject*
StSsdHit::clone() const { return new StSsdHit(*this); }

unsigned int
StSsdHit::ladder() const
{
    unsigned long numwaf = (mHardwarePosition>>4) & ~(~0UL<<9);
    return (numwaf/mWaferPerLadder+1);
}

unsigned int
StSsdHit::wafer() const
{
    unsigned long numwaf = (mHardwarePosition>>4) & ~(~0UL<<9);
    return (numwaf-(numwaf/mWaferPerLadder)*mWaferPerLadder+1);
}

unsigned int
StSsdHit::centralStripNSide() const
{
  return bits(13, 10);     // bits 13-22
}

unsigned int
StSsdHit::centralStripPSide() const
{
  return bits(23, 5);      // bits 23-27
}

unsigned int
StSsdHit::clusterSizeNSide() const
{
    return bits(28, 2);    // bits 28-29
}

unsigned int
StSsdHit::clusterSizePSide() const
{
    return bits(30, 2);    // bits 30-31
}
