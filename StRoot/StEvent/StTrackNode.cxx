/***************************************************************************
 *
 * $Id: StTrackNode.cxx,v 2.8 2001/04/05 04:00:58 ullrich Exp $
 *
 * Author: Thomas Ullrich, Sep 1999
 ***************************************************************************
 *
 * Description:
 *
 ***************************************************************************
 *
 * $Log: StTrackNode.cxx,v $
 * Revision 2.8  2001/04/05 04:00:58  ullrich
 * Replaced all (U)Long_t by (U)Int_t and all redundant ROOT typedefs.
 *
 * Revision 2.7  2000/08/17 00:35:27  ullrich
 * Added hooks for handling tpt tracks.
 *
 * Revision 2.6  2000/03/23 13:49:31  ullrich
 * Not implemented track type 'secondary' now handled
 * in a more clean way; entries(secondary) returns 0.
 *
 * Revision 2.5  1999/12/01 20:04:58  ullrich
 * Fixed bug in track() method.
 *
 * Revision 2.4  1999/11/09 15:44:17  ullrich
 * Removed method unlink() and all calls to it.
 *
 * Revision 2.3  1999/11/05 15:27:10  ullrich
 * Added non-const versions of several methods
 *
 * Revision 2.2  1999/10/28 22:27:44  ullrich
 * Adapted new StArray version. First version to compile on Linux and Sun.
 *
 * Revision 2.1  1999/10/13 19:45:44  ullrich
 * Initial Revision
 *
 **************************************************************************/
#include <iostream.h>
#include "StTrackNode.h"
#include "StGlobalTrack.h"
#include "StPrimaryTrack.h"

ClassImp(StTrackNode)

static const char rcsid[] = "$Id: StTrackNode.cxx,v 2.8 2001/04/05 04:00:58 ullrich Exp $";

StTrackNode::StTrackNode() { /* noop */ }

StTrackNode::~StTrackNode() { /* noop */ }

void
StTrackNode::addTrack(StTrack* track)
{
    if (track) {
        switch (track->type()) {
        case primary:
            mReferencedTracks.push_back(track);
            break;
        case secondary:                // not implemented yet
            cerr << "StTrackNode::addTrack(): track type 'secondary' not implemented yet." << endl;
            break;
        case global:
        case tpt:
            mOwnedTracks.push_back(track);
            break;
        default:
            cerr << "StTrackNode::addTrack(): cannot add, unknown track type." << endl;
            return;
            break;
        }
        track->setNode(this);
    }
}

void
StTrackNode::removeTrack(StTrack* track)
{
    StPtrVecTrackIterator  iter;
    StSPtrVecTrackIterator iterS;
    if (track) {
        switch (track->type()) {
        case primary:
            for (iter = mReferencedTracks.begin(); iter != mReferencedTracks.end(); iter++)
                if (*iter == track) mReferencedTracks.erase(iter);
            break;
        case secondary:                // not implemented yet
            cerr << "StTrackNode::removeTrack(): track type 'secondary' not implemented yet." << endl;
            break;
        case global:
        case tpt:
            for (iterS = mOwnedTracks.begin(); iterS != mOwnedTracks.end(); iterS++)
                if (*iterS == track) mOwnedTracks.erase(iterS);
            break;
        default:
            cerr << "StTrackNode::removeTrack(): cannot remove, unknown track type." << endl;
            break;
        }
        track->setNode(0);
    }
}

unsigned int
StTrackNode::entries() const
{
    return mReferencedTracks.size() + mOwnedTracks.size();
}

const StTrack*
StTrackNode::track(unsigned int i) const
{
    if (i < mOwnedTracks.size())
        return mOwnedTracks[i];
    else {
        i -= mOwnedTracks.size();
        if (i < mReferencedTracks.size())
            return mReferencedTracks[i];
        else
            return 0;
    }
}

StTrack*
StTrackNode::track(unsigned int i)
{
    if (i < mOwnedTracks.size())
        return mOwnedTracks[i];
    else {
        i -= mOwnedTracks.size();
        if (i < mReferencedTracks.size())
            return mReferencedTracks[i];
        else
            return 0;
    }
}
    
unsigned int
StTrackNode::entries(StTrackType type) const
{
    StSPtrVecTrackConstIterator iterS;
    unsigned int           counter;

    switch (type) {
    case primary:
        return mReferencedTracks.size();
        break;
    case secondary:                // not implemented yet
        cerr << "StTrackNode::entries(): track type 'secondary' not implemented yet." << endl;
        return 0;
        break;
    case tpt:
    case global:
        for (counter = 0, iterS = mOwnedTracks.begin(); iterS != mOwnedTracks.end(); iterS++)
            if ((*iterS)->type() == type) counter++;
        return counter;
        break;
    default:
        cerr << "StTrackNode::entries(): unknown track type." << endl;
        return 0;
        break;
    }
}

const StTrack*
StTrackNode::track(StTrackType type, unsigned int i) const
{
    int          j;
    unsigned int k;

    switch (type) {
    case primary:
        if (i < mReferencedTracks.size())
            return mReferencedTracks[i];
        else
            return 0;
        break;
    case secondary:                // not implemented yet
        cerr << "StTrackNode::track(): track type 'secondary' not implemented yet." << endl;
        return 0;
        break;
    case tpt:
    case global:
        for (j=-1, k=0; k < mOwnedTracks.size(); k++) {
            if (mOwnedTracks[k]->type() == type) j++;
            if (j == static_cast<int>(i)) return mOwnedTracks[k];
        }
        return 0;
        break;
    default:
        cerr << "StTrackNode::track(): unknown track type." << endl;
        return 0;
        break;
    }
}
 
StTrack*
StTrackNode::track(StTrackType type, unsigned int i)
{
    int          j;
    unsigned int k;

    switch (type) {
    case primary:
        if (i < mReferencedTracks.size())
            return mReferencedTracks[i];
        else
            return 0;
        break;
    case secondary:                // not implemented yet
        cerr << "StTrackNode::track(): track type 'secondary' not implemented yet." << endl;
        return 0;
        break;
    case tpt:
    case global:
        for (j=-1, k=0; k < mOwnedTracks.size(); k++) {
            if (mOwnedTracks[k]->type() == type) j++;
            if (j == static_cast<int>(i)) return mOwnedTracks[k];
        }
        return 0;
        break;
    default:
        cerr << "StTrackNode::track(): unknown track type." << endl;
        return 0;
        break;
    }
}

