*
* $Id: lor.F,v 1.1.1.3 2009/02/18 20:33:30 fisyak Exp $
*
* $Log: lor.F,v $
* Revision 1.1.1.3  2009/02/18 20:33:30  fisyak
* *** empty log message ***
*
* Revision 1.1.1.1  2002/06/16 15:18:40  hristov
* Separate distribution  of Geant3
*
* Revision 1.1.1.1  1999/05/18 15:55:18  fca
* AliRoot sources
*
* Revision 1.1.1.1  1995/10/24 10:21:07  cernlib
* Geant
*
*
#include "geant321/pilot.h"
*CMZ :  3.21/02 29/03/94  15.41.38  by  S.Giani
*-- Author :
C
C *** VARIOUS VECTOR OPERATIONS ***
C
C COPIED FROM F14BLO.PAMLIB 23.4.82
C AUTHOR: V.BLOBEL (UNIVERSITY OF HAMBURG)
C DESYLIB
C
C *** BLANK COMMON REPLACED BY /VECUTY/ TO MATCH GEANT/GHEISHA CODE ***
C *** NOTE THAT P(10,100) HAS BECOME PV(10,200) DUE TO THIS         ***
C
C UN-USED ENTRIES REMOVED :
C     "PCOP" "PEXC" "PZER" "PWRT" "DOT4" "IMPU" "IMPULI" "ADD3"
C     "SUB3" "CROSS" "DOT" "SMUL" "NORZ" "PARPER" "PUNIT" "TRAP"
C
C *** ALL ENTRIES RE-WRITTEN AS SUBROUTINES USING ONLY NECESSARY ***
C *** "DOUBLE PRECISION" STMTS. AND ALL SPECIFIC FUNCTIONS HAVE  ***
C *** BEEN CHANGED TO THEIR GENERIC EQUIVALENCES                 ***
C *** NVE 29-MAR-1988 CERN GENEVA ***
C
C ORIGIN : H.FESEFELDT (22-JUNE-1984)
C
      SUBROUTINE LOR(K,L,M)
C
C --- PV-ARRAY ---
#include "geant321/mxgkgh.inc"
#include "geant321/s_blankp.inc"
C
      DOUBLE PRECISION A,B,C
C
      A=0.0
      DO 6 I=1,3
      A=A+PV(I,K)*PV(I,L)
 6    CONTINUE
      A=(A/(PV(4,L)+PV(5,L))-PV(4,K))/PV(5,L)
      B=PV(5,K)*PV(5,K)
      DO 8 I=1,3
      C=PV(I,K)+A*PV(I,L)
      B=B+C*C
      PV(I,M)=C
 8    CONTINUE
      PV(4,M)=SQRT(B)
      PV(5,M)=PV(5,K)
      RETURN
      END
