//
//  Filter.h
//  maximilianTest
//
//  Created by Andreu Grimalt-Reynes on 07/12/2014.
//
//

#ifndef __maximilianTest__Filter__
#define __maximilianTest__Filter__

#include <stdio.h>
#include <string>
#endif /* defined(__maximilianTest__Filter__) */
using namespace std;
class Filter{
    public:
        Filter(double atenuation, double freqAtenuation,double theFreqCuttoff,string theType);
    Filter(double atenuation, double freqAtenuation,double theFreqCuttoff,int theOrder,string theType);
        // parameters
        double atenuation,amplitude,order,freqAtenuation,freqCuttoff,T,wd1,wd2;
        
        double A,B,C,D,E,F,G;
        double a0=0;
        double a1=0;
        double a2=0;
        double a3=0;
        double a4=0;
        double b1=0;
        double b2=0;
        double b3=0;
        double b4=0;
        std::string type;
    
        double* doTheFiltering(Filter* filter,double *outs,double *inputs, double* theOutput);
    
};