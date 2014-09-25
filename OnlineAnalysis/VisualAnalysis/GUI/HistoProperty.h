/* 
 * File:   HistoProperty.h
 * Author: winckler
 *
 * Created on September 23, 2014, 4:45 PM
 */

#ifndef HISTOPROPERTY_H
#define	HISTOPROPERTY_H

#include <stdlib.h>
#include <TGTextEntry.h>
#include <TGButtonGroup.h>
#include <TGLabel.h>
#include <TGComboBox.h>
#include <TApplication.h>
#include <TGNumberEntry.h>
#include <typeinfo>

#include <string>
#include <iostream>
/////////////////////////////////////////////////////////////////////////////////////////////
//////////// auxilary class /////////////////////////////////////////////////////////////////

class GroupBox : public TGGroupFrame 
{


public:
   GroupBox(const TGWindow *p, const char *name, const char *title);
   TGTextEntry *GetTxtEntry() const { return fEntry->GetNumberEntry(); }
   TGNumberEntry *GetNumEntry() const { return fEntry; }
   TGComboBox  *GetCombo() const { return fCombo; }
   void DoSomething(Int_t id);
   void GetEntryData(char* val,Int_t BoxID=-1, Int_t dataint=0, Double_t datadouble=0.);
   template<class T> void SetNumEntry(T number)
   {
       if(typeid(T).name()==typeid(int).name())
            fEntry->SetIntNumber(number);
       
       if(typeid(T).name()==typeid(double).name())
            fEntry->SetNumber(number);
       
       if(typeid(T).name()==typeid(float).name())
            fEntry->SetNumber((double)number);
       
   }
   
private:
   TGComboBox  *fCombo; // combo box
   TGNumberEntry *fEntry; // text entry  
   Int_t fBoxID;
   Int_t fIntData;
   Double_t fDoubleData;
   ClassDef(GroupBox, 0);
};

/////////////////////////////////////////////////////////////////////////////////////////////
//////////// Main class /////////////////////////////////////////////////////////////////////

class HistoProperty : public TGGroupFrame
{
public:
    enum CombBoxOpt
    {
        kZoomNormal,
        kZoomParticleNr,
        kZoomFreqWindow,
        kUnzoom,
        kParentWindow,
        kDaughterWindow
    };
    
    

    HistoProperty(const TGWindow *Mainframe, const char* name);
    virtual ~HistoProperty();
    void SetUpGui();
    void GetData(char* val,Int_t BoxID=-1, Int_t dataint=0, Double_t datadouble=0.);
   
   
protected:

    //TGMainFrame *fMain;     // main frame
    GroupBox    *fEcho;     // echo mode (echo, password, no echo)
    GroupBox    *fAlign;    // alignment (left, right, center)
    GroupBox    *fAccess;   // read-only mode
    GroupBox    *fBorder;   // border mode
    
    Double_t fIntensity;
    Int_t fParticleNr;
    Int_t fFreqWindow;
    Int_t fParentWindow;
    Int_t fDaughterWindow;
    
    ClassDef(HistoProperty, 0);
};

#endif	/* HISTOPROPERTY_H */

