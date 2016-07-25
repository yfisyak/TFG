#!/bin/csh

# Unpack the source files and set up the build structure, e.g.
#  /tmp/cernlib/2003/src (and lib)

#set list=`ls src_*.gz`

#foreach ffile in $list
#  gunzip -c $ffile | tar xf -
#end

# Establish the environment variables for the build procedures
# Depending on the system, other directories may need to be added to the PATH
# e.g. for the build tools and alternative compilers.

#CERN_LEVEL=`gunzip -c src_Imakefile.tar.gz | tar tf - | awk -F/ '{print $1}'`
setenv CERN_LEVEL 2004
setenv CERN `pwd`
setenv CERN_ROOT $CERN/$CERN_LEVEL
setenv CVSCOSRC $CERN/$CERN_LEVEL/src
setenv PATH $CERN_ROOT/bin:$PATH

#export CERN
#export CERN_LEVEL
#export CERN_ROOT 
#export CVSCOSRC
#export PATH

# Create the build directory structure

cd $CERN_ROOT
mkdir -p build bin lib build/log

# Create the top level Makefile with imake

cd $CERN_ROOT/build
$CVSCOSRC/config/imake_boot

# Install kuipc and the scripts (cernlib, paw and gxint) in $CERN_ROOT/bin

gmake bin/kuipc > log/kuipc 2>&1
gmake scripts/Makefile
cd scripts
gmake install.bin > ../log/scripts 2>&1

# Install the libraries

cd $CERN_ROOT/build
gmake >& log/make.`date +%m%d`



