#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gfast.h"

/*!
 * @brief Computes matrix of Green's functions required in the CMT inversion
 *
 * @param[in] l1        length of arrays
 * @param[in] ldg       leading dimension of G matrix [must be at least 3*l1]
 * @param[in] x1        x UTM position of receiver (meters) [l1]
 * @param[in] y1        y UTM posiiton of receiver (meters) [l1]
 * @param[in] z1        z elevation above source (meters) [l1]
 * @param[in] azi       source receiver azimuth (degrees) [l1]
 *
 * @param[out] G        matrix of Green's functions stored in column major 
 *                      format [ldg x 6]
 *
 * @result 0 indicates success
 *
 * @author Brendan Crowell, PNSN
 * @author Ben Baker, ISTI - converted from Python to C
 * @date January, 2015
 *
 */
int GFAST_CMTgreenF(int l1, int ldg, 
                    double *x1, double *y1, double *z1, double *azi, 
                    double *G)
{
    const char *fcnm = "GFAST_CMTgreenF\0";
    double G1[6], G2[6], G3[6], 
           azi1, C1, C2, cosaz, cosaz2,  
           g111, g122, g133, g112, g113, g123,
           g211, g222, g233, g212, g213, g223,
           g311, g322, g333, g312, g313, g323,
           R, R3, sinaz, sinaz2, x, y, z;
    int i; 
    const double MU = 3.e10;
    const double K = 5*MU/3;
    const double pi180 = M_PI/180.0;
    //------------------------------------------------------------------------//
    //
    // Size check
    if (l1 < 1){
        printf("%s: Error invalid number of points %d\n", fcnm, l1);
        return 1;
    }
    if (ldg < 3*l1){
        printf("%s: Error ldg is too small %d\n", fcnm, ldg);
        return 1;
    }
    // Loop on points and fill up Green's functions matrix
    for (i=0; i<l1; i++){
        // define some constants for this point
        azi1 = azi[i]*pi180;
        sinaz = sin(azi1);
        cosaz = cos(azi1);
        sinaz2 = pow(sinaz, 2);
        cosaz2 = pow(cosaz, 2);
        // compute coefficients in greens functions
        x = x1[i];
        y = y1[i];
        z = z1[i];
        R = sqrt(pow(x,2) + pow(y,2) + pow(z,2));
        C1= 1.0/pow(R,2)/MU/M_PI/8.0;
        C2 = (3.0*K + MU)/(3.0*K + 4.0*MU);
        R3 = pow(R,3);
        // coefficients for row 1
        g111 = C1*( C2*(3.0*x*x*x/R3 - 3.0*x/R) + 2.0*x/R );
        g122 = C1*( C2*(3.0*x*y*y/R3 - x/R) );
        g133 = C1*( C2*(3.0*x*z*z/R3 - x/R) );
        g112 = C1*( C2*(3.0*x*x*y/R3 - y/R) + 2.0*y/R );
        g113 = C1*( C2*(3.0*x*x*z/R3 - z/R) + 2.0*z/R );
        g123 = C1*( C2*(3.0*x*y*z/R3) );
        // coefficients for row 2
        g211 = C1*( C2*(3.0*y*x*x/R3 - y/R) );
        g222 = C1*( C2*(3.0*y*y*y/R3 - 3.0*y/R) + 2.0*y/R );
        g233 = C1*( C2*(3.0*y*z*z/R3 - y/R) );
        g212 = C1*( C2*(3.0*y*x*y/R3 - x/R) );
        g213 = C1*( C2*(3.0*y*x*z/R3) );
        g223 = C1*( C2*(3.0*y*y*z/R3 - z/R) + 2.0*z/R );
        // coefficients for row 3
        g311 = C1*( C2*(3.0*z*x*x/R3 - z/R) );
        g322 = C1*( C2*(3.0*z*y*y/R3 - z/R) );
        g333 = C1*( C2*(3.0*z*z*z/R3 - 3.0*z/R) + 2.0*z/R );
        g312 = C1*( C2*(3.0*z*x*y/R3) );
        g313 = C1*( C2*(3.0*z*x*z/R3) + 2.0*x/R);
        g323 = C1*( C2*(3.0*z*y*z/R3) + 2.0*y/R);
        // row 1
        G1[0] = g111*cosaz2 + g122*sinaz2 + 2.0*g112*cosaz*sinaz;
        G1[1] = g111*sinaz2 + g122*cosaz2 - 2.0*g112*cosaz*sinaz;
        G1[2] = g133;
        G1[3] =-g111*sinaz*cosaz + g112*cosaz2 - g112*sinaz2 + g122*sinaz*cosaz;
        G1[4] = g113*cosaz + g123*sinaz;
        G1[5] = g123*cosaz - g113*sinaz;
        // row 2
        G2[0] = g211*cosaz2 + g222*sinaz2 + 2.0*g212*cosaz*sinaz;
        G2[1] = g211*sinaz2 + g222*cosaz2 - 2.0*g212*cosaz*sinaz;
        G2[2] = g233;
        G2[3] =-g211*sinaz*cosaz + g212*cosaz2 - g212*sinaz2 + g222*sinaz*cosaz;
        G2[4] = g213*cosaz + g223*sinaz;
        G2[5] = g223*cosaz - g213*sinaz;
        // row 3
        G3[0] = g311*cosaz2 + g322*sinaz2 + 2.0*g312*cosaz*sinaz;
        G3[1] = g311*sinaz2 + g322*cosaz2 - 2.0*g312*cosaz*sinaz;
        G3[2] = g333;
        G3[3] =-g311*sinaz*cosaz + g312*cosaz2 - g312*sinaz2 + g322*sinaz*cosaz;
        G3[4] = g313*cosaz + g323*sinaz;
        G3[5] = g323*cosaz - g313*sinaz;
        // Fill green's matrix in cache friendly way; column 1
        G[3*i   + 0*ldg] = G1[0];
        G[3*i+1 + 0*ldg] = G2[0];
        G[3*i+2 + 0*ldg] = G3[0];
        // Column 2
        G[3*i   + 1*ldg] = G1[1];
        G[3*i+1 + 1*ldg] = G2[1];
        G[3*i+2 + 1*ldg] = G3[1];
        // Column 3
        G[3*i   + 2*ldg] = G1[2];
        G[3*i+1 + 2*ldg] = G2[2];
        G[3*i+2 + 2*ldg] = G3[2];
        // Column 4
        G[3*i   + 3*ldg] = G1[3];
        G[3*i+1 + 3*ldg] = G2[3];
        G[3*i+2 + 3*ldg] = G3[3];
        // Column 5
        G[3*i   + 4*ldg] = G1[4];
        G[3*i+1 + 4*ldg] = G2[4];
        G[3*i+2 + 4*ldg] = G3[4];
        // Column 6 
        G[3*i   + 5*ldg] = G1[5];
        G[3*i+1 + 5*ldg] = G2[5];
        G[3*i+2 + 5*ldg] = G3[5];
    } // Loop on points
    return 0;
}