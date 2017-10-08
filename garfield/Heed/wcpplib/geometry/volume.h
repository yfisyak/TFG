#ifndef VOLUME_H
#define VOLUME_H
#include <iostream>
#include <vector>

#include "wcpplib/geometry/vec.h"
#include "wcpplib/geometry/trajestep.h"
#include "wcpplib/safetl/AbsPtr.h"

/*
The main definitions related to volume.

Copyright (c) 2000 Igor B. Smirnov

The file can be used, copied, modified, and distributed
according to the terms of GNU Lesser General Public License version 2.1
as published by the Free Software Foundation,
and provided that the above copyright notice, this permission notice,
and notices about any modifications of the original text
appear in all copies and in supporting documentation.
The file is provided "as is" without express or implied warranty.
*/

namespace Heed {

class gparticle;
class manip_absvol;
class volume;
class absvol;

/// Service class (array of manip_absvol).
class manip_absvol_treeid {
 public:
  /// Constructor
  manip_absvol_treeid() {}
  /// List of volumes
  std::vector<PassivePtr<manip_absvol> > eid;
  /// Get last address of manipulator
  manip_absvol* G_lamvol() const {
    return eid.empty() ? NULL : eid.back().get();
  }
  /// Get last address of volume
  absvol* G_lavol() const;

  friend int operator==(manip_absvol_treeid& tid1, manip_absvol_treeid& tid2);
  friend int operator!=(manip_absvol_treeid& tid1, manip_absvol_treeid& tid2);
  // The following two functions change reference frame of any object
  // derivative from absref
  void down_absref(absref* f);
  void up_absref(absref* f);

  // The following returns 1 if registered and 0 otherwise
  // Registered means that the route includes this manip_absvol,
  // not necessary the last volume.
  int check_manip_absvol_registered(manip_absvol* amvol);
  int check_absvol_registered(absvol* avol);
  void print(std::ostream& file, int l) const;
};

int operator==(manip_absvol_treeid& tid1, manip_absvol_treeid& tid2);
inline int operator!=(manip_absvol_treeid& tid1, manip_absvol_treeid& tid2) {
  if (tid1 == tid2) return 0;
  return 1;
}

/// Abstract base class for volumes.
/// The functions accept and return parameters expressed in the internal
/// coordinate system inherent to this volume.
/// For interface with external system please use manip_absvol.
class absvol : virtual public absref, public RegPassivePtr {
  // public RegPassivePtr is not necessary for general package
  // but may be useful in applications
 public:
  vfloat prec;
  bool s_sensitive;

  /// Destructor
  virtual ~absvol() {}

  /// Check if a point is inside the volume.
  /// If two volumes are adjacent, it may happen that a point belongs to both.
  /// To avoid this confusion the parameter dir is used.
  /// If dir == (0, 0, 0), and the point is exactly on the border,
  /// the behaviour is in general not specified.
  /// If dir != (0, 0, 0), and the point is on the border with precision prec,
  /// the exiting volume is ignored.
  virtual int check_point_inside(const point& fpt, const vec& dir) const = 0;

  /// Return 1 if point in this volume.
  /// It starts from embraced manipulators, if any
  /// If there are embraced volumes, it add some to namvol,
  /// otherwise it does not add namvol==0.
  /// The embraced volumes should not cross each other,
  /// since this function can return only one chain.
  /// But the borders can coincide with precision given to embraced volumes.
  /// If the point is on the border, it is considered inside volume only if
  /// dir is directed inside it.
  /// Also algorithm of volume is effective if it interrupts
  /// checking after first volume found.
  virtual int find_embed_vol(const point& fpt, const vec& dir,
                             manip_absvol_treeid* atid) const;

  /// range considering this volume, all embracing volumes
  /// sb=0 range restricted by precision reasons.
  /// sb=1 crossing border of this volume
  /// sb=2 crossing border of embraced volume
  /// s_ext=1 inside, and to check all embraced volumes
  /// s_ext=0 outside
  /// checks only one level in deep. It is assumed that
  /// from current volume the particle can pass either outside or
  /// to one of embracing volumes.
  /// In the last case *faeid is filled by its id.
  /// Otherwise *faeid is filled by NULL.
  virtual int range(trajestep& fts, int s_ext, int& sb,
                    PassivePtr<manip_absvol>& faeid) const;

  /// Find cross with current volume ignoring embraced ones.
  /// s_ext=1 exit, now point is inside, but embraced volumes are ingnored.
  /// s_ext=0 enter, now point is outside
  virtual int range_ext(trajestep& fts, int s_ext) const = 0;

  virtual void income(gparticle*) {}
  virtual void chname(char* nm) const { strcpy(nm, "absvol"); }
  virtual void print(std::ostream& file, int l) const;
  virtual absvol* copy() const;
  virtual std::vector<manip_absvol*> Gamanip_embed() const;
};

/*
class absref_transmit_2fixsyscoor : public absref_transmit {
 public:
  fixsyscoor* asys1;
  fixsyscoor* asys2;
  virtual absref* get_other(int n) {
    if (n == 0) return asys1;
    if (n == 1) return asys2;
    mcerr << "absref_transmit_2fixsyscoor::get_other: should never happen\n";
    spexit(mcerr);
    return NULL;  // to quiet compiler
  }
  absref_transmit_2fixsyscoor(void) : absref_transmit() { ; }
  absref_transmit_2fixsyscoor(fixsyscoor* fasys1, fixsyscoor* fasys2)
      : absref_transmit(), asys1(fasys1), asys2(fasys2) {
    qaref_other = 2;
  }
  macro_copy_total(absref_transmit_2fixsyscoor);
  virtual ~absref_transmit_2fixsyscoor() { ; }
};

class absref_transmit_fixsyscoor : public absref_transmit {
 public:
  std::vector<fixsyscoor>* asys;
  virtual absref* get_other(int n) { return &((*asys)[n]); }
  absref_transmit_fixsyscoor(void) : absref_transmit() { ; }
  absref_transmit_fixsyscoor(std::vector<fixsyscoor>* fasys)
      : absref_transmit(), asys(fasys) {
    qaref_other = asys->size();
  }
  macro_copy_total(absref_transmit_fixsyscoor);
  virtual ~absref_transmit_fixsyscoor() { ; }
};

class absref_transmit_absvol : public absref_transmit {
 public:
  std::vector<ActivePtr<absvol> >* avol;
  virtual absref* get_other(int n) { return (*avol)[n].get(); }
  absref_transmit_absvol(void) : absref_transmit() { ; }
  absref_transmit_absvol(std::vector<ActivePtr<absvol> >* favol)
      : absref_transmit(), avol(favol) {
    qaref_other = avol->size();
  }
  macro_copy_total(absref_transmit_absvol);
  virtual ~absref_transmit_absvol() { ; }
};
*/

/// Abstract base classs for volume "manipulators".
class manip_absvol : virtual public absref, public RegPassivePtr {
 public:
  /// Get the volume.
  virtual absvol* Gavol() const = 0;
  /// Get the coordinate system.
  virtual const abssyscoor* Gasc() const {
    // Return NULL if it is the same system
    return NULL;
  }
  virtual int m_check_point_inside(const point& fpt, const vec& dir) const;
  virtual int m_find_embed_vol(const point& fpt, const vec& fdir,
                               manip_absvol_treeid* atid) const;
  // Do the appropriate manipulations with atid and calls avol->find_embed_vol

  // The two following functions changes syscoor if necessary and
  // calls similar named functions of absvol
  virtual int m_range(trajestep& fts, int s_ext, int& sb,
                    PassivePtr<manip_absvol>& faeid) const;
  virtual int m_range_ext(trajestep& fts, int s_ext) const;
  // s_ext=1 inside, but embraced volumes are ingnored.
  // s_ext=0 outside

  // The following two functions change reference frame of any object
  inline void down_absref(absref* f) const {
    const abssyscoor* asc = Gasc();
    if (asc) f->down(asc);
  }
  inline void up_absref(absref* f) const {
    const abssyscoor* asc = Gasc();
    if (asc) f->up(asc);
  }
  void m_chname(char* nm) const;
  virtual void m_print(std::ostream& file, int l) const;
  virtual manip_absvol* copy() const;
  virtual ~manip_absvol() {}
};

// *********  sh_manip_absvol  *********
class sh_manip_absvol : public manip_absvol {
 protected:
  fixsyscoor csys;

 public:
  virtual const abssyscoor* Gasc() const;
  void Psc(const fixsyscoor& fcsys) { csys = fcsys; }

 protected:
  virtual void get_components(ActivePtr<absref_transmit>& aref_tran);
  absref* aref_ptr[1];

 public:
  sh_manip_absvol();
  sh_manip_absvol(sh_manip_absvol& f);
  sh_manip_absvol(const sh_manip_absvol& f);
  sh_manip_absvol(const abssyscoor& f);
  sh_manip_absvol(const point& fc, const basis& fbas, const std::string& fname);
  virtual ~sh_manip_absvol() {}

  virtual void m_chname(char* nm) const;
  virtual void m_print(std::ostream& file, int l) const;
  virtual sh_manip_absvol* copy() const;
};

/*
class absref_transmit_2manip : public absref_transmit {
 public:
  ActivePtr<manip_absvol>* amvol1;
  ActivePtr<manip_absvol>* amvol2;
  virtual absref* get_other(int n) {
    absref* vol = 0;
    if (n == 0) vol = amvol1->get();
    if (n == 1) vol = amvol2->get();
    mcerr << "absref_transmit_2manip::get_other: should never happen\n";
    spexit(mcerr);
    return vol;
  }
  absref_transmit_2manip(void) : absref_transmit() { ; }
  absref_transmit_2manip(ActivePtr<manip_absvol>* famvol1,
                         ActivePtr<manip_absvol>* famvol2)
      : absref_transmit(), amvol1(famvol1), amvol2(famvol2) {
    qaref_other = 2;
  }
  macro_copy_total(absref_transmit_2manip);
  virtual ~absref_transmit_2manip() { ; }
};

class absref_transmit_manip : public absref_transmit {
 public:
  std::vector<ActivePtr<manip_absvol> >* amvol;
  virtual absref* get_other(int n) { return (*amvol)[n].get(); }
  absref_transmit_manip(void) : absref_transmit() { ; }
  absref_transmit_manip(std::vector<ActivePtr<manip_absvol> >* famvol)
      : absref_transmit(), amvol(famvol) {
    qaref_other = amvol->size();
  }
  macro_copy_total(absref_transmit_manip);
  virtual ~absref_transmit_manip() { ; }
};
*/

}

#endif
