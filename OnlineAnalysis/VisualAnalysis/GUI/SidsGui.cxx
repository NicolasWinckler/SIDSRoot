/* 
 * File:   SidsGui.cxx
 * Author: winckler
 * 
 * Created on August 7, 2014, 5:01 PM
 */


#include "SidsGui.h"




const char gHelpDND[] = "\
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

const char gReadyMsg[] = "Ready. You can drag list tree items to any \
pad in the canvas, or to the \"Base\" folder of the list tree itself...";


const char *dnd_types[] = {
   "ROOT files",    "*.root",
   "ROOT macros",   "*.C",
   "All files",     "*",
    0,               0
};

static Atom_t gRootObj  = kNone;





//______________________________________________________________________________
SidsGui::SidsGui(const TGWindow *p, int w, int h,MQconfig SamplerConfig, std::string Filename) :
   TGMainFrame(p, w, h), fEc(NULL), fEc2(NULL), fButtonExit(NULL), 
        fButtonValidate(NULL), fFileQualityTag(NULL),
        fMenuBar(NULL), fMenuFile(NULL),
        fMenuHelp(NULL), fCanvas1(NULL), fCanvas2(NULL), fListTree(NULL),
        fBaseLTI(NULL), fStatus(NULL), fStatusBar(NULL), 
        fGraph(NULL), fHisto_px(NULL), fParentTrace(NULL), fDaughterTrace(NULL),
        fNEC(NULL), fPfreq(NULL), fControlFrame(NULL),
        fFileName(Filename.c_str()), fFileInfo(), 
        fDecayData() ,fDetectorID("RSA51") , fParentFreq(0.), fDecayCounter(0), 
        fHisto1DCounter(0), fHisto2DCounter(0),fHeaderCounter(0),
        fReadyToSend(false), fSampler(false),
        fParConfig(SamplerConfig)

{
   // Constructor.

   SetCleanup(kDeepCleanup);
   const TGPicture *pic = 0;
   TGListTreeItem *item;
   ////////////////////////////////////////////////////// 0th layer (menu)
   fMenuBar = new TGMenuBar(this, 35, 50, kHorizontalFrame);

   fMenuFile = new TGPopupMenu(gClient->GetRoot());
   fMenuFile->AddEntry(" &Open...\tCtrl+O", M_FILE_OPEN, 0, gClient->GetPicture("bld_open.png"));
   fMenuFile->AddEntry(" &Browse...\tCtrl+B", M_FILE_BROWSE);
   fMenuFile->AddEntry(" &New Canvas\tCtrl+N", M_FILE_NEWCANVAS);
   fMenuFile->AddEntry(" &Close Window\tCtrl+W", M_FILE_CLOSEWIN);
   fMenuFile->AddSeparator();
   fMenuFile->AddEntry(" E&xit\tCtrl+Q", M_FILE_EXIT, 0, gClient->GetPicture("bld_exit.png"));
   fMenuFile->Connect("Activated(Int_t)", "SidsGui", this,"HandleMenu(Int_t)");

   fMenuHelp = new TGPopupMenu(gClient->GetRoot());
   fMenuHelp->AddEntry(" &About...", M_HELP_ABOUT, 0, gClient->GetPicture("about.xpm"));
   fMenuHelp->Connect("Activated(Int_t)", "SidsGui", this, "HandleMenu(Int_t)");

   fMenuBar->AddPopup("&File", fMenuFile, new TGLayoutHints(kLHintsTop|kLHintsLeft, 0, 4, 0, 0));
   fMenuBar->AddPopup("&Help", fMenuHelp, new TGLayoutHints(kLHintsTop|kLHintsRight));

   AddFrame(fMenuBar, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 5));

   ////////////////////////////////////////////////////// 1st layer
   TGHorizontalFrame *hfrm = new TGHorizontalFrame(this, 10, 10);
   TGCanvas *canvas = new TGCanvas(hfrm, 150, 100);
   fListTree = new TGListTree(canvas, kHorizontalFrame);
   fListTree->Associate(this);
   fEc = new TRootEmbeddedCanvas("AdditionalCanvas", hfrm, 400, 400);
   hfrm->AddFrame(canvas, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5, 5));
   hfrm->AddFrame(fEc, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   //AddFrame(hfrm, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   fEc->SetDNDTarget(kTRUE);
   fCanvas1 = fEc->GetCanvas();
   
   fCanvas1->Divide(2, 2);
   //fCanvas1->SetBorderMode(0);
   fCanvas1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)","SidsGui",this,
               "EventInfo(Int_t,Int_t,Int_t,TObject*)");
   
   
   gRootObj  = gVirtualX->InternAtom("application/root", kFALSE);


   TString rootsys(gSystem->UnixPathName(gSystem->Getenv("ROOTSYS")));
#ifdef G__WIN32
   // remove the drive letter (e.g. "C:/") from $ROOTSYS, if any
   if (rootsys[1] == ':' && rootsys[2] == '/')
      rootsys.Remove(0, 3);
#endif
   
   
   string name=fParConfig.GetValue<string>("InputFile");
    fFileName=name.c_str();
    TFile* rootfile0 = new TFile(fFileName);
    RootFileManager(rootfile0);
   
   // open the base list tree item and allow to drop into it
   fListTree->OpenItem(fBaseLTI);
   fListTree->GetFirstItem()->SetDNDTarget(kTRUE);

   // connect the DataDropped signal to be able to handle it
   fListTree->Connect("DataDropped(TGListTreeItem*, TDNDData*)", "SidsGui",
                      this, "DataDropped(TGListTreeItem*,TDNDData*)");

   ////////////////////////////////////////////////////// 1st layer 2nd col
   ///
   
   fEc2 = new TRootEmbeddedCanvas ("AnalysisCanvas",hfrm,600,400);
   fEc2->SetDNDTarget(kTRUE);
   //fEc2->GetCanvas()->EditorBar();
   TVirtualPadEditor::ShowEditor();
   fCanvas2 = fEc2->GetCanvas();
   fCanvas2->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)","SidsGui",this,
               "EventInfo(Int_t,Int_t,Int_t,TObject*)");
   
   
   fCanvas2->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)","SidsGui",this,
               "DoDoubleClick(Int_t,Int_t,Int_t,TObject*)");
   
   
   hfrm->AddFrame(fEc2, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   
   fControlFrame = new TGVerticalFrame(hfrm, 200, 400);
   
   /// QUALITY BUTTON
   
   fFileQualityTag = new SidsQualityTagField(fControlFrame,"Tag and comment");
   fFileQualityTag->GetTagEntry()->Connect("TextChanged(char*)", "SidsQualityTagField",
                                   fFileQualityTag, "GetTagField(char*)");
   fFileQualityTag->GetCommentEntry()->Connect("TextChanged(char*)", "SidsQualityTagField",
                                   fFileQualityTag, "GetCommentField(char*)");

   fControlFrame->AddFrame(fFileQualityTag, new TGLayoutHints( kLHintsExpandX,2,2,5,5));
   
      
   
   /// DRAW BUTTON
   TGTextButton *draw = new TGTextButton(fControlFrame,"&Draw");
   draw->Connect("Clicked()","SidsGui",this,"DoDraw()");
   
   fControlFrame->AddFrame(draw, new TGLayoutHints(kLHintsExpandX,2,2,2,5));
   
   
   
   
   /// ADD DECAY BUTTON
   TGTextButton *AddDecay = new TGTextButton(fControlFrame,"&Add EC-Decay");
   AddDecay->Connect("Clicked()","SidsGui",this,"AddDecay()");
   
   fControlFrame->AddFrame(AddDecay, new TGLayoutHints(kLHintsExpandX,2,2,2,5));
   
   
   
   /// REMOVE DECAY BUTTON
   TGTextButton *RemoveDecay = new TGTextButton(fControlFrame,"&Remove EC-Decay");
   RemoveDecay->Connect("Clicked()","SidsGui",this,"RemoveDecay()");
   
   fControlFrame->AddFrame(RemoveDecay, new TGLayoutHints(kLHintsExpandX,2,2,2,5));
   
   
  
   hfrm->AddFrame(fControlFrame, new TGLayoutHints(kLHintsRight | kLHintsExpandY));
   AddFrame(hfrm, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   
   
   
   
   ////////////////////////////////////////////////////// 2nd layer
   TGHorizontalFrame *hf3 = new TGHorizontalFrame(this, 10, 10);

   fStatus = new TGLabel(hf3, new TGHotString(gReadyMsg));
   fStatus->SetTextJustify(kTextLeft);
   fStatus->SetTextColor(0x0000ff);
   hf3->AddFrame(fStatus, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY,10, 10, 10, 10));
   
   
   TGVerticalFrame *ExitAndValidFrame = new TGVerticalFrame(hf3, 10, 10);
   
   
   fButtonValidate = new TGTextButton(ExitAndValidFrame, "&Validate");
   fButtonValidate->Resize(fButtonValidate->GetDefaultSize());
   fButtonValidate->Connect("Clicked()" , "SidsGui", this, "DoValidate()");
   ExitAndValidFrame->AddFrame(fButtonValidate, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));

   fButtonExit = new TGTextButton(ExitAndValidFrame, "        &Exit...        ");
   fButtonExit->Resize(fButtonExit->GetDefaultSize());
   fButtonExit->SetToolTipText("Exit Application (ROOT)");
   fButtonExit->Connect("Clicked()" , "SidsGui", this, "DoExit()");
   ExitAndValidFrame->AddFrame(fButtonExit, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));

   hf3->AddFrame(ExitAndValidFrame, new TGLayoutHints(kLHintsCenterY | kLHintsRight, 10, 10, 10, 10));

   AddFrame(hf3, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 5, 5, 5, 5));


    // status bar
    Int_t parts[] = {45, 15, 10, 30};
    fStatusBar = new TGStatusBar(this, 50, 10, kVerticalFrame);
    fStatusBar->SetParts(parts, 4);
    fStatusBar->Draw3DCorner(kFALSE);
    AddFrame(fStatusBar, new TGLayoutHints(kLHintsExpandX, 0, 0, 10, 0));
   

    ////////////////////////////////////////////////////// load number of EC histogram
    
    string outputfilename=fParConfig.GetValue<string>("OutputFile");
    string treename=fParConfig.GetValue<string>("TreeName");
    string branchname=fParConfig.GetValue<string>("Branch");
    
    
    
    EsrTree DecayTree(outputfilename,treename,branchname);
    //EsrTree *DecayTree = new EsrTree(outputfilename,treename,branchname);
    std::vector<EsrInjData> DataList=DecayTree.GetEsrData();
    
    int binNumber=(int)DataList.size();
    fNEC = new TH1I("NEC","Number of EC vs injection number",binNumber,1.,(Double_t)binNumber);
    fPfreq = new TH1F("ParentFreq","Frequency of parent ions vs injection number",binNumber,1.,(Double_t)binNumber);
    for(unsigned int i(0); i<DataList.size(); i++)
    {
        int NumbEC=DataList[i].GetNEC();
        float freq=DataList[i].GetCoolParentFreq();
        fNEC->SetBinContent(i+1,NumbEC);
        fPfreq->SetBinContent(i+1,freq);
    }//*/
    //delete DecayTree;
    AddToListTree(fNEC);
    AddToListTree(fPfreq);
   //////////////////////////////////////////////////////
   
   SetWindowName("Single Ion Decay Spectroscopy Analysis");
   MapSubwindows();
   Resize(GetDefaultSize());
   Connect("CloseWindow()", "SidsGui", this, "DoCloseWindow()");
   DontCallClose(); // to avoid double deletions.
   MapRaised();
}

//______________________________________________________________________________
SidsGui::~SidsGui()
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
   
}


/// ////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////// file/object managing function /////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////
//______________________________________________________________________________
void SidsGui::OpenRootFile()
{
    static TString dir(".");
    fFileInfo.fFileTypes = dnd_types;
    fFileInfo.fIniDir    = StrDup(dir);
    new TGFileDialog(gClient->GetRoot(), this, kFDOpen, &fFileInfo);
    dir = fFileInfo.fIniDir;
    fFileName=fFileInfo.fFilename;
    std::cout<<"Opening "<<fFileName <<std::endl;
    //std::cout<<boost::filesystem::basename(fFileName.Data())<<std::endl;
    
    TFile* rootfile = new TFile(fFileName);
    
    RootFileManager(rootfile);
}


void SidsGui::AddToRootFile(TObject* obj, string outputFileName, string fileOption)
{
    
    TFile* rootfile = new TFile(outputFileName.c_str(),fileOption.c_str());
    obj->Write("",kWriteDelete);
    rootfile->Close();
    delete rootfile;
}

void SidsGui::SaveHisto(string outputFileName)
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
    
    if(fParentTrace)
        AddToRootFile(fParentTrace,outputFileName,"UPDATE");
    
    if(fDaughterTrace)
        AddToRootFile(fDaughterTrace,outputFileName,"UPDATE");
    
    if(fNEC)
        AddToRootFile(fNEC,outputFileName,"UPDATE");
    
    if(fPfreq)
        AddToRootFile(fPfreq,outputFileName,"UPDATE");
    
    
}

//______________________________________________________________________________
int SidsGui::RootFileManager(TFile* rootfile)
{

    if (!rootfile->IsOpen()) 
    {
      std::cout<<"[ERROR] Cannot open file "<<fFileName.Data()<<std::endl;
      
      return 0;
    }

    TList* list = rootfile->GetListOfKeys() ;
    if (!list) 
    { 
          std::cout<<"[ERROR] No key found in file "<<fFileName.Data()<<std::endl;
          return 0;
    }
    
    fBaseLTI = fListTree->AddItem(0, (boost::filesystem::basename(fFileName.Data())).c_str());
    
    TDirectory *CurrentDir=gDirectory;
    ReadDir(CurrentDir);
    return 1;
}


//______________________________________________________________________________
void SidsGui::ReadDir(TDirectory *dir) 
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
void SidsGui::SeekObject(TKey *key)
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
    printf("[INFO] Histo name:%s title:%s\n", obj->GetName(), obj->GetTitle());

    
    
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
        //std::cout<<"Header name = "<<obj->GetName()<<std::endl;
        
        //RsaHeader->Show();
        if(RsaHeader)
        {
            fHeaders.push_back(new Header(*RsaHeader));
            fHeaderCounter++;
        }
    }
    
}




//______________________________________________________________________________
int SidsGui::AddToListTree(TObject* obj)
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
void SidsGui::HandleMenu(Int_t menu_id)
{
   // Handle menu events.

   TRootHelpDialog *hd;
   

   switch (menu_id) {
      case M_FILE_EXIT:
      {
         // close the window and quit application
         DoCloseWindow();
         gApplication->Terminate(0);
         break;
      }
      case M_FILE_OPEN:
      {
         OpenRootFile();
         break;
      }
      case M_FILE_BROWSE:
         // start a root object browser
         new TBrowser();
         break;
      case M_FILE_NEWCANVAS:
         // open a root canvas
         gROOT->MakeDefCanvas();
         break;
      case M_FILE_CLOSEWIN:
         DoCloseWindow();
         break;
      case M_HELP_ABOUT:
         hd = new TRootHelpDialog(this, "About Drag and Drop...", 550, 250);
         hd->SetText(gHelpDND);
         hd->Popup();
         break;
   }
}

//______________________________________________________________________________
void SidsGui::ResetStatus()
{
   // Restore the original text of the status label and its original color.

   fStatus->SetTextColor(0x0000ff);
   fStatus->ChangeText(gReadyMsg);
}


/// ////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////// Signal/slot functions /////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void SidsGui::DoCloseWindow()
{
   // Do some cleanup, disconnect signals and then really close the main window.

   if (fGraph) { delete fGraph; fGraph = 0; }
   if (fHisto_px) { delete fHisto_px; fHisto_px = 0; }
   if (fParentTrace) { delete fParentTrace; fParentTrace = 0; }
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
void SidsGui::DataDropped(TGListTreeItem *, TDNDData *data)
{
   // Handle the drop event in the TGListTree. This will just create a new
   // list tree item and copy the received data into it.

   fStatus->SetTextColor(0xff0000);
   fStatus->ChangeText("I received data!!!");
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
      fStatus->ChangeText(tmp);
   }
   TTimer::SingleShot(3000, "SidsGui", this, "ResetStatus()");
   fCanvas1->Update();
   fCanvas2->cd();
   fCanvas2->Update();
}

//______________________________________________________________________________
void SidsGui::DoDraw() 
{
    
    string Suffix;
    string histokickerID;
    //string fftSuffix("_fft");
    if(!fParConfig.GetValue<string>("DetectorID").empty())
        fDetectorID=fParConfig.GetValue<string>("DetectorID");
    
    if(!fParConfig.GetValue<string>("DetectorSuffix").empty())
        Suffix=fParConfig.GetValue<string>("DetectorSuffix");
    else
        Suffix="_mtpsd6";
    
    string histoID(fDetectorID);
    histoID+=Suffix;
    
    if(!fParConfig.GetValue<string>("KickerPrefix").empty())
        histokickerID=fParConfig.GetValue<string>("KickerPrefix");
    else
        histokickerID="Oscil";
    
    
    for(unsigned int i(0);i<f2DHisto.size();i++)
    {
        if(f2DHisto[i])
        {
            string histoname=string(f2DHisto[i]->GetName());
            size_t found = histoname.find(histoID);

            if(found!=std::string::npos)
            {
                /*
                if(fHisto_px)
                {
                    f2DHisto[i]->GetXaxis()->UnZoom();
                    f2DHisto[i]->GetYaxis()->UnZoom();
                    delete fHisto_px;
                    fHisto_px=NULL;
                }*/
                //fHisto_px=f2DHisto[i]->ProjectionX();
                //Int_t binMax=fHisto_px->GetMaximumBin();
                Int_t zoomwindow=fParConfig.GetValue<Int_t>("BinZoomTH2Window");
                Int_t binMax=f2DHisto[i]->ProjectionX()->GetMaximumBin();
                
                Double_t Xmin=0.;
                Double_t Xmax=0.;
                
                if((binMax-zoomwindow)>=f2DHisto[i]->GetXaxis()->GetFirst())
                    Xmin=f2DHisto[i]->GetXaxis()->GetBinCenter(binMax-zoomwindow);
                else
                    Xmin=f2DHisto[i]->GetXaxis()->GetBinCenter(f2DHisto[i]->GetXaxis()->GetFirst());
                
                if((binMax+zoomwindow)<=f2DHisto[i]->GetXaxis()->GetLast())
                    Xmax=f2DHisto[i]->GetXaxis()->GetBinCenter(binMax+zoomwindow);
                else
                    Xmax=f2DHisto[i]->GetXaxis()->GetBinCenter(f2DHisto[i]->GetXaxis()->GetLast());
                
                //std::cout<<"Xmin : "<< Xmin<<std::endl;
                //std::cout<<"Xax : "<< Xmax<<std::endl;
                f2DHisto[i]->GetXaxis()->SetRangeUser(Xmin,Xmax);
                
                histoname+="_Rebinned";

                fCanvas2->cd();
    
                TH2D* histo=dynamic_cast<TH2D*>(f2DHisto[i]->RebinX(2,histoname.c_str()));
                if(histo)
                {
                    //histo->SetMaximum(5.e-7);
                    histo->Draw("zcol");
                }
                
                fCanvas2->Update();
                
                /// canvas1 PAD1 projection on frequency axis
                fCanvas1->cd(1);
                
                Int_t BinPWindow=fParConfig.GetValue<int>("BinPWindow"); 
                Int_t BinDWindow=fParConfig.GetValue<int>("BinDWindow"); 
                Int_t BinDist=fParConfig.GetValue<int>("BinDistancePDfreq"); 
                Double_t sigma=fParConfig.GetValue<double>("BinSigmaPeak");
                Double_t threshold=fParConfig.GetValue<double>("ThresholdPeak");
                
                
                FindTraces(f2DHisto[i],BinPWindow,BinDWindow,BinDist,sigma,"",threshold);
                
                fHisto_px->Draw();
                break;
            }
        }
    }
    
   
    
    /// canvas1 PAD2
    fCanvas1->cd(2);
    if(fParentTrace)
        fParentTrace->Draw();
    if(fDaughterTrace)
    {
        fDaughterTrace->SetLineColor(2);
        fDaughterTrace->Draw("SAME");
    }
    
    /// canvas1 PAD3 Number of EC vs inj
    fCanvas1->cd(3);
    if(fNEC)
        fNEC->Draw();
    
    /// canvas1 PAD4 kicker signals
    fCanvas1->cd(4);
    bool drawsame=false;
    for(unsigned int i(0);i<f1DHisto.size();i++)
    {
        if(f1DHisto[i])
        {
            string histoname=string(f1DHisto[i]->GetName());
            size_t found = histoname.find(histokickerID);

            if(found!=std::string::npos)
            {
                if(drawsame)
                {
                    f1DHisto[i]->SetLineColor(i);
                    f1DHisto[i]->Draw("SAME");
                }
                else
                {
                    f1DHisto[i]->SetLineColor(i);
                    f1DHisto[i]->Draw();
                    drawsame=true;
                }
                
                
            }
        }
    }
    
    fCanvas1->Update();
}



//______________________________________________________________________________
void SidsGui::AddDecay()
{
    TString name("Decay");
    Int_t index=fDecayCounter+1;
    name+=index;
    fDecayField.push_back(new SidsDecayTxtField(fControlFrame,name.Data(),fDecayCounter));
    
    if(fDecayCounter<fDecayField.size())
    {
        std::cout<<"Add decay "  <<name.Data() <<std::endl;
        fControlFrame->AddFrame(fDecayField[fDecayCounter], new TGLayoutHints(kLHintsExpandX,2,2,5,5));
        fDecayField[fDecayCounter]->GetCoordX()->Connect("TextChanged(char*)", "SidsDecayTxtField",
                                   fDecayField[fDecayCounter], "GetDecayFreqField(char*)");

        fDecayField[fDecayCounter]->GetCoordY()->Connect("TextChanged(char*)", "SidsDecayTxtField",
                                   fDecayField[fDecayCounter], "GetDecayTimeField(char*)");
        
        //fDecayField[fDecayCounter]->SetCoordX(666);
        //fDecayField[fDecayCounter]->SetCoordY(666);
    }
    fDecayCounter++;
    fControlFrame->MapSubwindows();
    //MapSubwindows();
    fControlFrame->MapRaised();
    
    //Resize(GetDefaultSize());
    Resize();
}



//______________________________________________________________________________
void SidsGui::RemoveDecay()
{
    Int_t index=fDecayCounter-1;
    
    if(index>=0 && index==fDecayField.size()-1)
    {
        TString name("Decay");
        name+=fDecayCounter;
        std::cout<<"Remove decay "  <<name.Data() <<std::endl;
        
        //fControlFrame->RemoveFrame(fDecayField[index]);
        fDecayField[index]->ClearWindow();
        fControlFrame->RemoveFrame(fDecayField[index]);
        delete fDecayField[index];
        fDecayField.pop_back();
        fDecayCounter--;
    }
    
    fControlFrame->MapSubwindows();
    fControlFrame->MapRaised();
    //Resize();
    Resize(GetDefaultSize());
}
//______________________________________________________________________________
void SidsGui::EventInfo(Int_t event, Int_t px, Int_t py, TObject *selected)
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
void SidsGui::DoExit()
{
    TVirtualPadEditor::Terminate();
    gApplication->Terminate(0);
}
//______________________________________________________________________________
void SidsGui::DoDoubleClick(Int_t event, Int_t px, Int_t py, TObject *selected)
{
       Float_t x = gPad->PadtoX(gPad->AbsPixeltoX(px));
       Float_t y = gPad->PadtoY(gPad->AbsPixeltoY(py));
       Int_t binx,biny;
       if(event==kButton1Double)
       {
            if(selected->InheritsFrom("TH2"))
            {
                binx = ((TH2D*)selected)->GetXaxis()->FindBin(x);
                biny = ((TH2D*)selected)->GetYaxis()->FindBin(y);
                if(fDecayCounter-1>=0 && fDecayCounter-1<fDecayField.size())
                {
                    fDecayField[fDecayCounter-1]->SetCoordX(binx);
                    fDecayField[fDecayCounter-1]->SetCoordY(biny);
                    
                    cout<<"Print Info :"<<endl;
                    cout<<"Coord : (x="<<x<<", y="<< y <<")"<<endl;
                    cout<<"Bin : (binx="<<binx<<", biny="<<biny<<")"<<endl;
                }
            }
            
       }
}

//______________________________________________________________________________
void SidsGui::DoValidate()
{
    //std::cout<<"Decay Rev. Freq is : "  <<fDecayFreq[0]<<std::endl;
    //std::cout<<"Decay Time is : "       <<fDecayTime[0]<<std::endl;
    
    
    //std::cout<<"Qualitytag is : "  << fFileQualityTag->GetTag() <<std::endl;
    //std::cout<<"comment is : "       << fFileQualityTag->GetComment() <<std::endl;
    string headerSuffix("_header");
    string headerID(fDetectorID);
    headerID+=headerSuffix;
    
    int detectorID=SIDS::kRSA51;
    float freqresolution=31.25;
    float freqoffset=0.;
    float timeresolution=0.032;
    
    for(unsigned int i(0);i<fHeaders.size();i++)
    {
        if(fHeaders[i])
        {
            string headerName=string(fHeaders[i]->GetName());
            size_t found = headerName.find(headerID);
            
            if(found!=std::string::npos)
            {
                if(headerName=="RSA51_header")
                    detectorID=SIDS::kRSA51;
                
                if(headerName=="RSA52_header")
                    detectorID=SIDS::kRSA52;
                
                if(headerName=="RSA30_header")
                    detectorID=SIDS::kRSA30;
                
                
                freqoffset=(float)(fHeaders[i]->GetCenterFrequency()-fHeaders[i]->GetSpan()/2.);
                timeresolution=(float)(fHeaders[i]->GetFrameLength());
                freqresolution=1./timeresolution;
                fHeaders[i]->Show();
            }
            
        }
    }
        
    
    EsrInjData DecayData(fFileName.Data(),detectorID,timeresolution,freqresolution,freqoffset);
    DecayData.SetUserName(fParConfig.GetValue<string>("UserName"));
    DecayData.SetQualityTag(fFileQualityTag->GetTag());
    DecayData.SetFileComment(fFileQualityTag->GetComment());
    DecayData.SetCoolParentFreq(fParentFreq);
    
    for(unsigned int i(0);i<fDecayField.size();i++)
    {
        std::cout<<"Title : "<<std::string(fDecayField[i]->GetTitle())<<std::endl;
        std::cout<<"Decay Rev. Freq is : "  <<fDecayField[i]->GetDecayFreq() <<std::endl;
        std::cout<<"Decay Time is : "       <<fDecayField[i]->GetDecayTime() <<std::endl;
        
        EsrBinDecayEvent Event(SIDS::kECDecay);
        Event.SetBinDecayTime(fDecayField[i]->GetDecayTime());
        Event.SetBinDecayFreq(fDecayField[i]->GetDecayFreq());
        DecayData.AddEvent(Event);
    }
    
    fDecayData=DecayData;
    
    vector<EsrDecayEvent> DecayList=fDecayData.GetECData();
    
    std::cout<<"---------------- PRINT DECAYS ------------"<<std::endl;
    for(unsigned int i(0);i<DecayList.size();i++)
    {
        DecayList[i].PrintEvent();        
    }

    string outputfilename=fParConfig.GetValue<string>("OutputFile");
    string treename=fParConfig.GetValue<string>("TreeName");
    string branchname=fParConfig.GetValue<string>("Branch");
    EsrTree DecayTree(outputfilename,treename,branchname);
    DecayTree.UpdateTree(DecayData);
    //fReadyToSend=true;
    //fSampler=true;
    if(fReadyToSend && fSampler)
        StartSampler();
    
    DoExit();
}





/// ////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////// Other function /////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////
//______________________________________________________________________________
void SidsGui::FindTraces(TH2D* hist2d, Int_t BinPWindow, Int_t BinDWindow, Int_t BinDist, Double_t sigma, Option_t* option, Double_t threshold)
{
    int Rebinning=fParConfig.GetValue<int>("BinningTraces");
    int NPeak2Search=2;
    int NfounPeak=0;
    hist2d->GetXaxis()->UnZoom();
    hist2d->GetYaxis()->UnZoom();
    
    if(!fHisto_px)
    {
        fHisto_px=hist2d->ProjectionX();
        AddToListTree(fHisto_px);
    }
    else
    {
        fHisto_px->GetXaxis()->UnZoom();
        /*
        delete fHisto_px;
        fHisto_px=NULL;
        fHisto_px=hist2d->ProjectionX();
        */
    }
    
    
    TSpectrum *sp = new TSpectrum(NPeak2Search);
    NfounPeak = sp->Search(fHisto_px,sigma,option,threshold);
    Float_t *xpeaks = sp->GetPositionX();
    fParentFreq=xpeaks[0];
    int parentBin=fHisto_px->GetXaxis()->FindBin(fParentFreq);
    //cout<<"xpeaks[0] = "<<xpeaks[0]<<endl;
    
    int binmin=0;
    int binmax=0;
    if(!fParentTrace)
    {
        
        if((parentBin-BinPWindow)>=fHisto_px->GetXaxis()->GetFirst())
            binmin=parentBin-BinPWindow;
        else
            binmin=fHisto_px->GetXaxis()->GetFirst();
        
        if((parentBin+BinPWindow)<=fHisto_px->GetXaxis()->GetLast())
            binmax=parentBin+BinPWindow;
        else
            binmax=fHisto_px->GetXaxis()->GetLast();
        
        
        fParentTrace = hist2d->ProjectionY("Parent",binmin,binmax);
        fParentTrace->Rebin(Rebinning);
        AddToListTree(fParentTrace);
    }
    else
    {
        fParentTrace->GetXaxis()->UnZoom();
        /*
        delete fParentTrace;
        fParentTrace=NULL;
        fParentTrace = hist2d->ProjectionY("Parent",parentBin-BinPWindow,parentBin+BinPWindow);
        fParentTrace->Rebin(Rebinning);
        */
    }
    
    if(!fDaughterTrace)
    {
        if((BinDist+parentBin-BinDWindow)>=fHisto_px->GetXaxis()->GetFirst())
            binmin=BinDist+parentBin-BinDWindow;
        else
            binmin=fHisto_px->GetXaxis()->GetFirst();
        
        if((BinDist+parentBin+BinDWindow)<=fHisto_px->GetXaxis()->GetLast())
            binmax=BinDist+parentBin+BinDWindow;
        else
            binmax=fHisto_px->GetXaxis()->GetLast();
        
        fDaughterTrace = hist2d->ProjectionY("Daughter",binmin,binmax);
        fDaughterTrace->Rebin(Rebinning);
        AddToListTree(fDaughterTrace);
    }
    else
    {
        fDaughterTrace->GetXaxis()->UnZoom();
        /*
        delete fDaughterTrace;
        fDaughterTrace=NULL;
        fDaughterTrace = hist2d->ProjectionY("Daughter",BinDist+parentBin-BinDWindow,BinDist+parentBin+BinDWindow);
        fDaughterTrace->Rebin(Rebinning);
        */
    }
                
    
}


/// ////////////////////////////////////////////////////////////////////////////////////////////////
/// ////////////////////////////// MQ function /////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////////////////
//______________________________________________________________________________
void SidsGui::StartSampler()
{
    
#ifndef __CINT__
    typedef boost::archive::binary_oarchive TBoostBinPayloadOut; // boost binary format
    typedef EsrSamplerTask<TBoostBinPayloadOut> TLoader;

    EsrSidsSampler<TLoader> sampler;
    //TLoader* Loader = new TLoader(fDecayData);
    std::shared_ptr<TLoader> Loader(std::make_shared<TLoader>(fDecayData));
    sampler.SetTask(Loader);
    //fDecayData
    #ifdef NANOMSG
    FairMQTransportFactory* transportFactory = new FairMQTransportFactoryNN();
    #else
    FairMQTransportFactory* transportFactory = new FairMQTransportFactoryZMQ();
    #endif

    sampler.SetTransport(transportFactory);


    sampler.SetProperty(EsrSidsSampler<TLoader>::Id, fParConfig.GetValue<string>("ID").c_str());
    sampler.SetProperty(EsrSidsSampler<TLoader>::InputFile, fParConfig.GetValue<string>("InputFile").c_str());
    sampler.SetProperty(EsrSidsSampler<TLoader>::EventRate, fParConfig.GetValue<int>("EventRate"));
    sampler.SetProperty(EsrSidsSampler<TLoader>::NumIoThreads, fParConfig.GetValue<int>("NumIoThreads"));

    sampler.SetProperty(EsrSidsSampler<TLoader>::NumInputs, fParConfig.GetValue<int>("NumInputs"));
    sampler.SetProperty(EsrSidsSampler<TLoader>::NumOutputs, fParConfig.GetValue<int>("NumOutputs"));

    sampler.ChangeState(EsrSidsSampler<TLoader>::INIT);

    // INPUT: 0 - command
    // sampler.SetProperty(EsrSidsSampler::InputSocketType, ZMQ_SUB, 0);
    // sampler.SetProperty(EsrSidsSampler::InputRcvBufSize, 1000, 0);
    // sampler.SetProperty(EsrSidsSampler::InputAddress, "tcp://localhost:5560", 0);

    // OUTPUT: 0 - data
    sampler.SetProperty(EsrSidsSampler<TLoader>::OutputSocketType, fParConfig.GetValue<string>("OutputSocketType").c_str(), 0);
    sampler.SetProperty(EsrSidsSampler<TLoader>::OutputSndBufSize, fParConfig.GetValue<int>("OutputSndBufSize"), 0);
    sampler.SetProperty(EsrSidsSampler<TLoader>::OutputMethod, fParConfig.GetValue<string>("OutputMethod").c_str(), 0);
    sampler.SetProperty(EsrSidsSampler<TLoader>::OutputAddress, fParConfig.GetValue<string>("OutputAddress").c_str(), 0);

    
    
    // OUTPUT: 1 - logger
    // sampler.SetProperty(EsrSidsSampler::OutputSocketType, ZMQ_PUB, 1);
    // sampler.SetProperty(EsrSidsSampler::OutputSndBufSize, 1000, 1);
    // sampler.SetProperty(EsrSidsSampler::OutputAddress, "tcp://*:5561", 1);

    sampler.ChangeState(EsrSidsSampler<TLoader>::SETOUTPUT);
    sampler.ChangeState(EsrSidsSampler<TLoader>::SETINPUT);
    
    
    //sampler.ChangeState(EsrSidsSampler<TLoader>::RUN);

    /*boost::unique_lock<boost::mutex> lock(sampler.fRunningMutex);
    while (!sampler.fRunningFinished)
    {
        sampler.fRunningCondition.wait(lock);
    }*/
    
    //*
    try
    {
        sampler.ChangeState(EsrSidsSampler<TLoader>::RUN);
        
    }
    catch (boost::archive::archive_exception& e)
    {
        std::cout << e.what();
    }
    //*/

    sampler.ChangeState(EsrSidsSampler<TLoader>::STOP);
    sampler.ChangeState(EsrSidsSampler<TLoader>::END);
    
#endif //__CINT__
}

#ifndef __CINT__
/*
typedef boost::archive::binary_oarchive TBoostBinPayloadOut; // boost binary format
typedef EsrSamplerTask<TBoostBinPayloadOut> TLoader;

static void s_signal_handler(int signal, EsrSidsSampler<TLoader> sampler)
{
    cout << endl << "Caught signal " << signal << endl;

    sampler.ChangeState(EsrSidsSampler<TLoader>::STOP);
    sampler.ChangeState(EsrSidsSampler<TLoader>::END);

    cout << "Shutdown complete. Bye!" << endl;
    exit(1);
}

static void s_catch_signals(void)
{
    struct sigaction action;
    action.sa_handler = s_signal_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
}
//*/
#endif //__CINT__



ClassImp(SidsGui)
        
        
        