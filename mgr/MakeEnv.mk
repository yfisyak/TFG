#
#	Determine Make variables
ALL_TAGS = $@
TARGET   = $@
ALL_DEPS = $^
1ST_DEPS = $<
NEW_DEPS = $?
FUL_DEPS = $+
STEM     = $*

WINDOWNT := $(OS)

#
# Current Working Directory
#
  CWD := $(shell pwd)

#
# Determine STAF main env variables.
#



ifndef AFS
  AFS := /afs
  ifdef WINDOWNT
    AFS :=//sol/afs
  endif  
endif  
ifndef AFS_RHIC
  AFS_RHIC := /afs/rhic
  ifdef WINDOWNT
    AFS_RHIC :=//sol/afs_rhic
  endif  
endif  



ifndef STAF_HOME
  STAF_HOME := $(CWD)
endif

ifndef STAF_SYS_LEVEL
   STAF_SYS_LEVEL := dev
endif
# 	

UNAMES := $(shell uname -s)
UNAMER := $(shell uname -r)

#
# Determine STAF_ARCH variable.
#
ifdef STAR_SYS
  STAF_ARCH := $(STAR_SYS)
else
  STAF_ARCH := $(shell sys)
endif
#
# Determine Experiment variable.
#
ifndef EXPERIMENT
#	Is it STAR?
  EXPERIMENT :=STAR
  QWE := $(subst star,LENIN,$(CWD))
  ifneq ($(QWE),$(CWD))
    EXPERIMENT :=STAR
  endif
#	Is it PHENIX?
  QWE := $(subst phenix,LENIN,$(CWD))
  ifneq ($(QWE),$(CWD))
    EXPERIMENT :=PHENIX
  endif
endif
  
#
#	Default value for STAF_SYS
ifndef STAF_SYS  
  STAF_SYS := $(STAR)/asps/..
endif
STAF_SYS := $(strip $(wildcard $(STAF_SYS)))


#
#	Default value for STAF_ANA
ifndef STAF_ANA  
    STAF_ANA := $(STAR)  
endif
STAF_ANA := $(strip $(wildcard $(STAF_ANA)))


ifdef STAF_SYS
  STAF_SYS_INCS := $(STAF_SYS)/inc
#
#	default staf libs
  STAF_SYS_LIB := $(STAF_SYS)/lib
  QWE := $(strip $(wildcard $(STAF_SYS_LIB)))
  ifndef QWE
    STAF_SYS_LIB := $(STAF_SYS)/.$(STAF_ARCH)/lib
  endif
  
  STAF_SYS_LIBS := $(wildcard $(STAF_SYS_LIB)/lib*.a)

  STAF_SYS_BIN := $(STAF_SYS)/bin
  QWE := $(strip $(wildcard $(STAF_SYS_BIN)))
  ifndef QWE
    STAF_SYS_LIBS := $(STAF_SYS)/.$(STAF_ARCH)/bin
  endif
  STIC := $(STAF_SYS_BIN)/stic
  STAFGEN := $(STAF_SYS_BIN)/stafGen
  PAMIGEN := $(STAF_SYS_BIN)/pamiGen.csh
  MAKE_PAMSWITCH := $(STAF_SYS_BIN)/make_pamSwitch

endif


ifdef STAF_ANA
  STAF_ANA_INCS := $(STAF_ANA)/inc
  QWE := $(strip $(wildcard $(STAF_ANA_INCS)))
  ifndef QWE
#	complicated  case
    STAF_ANA_INCS := $(shell find $(STAF_ANA) -name inc -type d)
  endif
#
#	STAF PAM default libs
  STAF_ANA_LIBS:= $(STAF_ANA)/lib
  QWE := $(strip $(wildcard $(STAF_ANA_LIBS)))
  ifndef QWE
    STAF_ANA_LIBS := $(STAF_ANA)/.$(STAF_ARCH)/lib
  endif
   STAF_ANA_LIBS := $(wildcard $(STAF_ANA_LIBS)/lib*.a)
endif

ifndef STAF_CERN
  STAF_CERN := /cern/pro
endif
STAF_CERN_INCS := $(STAF_CERN)/include/cfortran 
STAF_CERN_LIBS := $(shell cernlib geant321 pawlib graflib mathlib)

ifndef STAF_UTILS
  STAF_UTILS := $(AFS_RHIC)/phenix/software/pro/utils
endif
STAF_UTILS_INCS := $(STAF_UTILS)/include
STAF_UTILS_LIBS := $(wildcard $(STAF_UTILS)/lib/lib*.a)



