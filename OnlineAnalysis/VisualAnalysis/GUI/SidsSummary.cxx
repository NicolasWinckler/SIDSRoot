/* 
 * File:   SidsSummary.cxx
 * Author: winckler
 * 
 * Created on October 4, 2014, 3:09 AM
 */


#include "SidsSummary.h"





const char gHelpDnd[] = "\
                     Drag & Drop (DnD)\n\
Drag and Drop support is implemented on Linux via Xdnd, the\n\
drag and drop protocol for X window system, and on Windows\n\
via the Clipboard.\n\
Users can selects something in ROOT with a mouse press, drags\n\
it (moves the mouse while keeping the mouse button pressed) and\n\
releases the mouse button somewhere else. When the button is\n\
released the selected data is \"dropped\" at that location. This\n\
way, a histogram from an opened ROOT file in the browser can be\n\
dragged to any TCanvas. A script file from the browser can be\n\
dropped to a TGTextView or TGTextEdit widget in TGTextEditor.\n\
On Linux, it is possible to drag objects between ROOT and an\n\
external application. For example to drag a macro file from the\n\
ROOT browser to the Kate editor. On Windows, drag and drop works\n\
only within a single ROOT application, but it is possible to drag\n\
from the Windows Explorer to ROOT (e.g. a picture file to a canvas\n\
or a text file to a text editor).\n\
";

const char gReadyMessage[] = "Ready. You can drag list tree items to any \
pad in the canvas, or to the \"Base\" folder of the list tree itself...";


const char *DND_Types[] = {
   "ROOT files",    "*.root",
   "ROOT macros",   "*.C",
   "All files",     "*",
    0,               0
};

static Atom_t gRootObj  = kNone;





//______________________________________________________________________________
SidsSummary::SidsSummary(const TGWindow *p, int w, int h,MQconfig SamplerConfig, std::string Filename) :
   TGMainFrame(p, w, h), fEc(NULL), fEc2(NULL), fButtonExit(NULL), 
        fMenuBar(NULL), fMenuFile(NULL),
        fMenuHelp(NULL), fCanvas1(NULL), fCanvas2(NULL), fListTree(NULL),
        fBaseLTI(NULL), fStatus(NULL), fStatusBar(NULL), 
        fGraph(NULL), fHisto_px(NULL), fECDecayTimes(NULL), fDaughterTrace(NULL),
        fNEC(NULL), fPfreq(NULL), fECFreq(NULL), fVslider1(NULL), fControlFrame(NULL),
        fFileName(Filename.c_str()), fFileInfo(), 
        fDecayData() ,fDetectorID("RSA51") , fParentFreq(0.),
        fParConfig(SamplerConfig), fInputFile(NULL), fSliderScale(1.),
        fDecayCounter(0), fHisto1DCounter(0), fHisto2DCounter(0), fHeaderCounter(0),
        fReadyToSend(false), fSampler(false)

{
   // Constructor.
    
    InitParameters();
    SetupGUI();
   
}

//______________________________________________________________________________
SidsSummary::~SidsSummary()
{
   // Destructor. Doesnt't do much here.
    //TVirtualPadEditor::Terminate();
    //SafeDelete(fEditor);
    for(unsigned int i(0);i<f1DHisto.size() ; i++)
    {
        if(f1DHisto[i])
        {
             delete f1DHisto[i];
             f1DHisto[i]=NULL;
        }
    }
    f1DHisto.clear();
   
   for(unsigned int i(0);i<f2DHisto.size() ; i++)
    {
        if(f2DHisto[i])
        {
             delete f2DHisto[i];
             f2DHisto[i]=NULL;
        }
    }
    f2DHisto.clear();
   
   for(unsigned int i(0);i<fHeaders.size() ; i++)
    {
        if(fHeaders[i])
        {
             delete fHeaders[i];
             fHeaders[i]=NULL;
        }
    }
    fHeaders.clear();
    
    if(fHisto_px)
    {
        delete fHisto_px;
        fHisto_px=NULL;
    }
    
    if(fECDecayTimes)
    {
        delete fECDecayTimes;
        fECDecayTimes=NULL;
    }
    
    if(fDaughterTrace)
    {
        delete fDaughterTrace;
        fDaughterTrace=NULL;
    }
    
    if(fNEC)
    {
        delete fNEC;
        fNEC=NULL;
    }
    
    if(fPfreq)
    {
        delete fPfreq;
        fPfreq=NULL;
    }
    
    TSeqCollection* fileCollection=gROOT->GetListOfFiles();
    TIter nextF(fileCollection);
    TFile *file = 0;
    while ((file = (TFile *) nextF())) 
    {
        if(file)
        {
            if(file->IsOpen())
                file->Close();
            delete file;
        }
    }
    
    if(fInputFile)
    {
        if(fInputFile->IsOpen())
            fInputFile->Close();
        delete fInputFile;
        fInputFile=NULL;
    }
   
}




/// ////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////// Setup the GUI /////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////
//______________________________________________________________________________
void SidsSummary::SetupGUI()
{
    SetCleanup(kDeepCleanup);
   ////////////////////////////////////////////////////// 0th layer (menu)
   fMenuBar = new TGMenuBar(this, 35, 50, kHorizontalFrame);

   fMenuFile = new TGPopupMenu(gClient->GetRoot());
   fMenuFile->AddEntry(" &Open...\tCtrl+O", FILE_OPEN, 0, gClient->GetPicture("bld_open.png"));
   fMenuFile->AddEntry(" &Browse...\tCtrl+B", FILE_BROWSE);
   fMenuFile->AddEntry(" &New Canvas\tCtrl+N", FILE_NEWCANVAS);
   fMenuFile->AddEntry(" &Close Window\tCtrl+W", FILE_CLOSEWIN);
   fMenuFile->AddSeparator();
   fMenuFile->AddEntry(" E&xit\tCtrl+Q", FILE_EXIT, 0, gClient->GetPicture("bld_exit.png"));
   fMenuFile->Connect("Activated(Int_t)", "SidsSummary", this,"HandleMenu(Int_t)");

   fMenuHelp = new TGPopupMenu(gClient->GetRoot());
   fMenuHelp->AddEntry(" &About...", HELP_ABOUT, 0, gClient->GetPicture("about.xpm"));
   fMenuHelp->Connect("Activated(Int_t)", "SidsSummary", this, "HandleMenu(Int_t)");

   fMenuBar->AddPopup("&File", fMenuFile, new TGLayoutHints(kLHintsTop|kLHintsLeft, 0, 4, 0, 0));
   fMenuBar->AddPopup("&Help", fMenuHelp, new TGLayoutHints(kLHintsTop|kLHintsRight));

   AddFrame(fMenuBar, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 5));

   ////////////////////////////////////////////////////// 1st layer
   TGHorizontalFrame *hfrm = new TGHorizontalFrame(this, 10, 10);
   TGCanvas *canvas = new TGCanvas(hfrm, 150, 100);
   fListTree = new TGListTree(canvas, kHorizontalFrame);
   fListTree->Associate(this);
   fEc = new TRootEmbeddedCanvas("SummaryCanvas", hfrm, 400, 400);
   hfrm->AddFrame(canvas, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5, 5));
   hfrm->AddFrame(fEc, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   //AddFrame(hfrm, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fEc->SetDNDTarget(kTRUE);
   fCanvas1 = fEc->GetCanvas();
   
   fCanvas1->Divide(2, 2);
   //fCanvas1->SetBorderMode(0);
   fCanvas1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)","SidsSummary",this,
               "EventInfo(Int_t,Int_t,Int_t,TObject*)");
   
   
   gRootObj  = gVirtualX->InternAtom("application/root", kFALSE);


   TString rootsys(gSystem->UnixPathName(gSystem->Getenv("ROOTSYS")));
#ifdef G__WIN32
   // remove the drive letter (e.g. "C:/") from $ROOTSYS, if any
   if (rootsys[1] == ':' && rootsys[2] == '/')
      rootsys.Remove(0, 3);
#endif
   
   fBaseLTI = fListTree->AddItem(0,"Status");
    
    
    if(fNEC)
        AddToListTree(fNEC);
    
    if(fPfreq)
        AddToListTree(fPfreq);
   
   if(fECDecayTimes)
        AddToListTree(fECDecayTimes);
   
   if(fECFreq)
        AddToListTree(fECFreq);
    
   // open the base list tree item and allow to drop into it
   fListTree->OpenItem(fBaseLTI);
   fListTree->GetFirstItem()->SetDNDTarget(kTRUE);

   // connect the DataDropped signal to be able to handle it
   fListTree->Connect("DataDropped(TGListTreeItem*, TDNDData*)", "SidsSummary",
                      this, "DataDropped(TGListTreeItem*,TDNDData*)");

   ////////////////////////////////////////////////////// 
   ///
   
   
   fEc2 = new TRootEmbeddedCanvas ("MainCanvas",hfrm,600,400);
   fEc2->SetDNDTarget(kTRUE);
   //fEc2->GetCanvas()->EditorBar();
   TVirtualPadEditor::ShowEditor();
   fCanvas2 = fEc2->GetCanvas();
   fCanvas2->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)","SidsSummary",this,
               "EventInfo(Int_t,Int_t,Int_t,TObject*)");
   
   
   
   
   hfrm->AddFrame(fEc2, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   
   
   
   
   
   fControlFrame = new TGVerticalFrame(hfrm, 200, 400);
   
   TGHorizontalFrame *hfrmDrawNBox = new TGHorizontalFrame(fControlFrame, 2, 2);
   
   //*
   TGComboBox *combo = new TGComboBox(hfrmDrawNBox);
   combo->AddEntry("RSA30", SIDS::kRSA30);
   combo->AddEntry("RSA51", SIDS::kRSA51);
   combo->AddEntry("RSA52", SIDS::kRSA52);
   combo->Connect("Selected(Int_t)", "SidsSummary", this, "ChangeMode(Int_t)");
   //combo->Select(SIDS::kRSA51);
   hfrmDrawNBox->AddFrame(combo, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    combo->Resize(100, 20);
   // */
   fControlFrame->AddFrame(hfrmDrawNBox, new TGLayoutHints(kLHintsExpandX,2,2,2,5));
   
   
   
  
   hfrm->AddFrame(fControlFrame, new TGLayoutHints(kLHintsRight | kLHintsExpandY));
   AddFrame(hfrm, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   
   
   

   ////////////////////////////////////////////////////// 2nd layer
   //TGHorizontalFrame *hf3 = new TGHorizontalFrame(this, 10, 10);
   TGHorizontalFrame *hf3 = new TGHorizontalFrame(this, 10, 10);
   //hf3 = new TGVerticalFrame(this, 0, 0, 0);

   combo->Select(SIDS::kRSA51);

   
   TGVerticalFrame *ExitAndValidFrame = new TGVerticalFrame(hf3, 10, 10);
   
   

   fButtonExit = new TGTextButton(ExitAndValidFrame, "        &Exit...        ");
   fButtonExit->Resize(fButtonExit->GetDefaultSize());
   fButtonExit->SetToolTipText("Exit Application (ROOT)");
   fButtonExit->Connect("Clicked()" , "SidsSummary", this, "DoExit()");
   ExitAndValidFrame->AddFrame(fButtonExit, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));

   hf3->AddFrame(ExitAndValidFrame, new TGLayoutHints(kLHintsCenterY | kLHintsRight, 10, 10, 10, 10));
   AddFrame(hf3, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 5, 5, 5, 5));

    // status bar
    Int_t parts[] = {45, 15, 10, 30};
    fStatusBar = new TGStatusBar(this, 50, 10, kVerticalFrame);
    fStatusBar->SetParts(parts, 4);
    fStatusBar->Draw3DCorner(kFALSE);
    AddFrame(fStatusBar, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));
    
   //////////////////////////////////////////////////////
   
   SetWindowName("Single Ion Decay Spectroscopy Analysis");
   MapSubwindows();
   Resize(GetDefaultSize());
   Connect("CloseWindow()", "SidsSummary", this, "DoCloseWindow()");
   DontCallClose(); // to avoid double deletions.
   MapRaised();
   
}


//______________________________________________________________________________
void SidsSummary::ChangeMode(Int_t BoxID)
{
    
    switch(BoxID)
    {
        case SIDS::kRSA30 :
        {
            string DetId="RSA30";
            fParConfig.SetValue("DetectorID",DetId);
            DoDraw();
            break;
        }
        
        case SIDS::kRSA51 :
        {
            string DetId="RSA51";
            fParConfig.SetValue("DetectorID",DetId);
            DoDraw();
            break;
        }
        
        case SIDS::kRSA52 :
        {
            string DetId="RSA52";
            fParConfig.SetValue("DetectorID",DetId);
            DoDraw();
            break;
        }
    
    
    }
    
}


//______________________________________________________________________________
void SidsSummary::InitParameters()
{
    if(!fParConfig.GetValue<string>("DetectorID").empty())
        fDetectorID=fParConfig.GetValue<string>("DetectorID");
    else 
        fDetectorID="RSA51";
    fFileName=fParConfig.GetValue<string>("InputFile").c_str();
    string inputfilename=fParConfig.GetValue<string>("InputFile");
    string treename=fParConfig.GetValue<string>("TreeName");
    string branchname=fParConfig.GetValue<string>("Branch");
    string dirname=fParConfig.GetValue<string>("InputDirectory");
    
    EsrTree *DecayTree = new EsrTree(inputfilename,treename,branchname);
    fDataList=DecayTree->GetEsrData();
    delete DecayTree;
    
    vector<string> analyzedFiles;
    for(unsigned int k(0); k<fDataList.size(); k++)
    {
        string temp=fDataList[k].GetFileName();
        analyzedFiles.push_back(temp);
    }
    
    SIDSFileManager Fileman(dirname,analyzedFiles);
    map<string,int> duplicates=Fileman.GetDuplicatesList();
    map<string,vector<int> > duplicatesIdx;
    
    for(unsigned int k(0);k<fDataList.size() ;k++)
    {
        string temp=fDataList[k].GetFileName();
        if(duplicates.count(temp))
        {
            duplicatesIdx[temp].push_back(k);
        }
    }
    
    int sumduplicate=0;
    for(auto p : duplicates)
    {
        sumduplicate+=p.second;
    }
    ///*
    
    for(auto p : duplicatesIdx)
    {
        
        //for(auto q : p.second)
        //    MQLOG(INFO)<<"Index = "<<q<<"  for file "<<p.first;
    }
    //*/
    int binNumbECHist=2000;
    double tmin=0.;
    double tmax=70.;
    
    int binNumber=(int)fDataList.size()-sumduplicate+(int)duplicates.size();
    if(binNumber<=3)
        binNumber=3;
    fECFreq = new TH1D("Freq-dist","f_{EC}-f_{D} distribution",4000,-4000.,4000.);
    fNEC = new TH1I("NEC","Number of EC vs injection number",binNumber,1.,(Double_t)binNumber);
    fPfreq = new TH1F("ParentFreq","Frequency of parent ions vs injection number",binNumber,1.,(Double_t)binNumber);
    fECDecayTimes = new TH1D("EC-Decay","EC-Decay times",binNumbECHist,tmin,tmax);
    
    int ii=0;// different iterator to prevent on the holes caused by setbincontent(i)
    for(unsigned int i(0); i<fDataList.size(); i++)
    {
        string filename=fDataList[i].GetFileName();
        int NumbEC=fDataList[i].GetNEC();
        float freq=fDataList[i].GetCoolParentFreq();
        
        if(!duplicates.count(filename) || (duplicates.count(filename) && i==duplicatesIdx[filename].back()) )
        {
            fNEC->SetBinContent(ii+1,NumbEC);
            fPfreq->SetBinContent(ii+1,freq);
            vector<EsrDecayEvent> ECDecayEvents=fDataList[i].GetECData();

            double f_Dcool=(double)freq+1584.;

            for(unsigned int j(0);j<ECDecayEvents.size();j++)
            {
                double t_ec=(double)ECDecayEvents[j].GetDecayTime();
                double f_ec=(double)ECDecayEvents[j].GetDecayFreq()-f_Dcool;
                fECDecayTimes->Fill(t_ec);
                fECFreq->Fill(f_ec);
            }
            ii++;
        }
       
    }
    
    
}

/// ////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////// file/object managing function /////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////
//______________________________________________________________________________
void SidsSummary::OpenRootFile()
{
    

    static TString dir(".");
    fFileInfo.fFileTypes = DND_Types;
    fFileInfo.fIniDir    = StrDup(dir);
    new TGFileDialog(gClient->GetRoot(), this, kFDOpen, &fFileInfo);
    dir = fFileInfo.fIniDir;
    fFileName=fFileInfo.fFilename;
    cout<<"[INFO] Opening " << fFileName <<endl;
    //cout<<boost::filesystem::basename(fFileName.Data())<<endl;
    
    fInputFile=TFile::Open(fFileName,"READ");
    if(!RootFileManager(fInputFile,fFileName))
        DoExit();
}


void SidsSummary::AddToRootFile(TObject* obj, const string & outputFileName, const string & fileOption)
{
    
    TFile* rootfile = new TFile(outputFileName.c_str(),fileOption.c_str());
    obj->Write("",kWriteDelete);
    rootfile->Close();
    delete rootfile;
}

void SidsSummary::SaveHisto(const string &  outputFileName)
{
    for(unsigned int i(0);i<f1DHisto.size();i++)
        if(f1DHisto[i])
            AddToRootFile(f1DHisto[i],outputFileName,"UPDATE");
    
    for(unsigned int i(0);i<f2DHisto.size();i++)
        if(f2DHisto[i])
            AddToRootFile(f2DHisto[i],outputFileName,"UPDATE");
    
    for(unsigned int i(0);i<fHeaders.size();i++)
        if(fHeaders[i])
            AddToRootFile(fHeaders[i],outputFileName,"UPDATE");
    
    if(fHisto_px)
        AddToRootFile(fHisto_px,outputFileName,"UPDATE");
    
    if(fECDecayTimes)
        AddToRootFile(fECDecayTimes,outputFileName,"UPDATE");
    
    if(fDaughterTrace)
        AddToRootFile(fDaughterTrace,outputFileName,"UPDATE");
    
    if(fNEC)
        AddToRootFile(fNEC,outputFileName,"UPDATE");
    
    if(fPfreq)
        AddToRootFile(fPfreq,outputFileName,"UPDATE");
    
    
}

//______________________________________________________________________________
int SidsSummary::RootFileManager(TFile* file, const TString & filename)
{
    
     if(file)
    {
        if (!file->IsOpen()) 
        {
          cout<<"[ERROR] Cannot open file " << file->GetName() <<endl;

          return 0;
        }
        else
            if(file->ReOpen("READ")<0)
                cout<<"[ERROR] Failed reopening file " << file->GetName() <<endl;
    }
    else
    {
        
        cout<<"[ERROR] Could not open the file " << filename <<endl;
        return 0;
       
    }
        

    TList* list = file->GetListOfKeys() ;
    if (!list) 
    { 
          cout<<"[ERROR] No key found in file "<<file->GetName()<<endl;
          return 0;
    }
    
    fBaseLTI = fListTree->AddItem(0, (boost::filesystem::basename(file->GetName())).c_str());
    
    TDirectory *CurrentDir=gDirectory;
    ReadDir(CurrentDir);
    
    
    return 1;
}


//______________________________________________________________________________
void SidsSummary::ReadDir(TDirectory *dir) 
{
   TDirectory *dirsav = gDirectory;
   TIter next(dir->GetListOfKeys());
   TKey *key;
   while ((key = (TKey*)next())) 
   {
      if (key->IsFolder()) 
      {
         dir->cd(key->GetName());
         TDirectory *subdir = gDirectory;
         ReadDir(subdir);
         dirsav->cd();
         continue;
      }
      else
          SeekObject(key);
      
    }
}

//______________________________________________________________________________
void SidsSummary::SeekObject(TKey *key)
{
    TObject* obj ;
    obj = key->ReadObj();
    if ((strcmp(obj->IsA()->GetName(),"TProfile")!=0)
            && (!obj->InheritsFrom("TH2"))
            && (!obj->InheritsFrom("TH1"))  
            && (!obj->InheritsFrom("Header"))
            )
    {
        printf("[Warning] Object %s is not a header nor 1D nor 2D histogram : "
           "will not be converted\n",obj->GetName());
    }
    printf("[INFO] Found object with name:%s and title:%s\n", obj->GetName(), obj->GetTitle());

    
    
    /// SEARCH FOR TH1D
    if(obj->InheritsFrom("TH1D"))
    {
        TH1D* histo1D=NULL;
        histo1D=(TH1D*)gDirectory->Get(obj->GetName());

        if(histo1D) 
        {
            f1DHisto.push_back(new TH1D(*histo1D));
            AddToListTree(f1DHisto[fHisto1DCounter]);
            fHisto1DCounter++;
        }
    }
    
    /// SEARCH FOR TH1F
    if(obj->InheritsFrom("TH1F"))
    {
        TH1D* histo1D=NULL;
        histo1D=(TH1D*)gDirectory->Get(obj->GetName());

        if(histo1D) 
        {
            f1DHisto.push_back(new TH1D(*histo1D));
            AddToListTree(f1DHisto[fHisto1DCounter]);
            fHisto1DCounter++;
        }
    }
    
    
    /// SEARCH FOR TH2D
    if(obj->InheritsFrom("TH2D"))
    {
        
        TH2D* histo2D=NULL;
        histo2D=(TH2D*)gDirectory->Get(obj->GetName());
                
        if(histo2D) 
        {
            f2DHisto.push_back(new TH2D(*histo2D));
            AddToListTree(f2DHisto[fHisto2DCounter]);
            fHisto2DCounter++;
        }
        
    }
    
    /// SEARCH FOR HEADERS
    if(obj->InheritsFrom("Header"))
    {
        Header* RsaHeader=NULL;
        RsaHeader=(Header*)gDirectory->Get(obj->GetName());
        //cout<<"Header name = "<<obj->GetName()<<endl;
        
        //RsaHeader->Show();
        if(RsaHeader)
        {
            fHeaders.push_back(new Header(*RsaHeader));
            fHeaderCounter++;
        }
    }
    
}




//______________________________________________________________________________
int SidsSummary::AddToListTree(TObject* obj)
{
    const TGPicture *pic = 0;
    TGListTreeItem *item;

    if ((strcmp(obj->IsA()->GetName(),"TProfile")!=0)
            && (!obj->InheritsFrom("TH2"))
            && (!obj->InheritsFrom("TH1"))  
            ) 
    {
        printf("[Warning] Object %s is not an 1D or 2D histogram : "
           "will not be added to the ListTree \n",obj->GetName()) ;
        return 0;
    }
    //printf("Histo name:%s title:%s\n",obj->GetName(),obj->GetTitle());

    
    /// SEARCH FOR TH2
    if(obj->InheritsFrom("TH2"))
    {
        pic = gClient->GetPicture("h2_t.xpm");
        item = fListTree->AddItem(fBaseLTI, obj->GetName(), obj, pic, pic);
        fListTree->SetToolTipItem(item, "2D Histogram");
        item->SetDNDSource(kTRUE);
        return 1;
    }
    
    /// SEARCH FOR TH1
    if(obj->InheritsFrom("TH1"))
    {
        pic = gClient->GetPicture("h1_t.xpm");
        item = fListTree->AddItem(fBaseLTI, obj->GetName(), obj, pic, pic);
        fListTree->SetToolTipItem(item, "1D Histogram");
        item->SetDNDSource(kTRUE);
        return 1;

    }
    
    return 0;
    
    
    
}


//______________________________________________________________________________
void SidsSummary::HandleMenu(Int_t menu_id)
{
   // Handle menu events.

   TRootHelpDialog *hd;
   

   switch (menu_id) {
      case FILE_EXIT:
      {
         // close the window and quit application
         DoCloseWindow();
         gApplication->Terminate(0);
         break;
      }
      case FILE_OPEN:
      {
         OpenRootFile();
         break;
      }
      case FILE_BROWSE:
         // start a root object browser
         new TBrowser();
         break;
      case FILE_NEWCANVAS:
         // open a root canvas
         gROOT->MakeDefCanvas();
         break;
      case FILE_CLOSEWIN:
         DoCloseWindow();
         break;
      case HELP_ABOUT:
         hd = new TRootHelpDialog(this, "About Drag and Drop...", 550, 250);
         hd->SetText(gHelpDnd);
         hd->Popup();
         break;
   }
}

//______________________________________________________________________________
void SidsSummary::ResetStatus()
{
   // Restore the original text of the status label and its original color.

   fStatus->SetTextColor(0x0000ff);
   fStatus->ChangeText(gReadyMessage);
}


/// ////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////// Signal/slot functions /////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void SidsSummary::DoCloseWindow()
{
   // Do some cleanup, disconnect signals and then really close the main window.

   if (fGraph) { delete fGraph; fGraph = 0; }
   if (fHisto_px) { delete fHisto_px; fHisto_px = 0; }
   if (fECDecayTimes) { delete fECDecayTimes; fECDecayTimes = 0; }
   if (fDaughterTrace) { delete fDaughterTrace; fDaughterTrace = 0; }
   if (fNEC) { delete fNEC; fNEC = NULL; }
   if (fPfreq) { delete fPfreq; fPfreq = NULL; }
   fMenuFile->Disconnect("Activated(Int_t)", this, "HandleMenu(Int_t)");
   fMenuHelp->Disconnect("Activated(Int_t)", this, "HandleMenu(Int_t)");
   fButtonExit->Disconnect("Clicked()" , this, "CloseWindow()");
   fListTree->Disconnect("DataDropped(TGListTreeItem*, TDNDData*)", this,
                         "DataDropped(TGListTreeItem*,TDNDData*)");
   //TVirtualPadEditor::Terminate();
   
    for(unsigned int i(0);i<f1DHisto.size() ; i++)
    {
        if(f1DHisto[i])
        {
             delete f1DHisto[i];
             f1DHisto[i]=NULL;
        }
    }
    f1DHisto.clear();
   
    for(unsigned int i(0);i<f2DHisto.size() ; i++)
    {
        if(f2DHisto[i])
        {
             delete f2DHisto[i];
             f2DHisto[i]=NULL;
        }
    }
    f2DHisto.clear();
   
    for(unsigned int i(0);i<fHeaders.size() ; i++)
    {
        if(fHeaders[i])
        {
             delete fHeaders[i];
             fHeaders[i]=NULL;
        }
    }
    fHeaders.clear();
    fHisto1DCounter=0;
    fHisto2DCounter=0;
    fDecayCounter=0;
    delete fListTree;
    CloseWindow();
}

//______________________________________________________________________________
void SidsSummary::DataDropped(TGListTreeItem *, TDNDData *data)
{
   // Handle the drop event in the TGListTree. This will just create a new
   // list tree item and copy the received data into it.

   //fStatus->SetTextColor(0xff0000);
   //fStatus->ChangeText("I received data!!!");
   if (data) 
   {
      const TGPicture *pic = 0;
      TGListTreeItem *itm = 0;
      char tmp[1000];
      if (data->fDataType == gRootObj) 
      {
         TBufferFile buf(TBuffer::kRead, data->fDataLength, (void *)data->fData);
         buf.SetReadMode();
         TObject *obj = (TObject *)buf.ReadObjectAny(TObject::Class());
         sprintf(tmp, "Received DND data : Type = \"%s\"; Length = %d bytes;",
                 obj->ClassName(), data->fDataLength);
         if (obj->InheritsFrom("TGraph"))
            pic = gClient->GetPicture("f1_t.xpm");
         else if (obj->InheritsFrom("TH2"))
            pic = gClient->GetPicture("h2_t.xpm");
         else if (obj->InheritsFrom("TH1"))
            pic = gClient->GetPicture("h1_t.xpm");
         
            itm = fListTree->AddItem(fBaseLTI, obj->GetName(), obj, pic, pic);
            fListTree->SetToolTipItem(itm, obj->GetName());
         
      }
      else 
      {
         sprintf(tmp, "Received DND data: \"%s\"", (char *)data->fData);
         TObjString *ostr = new TObjString((char *)data->fData);
         TImage *img1 = TImage::Open("doc_t.xpm");
         TImage *img2 = TImage::Open("slink_t.xpm");
         if (img1 && img2) {
            img1->Merge(img2);
            pic = gClient->GetPicturePool()->GetPicture("doc_lnk", img1->GetPixmap(),
                                                        img1->GetMask());
            delete img2;
            delete img1;
         }
         else pic = gClient->GetPicture("doc_t.xpm");
         itm = fListTree->AddItem(fBaseLTI, "Link...", ostr, pic, pic);
         fListTree->SetToolTipItem(itm, (const char *)data->fData);
      }
      if (itm) itm->SetDNDSource(kTRUE);
      //fStatus->ChangeText(tmp);
   }
   //TTimer::SingleShot(3000, "SidsSummary", this, "ResetStatus()");
   fCanvas1->Update();
   fCanvas2->Update();
}




//______________________________________________________________________________
void SidsSummary::DoDraw() 
{
    fCanvas2->SetCrosshair();
    fCanvas2->cd();
    fECDecayTimes->Draw();
    fCanvas2->Update();
    
    
    fCanvas1->cd(1);
    fNEC->Draw();
    fCanvas1->Update();
    
    fCanvas1->cd(2);
    fECDecayTimes->Draw();
    fCanvas1->Update();
    
    fCanvas1->cd(3);
    fPfreq->Draw();
    fCanvas1->Update();
    
    fCanvas1->cd(4);
    fECFreq->Draw();
    fCanvas1->Update();
}



//______________________________________________________________________________
void SidsSummary::EventInfo(Int_t event, Int_t px, Int_t py, TObject *selected)
{
    const char *text0, *text1, *text3;
    char text2[50];
    if(selected->InheritsFrom("TH2"))
    {
         fCanvas2->cd();// to get rid of the fcanvas2 event info issue after DND
         //fCanvas2->Update();
    }
    text0 = selected->GetTitle();
    fStatusBar->SetText(text0,0);
    text1 = selected->GetName();
    fStatusBar->SetText(text1,1);
    if (event == kKeyPress)
       sprintf(text2, "%c", (char) px);
    else
       sprintf(text2, "%d,%d", px, py);
    fStatusBar->SetText(text2,2);
    text3 = selected->GetObjectInfo(px,py);
    fStatusBar->SetText(text3,3);
   
}

//______________________________________________________________________________
void SidsSummary::DoExit()
{
    TVirtualPadEditor::Terminate();
    gApplication->Terminate(0);
}


ClassImp(SidsSummary)
        
        
        