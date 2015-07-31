/****************************************************************
 * $Id: StBTofHitCollection.h,v 1.4 2015/07/28 23:03:05 smirnovd Exp $
 *****************************************************************
 * Author: Xin Dong, Dec 2008
 * Description: Local Barrel TOF hits collection
 *****************************************************************
 * $Log: StBTofHitCollection.h,v $
 * Revision 1.4  2015/07/28 23:03:05  smirnovd
 * Removed headers included twice by mistake while merging
 *
 * Revision 1.3  2015/07/28 22:55:43  smirnovd
 * Added cstddef C++ header defining size_t type
 *
 * Revision 1.2  2015/07/28 14:45:55  jeromel
 * Ill-defined size_t ambiguity removed by adding cstddef
 *
 * Revision 1.1  2009/02/02 21:57:10  dongx
 * first release
 *
 *
 ****************************************************************/
#ifndef ST_BTOF_HIT_COLLECTION_H
#define ST_BTOF_HIT_COLLECTION_H

#include <cstddef>
#include <vector>
#include <string>
#ifndef ST_NO_NAMESPACES
using std::vector;
using std::copy;
#endif

class StBTofHit;

#ifndef ST_NO_DEF_TEMPLATE_ARGS
typedef vector<StBTofHit*> hitVector;
#else
typedef vector<StBTofHit*, allocator<StBTofHit*> > hitVector;
#endif

/**
   \class StBTofHitCollection
   Class used as a collection for BTofHit in StBTofCollection.
 */
class StBTofHitCollection {
public:
    /// Default constructor
    StBTofHitCollection();
    virtual ~StBTofHitCollection();

    /// Add a StBTofHit into the vector
    bool       push_back(StBTofHit* hit);
    /// Returns the size of the collection vector
    size_t     size()  const;
    /// Returns the first element of the vector
    StBTofHit*  front() const;
    /// Returns the last element of the vector
    StBTofHit*  back()  const;
    /// Returns a BTofRawHit at index in the vector
    StBTofHit*  getHit(size_t index) const;

    void clear();
    
private:
    /// StBTofRawHit vector
    hitVector         mHitVector;
};
#endif
