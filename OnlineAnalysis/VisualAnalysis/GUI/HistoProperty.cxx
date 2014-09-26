/* 
 * File:   HistoProperty.cxx
 * Author: winckler
 * 
 * Created on September 23, 2014, 4:45 PM
 */

#include "HistoProperty.h"


/////////////////////////////////////////////////////////////////////////////////////////////
//////////// auxilary class /////////////////////////////////////////////////////////////////
//______________________________________________________________________________
GroupBox::GroupBox(const TGWindow *p, const char *name, const char *title) :
   TGGroupFrame(p, name), fCombo(NULL), fEntry(NULL),
    fBoxID(0),
    fIntData(0),
    fDoubleData(0.),
    fIntensity(0.),
    fParticleNr(0),
    fFreqWindow(0),
    fParentWindow(0),
    fDaughterWindow(0)
{
   // Group frame containing combobox and  entry.

    SetTitlePos(TGGroupFrame::kCenter);
   //TGGroupFrame *groupfrm = new TGGroupFrame(this,title);
   TGHorizontalFrame *groupfrm = new TGHorizontalFrame(this, 200, 50);
   //groupfrm->SetTitlePos(TGGroupFrame::kCenter);
   AddFrame(groupfrm, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
   TGLabel *label = new TGLabel(groupfrm, title);
   groupfrm->AddFrame(label, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
   
   
   fCombo = new TGComboBox(groupfrm);
   groupfrm->AddFrame(fCombo, new TGLayoutHints(kLHintsLeft | kLHintsExpandY,
                                            5, 0, 5, 5));
   fCombo->Resize(100, 20);

   fEntry = new TGNumberEntry(groupfrm);
   //AddFrame(fEntry, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
   groupfrm->AddFrame(fEntry, new TGLayoutHints(kLHintsLeft | kLHintsExpandY,5, 0, 5, 5));
   GetTxtEntry()->Connect("TextChanged(char*)", "GroupBox", this, "GetEntryData(char*,Int_t,Int_t,Double_t)");

           
}

void GroupBox::InitComboBoxSetting(Double_t Intensity, Int_t ParticleNr, Int_t FreqWindow, Int_t ParentWindow, Int_t DaughterWindow)
{
    //EmitVA("InitComboBoxSetting(Double_t,Int_t,Int_t,Int_t,Int_t)",5, Intensity,ParticleNr,FreqWindow,ParentWindow,DaughterWindow);
    fIntensity=Intensity;
    fParticleNr=ParticleNr;
    fFreqWindow=FreqWindow;
    fParentWindow=ParentWindow;
    fDaughterWindow=DaughterWindow;
    
    for (int BoxID = HistoProperty::CombBoxOpt::kZoomNormal; 
            BoxID != HistoProperty::CombBoxOpt::kDaughterWindow; 
            BoxID++ )
    {
       HistoProperty::CombBoxOpt id = static_cast<HistoProperty::CombBoxOpt>(BoxID);
       ChangeMode(id);
    }
    ChangeMode(fBoxID);
    
}

void GroupBox::ChangeMode(Int_t id)
{
    fBoxID=id;
    switch(id)
    {
        
        case HistoProperty::CombBoxOpt::kZoomNormal :
        {
            std::cout<<"Normal mode"<< fIntensity<< std::endl;
            	//SetFormat
            fEntry->SetFormat(TGNumberFormat::kNESReal);
            SetNumEntry<Double_t>(fIntensity);
            break;
        }
        
        case HistoProperty::CombBoxOpt::kZoomParticleNr :
        {
            std::cout<<"Particle mode"<<std::endl;
            fEntry->SetFormat(TGNumberFormat::kNESInteger);
            SetNumEntry<Int_t>(fParticleNr);
            break;
        }
        
        case HistoProperty::CombBoxOpt::kZoomFreqWindow :
        {
            std::cout<<"zoom freq mode"<<std::endl;
            fEntry->SetFormat(TGNumberFormat::kNESInteger);
            SetNumEntry<Int_t>(fFreqWindow);
            break;
        }
        
        case HistoProperty::CombBoxOpt::kUnzoom :
        {
            std::cout<<"unzoom mode"<<std::endl;
            break;
        }
        
        case HistoProperty::CombBoxOpt::kParentWindow :
        {
            std::cout<<"Parent Window mode val"<<std::endl;
            fEntry->SetFormat(TGNumberFormat::kNESInteger);
            SetNumEntry<Int_t>(fParentWindow);
            break;
        }
        
        case HistoProperty::CombBoxOpt::kDaughterWindow :
        {
            std::cout<<"Daughter Window mode val"<<std::endl;
            fEntry->SetFormat(TGNumberFormat::kNESInteger);
            SetNumEntry<Int_t>(fDaughterWindow);
            break;
        }
        
        
    }
    
}

void GroupBox::GetEntryData(char* val,Int_t BoxID, Int_t dataint, Double_t datadouble)
{
    
    switch(fBoxID)
    {
        
        case HistoProperty::CombBoxOpt::kZoomNormal :
        {
            
            fDoubleData=atof(val);
            fIntensity=fDoubleData;
            //fEntry->SetFormat(TGNumberFormat::kNESReal);
            std::cout<<"Normal mode val="<<fDoubleData <<std::endl;
            break;
        }
        
        case HistoProperty::CombBoxOpt::kZoomParticleNr :
        {
            std::cout<<"Particle mode val="<< fIntData<<std::endl;
            fIntData=atoi(val);
            fParticleNr=fIntData;
            //fEntry->SetFormat(TGNumberFormat::kNESInteger);
            break;
        }
        
        case HistoProperty::CombBoxOpt::kZoomFreqWindow :
        {
            std::cout<<"zoom freq mode val="<< fIntData<<std::endl;
            fIntData=atoi(val);
            fFreqWindow=fIntData;
            //fEntry->SetFormat(TGNumberFormat::kNESInteger);
            //SetNumEntry<Int_t>(fIntData);
            break;
        }
        
        case HistoProperty::CombBoxOpt::kUnzoom :
        {
            std::cout<<"unzoom mode no val"<<std::endl;
            
            break;
        }
        
        case HistoProperty::CombBoxOpt::kParentWindow :
        {
            std::cout<<"unzoom mode no val"<<std::endl;
            fIntData=atoi(val);
            fParentWindow=fIntData;
            break;
        }
        
        case HistoProperty::CombBoxOpt::kDaughterWindow :
        {
            std::cout<<"unzoom mode no val"<<std::endl;
            fIntData=atoi(val);
            fDaughterWindow=fIntData;
            break;
        }
        
    }
    
    //Emit("GetEntryData(char*,Int_t,Int_t,Double_t)", val,fBoxID,fIntData,fDoubleData);
    EmitVA("GetEntryData(char*,Int_t,Int_t,Double_t)",4, val,fBoxID,fIntData,fDoubleData);
    
}


ClassImp(GroupBox)

/////////////////////////////////////////////////////////////////////////////////////////////
//////////// Main class /////////////////////////////////////////////////////////////////////

HistoProperty::HistoProperty(const TGWindow *Mainframe, const char* name) : 
    TGGroupFrame(Mainframe, name),
    //fMain(NULL),
    fZoomBox(NULL),
    fProjBox(NULL),
    fAccess(NULL),
    fBorder(NULL),
    fIntensity(0.),
    fParticleNr(0),
    fFreqWindow(),
    fParentWindow(0),
    fDaughterWindow(0)
{
    SetTitlePos(TGGroupFrame::kCenter);
    SetUpGui();
}

HistoProperty::~HistoProperty() 
{
    //delete fMain;
}


void HistoProperty::SetUpGui()
{
        // Main test window.

   TGComboBox  *combo;
   TGNumberEntry *entry; 

   

   // recusively delete all subframes on exit
   //SetCleanup(kDeepCleanup);
   TGHorizontalFrame* horframe = new TGHorizontalFrame(this, 200, 50);
   fZoomBox = new GroupBox(horframe, "Zoom", "Z axis:");
   horframe->AddFrame(fZoomBox, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
   combo = fZoomBox->GetCombo();
   entry = fZoomBox->GetNumEntry();
   
   
   combo->AddEntry("Normal", CombBoxOpt::kZoomNormal);
   combo->AddEntry("Particule number level", CombBoxOpt::kZoomParticleNr);
   combo->AddEntry("Unzoom", CombBoxOpt::kUnzoom);
   
   combo->Connect("Selected(Int_t)", "GroupBox", fZoomBox, "ChangeMode(Int_t)");
   fZoomBox->Connect("GetEntryData(char*,Int_t,Int_t,Double_t)", "HistoProperty", this, "GetBoxData(char*,Int_t,Int_t,Double_t)");
   combo->Select(CombBoxOpt::kZoomNormal);

   fProjBox = new GroupBox(horframe, "Projection", "Type:");
   horframe->AddFrame(fProjBox, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
   combo = fProjBox->GetCombo();
   entry = fProjBox->GetNumEntry();
   AddFrame(horframe,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
   

   // add entries
   combo->AddEntry("Parent Window (bin)", CombBoxOpt::kParentWindow);
   combo->AddEntry("Daughter Window (bin)", CombBoxOpt::kDaughterWindow);
   combo->AddEntry("Zoom Window", CombBoxOpt::kZoomFreqWindow);
   combo->Connect("Selected(Int_t)", "GroupBox", fProjBox, "ChangeMode(Int_t)");
   fProjBox->Connect("GetEntryData(char*,Int_t,Int_t,Double_t)", "HistoProperty", this, "GetBoxData(char*,Int_t,Int_t,Double_t)");
   combo->Select(CombBoxOpt::kZoomFreqWindow);
   
   TGTextButton *redraw = new TGTextButton(this,"&Redraw");
   redraw->Connect("Clicked()","HistoProperty",this,"SendBoxData(Double_t,Int_t,Int_t,Int_t,Int_t)");
   AddFrame(redraw, new TGLayoutHints(kLHintsExpandX,2,2,2,5));
   
   
}

void HistoProperty::GetBoxData(char* val,Int_t BoxID, Int_t dataint, Double_t datadouble)
{
    switch(BoxID)
    {
        
        case CombBoxOpt::kZoomNormal :
        {
            fIntensity=datadouble;
            //fEntry->SetFormat(TGNumberFormat::kNESReal);
            std::cout<<"Normal mode output val="<<datadouble <<std::endl;
            break;
        }
        
        case CombBoxOpt::kZoomParticleNr :
        {
            std::cout<<"Particle mode output val="<< dataint<<std::endl;
            //fEntry->SetFormat(TGNumberFormat::kNESInteger);
            fParticleNr=dataint;
            break;
        }
        
        case CombBoxOpt::kZoomFreqWindow :
        {
            std::cout<<"zoom freq mode output val="<< dataint<<std::endl;
            fFreqWindow=dataint;
            //fEntry->SetFormat(TGNumberFormat::kNESInteger);
            //SetNumEntry<Int_t>(fIntData);
            break;
        }
        
        case CombBoxOpt::kUnzoom :
        {
            std::cout<<"unzoom mode no val"<<std::endl;
            break;
        }
        
        case CombBoxOpt::kParentWindow :
        {
            std::cout<<"Parent Window mode output val="<< dataint<<std::endl;
            fParentWindow=dataint;
            break;
        }
        
        case CombBoxOpt::kDaughterWindow :
        {
            std::cout<<"Daughter Window mode output val="<< dataint<<std::endl;
            fDaughterWindow=dataint;
            break;
        }
        
    }
}


void HistoProperty::SendBoxData(Double_t Intensity, Int_t ParticleNr, Int_t FreqWindow, Int_t ParentWindow, Int_t DaughterWindow)
{
    
    EmitVA("SendBoxData(Double_t,Int_t,Int_t,Int_t,Int_t)",5, fIntensity,fParticleNr,fFreqWindow,fParentWindow,fDaughterWindow);
}

ClassImp(HistoProperty)
