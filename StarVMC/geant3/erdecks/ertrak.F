*
* $Id: ertrak.F,v 1.9 2018/06/28 19:29:20 fisyak Exp $
*
* $Log: ertrak.F,v $
* Revision 1.9  2018/06/28 19:29:20  fisyak
* Merge with geant3
*
* Revision 1.8  2013/02/22 20:14:17  fisyak
* Freeze
*
* Revision 1.7  2011/02/11 15:51:14  fisyak
* Merge with latest updates
*
* Revision 1.6  2009/10/01 18:50:20  fisyak
* Add option Mix to geane
*
* Revision 1.5  2009/04/21 20:25:15  fisyak
* Add mixed option 'M'
*
* Revision 1.4  2009/03/30 14:51:54  fisyak
* Merge with current VMC version, add comments
*
* Revision 1.3  2009/02/25 00:08:11  fisyak
* Clean up mess created after merging
*
* Revision 1.1.1.1  2008/12/10 20:46:28  fisyak
* Merge with macos version
*
* Revision 1.2  2003/11/28 11:23:55  brun
* New version of geant321 with all geant3 routines renamed from G to G3
*
* Revision 1.1.1.1  2002/07/24 15:56:26  rdm
* initial import into CVS
*
* Revision 1.1.1.1  2002/06/16 15:18:35  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:15  fca
* AliRoot sources
*
* Revision 1.1.1.1  1996/03/06 15:37:34  mclareni
* Add geane321 source directories
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.49  by  S.Giani
*-- Author :
      SUBROUTINE ERTRAK (X1, P1, X2, P2, IPA, CHOPT)
*
************************************************************************
*                                                                      *
*          Perform the tracking of the track from point X1 to          *
*                    point X2                                          *
*          (Before calling this routine the user should also provide   *
*                    the input informations in /EROPTS/ and /ERTRIO/   *
*                    using subroutine EUFIL(L/P/V)                     *
*                 X1       - Starting coordinates (Cartesian)          *
*                 P1       - Starting 3-momentum  (Cartesian)          *
*                 X2       - Final coordinates    (Cartesian)          *
*                 P2       - Final 3-momentum     (Cartesian)          *
*                 IPA      - Particle code (a la GEANT) of the track   *
*                                                                      *
*                 CHOPT                                                *
*                     'B'   'Backward tracking' - i.e. energy loss     *
*                                        added to the current energy   *
*                     'E'   'Exact' calculation of errors assuming     *
*                                        helix (i.e. pathlength not    *
*                                        assumed as infinitesimal)     *
*                     'L'   Tracking upto prescribed Lengths reached   *
*                     'M'   'Mixed' prediction (not yet coded)         *
*                     'O'   Tracking 'Only' without calculating errors *
*                     'P'   Tracking upto prescribed Planes reached    *
*                     'V'   Tracking upto prescribed Volumes reached   *
*                     'X'   Tracking upto prescribed Point approached  *
*                                                                      *
*                Interface with GEANT :                                *
*             Track parameters are in /CGKINE/ and /GCTRAK/            *
*                                                                      *
*          ==>Called by : USER                                         *
*             Authors   M.Maire, E.Nagy  *********                     *
*                                                                      *
************************************************************************
*
#include "geant321/gcbank.inc"
#include "geant321/gcnum.inc"
#include "geant321/gckine.inc"
#include "geant321/gctmed.inc"
#include "geant321/gctrak.inc"
#include "geant321/gcunit.inc"
#include "geant321/ertrio.inc"
#include "geant321/erwork.inc"
#include "geant321/trcom3.inc"
*
#ifndef SINGLEFIELD
      DOUBLE PRECISION X1D(3), HID(3)
#endif
      DIMENSION      P1(3), P2(3), X1(3), X2(3), DUM(15), IOPT(30)
      EQUIVALENCE    (IOPT(1),IOPTB), (IOPT(2),IOPTE), (IOPT(3),IOPTL),
     ,               (IOPT(4),IOPTM), (IOPT(5),IOPTO), (IOPT(6),IOPTP),
     ,               (IOPT(7),IOPTV), (IOPT(8),IOPTX), (IOPT(9),IOPTZ),
     ,               (IOPT(10),IOPTC)
      CHARACTER      CHOPT*(*)
*
*
* *** Decode character option
*
      CHOPTI = CHOPT(1:len(CHOPT))
      CALL UOPTC (CHOPTI, 'BELMOPVXZC', IOPT)
*     'B'   'Backward tracking' - i.e. energy loss added to the current energy
      IF (IOPTB.EQ.0) THEN
         BACKTR = 1.
      ELSE
         BACKTR = -1.
      ENDIF
*     
      LEVMIX = IOPTM.NE.0 ! 'M'   Mixed prediction, i.e. for 'P' paramters already in SC system or 'V' parametersin SD
      LEEXAC = IOPTE.NE.0 ! 'E'   Exact calculation of errors assuming helix (i.e. pathlength not assumed as infinitesimal)
      LELENG = IOPTL.NE.0 ! 'L'   Tracking upto prescribed Lengths reached
      LEONLY = IOPTO.NE.0 ! 'O'   Tracking 'Only' without calculating errors  
      LEPLAN = IOPTP.NE.0 ! 'P'   Tracking upto prescribed Planes reached    
      LEVOLU = IOPTV.NE.0 ! 'V'   Tracking upto prescribed Volumes reached   
      LEPOIN = IOPTX.NE.0 ! 'X'   Tracking upto prescribed Point approached (not yet coded) 
      LMAXVO = IOPTZ.NE.0 ! 'Z'   Tracking restricted to perigee in [rmin,rmax] and |z| <= zmax
      LECONT = IOPTC.ne.0 ! 'C'   continue tracking from current point
*
* *** Check consistency of the Ch-options
*
         IF ((LELENG .AND. LEVOLU) .OR. (LELENG .AND. LEPLAN) .OR.
     +       (LEVOLU .AND. LEPLAN)) THEN
            WRITE (LOUT, 779)
            GO TO 99
         ENDIF
*
* *** Initialization
*
      IF (NEPRED.LE.0) THEN
         WRITE (LOUT, 780)
         GO TO 99
      ENDIF
      ILPRED = 0
      TLGCM2 = 0.
      TLRAD  = 0.
      CALL VZERO (IEPRED ,   MXPRED)
      CALL VZERO (ERXOUT , 3*MXPRED)
      CALL VZERO (ERPOUT , 3*MXPRED)
      CALL VZERO (ERROUT ,15*MXPRED)
*
      if (LECONT) GO TO 113
      DO 10 I = 1, 3
         ERXIN(I) = X1(I)
   10 CONTINUE
*
      PMOM2 = P1(1)**2 + P1(2)**2 + P1(3)**2
      IF(PMOM2.LE.1.E-20) THEN
         WRITE (LOUT, 778)
         GO TO 99
      ENDIF
      PABS = SQRT (PMOM2)
      ERPIN(1) = 1./PABS
      ERPIN(2) = ASIN (P1(3)*ERPIN(1))
      IF (ABS (P1(1)) .LT. 1.E-30) P1(1) = 1.E-30
      ERPIN(3) = ATAN2 (P1(2), P1(1))
*
* *** Initialize GCKINE common
*
      IF((IPA.LE.0).OR.(IPA.GT.NPART)) THEN
          WRITE (LOUT, 777) IPA
          GO TO 99
      ENDIF
*
      ITRA = 1
      ISTAK = 0
      IPART = IPA
      JPA = LQ(JPART-IPART)
      DO 26 I=1,5
         NAPART(I) = IQ(JPA+I)
   26 CONTINUE
      ITRTYP = Q(JPA+6)
      AMASS  = Q(JPA+7)
      CHARGE = Q(JPA+8)
      CHTR   = CHARGE*BACKTR
      TLIFE  = Q(JPA+9)
*
* *** Starting field
*
      CALL VZERO (HI, 9)
      CALL VZERO (HF, 9)
      IF (IFIELD.EQ.3) THEN
         HI(3) = FIELDM
      ELSEIF (IFIELD.NE.0) THEN
#ifdef SINGLEFIELD
         CALL GUFLD (X1, HI)
#else
            DO J=1,3
               X1D(J)=X1(J)
            END DO
            CALL GUFLD(X1D, HID)
            DO J=1,3
               HI(J)=HID(J)
            END DO
#endif
      ENDIF
*
* *** Error matrix into SC System
*
      IF (LEPLAN .AND. .NOT. LEVMIX .OR. LEVOLU .AND. LEVMIX) THEN
         CALL VZERO (DUM,15)
* Transforms Parameters SC => SD
*                          FROM   SC (PC)   VARIABLES (1/P,LAMBDA,PHI,YT,ZT) 
*                           TO    SD   VARIABLES (1/P,V',W',V,W) (at fixed U)
*                     SPU = sign of u-component of particle momentum (output)              
*                     ERPLI(1,1) = DJ - unit vector in v-direction
*                     ERPLI(1,2) = DK - unit vector in w-direction of detector system
*                     PC        covPC     PD        covPD,    
         CALL TRSCSD (ERPIN(1), DUM(1),   ERPIN(1), DUM(1),   HI(1),
     +                CHARGE, IERR, SPU, ERPLI(1,1), ERPLI(1,2))
         IF(IERR.NE.0) THEN 
*     
*     *** Tracking error - floating point exception
*     
            IERTR=1
            WRITE (LOUT, 780)
            GOTO 99
         ENDIF
         IF (LEONLY) GOTO 35
* Transforms Parameters and Error Matrix SD => SC
*                         FROM  SD   VARIABLES (1/P,V',W',V,W)
*                         TO    SC   VARIABLES (1/P,LAMBDA,PHI,YT,ZT)
*                     SPU       sign of u-component of particle momentum (input)
*                     PD        covPD     PC        covPC
         CALL TRSDSC (ERPIN(1), ERRIN(1), DUM(1),   ERRIN(1), HI(1),
     +                CHARGE, IERR, SPU, ERPLI(1,1), ERPLI(1,2))
         IF(IERR.NE.0) THEN
*     
*     *** Tracking error - floating point exception
*     
            IERTR=1
            WRITE (LOUT, 780)
            GOTO 99
         ENDIF
         DO 29 I = 1, 5
            DO 28 J = 1, 5
               ASDSC(I,J) = A(I,J) ! SD => SC 
   28       CONTINUE
   29    CONTINUE
      ENDIF
      IF (LEONLY) GOTO 35
*
* *** Error matrix into direction of tracking
*
      IF (BACKTR .LT. 0.) CALL ERBCER (ERRIN(1)) ! reverse 
*
* *** Error matrix into double precision
*
      DO 30 I = 1,15
         EI(I) = ERRIN(I)
   30 CONTINUE
*
   35 CONTINUE
      DO 41 I = 1, 3
         VERT(I)  = X1(I)
         PVERT(I) = P1(I)*BACKTR
   41 CONTINUE
*
* *** Initialize GCTRAK common
*
      PVERT(4) = SQRT (PMOM2 + AMASS**2)
      VECT(7) = PABS
      DO 51 I=1,3
         VECT(I) = VERT(I)
         VECT(I+3) = PVERT(I)/VECT(7)
   51 CONTINUE
      GETOT = PVERT(4)
      GEKIN = GETOT - AMASS
      IF(GEKIN.LT.0.) GEKIN = 0.
      CALL G3EKBIN
*
* *** Additional EMC initialization
*
      IF (.NOT.LEONLY) CALL ERPINI  ! initialize B
*
* *** Ready for tracking
*
 113  CALL ERTRGO
*
* *** Copy the final point and momentum into the output buffer
*
      DO 60 I = 1, 3
         X2(I) = VECT(I)
         P2(I) = VECT(7)*BACKTR*VECT(I+3)
   60 CONTINUE
*
  777 FORMAT(/,4X,'Error in ERTRAK : particle type ', I4,
     *        '  unknown in GEANT' )
  778 FORMAT(/,4X,'Error in ERTRAK : Nul Momentum. Tracking stops now')
  779 FORMAT(/,4X,'Error in ERTRAK : Inconsistent character options',
     +                               '. Tracking stops now')
  780 FORMAT(/,4X,'Error in ERTRAK : No prediction. Tracking stops now')
*                                                           END ERTRAK
  99  END
