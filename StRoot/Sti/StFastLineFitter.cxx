//StFastLineFitter.cxx
//M.L. Miller (Yale Software)
//03/01

#include <math.h>
#include "Stiostream.h"
#include "Sti/Base/Messenger.h"
#include "Sti/Base/MessageType.h"
#include "StFastLineFitter.h"

using std::cout;
using std::endl;

StFastLineFitter::StFastLineFitter()
{
    cout <<"StFastLineFitter::StFastLineFitter()"<<endl;
}

StFastLineFitter::~StFastLineFitter()
{
    cout <<"StFastLineFitter::~StFastLineFitter()"<<endl;
}

void StFastLineFitter::clear()
{
    mx.clear();
    my.clear();
    mw.clear();
    mchisq = msiga = msigb = mslope = mintercept = 0.;
    mcode = 0;
    return;
}

void StFastLineFitter::print() const
{
    cout <<"\nStFastLineFitter::print()-------------------"<<endl;
    cout <<"slope\tintercept\tchi2\tsigmaA\tsigmaB\tsize"<<endl;
    cout <<slope()<<"\t"<<intercept()<<"\t"<<chiSquared()<<"\t";
    cout <<sigmaA()<<"\t";
    cout <<sigmaB()<<"\t"<<numberOfPoints()<<endl;
    cout <<"Point Collection------------------"<<endl;
    for (int i=0; i<numberOfPoints(); ++i) {
	cout <<mx[i]<<"\t"<<my[i]<<"\t"<<mw[i]<<endl;
    }
    return;
}

//Change later to catch/cache error codes of fit
bool StFastLineFitter::fit()
{
    mcode = dofit();
    return (mcode==0);
}

int StFastLineFitter::dofit()
{
    double sum,sx,sy,sxx,sxy,syy,det;
    double chi;
    int i;
    
  //Executable Statements
    chi=99999999.0;
    
    double n=numberOfPoints();
    //n must be >= 2 for this guy to work
    
    if (n < 2) 	{
	return(1); //to few points, abort
    }
    
    //initialization  
    sum = sx = sy = sxx = sxy = syy = 0.;
    
    //find sum , sumx ,sumy, sumxx, sumxy
    
    for (i=0; i<n; ++i) {
	sum = sum + mw[i];
	sx = sx  + (mw[i])*(mx[i]);
	sy = sy  + (mw[i])*(my[i]);
	sxx = sxx + (mw[i])*(mx[i])*(mx[i]);
	sxy = sxy + (mw[i])*(mx[i])*(my[i]);
	syy = syy + (mw[i])*(my[i])*(my[i]);
    }
    
    det = sum*sxx-sx*sx;
    if (fabs(det) < 1.0e-20) return(2); //Zero determinant, abort
    
    //compute the best fitted parameters A,B
    
    mslope = (sum*sxy-sx*sy)/det;
    mintercept = (sy*sxx-sxy*sx)/det;
    
    //calculate chi-square
    
    chi = 0.0;
    for (i=0; i<n; ++i)	{
	chi = chi+(mw[i])*((my[i])-mslope*(mx[i])-mintercept)*
	    ((my[i])-mslope*(mx[i])-mintercept);
    }
    
    /* calculate estimated variance */
    /* double varsq=chi/(static_cast<double>(n)-2.) */
    
    /*  calculate covariance matrix */
    /*  siga=::sqrt(varsq*sxx/det) */
    /*  sigb=::sqrt(varsq*sum/det) */
    
    msiga = sum/det;
    msigb = sxx/det;
    
    mchisq = chi;
    return(0); //Fit Worked
    
}







