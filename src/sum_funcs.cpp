// Functions to approximate the infinite sum in the density function

#include <Rcpp.h>
// [[Rcpp::plugins(cpp11)]]
#define _USE_MATH_DEFINES
#include <math.h>
#include "funcs.h"




//////////                                                           //////////
///////////////////////////////// Small Time //////////////////////////////////
//////////                                                           //////////


// BGK2014 style truncated sum
double small_sum_2014(double t, double a, double w, int ks)
{
  double gamma = -a*a/(2*t);
  double sum = w * exp(gamma*w*w); // start at j=0
  for (int j = ks; j > 0; j--) { // iterate through all ks
    sum += (2*j + w) * exp(gamma*(2*j + w)*(2*j + w))
         - (2*j - w) * exp(gamma*(2*j - w)*(2*j - w));
  }
  return sum;
}


// BGK2017 style truncated sum
double small_sum_2017(double t, double a, double w, int ks)
{
  double gamma = -a*a/(2*t);
  double sum = w * exp(gamma*w*w); // start at j=0
  double rj;
  int j = 1;
  while (j <= 2*ks) { // start at j=1
    rj = j + 1 - w; // j is odd
    sum -= rj * exp(gamma*rj*rj);
    j++;
    rj = j + w; // j is even
    sum += rj * exp(gamma*rj*rj);
    j++;
  }
  return sum;
}


// term < eps BGK2014 style truncated sum, with minimum terms
double small_sum_eps_14(double t, double a, double w, double mult, double eps)
{
  eps /= mult;
  double gamma = -a*a/(2*t);
  double sum = w * exp(gamma * w * w); // start at j=0 term
  double minterms = sqrt(t)/(2*a) - w/2; // minimum number of terms
  double pterm = 0;
  double nterm = sum;
  int j = 0;
  while (j < minterms) { // capture increasing terms
    j++;
    pterm = (w + 2*j) * exp(gamma * (w + 2*j) * (w + 2*j));
    nterm = (w - 2*j) * exp(gamma * (w - 2*j) * (w - 2*j));
    sum += pterm + nterm;
  }
  while (fabs(nterm) > eps) { // at this point, the negative term is greater
    j++;
    pterm = (w + 2*j) * exp(gamma * (w + 2*j) * (w + 2*j));
    nterm = (w - 2*j) * exp(gamma * (w - 2*j) * (w - 2*j));
    sum += pterm + nterm;
  }
  return sum;
}


// term < eps BGK2017 style truncated sum, with minimum terms
double small_sum_eps_17(double t, double a, double w, double mult, double eps)
{
  eps /= mult;
  double gamma = -a*a/(2*t);
  double sum = w * exp(gamma * w * w); // start at j=0 term
  double minterms = sqrt(t)/(2*a) - w/2; // minimum number of terms
  double rj = 1 - w;
  double oterm = rj * exp(gamma*rj*rj);
  double eterm = 0;
  int j = 0;
  while (j < minterms) { // capture increasing terms
    j++;
    rj = j + 1 - w; // j is odd
    oterm = rj * exp(gamma*rj*rj);
    sum -= oterm;
    j++;
    rj = j + w; // j is even
    eterm = rj * exp(gamma*rj*rj);
    sum += eterm;
  }
  while (fabs(oterm) > eps) { // at this point, the odd (negative) term is greater
    j++;
    rj = j + 1 - w; // j is odd
    oterm = rj * exp(gamma*rj*rj);
    sum -= oterm;
    j++;
    rj = j + w; // j is even
    eterm = rj * exp(gamma*rj*rj);
    sum += eterm;
  }
  return sum;
}



//////////                                                           //////////
///////////////////////////////// Large Time //////////////////////////////////
//////////                                                           //////////


// Navarro2009 style truncated sum
double large_sum_Nav(double t, double a, double w, int kl)
{
  double gamma = -M_PI*M_PI*t/(2*a*a);
  double sum = 0.0;
  for (int j = 1; j <= kl; j++) {
    sum += j * sin(j*w*M_PI) * exp(gamma*j*j);
  }
  return sum;
}
