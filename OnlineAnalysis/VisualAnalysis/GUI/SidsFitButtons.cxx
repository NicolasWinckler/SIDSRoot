/* 
 * File:   SidsFitButtons.cxx
 * Author: winckler
 * 
 * Created on October 6, 2014, 7:44 PM
 */

#include "SidsFitButtons.h"

SidsFitButtons::SidsFitButtons(const TGWindow *p, int ButtonNumber,const char *name, std::string label1,std::string label2, std::string label3) :  TGGroupFrame(p,name),
fEntryNumber(ButtonNumber), fButtonCounter(0), fVal1(0.), fVal2(0.), fVal3(0.), 
        fHorFrame(NULL),fEntry1(NULL), fEntry2(NULL), fEntry3(NULL)
{
    SetTitlePos(TGGroupFrame::kCenter);
    TGLabel* flabel1;
    TGLabel* flabel2;
    TGLabel* flabel3;
    fHorFrame = new TGHorizontalFrame(this, 200, 50);
    
    flabel1 = new TGLabel(fHorFrame, label1.c_str());
    fHorFrame->AddFrame(flabel1, new TGLayoutHints( kLHintsExpandY,2,2,2,2));
    
    fEntry1 = new TGNumberEntry(fHorFrame,0,6,1,TGNumberFormat::kNESReal);
    fHorFrame->AddFrame(fEntry1, new TGLayoutHints(kLHintsExpandX,6,2,2,2));
    fEntry1->GetNumberEntry()->Connect("TextChanged(char*)", "SidsFitButtons", this, "GetEntry1(char*)");
    
    if(ButtonNumber>1)
    {
        flabel2 = new TGLabel(fHorFrame, label2.c_str());
        fHorFrame->AddFrame(flabel2, new TGLayoutHints( kLHintsExpandY,20,2,2,2));

        fEntry2 = new TGNumberEntry(fHorFrame,0,6,2,TGNumberFormat::kNESReal);
        fEntry2->GetNumberEntry()->Connect("TextChanged(char*)", "SidsFitButtons", this, "GetEntry2(char*)");
        fHorFrame->AddFrame(fEntry2, new TGLayoutHints(kLHintsExpandX,2,2,5,5));
    }
    
    if(ButtonNumber>2)
    {
        flabel3 = new TGLabel(fHorFrame, label3.c_str());
        fHorFrame->AddFrame(flabel3, new TGLayoutHints( kLHintsExpandY,20,2,2,2));

        fEntry3 = new TGNumberEntry(fHorFrame,0,6,3,TGNumberFormat::kNESReal);
        fHorFrame->AddFrame(fEntry3, new TGLayoutHints(kLHintsExpandX,6,2,2,2));

        fEntry3->GetNumberEntry()->Connect("TextChanged(char*)", "SidsFitButtons", this, "GetEntry3(char*)");
    }
    AddFrame(fHorFrame, new TGLayoutHints(kLHintsExpandX, 0, 0, 2, 2));
    /*
    if(fEntryNumber<1)
        fEntryNumber=1;
    for(int i=0;i<fEntryNumber;i++)
    {
        AddButton();
    }
    */
}


SidsFitButtons::~SidsFitButtons() {
}

void SidsFitButtons::AddButton()
{
    fEntry.push_back(new TGNumberEntry(fHorFrame,0,6,fButtonCounter,TGNumberFormat::kNESReal));
    fHorFrame->AddFrame(fEntry[fButtonCounter], new TGLayoutHints(kLHintsExpandX,2,2,5,5));
    fEntry[fButtonCounter]->GetNumberEntry()->Connect("TextChanged(char*)", "SidsFitButtons", this, "GetEntryData(char*, Int_t)");
    fEntryVal.push_back(0.);
    fButtonCounter++;
}
void SidsFitButtons::GetEntry1(char* val)
{
    fVal1=atof(val);
}

void SidsFitButtons::GetEntry2(char* val)
{
    fVal2=atof(val);
}

void SidsFitButtons::GetEntry3(char* val)
{
    fVal3=atof(val);
}


ClassImp(SidsFitButtons)
