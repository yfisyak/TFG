if ("$OPTSTAR" == "$XOPTSTAR") then
  setenv XOPTSTAR ${OPTSTAR}/${STAR_HOST_SYS}
  if (! -d $XOPTSTAR) mkdir -p $XOPTSTAR
endif
setenv FORCE_32BITS FALSE
setenv CC  "gcc"
setenv CXX "g++"
setenv F77 "gfortran"
switch (${STAR_HOST_SYS})
  case "*x8664*":
        setenv CC       "gcc -m64"
        setenv CXX      "g++ -m64"
        setenv F77      "gfortran -m64"
	setenv CFLAGS   "-m64 -fPIC"
	setenv CXXFLAGS "-m64 -fPIC"
	setenv LDFLAGS  "-m64"
	setenv FCFLAGS  "-m64 -fPIC"
        setenv arch     "x86_64"
     breaksw
  default:
        setenv FORCE_32BITS TRUE
        setenv CC       "gcc -m32"
        setenv CXX      "g++ -m32"
        setenv F77      "gfortran -m32"
	setenv CFLAGS   "-m32 -fPIC"
	setenv CXXFLAGS "-m32 -fPIC"
	setenv LDFLAGS  "-m32"
	setenv FCFLAGS  "-m32 -fPIC"
        setenv arch     "i386"
     breaksw
endsw
setenv PERL_EXT_CFLAGS   "$CFLAGS"
setenv PERL_EXT_CPPFLAGS "$CXXFLAGS"

switch (${STAR_HOST_SYS})
  case "*icc*"
        setenv CC  icc
        setenv CXX icpc
        setenv F77 ifort
        setenv FC  ifort
     breaksw
  default:
        setenv F77 gfortran
        setenv FC gfortran
endsw
# 
foreach pkg ( apr-1.5.2 apr-util-1.5.4 apache-log4cxx-0.10.0.CVS  fastjet-3.0.3 fftw-3.3.5  texinfo-6.3  gsl   xrootd-4.4.1  Python-2.7.12 pyparsing-1.5.7 ) #gsl  gsl-2.1 
    cd ~/sources/.${STAR_HOST_SYS}
    if ( -r ${pkg}.Done) continue
    if (! -r ${pkg}) then
      if ($pkg != "xrootd-4.4.1") then
        if (-r ~/sources/${pkg}) then
          dirsync  ~/sources/${pkg} ${pkg}
        else 
          if (-r ~/sources/${pkg}.tar) then
            tar xf ~/sources/${pkg}.tar
          else
            break;
          endif
        endif
      else 
        mkdir ${pkg}
      endif
    endif
    cd ${pkg}
    switch ($pkg)
      case "xrootd-4.4.1":
# has problem with gcc 4.8.2
          cmake ../../${pkg} -DCMAKE_FORCE_32BITS=${FORCE_32BITS} -DCMAKE_INSTALL_PREFIX=${XOPTSTAR} -DENABLE_PERL=FALSE -DCMAKE_INSTALL_LIBDIR=lib -DCMAKE_SHARED_LINKER_FLAGS=$LDFLAGS
	  make 
          if ( $?) break;
	  make install
          if ( $?) break;
          touch ../${pkg}.Done
      case "apr-util*":
          ./configure --prefix=$XOPTSTAR --with-apr=$XOPTSTAR
          make install
          if ( $?) break;
          touch ../${pkg}.Done
          breaksw
      case "apache-log4cxx-0.10.0.CVS":
           ./configure --prefix=$XOPTSTAR --with-apr=$XOPTSTAR --with-apr-util=$XOPTSTAR --disable-libtool
	   make clean
           make
          if ( $?) break;
           make install
          if ( $?) break;
          touch ../${pkg}.Done
          breaksw
      case "pyparsing-1.5.7":
          ${XOPTSTAR}/bin/python setup.py install
	  if ( $?) break;
          touch ../${pkg}.Done
          breaksw
     case "gsl":
#	setenv PREFIX $XOPTSTAR
#	libtoolize --force
#	aclocal
#	autoheader
#	automake --force-missing --add-missing
#	autoconf
      sh -x ./autogen.sh
#      if (! -r ./doc/version.texi) then
#    echo "set UPDATED $(date +'%d %B %Y')" > doc/version.texi 
#    echo "set UPDATED-MONTH $(date +'%B %Y')" >> doc/version.texi 
#    echo "set EDITION $(PACKAGE_VERSION)" >> doc/version.texi 
#    echo "set VERSION $(PACKAGE_VERSION)" >> doc/version.texi       
#      endif
      case "gsl*":
#          ./configure -arch "$arch" --prefix=PREFIX=$XOPTSTAR
          ./configure --prefix=$XOPTSTAR --enable-maintainer-mode
          make
	  if ( $?) break;
          make install
	  if ( $?) break;
          touch ../${pkg}.Done
          breaksw
      case "texinfo*":
          ./configure --prefix=$XOPTSTAR --disable-rpath --enable-maintainer-mode
           make
	  if ( $?) break;
           make install
	  if ( $?) break;
          touch ../${pkg}.Done
           breaksw
      case "apr-1.5.1":
      default:
          ./configure --prefix=$XOPTSTAR
          make install
          touch ../${pkg}.Done
          breaksw
     endsw
  endif 
end
