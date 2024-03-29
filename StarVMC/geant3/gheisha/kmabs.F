*
* $Id: kmabs.F,v 1.1.1.3 2009/02/18 20:33:31 fisyak Exp $
*
* $Log: kmabs.F,v $
* Revision 1.1.1.3  2009/02/18 20:33:31  fisyak
* *** empty log message ***
*
* Revision 1.1.1.1  2002/06/16 15:18:40  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:18  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:20:58  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.38  by  S.Giani
*-- Author :
      SUBROUTINE KMABS(NOPT)
C
C *** CHARGED KAON ABSORPTION BY A NUCLEUS ***
C *** NVE 04-MAR-1988 CERN GENEVA ***
C
C ORIGIN : H.FESEFELDT (09-JULY-1987)
C
C PRODUCTION OF A HYPERFRAGMENT WITH SUBSEQUENT DECAY
C PANOFSKY RATIO (K- P --> LAMBDA PI0/K- P --> LAMBDA GAMMA) = 3/2
C
#include "geant321/s_defcom.inc"
      DIMENSION RNDM(4)
C
      CALL COMPO
      PV(1,1)=0.
      PV(2,1)=0.
      PV(3,1)=0.
      PV(4,1)=RMASS(13)
      PV(5,1)=RMASS(13)
      PV(6,1)=-1.
      PV(7,1)=TOF
      PV(8,1)=IPART
      PV(9,1)=0.
      PV(10,1)=USERW
      IER(84)=IER(84)+1
      IF(ATNO2.GT.1.5) GOTO 30
      CALL GRNDM(RNDM,4)
      RAN=RNDM(1)
      TOF1=-12.5*LOG(RAN)
      TOF1= 20.*TOF1
      RAN=RNDM(2)
      ISW=1
      IF(RAN.LT.0.33) ISW=2
      NOPT=ISW
      PV(1,3)=0.
      PV(2,3)=0.
      PV(3,3)=0.
      PV(4,3)=RMASS(18)
      PV(5,3)=RMASS(18)
      PV(6,3)=0.
      PV(7,3)=TOF+TOF1
      PV(8,3)=18.
      PV(9,3)=0.
      PV(10,3)=0.
      PCM=RMASS(13)+RMASS(14)-RMASS(18)
      COST=-1.+RNDM(3)*2.
      SINT=SQRT(ABS(1.-COST*COST))
      PHI=RNDM(4)*TWPI
      IF(ISW.EQ.1) GOTO 1
      PV(1,2)=PCM*COST*SIN(PHI)
      PV(2,2)=PCM*COST*COS(PHI)
      PV(3,2)=PCM*SINT
      PV(4,2)=PCM
      PV(5,2)=0.
      PV(6,2)=0.
      PV(7,2)=TOF+TOF1
      PV(8,2)=1.
      PV(9,2)=0.
      PV(10,2)=0.
      GOTO 2
    1 PCM=PCM*PCM-RMASS(8)*RMASS(8)
      IF(PCM.LE.0.) PCM=0.
      PCM=SQRT(PCM)
      PV(1,2)=PCM*COST*SIN(PHI)
      PV(2,2)=PCM*COST*COS(PHI)
      PV(3,2)=PCM*SINT
      PV(4,2)=SQRT(PCM*PCM+RMASS(8)*RMASS(8))
      PV(5,2)=RMASS(8)
      PV(6,2)=0.
      PV(7,2)=TOF+TOF1
      PV(8,2)=8.
      PV(9,2)=0.
      PV(10,2)=0.
    2 INTCT=INTCT+1.
      CALL SETCUR(2)
      NTK=NTK+1
      CALL SETTRK(3)
      IF(NPRT(3))
     *WRITE(NEWBCD,1002) XEND,YEND,ZEND,P,ISW
1002  FORMAT(1H0,'KAON ABSORBTION   POSITION',3(2X,F8.2),2X,
     * 'K MOMENTUM',2X,F8.4,2X,'INT CODE',2X,I2)
      GO TO 9999
C**
C** STAR PRODUCTION FOR PION ABSORPTION IN HEAVY ELEMENTS
C**
   30 ENP(1)=0.300
      ENP(3)=0.150
      NT=1
      TEX=ENP(1)
      BLACK=0.5*LOG(ATNO2)
      CALL POISSO(BLACK,NBL)
      IF(NPRT(3))
     *WRITE(NEWBCD,3003) NBL,TEX
      IF(NT+NBL.GT.MXGKPV-2) NBL=MXGKPV-2-NT
      IF(NBL.LE.0) NBL=1
      EKIN=TEX/NBL
      EKIN2=0.
      DO 31 I=1,NBL
      IF(NT.EQ.MXGKPV-2) GOTO 31
      CALL GRNDM(RNDM,4)
      RAN2=RNDM(1)
      EKIN1=-EKIN*LOG(RAN2)
      EKIN2=EKIN2+EKIN1
      IPA1=16
      PNRAT=1.-ZNO2/ATNO2
      IF(RNDM(2).GT.PNRAT) IPA1=14
      NT=NT+1
      COST=-1.+RNDM(3)*2.
      SINT=SQRT(ABS(1.-COST*COST))
      PHI=TWPI*RNDM(4)
      IPA(NT)=-IPA1
      PV(5,NT)=ABS(RMASS(IPA1))
      PV(6,NT)=RCHARG(IPA1)
      PV(7,NT)=2.
      PV(4,NT)=EKIN1+PV(5,NT)
      PP=SQRT(ABS(PV(4,NT)**2-PV(5,NT)**2))
      PV(1,NT)=PP*SINT*SIN(PHI)
      PV(2,NT)=PP*SINT*COS(PHI)
      PV(3,NT)=PP*COST
      IF(EKIN2.GT.TEX) GOTO 33
   31 CONTINUE
   33 TEX=ENP(3)
      BLACK=0.50*LOG(ATNO2)
      CALL POISSO(BLACK,NBL)
      IF(NT+NBL.GT.MXGKPV-2) NBL=MXGKPV-2-NT
      IF(NBL.LE.0) NBL=1
      EKIN=TEX/NBL
      EKIN2=0.
      IF(NPRT(3))
     *WRITE(NEWBCD,3004) NBL,TEX
      DO 32 I=1,NBL
      IF(NT.EQ.MXGKPV-2) GOTO 32
      CALL GRNDM(RNDM,4)
      RAN2=RNDM(1)
      EKIN1=-EKIN*LOG(RAN2)
      EKIN2=EKIN2+EKIN1
      NT=NT+1
      COST=-1.+RNDM(2)*2.
      SINT=SQRT(ABS(1.-COST*COST))
      PHI=TWPI*RNDM(3)
      RAN=RNDM(4)
      IPA(NT)=-30
      IF(RAN.GT.0.60) IPA(NT)=-31
      IF(RAN.GT.0.90) IPA(NT)=-32
      PV(5,NT)=(ABS(IPA(NT))-28)*RMASS(14)
      PV(6,NT)=1.
      IF(IPA(NT).EQ.-32) PV(6,NT)=2.
      PV(7,NT)=2.
      PV(4,NT)=PV(5,NT)+EKIN1
      PP=SQRT(ABS(PV(4,NT)**2-PV(5,NT)**2))
      PV(1,NT)=PP*SINT*SIN(PHI)
      PV(2,NT)=PP*SINT*COS(PHI)
      PV(3,NT)=PP*COST
      IF(EKIN2.GT.TEX) GOTO 40
   32 CONTINUE
C**
C** STORE ON EVENT COMMON
C**
   40 CALL GRNDM(RNDM,1)
      RAN=RNDM(1)
      TOF1=-12.5*LOG(RAN)
      TOF1=20.*TOF1
      DO 41 I=2,NT
      IF(PV(7,I).LT.0.) PV(5,I)=-PV(5,I)
      PV(7,I)=TOF+TOF1
      PV(8,I)=ABS(IPA(I))
      PV(9,I)=0.
   41 PV(10,I)=0.
      INTCT=INTCT+1.
      CALL SETCUR(2)
      NTK=NTK+1
      IF(NT.EQ.2) GO TO 9999
      DO 50 I=3,NT
      IF(NTOT.LT.NSIZE/12) GOTO 43
      GO TO 9999
   43 CALL SETTRK(I)
   50 CONTINUE
C
 3003 FORMAT(1H ,I3,' BLACK TRACK PARTICLES PRODUCED WITH TOTAL KINETIC
     * ENERGY OF ',F8.3,' GEV')
 3004 FORMAT(1H ,I5,' HEAVY FRAGMENTS WITH TOTAL ENERGY OF',F8.4,' GEV')
C
 9999 CONTINUE
      END
