*
* $Id: part3.inc,v 1.1.1.3 2009/02/18 20:33:10 fisyak Exp $
*
* $Log: part3.inc,v $
* Revision 1.1.1.3  2009/02/18 20:33:10  fisyak
* *** empty log message ***
*
* Revision 1.1.1.1  2002/06/16 15:18:38  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:17  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:20:42  cernlib
* Geant
*
*
#ifndef CERNLIB_GEANT321_PART3_INC
#define CERNLIB_GEANT321_PART3_INC
*
*
* part3.inc
*
*
*=== part3 ============================================================*
*
*----------------------------------------------------------------------*
*     Include file: part3 copy        Revised on 20-7-90 by A. Ferrari *
*     Note: see also part copy and part2 copy                          *
*     Changes: none                                                    *
*     Included in the following subroutines or functions: not updated  *
*                                                                      *
*     Description of the common block(s) and variable(s)               *
*                                                                      *
*         Kptoip = conversion from part to paprop numbering            *
*         Iptokp = conversion from paprop to part numbering            *
*                                                                      *
*----------------------------------------------------------------------*
*
      PARAMETER (IDMAX8=183)
      CHARACTER*8  ANAMC
      COMMON / FKPART / AMC  (IDMAX8), GAC    (IDMAX8), TAUC   (IDMAX8),
     &                AMCDSC (IDMAX8), ICHC   (IDMAX8), IBARC  (IDMAX8),
     &                K1C    (IDMAX8), K2C    (IDMAX8), KPTOIP (IDMAX8),
     &                IPTOKP (39)
      COMMON / FKCHPA / ANAMC (IDMAX8)
 

#endif
