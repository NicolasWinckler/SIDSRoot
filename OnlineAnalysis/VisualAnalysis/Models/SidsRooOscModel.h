/* 
 * File:   SidsRooOscModel.h
 * Author: winckler
 *
 * Created on October 6, 2014, 3:19 PM
 */

#ifndef SIDSROOOSCMODEL_H
#define	SIDSROOOSCMODEL_H

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
class RooRealVar;
class SidsRooOscModel : public RooAbsPdf
{
public:
SidsRooOscModel() {} ;
SidsRooOscModel(const char *name,const char *title,RooAbsReal& _x,
RooAbsReal& _lambda, RooAbsReal& _amp,
RooAbsReal& _omega, RooAbsReal& _phi);
SidsRooOscModel(const SidsRooOscModel& other, const char* name);
virtual TObject* clone(const char* newname) const { return new SidsRooOscModel(*this,newname); }
inline virtual ~SidsRooOscModel() { };
Int_t getAnalyticalIntegral(RooArgSet& allVars,RooArgSet& analVars,const char* rangeName=0) const;
Double_t analyticalIntegral(Int_t code,const char* rangeName = 0) const;
//Double_t analyticalIntegral(Int_t code) const;
protected:
//Double_t evaluate(const RooDataSet* dset) const;
Double_t evaluate() const ;
RooRealProxy x;
RooRealProxy lambda;
RooRealProxy amp;
RooRealProxy omega;
RooRealProxy phi;
private:
ClassDef(SidsRooOscModel,0);
};

#endif	/* SIDSROOOSCMODEL_H */

