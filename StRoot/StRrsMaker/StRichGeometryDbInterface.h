/*************************************************************************
 * $Id: StRichGeometryDbInterface.h,v 1.2 2000/02/08 16:26:04 lasiuk Exp $
 *
 * Description:
 *
 * $Log: StRichGeometryDbInterface.h,v $
 * Revision 1.2  2000/02/08 16:26:04  lasiuk
 * rmove vector and StGlobals from Interface.
 * allocate space for survey parameters
 * calculate sector origins and pad corner positions
 *
 * Revision 1.1  2000/01/25 22:02:20  lasiuk
 * Second Revision
 *************************************************************************/
#ifndef ST_RICH_GEOMETRY_INTERFACE_H
#define ST_RICH_GEOMETRY_INTERFACE_H

#include <iostream.h>

class StRichGeometryDbInterface {
public:
    
    virtual ~StRichGeometryDbInterface() {}
    //StRichGeometryInterface(const StRichGeometryInterface&);
    //StRichGeometryInterface&(const StRichGeometryInterface&);

    virtual double version() const = 0;
    virtual double detectorLength() const = 0;
    virtual double detectorWidth() const = 0;

    virtual double quadrantZ0(int) const = 0;
    virtual double quadrantX0(int) const = 0;
    virtual double quadrantZOrigin(int) const = 0;
    virtual double quadrantXOrigin(int) const = 0;

    virtual double quadrantGapInZ() const = 0;
    virtual double quadrantGapInX() const = 0;    

    virtual int    numberOfPadsInARow()            const = 0; // X
    virtual int    numberOfPadsInAQuadrantRow()    const = 0; // X
    virtual int    numberOfRowsInAColumn()         const = 0; // Z
    virtual int    numberOfRowsInAQuadrantColumn() const = 0; // Z
    virtual int    numberOfPads()                  const = 0;

    virtual double padLength() const = 0;
    virtual double padWidth() const = 0;
    virtual double padPitch() const = 0;
    virtual double rowPitch() const = 0;
    virtual double rowSpacing() const = 0;
    virtual double padSpacing() const = 0;
    
    virtual double wirePitch() const = 0;
    virtual double firstWirePositionInX() const = 0;
    virtual int    numberOfWires() const = 0;

    virtual double anodeToPadSpacing() const = 0;
	
    //
    // Survey Geometry
    virtual double radialDistanceToRich()    const = 0;
    virtual double inclinationAngle()        const = 0;
    
    virtual void   print(ostream& os = cout) const = 0;
};
#endif
