#include "RooFit.h"
#include "Riostream.h"
#include "Riostream.h"
#include <math.h>

#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooRandom.h"
#include "RooMath.h"

#include "SidsRooOscModel.h"

using namespace std;


ClassImp(SidsRooOscModel)
// Constructor
SidsRooOscModel::SidsRooOscModel(const char *name,const char *title,RooAbsReal& _x,
RooAbsReal& _lambda, RooAbsReal& _amp, RooAbsReal& _omega, RooAbsReal& _phi) :
RooAbsPdf(name,title),
x("x","Dependent",this,_x),
lambda("lambda","Lambda",this,_lambda),
amp("amp","Amp",this,_amp),
omega("omega","Omega",this,_omega),
phi("phi","Phi",this,_phi)
{
}
// Copy constructor
SidsRooOscModel::SidsRooOscModel(const SidsRooOscModel& other, const char* name) : RooAbsPdf(other,name),
x("x",this,other.x),lambda("lambda",this,other.lambda),
amp("amp",this,other.amp),omega("omega",this,other.omega),
phi("phi",this,other.phi)
{
}
// Implementation of value calculation
//Double_t SidsRooOscModel::evaluate(const RooDataSet* dset) const
Double_t SidsRooOscModel::evaluate() const
{
Double_t exp=TMath::Exp(-lambda*x);
Double_t cos=1+amp*TMath::Cos(omega*x+phi);
return cos*exp;
}
//////////////
Int_t SidsRooOscModel::getAnalyticalIntegral(RooArgSet& allVars,RooArgSet& analVars,const char* ) const
{
//if (matchArgs(allVars, analVars, x,amp,omega,phi)) return 4;
//if (matchArgs(allVars, analVars, x,amp,omega)) return 3;
//if (matchArgs(allVars, analVars, x,amp)) return 2;
if (matchArgs(allVars, analVars, x)) return 1;
return 0;
}
Double_t SidsRooOscModel::analyticalIntegral(Int_t code, const char* rangeName) const
{
//assert(code==1 || code==2) ;
Double_t valueOfIntegral;
if(code == 1)
{
//calculation of integral for variables from code 1
Double_t part0=TMath::Exp(-lambda*x.min())-TMath::Exp(-lambda*x.max());
Double_t factor=lambda*amp/(omega*omega+lambda*lambda);
Double_t part1a=(omega*TMath::Sin(omega*x.max()+phi)-lambda*TMath::Cos(omega*x.max()+phi))*TMath::Exp(-lambda*x.max());
Double_t part1b=(omega*TMath::Sin(omega*x.min()+phi)-lambda*TMath::Cos(omega*x.min()+phi))*TMath::Exp(-lambda*x.min());
valueOfIntegral=(part0+factor*(part1a-part1b))/lambda;
}
return valueOfIntegral;
}