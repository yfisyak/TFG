*
* $Id: gpgkin.F,v 1.1.1.1 2004/01/15 00:12:17 potekhin Exp $
*
* $Log: gpgkin.F,v $
* Revision 1.1.1.1  2004/01/15 00:12:17  potekhin
*
* Revision 1.1.1.1  1997/11/03 15:30:47  atlascvs
* Importing CERNLIB version 08.21.
*
* Revision 1.1.1.1  1995/10/24 10:21:42  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.23  by  S.Giani
*-- Author :
      SUBROUTINE GPGKIN
C.
C.    ******************************************************************
C.    *                                                                *
C.    *        Print kinematics of secondary tracks at the current     *
C.    *        interaction point.                                      *
C.    *                                                                *
C.    *    ==>Called by : <USER>,GUSTEP,GDEBUG                         *
C.    *       Author    R.Brun  *********                              *
C.    *                                                                *
C.    ******************************************************************
C.
#include "geant321/gcbank.inc"
#include "geant321/gcking.inc"
#include "geant321/gcunit.inc"
      CHARACTER*4 UNITK,UNITE,UNITSK,UNITSE
C.
C.    ------------------------------------------------------------------
C.
      SEK=0.
      SET=0.
      DO 10 I=1,NGKINE
         ITY=GKIN(5,I)
         ET=GKIN(4,I)
         TOFDN=TOFD(I)*1.E+9
         JP=LQ(JPART-ITY)
         EK=ET-Q(JP+7)
         SEK=SEK+EK
         SET=SET+ET
         CALL GEVKEV(EK,EKU,UNITK)
         CALL GEVKEV(ET,ETU,UNITE)
         IF(I.LT.NGKINE)THEN
            WRITE(CHMAIL,1000)I,(IQ(JP+J),J=1,4),EKU,UNITK,ETU,UNITE
     +                         ,TOFDN
         ELSE
            CALL GEVKEV(SEK,SEKU,UNITSK)
            CALL GEVKEV(SET,SETU,UNITSE)
            WRITE(CHMAIL,2000)I,(IQ(JP+J),J=1,4),EKU,UNITK,ETU,UNITE
     +                         ,TOFDN,SEKU,UNITSK,SETU,UNITSE
         ENDIF
         CALL GMAIL(0,0)
  10  CONTINUE
*
 1000 FORMAT(4X,'===>',I3,1X,4A4,' EK=',F7.3,A,' ET=',F7.3,A,
     +       ' TOFD=',F10.3,' ns')
 2000 FORMAT(4X,'===>',I3,1X,4A4,' EK=',F7.3,A,' ET=',F7.3,A,
     +       ' TOFD=',F10.3,' ns   SEK=',F7.3,A,' SET=',F7.3,A)
      END
