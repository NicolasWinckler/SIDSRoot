/* 
 * File:   SidsFitButtons.h
 * Author: winckler
 *
 * Created on October 6, 2014, 7:44 PM
 */

#ifndef SIDSFITBUTTONS_H
#define	SIDSFITBUTTONS_H

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
#include <vector>
#include <string>

class SidsFitButtons : public TGGroupFrame
{
public:
    SidsFitButtons(const TGWindow *p, int ButtonNumber=1,const char *name="", std::string label1="",std::string label2="", std::string label3="");
    virtual ~SidsFitButtons();
    void AddButton();
    void GetEntry1(char* val);
    void GetEntry2(char* val);
    void GetEntry3(char* val);
    
    double GetVal1(){return fVal1;}
    double GetVal2(){return fVal2;}
    double GetVal3(){return fVal3;}
    
    void SetEntry1(Double_t number){fEntry1->SetNumber(number);}
    void SetEntry2(Double_t number){fEntry2->SetNumber(number);}
    void SetEntry3(Double_t number){fEntry3->SetNumber(number);}
    
protected:
    int fEntryNumber;
    int fButtonCounter;
    double fVal1;
    double fVal2;
    double fVal3;
    TGHorizontalFrame* fHorFrame;
    TGNumberEntry* fEntry1;
    TGNumberEntry* fEntry2;
    TGNumberEntry* fEntry3;
    std::vector<TGNumberEntry*> fEntry;
    std::vector<double> fEntryVal;
    ClassDef(SidsFitButtons, 0);
};







#endif	/* SIDSFITBUTTONS_H */

