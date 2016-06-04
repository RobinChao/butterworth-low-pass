//
//  Filter.cpp
//  maximilianTest
//
//  Created by Andreu Grimalt-Reynes on 07/12/2014.
//
//

/* Lowpass/highpass Butterworth order 2 and 3 filter implementation. The ony stable configuration is an order 2 low pass filter.
 */

#include "Filter.h"
#include <math.h>

// Calculate the order of the filter given a cuttoff frequency, frequency of attenuation and attenuation level
Filter::Filter(double theAtenuation, double theFreqAtenuation, double theFreqCuttoff,string theType){
    atenuation=theAtenuation;
    freqAtenuation=theFreqAtenuation;
    freqCuttoff=theFreqCuttoff;
    // period
    T=1/44100.0;
    // angular frequencies
    wd1=2*M_PI*freqCuttoff;
    wd2=2*M_PI*freqAtenuation;
    // from filter theory...
    double wa1=(2/T)*tan(wd1*T/2);
    double wa2=(2/T)*tan(wd2*T/2);
    amplitude=pow(10, atenuation/20);
    
    // low pass filter
    if(theType=="lowPass"){
        order=log10(amplitude)/log10(wa2/wa1);
        order=(ceil(order));
    }
    // high pass filter
    if(theType=="highPass"){
        float wlp=wa1*wa1/wa2;
        order=log10(amplitude)/log10(wlp/wa1);
        order=(ceil(order));
    }
    
    printf("%f %s",order," order\n");
    // Attenuation should be higher than cuttoff
    if(order<0){
        printf("Error: Check the attenuation and cuttoff values (e.g for low pass att > cuttoff");
        exit(0);
    }
}

// Make a Butterworth filter: Calculate the coefficients of the difference equation given a cuttoff frequency
Filter::Filter(double theAtenuation, double theFreqAtenuation, double theFreqCuttoff,int theOrder,string theType){

    freqCuttoff=theFreqCuttoff;
    T=1/44100.0;
    wd1=2*M_PI*freqCuttoff;
    type=theType;
    switch(theOrder){
        case 1:
            
            break;
        // order 2
        case 2:
            
            //LOW
            if(type=="lowPass"){
                A=pow(tan(wd1*T/2),2);
                B=sqrt(2)*tan(wd1*T/2);
                C=A;
                
                D=1+B+C;
                
                a0=A/D;
                a1=2*A/D;
                a2=A/D;
                b1=(-1)*(-2+2*C)/D;
                b2=(-1)*(1-B+C)/D;
            }
            
            // HIGH
            if(type=="highPass"){
                // not stable at all
                A=1;
                B=pow(tan(wd1*T/2),4);
                C=-sqrt(2)*pow(tan(wd1*T/2),2);
                D=1+B-C;
                
                a0=A;
                a1=-2*A;
                a2=A;
                b1=(-1)*(-2+2*B)/D;
                b2=(-1)*(1+B+C)/D;
            }
            
            
            break;
        // order 3
        case 3:
            if(type=="lowPass"){
                // not stable when cascading
                A=pow(tan(wd1*T/2),3);
                B=2*tan(wd1*T/2);
                C=2*pow(tan(wd1*T/2),2);
                
                D=A;
                
                E=1+B+C+A;
                
                a0=A/E;
                a1=3*A/E;
                a2=3*A/E;
                a3=A/E;
                b1=(-1)*(-3-B+C+3*D)/E;
                b2=(-1)*(3-B-C+3*D)/E;
                b3=(-1)*(-1+B-C+D)/E;
                //printf("%f",B);
            }
            //HIGH
            if(type=="highPass"){
                // not stable when cascading
                A=1;
                B=2*pow(tan(wd1*T/2),2);
                C=-2*pow(tan(wd1*T/2),4);
                D=pow(tan(wd1*T/2),6);
                E=1+B-C+D;
                
                a0=-A/E;
                a1=3*A/E;
                a2=-3*A/E;
                a3=A/E;
                b1=(-1)*(-3-B-C+3*D)/E;
                b2=(-1)*(3-B+C+3*D)/E;
                b3=(-1)*(-1+B+C+D)/E;
            }
            break;
    }
}
// Perform the filtering operation: Apply the filter difference equation to a given input
double* Filter::doTheFiltering(Filter* filter,double *outs,double *inputs, double* theOutput){
    theOutput[0]=filter->a0*outs[0]+filter->a1*outs[1]+filter->a2*outs[2]+filter->a3*outs[3]+filter->b1*inputs[1]+filter->b2*inputs[2]+filter->b3*inputs[3];
    return theOutput;
}