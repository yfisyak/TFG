*
* $Id: pcodat.inc,v 1.2 2013/02/22 20:14:17 fisyak Exp $
*
* $Log: pcodat.inc,v $
* Revision 1.2  2013/02/22 20:14:17  fisyak
* Freeze
*
* Revision 1.1.1.1  2002/06/16 15:18:38  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:17  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:20:43  cernlib
* Geant
*
*
#ifndef CERNLIB_GEANT321_PCODAT_INC
#define CERNLIB_GEANT321_PCODAT_INC
*
*
* pcodat.inc
*
C --- DATA STMTS. FOR GEANT/GHEISHA PARTICLE CODE CONVERSIONS ---
C --- KIPART(I)=GHEISHA CODE CORRESPONDING TO GEANT   CODE I ---
C --- IKPART(I)=GEANT   CODE CORRESPONDING TO GHEISHA CODE I ---
C
      DATA KIPART/
     $               1,   3,   4,   2,   5,   6,   8,   7,
     $               9,  12,  10,  13,  16,  14,  15,  11,
     $              35,  18,  20,  21,  22,  26,  27,  33,
     $              17,  19,  23,  24,  25,  28,  29,  34,
     $              35,  35,  35,  35,  35,  35,  35,  35,
     $              35,  35,  35,  35,  30,  31,  32,  35,
     $              36,  35,  35,  35,  35,  35,  35,  35,
     $              35,  35,  35,  35,  35,  35,  35,  35,
     $              35,  35,  35,  35,  35,  35,  35,  35,
     $              35,  35,  35,  37,  38,  39,  40/
C
      DATA IKPART/
     $               1,   4,   2,   3,   5,   6,   8,   7,
     $               9,  11,  16,  10,  12,  14,  15,  13,
     $              25,  18,  26,  19,  20,  21,  27,  28,
     $              29,  22,  23,  30,  31,  45,  46,  47,
     $              24,  32,  48,  49,  76,  77,  78,  79/
C

#endif
