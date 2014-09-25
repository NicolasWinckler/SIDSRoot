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
   TGGroupFrame(p, name), fCombo(NULL), fEntry(NULL)
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
void GroupBox::DoSomething(Int_t id)
{
    fBoxID=id;
    switch(id)
    {
        
        case HistoProperty::CombBoxOpt::kZoomNormal :
        {
            std::cout<<"Normal mode"<<std::endl;
            	//SetFormat
            fEntry->SetFormat(TGNumberFormat::kNESReal);
            break;
        }
        
        case HistoProperty::CombBoxOpt::kZoomParticleNr :
        {
            std::cout<<"Particle mode"<<std::endl;
            fEntry->SetFormat(TGNumberFormat::kNESInteger);
            break;
        }
        
        case HistoProperty::CombBoxOpt::kZoomFreqWindow :
        {
            std::cout<<"zoom freq mode"<<std::endl;
            fEntry->SetFormat(TGNumberFormat::kNESInteger);
            break;
        }
        
        case HistoProperty::CombBoxOpt::kUnzoom :
        {
            std::cout<<"unzoom mode"<<std::endl;
            break;
        }
        
        case HistoProperty::CombBoxOpt::kParentWindow :
        {
            std::cout<<"unzoom mode no val"<<std::endl;
            break;
        }
        
        case HistoProperty::CombBoxOpt::kDaughterWindow :
        {
            std::cout<<"unzoom mode no val"<<std::endl;
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
            //fEntry->SetFormat(TGNumberFormat::kNESReal);
            std::cout<<"Normal mode val="<<fDoubleData <<std::endl;
            break;
        }
        
        case HistoProperty::CombBoxOpt::kZoomParticleNr :
        {
            std::cout<<"Particle mode val="<< fIntData<<std::endl;
            fIntData=atoi(val);
            //fEntry->SetFormat(TGNumberFormat::kNESInteger);
            break;
        }
        
        case HistoProperty::CombBoxOpt::kZoomFreqWindow :
        {
            std::cout<<"zoom freq mode val="<< fIntData<<std::endl;
            fIntData=atoi(val);
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
            break;
        }
        
        case HistoProperty::CombBoxOpt::kDaughterWindow :
        {
            std::cout<<"unzoom mode no val"<<std::endl;
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
    fEcho(NULL),
    fAlign(NULL),
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
   fEcho = new GroupBox(horframe, "Zoom", "Z axis:");
   horframe->AddFrame(fEcho, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
   combo = fEcho->GetCombo();
   entry = fEcho->GetNumEntry();
   // add entries
   //combo->AddEntry("Normal", TGNumberFormat::kNESReal);
   //combo->AddEntry("Particule number level", TGNumberFormat::kNESInteger);
   //combo->AddEntry("Unzoom", TGTextEntry::kNoEcho);
   
   combo->AddEntry("Normal", CombBoxOpt::kZoomNormal);
   combo->AddEntry("Particule number level", CombBoxOpt::kZoomParticleNr);
   combo->AddEntry("Unzoom", CombBoxOpt::kUnzoom);
   
   combo->Connect("Selected(Int_t)", "GroupBox", fEcho, "DoSomething(Int_t)");
   fEcho->Connect("GetEntryData(char*,Int_t,Int_t,Double_t)", "HistoProperty", this, "GetData(char*,Int_t,Int_t,Double_t)");

   combo->Select(CombBoxOpt::kZoomNormal);

   fAlign = new GroupBox(horframe, "Projection", "Type:");
   horframe->AddFrame(fAlign, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
   combo = fAlign->GetCombo();
   entry = fAlign->GetNumEntry();
   AddFrame(horframe,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
   

   // add entries
   combo->AddEntry("Parent Window (bin)", TGNumberFormat::kNESInteger);
   combo->AddEntry("Daughter Window (bin)", TGNumberFormat::kNESInteger);
   combo->AddEntry("Zoom Window", TGNumberFormat::kNESInteger);
   combo->Connect("Selected(Int_t)", "TGNumberEntry", entry, "ValueChanged(Long_t)");
   combo->Select(TGNumberFormat::kNESInteger);
   
   
   
/*
   fAccess = new GroupBox(this, "Access", "Read-only:");
   AddFrame(fAccess, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
   combo = fAccess->GetCombo();
   entry = fAccess->GetEntry();
   // add entries
   combo->AddEntry("False", 1);
   combo->AddEntry("True", 0);
   combo->Connect("Selected(Int_t)", "TGTextEntry", entry, "SetEnabled(Int_t)");
   combo->Select(1);

   fBorder = new GroupBox(this, "Border", "Drawn:");
   AddFrame(fBorder, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
   combo = fBorder->GetCombo();
   entry = fBorder->GetEntry();
   // add entries
   combo->AddEntry("False", 0);
   combo->AddEntry("True", 1);
   combo->Connect("Selected(Int_t)", "TGTextEntry", entry, "SetFrameDrawn(Int_t)");
   combo->Select(1);
*/
   // terminate ROOT session when window is closed
   //Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
   //DontCallClose();

   //MapSubwindows();
   //Resize();

   // set minimum width, height
   //SetWMSizeHints(GetDefaultWidth(), GetDefaultHeight(),1000, 1000, 0, 0);
   //SetWindowName("Text Entries");
   //MapRaised();
}

void HistoProperty::GetData(char* val,Int_t BoxID, Int_t dataint, Double_t datadouble)
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
            std::cout<<"unzoom mode no val"<<std::endl;
            break;
        }
        
        case CombBoxOpt::kDaughterWindow :
        {
            std::cout<<"unzoom mode no val"<<std::endl;
            break;
        }
        
    }
}


ClassImp(HistoProperty)
