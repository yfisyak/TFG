//
// Pibero Djawotho <pibero@iucf.indiana.edu>
// Indiana University
// November 17, 2005
//

// STAR
#include "StThreeVectorD.hh"

// Local
#include "Line.hh"
#include "Track.hh"

#define MAX_SLOPE 0.1

void Track::merge(Track* track)
{
  unsigned int oldSize = size();
  copy(track->begin(), track->end(), back_inserter(*this));
  inplace_merge(begin(), begin()+oldSize, end(), LessHit());
}

bool Track::fit()
{
  //
  // Fit lines to track projections in zx- and zy-plane
  //
  mXfitter.ClearPoints();
  mYfitter.ClearPoints();
  for (iterator i = begin(); i != end(); ++i) {
    StHit* hit = *i;
    double x = hit->position().x();
    double y = hit->position().y();
    double z = hit->position().z();
    mXfitter.AddPoint(&z, x);
    mYfitter.AddPoint(&z, y);
  }
  mXfitter.Eval();
  mYfitter.Eval();
  StThreeVectorD origin(x0(), y0(), 0);
  StThreeVectorD direction(dxdz(), dydz(), 1);
  Line line(origin, direction);
  mLength = abs(line.perigee(lastHit()->position()) - line.perigee(firstHit()->position()));

  return true;
}

bool Track::ok() const
{
  return fabs(dxdz()) <= MAX_SLOPE && fabs(dydz()) <= MAX_SLOPE;
}

bool Track::accept(StHit* hit) const
{
  double dx = x0() + dxdz() * hit->position().z() - hit->position().x();
  double dy = y0() + dydz() * hit->position().z() - hit->position().y();
  double dr = hypot(dx, dy);
  return fabs(dr) <= 5;
}

ostream& operator<<(ostream& os, Track& track)
{
  os << "Track " << &track << " has " << track.size() << " hits:\n";
  for (Track::iterator i = track.begin(); i != track.end(); ++i) {
    StHit* hit = *i;
    os << hit->position() << '\n';
  }
  os << "x0:\t" << track.x0() << " +/- " << track.x0error() << '\n'
     << "y0:\t" << track.y0() << " +/- " << track.y0error() << '\n'
     << "dxdz:\t" << track.dxdz() << " +/- " << track.dxdzError() << '\n'
     << "dydz:\t" << track.dydz() << " +/- " << track.dydzError() << '\n'
     << "chi2zx:\t" << track.chi2zx() << '\n'
     << "chi2zy:\t" << track.chi2zy() << '\n'
     << "probzx:\t" << TMath::Prob(track.chi2zx(), track.ndf()) << '\n'
     << "probzy:\t" << TMath::Prob(track.chi2zy(), track.ndf()) << '\n';
  return os;
}
