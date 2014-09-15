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
        fGraph(NULL), fHist1D(NULL), fHist2D(NULL), fControlFrame(NULL),
        fSidsHisto(NULL), fFileName(Filename.c_str()), fFileInfo(), 
        fDecayData() ,fDecayCounter(0),fHisto1DCounter(0), fReadyToSend(false), fSampler(false),
        fSamplerConfig(SamplerConfig)

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
   
   //fCanvas1->Divide(1, 2);
   //fCanvas1->SetBorderMode(0);
   fCanvas1->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)","SidsGui",this,
               "EventInfo(Int_t,Int_t,Int_t,TObject*)");
   
   /// example tree (start))
   fBaseLTI = fListTree->AddItem(0, "Base");
   
   gRootObj  = gVirtualX->InternAtom("application/root", kFALSE);

   TGraph *gr = (TGraph *)GetObject("Graph");
   pic = gClient->GetPicture("f1_t.xpm");
   item = fListTree->AddItem(fBaseLTI, gr->GetName(), gr, pic, pic);
   fListTree->SetToolTipItem(item, "Simple Graph");
   item->SetDNDSource(kTRUE);

   TH1F *hpx = (TH1F *)GetObject("1D Hist");
   pic = gClient->GetPicture("h1_t.xpm");
   item = fListTree->AddItem(fBaseLTI, hpx->GetName(), hpx, pic, pic);
   fListTree->SetToolTipItem(item, "1D Histogram");
   item->SetDNDSource(kTRUE);

   TH2F *h2 = (TH2F *)GetObject("2D Hist");
   pic = gClient->GetPicture("h2_t.xpm");
   h2->SetDrawOption("ZCOL");
   item = fListTree->AddItem(fBaseLTI, h2->GetName(), h2, pic, pic);
   fListTree->SetToolTipItem(item, "2D Histogram");
   item->SetDNDSource(kTRUE);

   TString rootsys(gSystem->UnixPathName(gSystem->Getenv("ROOTSYS")));
#ifdef G__WIN32
   // remove the drive letter (e.g. "C:/") from $ROOTSYS, if any
   if (rootsys[1] == ':' && rootsys[2] == '/')
      rootsys.Remove(0, 3);
#endif
   //TString link = TString::Format("/%s/tutorials/image/rose512.jpg", rootsys.Data());
   TString link ="/Users/winckler/Pictures/Bosch.jpg";
   if (!gSystem->AccessPathName(link.Data(), kReadPermission)) 
   {
        TImage *img = TImage::Open(link.Data());
        if (img) 
        {
            // create a 16x16 icon from the original picture
            img->Scale(16, 16);
            pic = gClient->GetPicturePool()->GetPicture("Fritz", img->GetPixmap(),
                                                        img->GetMask());
            delete img;
        }
        else pic = gClient->GetPicture("psp_t.xpm");
        link.Prepend("file://");
        TObjString *ostr = new TObjString(link.Data());
        item = fListTree->AddItem(fBaseLTI, "Fritz", ostr, pic, pic);
        fListTree->SetToolTipItem(item, link.Data());
        item->SetDNDSource(kTRUE);
   }

   // open the base list tree item and allow to drop into it
   fListTree->OpenItem(fBaseLTI);
   fListTree->GetFirstItem()->SetDNDTarget(kTRUE);

   // connect the DataDropped signal to be able to handle it
   fListTree->Connect("DataDropped(TGListTreeItem*, TDNDData*)", "SidsGui",
                      this, "DataDropped(TGListTreeItem*,TDNDData*)");

   
   ////////////////////////////////////////////////////// 1st layer 2nd col
   ///
  
   
   
   fEc2 = new TRootEmbeddedCanvas ("AnalysisCanvas",hfrm,600,400);
   //fEc2->GetCanvas()->EditorBar();
   TVirtualPadEditor::ShowEditor();
   fCanvas2 = fEc2->GetCanvas();
   fCanvas2->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)","SidsGui",this,
               "EventInfo(Int_t,Int_t,Int_t,TObject*)");
   
   
   fCanvas2->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)","SidsGui",this,
               "DoDoubleClick(Int_t,Int_t,Int_t,TObject*)");
   
   /*
   fEditorFrame = new TGCompositeFrame(fEc2, 175, fEc2->GetHeight()+4, kFixedWidth); 
   fEc2->AddFrame(fEditorFrame, new TGLayoutHints( kLHintsNormal|kLHintsExpandY, 0, 0, 0, 0 ));
   CreateEditor();
   fEditor->Show();
   fEc2->ShowFrame(fEditorFrame);
   */
   //hfrm2->AddFrame(canvas2, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5, 5));
   
   
   hfrm->AddFrame(fEc2, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   
   fControlFrame = new TGVerticalFrame(hfrm, 200, 400);
   
   /// QUALITY BUTTON
   
   fFileQualityTag = new TGNumberEntry(fControlFrame,0,6,666,TGNumberFormat::kNESInteger);
   fControlFrame->AddFrame(fFileQualityTag, new TGLayoutHints( kLHintsExpandX,2,2,2,5));
   
   
   //TGLabel* QualityLabel = new TGLabel(fControlFrame, "Quality Tag");
   
   
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
   

    fFileName=fSamplerConfig.GetInputFile().c_str();
    TFile* rootfile0 = new TFile(fFileName);
    SeekHisto(rootfile0);
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
}

//______________________________________________________________________________
void SidsGui::DoCloseWindow()
{
   // Do some cleanup, disconnect signals and then really close the main window.

   if (fGraph) { delete fGraph; fGraph = 0; }
   if (fHist1D) { delete fHist1D; fHist1D = 0; }
   if (fHist2D) { delete fHist2D; fHist2D = 0; }
   if (fSidsHisto) { delete fSidsHisto; fSidsHisto = 0; }
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
   fHisto1DCounter=0;
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
         TH2D* h2dObj;
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
}

//______________________________________________________________________________
TObject *SidsGui::GetObject(const char *obj)
{
   // Return the object specified in argument. If the object doesn't exist yet,
   // it is firt created.

   if (!strcmp(obj, "Graph")) {
      if (fGraph == 0) {
         const Int_t n = 20;
         Double_t x[n], y[n];
         for (Int_t i=0;i<n;i++) {
           x[i] = i*0.1;
           y[i] = 10*sin(x[i]+0.2);
         }
         fGraph = new TGraph(n, x, y);
      }
      return fGraph;
   }
   else if (!strcmp(obj, "1D Hist")) {
      if (fHist1D == 0) {
         fHist1D = new TH1F("1D Hist","This is the px distribution",100,-4,4);
         Float_t px, py;
         for ( Int_t i=0; i<10000; i++) {
            gRandom->Rannor(px, py);
            fHist1D->Fill(px);
         }
      }
      return fHist1D;
   }
   else if (!strcmp(obj, "2D Hist")) {
      if (fHist2D == 0) {
         Double_t params[] = {
            130,-1.4,1.8,1.5,1, 150,2,0.5,-2,0.5, 3600,-2,0.7,-3,0.3
         };
         TF2 *f2 = new TF2("f2","xygaus + xygaus(5) + xylandau(10)",
                           -4, 4, -4, 4);
         f2->SetParameters(params);
         fHist2D = new TH2F("2D Hist","xygaus+xygaus(5)+xylandau(10)",
                            20, -4, 4, 20, -4, 4);
         fHist2D->FillRandom("f2",40000);
         fHist2D->SetDrawOption("ZCOL");
      }
      return fHist2D;
   }
   return 0;
}
void SidsGui::OpenRootFile()//TGFileInfo fileInfo)
{
    static TString dir(".");
   fFileInfo.fFileTypes = dnd_types;
   fFileInfo.fIniDir    = StrDup(dir);
    new TGFileDialog(gClient->GetRoot(), this, kFDOpen, &fFileInfo);
    dir = fFileInfo.fIniDir;
    fFileName=fFileInfo.fFilename;
    std::cout<<"Opening  ="<<fFileName <<std::endl;
    //std::cout<<boost::filesystem::basename(fFileName.Data())<<std::endl;
    
    TFile* rootfile = new TFile(fFileName);
    
    SeekHisto(rootfile);
    //h_iqt_spectrum
    //TH2D* fRooData= (TH2D*) rootfile->Get(DataName.c_str());
}



//______________________________________________________________________________
void SidsGui::HandleMenu(Int_t menu_id)
{
   // Handle menu events.

   TRootHelpDialog *hd;
   

   switch (menu_id) {
      case M_FILE_EXIT:
         // close the window and quit application
         DoCloseWindow();
         gApplication->Terminate(0);
         break;
      case M_FILE_OPEN:
      {
         
         OpenRootFile();
         //std::cout<<"dir="<<dir<<std::endl;
         //std::cout<<"file="<<fi.fFilename <<std::endl;
         // doesn't do much, but can be used to open a root file...
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


int SidsGui::SeekHisto(TFile* rootfile)
{

    if (!rootfile->IsOpen()) 
    {
      std::cout<<"Cannot open file "<<fFileName.Data()<<std::endl;
      return 0;
    }

    TList* list = rootfile->GetListOfKeys() ;
    if (!list) 
    { 
          std::cout<<"No key found in file "<<fFileName.Data()<<std::endl;
          return 0;
    }
    
    fBaseLTI = fListTree->AddItem(0, (boost::filesystem::basename(fFileName.Data())).c_str());
    TIter next(list) ;
    TKey* key ;
    TObject* obj ;

    const TGPicture *pic = 0;
    TGListTreeItem *item;
    
    while ( key = (TKey*)next() ) 
    {
        obj = key->ReadObj();
        if ((strcmp(obj->IsA()->GetName(),"TProfile")!=0)
                && (!obj->InheritsFrom("TH2"))
                && (!obj->InheritsFrom("TH1"))  ) 
        {
            printf("<W> Object %s is not 1D or 2D histogram : "
               "will not be converted\n",obj->GetName()) ;
        }
        printf("Histo name:%s title:%s\n",obj->GetName(),obj->GetTitle());
        
        //TH2 *h2 = (TH2F *)GetObject("2D Hist");
        if(obj->InheritsFrom("TH2D"))
        {
            //TH2D* th2obj;
            
            rootfile->GetObject(obj->GetName(),fSidsHisto);
            pic = gClient->GetPicture("h2_t.xpm");
            item = fListTree->AddItem(fBaseLTI, fSidsHisto->GetName(), fSidsHisto, pic, pic);
            fListTree->SetToolTipItem(item, "2D Histogram");
            item->SetDNDSource(kTRUE);
        }
        if(obj->InheritsFrom("TH1D"))
        {
            //TH2D* th2obj;
            TH1D* histo1D=NULL;
            rootfile->GetObject(obj->GetName(),histo1D);
            
            if(histo1D) 
            {
                f1DHisto.push_back(new TH1D(*histo1D));
                pic = gClient->GetPicture("h1_t.xpm");
                item = fListTree->AddItem(fBaseLTI, f1DHisto[fHisto1DCounter]->GetName(), f1DHisto[fHisto1DCounter], pic, pic);
                fListTree->SetToolTipItem(item, "1D Histogram");
                item->SetDNDSource(kTRUE);
                fHisto1DCounter++;
            }
        }
        
        
    }
    
    return 1;
}

void SidsGui::DoDraw() 
{
    //fEc2->GetCanvas()->EditorBar();
    //TVirtualPadEditor::ShowEditor();
    //fCanvas2 = fEc2->GetCanvas();
    
    //Int_t wid = fEc2->GetCanvasWindowId();
    //fCanvas2 = new TCanvas("MyCanvas", 10,10,wid);
    //fEc2->AdoptCanvas(fCanvas2);
    
    if(fSidsHisto)
    {
        Int_t binMax=fSidsHisto->ProjectionX()->GetMaximumBin();
        Double_t Xmin=fSidsHisto->GetXaxis()->GetBinCenter(binMax-175);
        Double_t Xmax=fSidsHisto->GetXaxis()->GetBinCenter(binMax+175);
        //std::cout<<"Xmin : "<< Xmin<<std::endl;
        //std::cout<<"Xax : "<< Xmax<<std::endl;
        fSidsHisto->GetXaxis()->SetRangeUser(Xmin,Xmax);
        TH2D* histo=dynamic_cast<TH2D*>(fSidsHisto->RebinX(2,"SchottkySpectrum"));
        if(histo)
        {
            //histo->SetMaximum(5.e-7);
            histo->Draw("zcol");
        }
    }
        
    
    
    fCanvas2->cd();
    fCanvas2->Update();
    
    
}




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

void SidsGui::EventInfo(Int_t event, Int_t px, Int_t py, TObject *selected)
{
   const char *text0, *text1, *text3;
   char text2[50];
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


void SidsGui::DoExit()
{
    TVirtualPadEditor::Terminate();
    gApplication->Terminate(0);
}

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


void SidsGui::DoValidate()
{
    //std::cout<<"Decay Rev. Freq is : "  <<fDecayFreq[0]<<std::endl;
    //std::cout<<"Decay Time is : "       <<fDecayTime[0]<<std::endl;
    
    
    EsrInjData DecayData(fFileName.Data(),SIDS::kRSA1,0.032,31.25);
    DecayData.SetUserName(fSamplerConfig.GetUserName());
    
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

    string outputfilename=fSamplerConfig.GetOutputFile();
    string treename=fSamplerConfig.GetTreeName();
    string branchname=fSamplerConfig.GetBranch();
    EsrTree DecayTree(outputfilename,treename,branchname);
    DecayTree.UpdateTree(DecayData);
    //fReadyToSend=true;
    //fSampler=true;
    if(fReadyToSend && fSampler)
        StartSampler();
}




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


    sampler.SetProperty(EsrSidsSampler<TLoader>::Id, fSamplerConfig.GetID().c_str());
    sampler.SetProperty(EsrSidsSampler<TLoader>::InputFile, fSamplerConfig.GetInputFile().c_str());
    sampler.SetProperty(EsrSidsSampler<TLoader>::EventRate, fSamplerConfig.GetEventRate());
    sampler.SetProperty(EsrSidsSampler<TLoader>::NumIoThreads, fSamplerConfig.GetNumIoThreads());

    sampler.SetProperty(EsrSidsSampler<TLoader>::NumInputs, fSamplerConfig.GetNumInputs());
    sampler.SetProperty(EsrSidsSampler<TLoader>::NumOutputs, fSamplerConfig.GetNumOutputs());

    sampler.ChangeState(EsrSidsSampler<TLoader>::INIT);

    // INPUT: 0 - command
    // sampler.SetProperty(EsrSidsSampler::InputSocketType, ZMQ_SUB, 0);
    // sampler.SetProperty(EsrSidsSampler::InputRcvBufSize, 1000, 0);
    // sampler.SetProperty(EsrSidsSampler::InputAddress, "tcp://localhost:5560", 0);

    // OUTPUT: 0 - data
    sampler.SetProperty(EsrSidsSampler<TLoader>::OutputSocketType, fSamplerConfig.GetOutputSocketType().c_str(), 0);
    sampler.SetProperty(EsrSidsSampler<TLoader>::OutputSndBufSize, fSamplerConfig.GetOutputSndBufSize(), 0);
    sampler.SetProperty(EsrSidsSampler<TLoader>::OutputMethod, fSamplerConfig.GetOutputMethod().c_str(), 0);
    sampler.SetProperty(EsrSidsSampler<TLoader>::OutputAddress, fSamplerConfig.GetOutputAddress().c_str(), 0);

    
    
    // OUTPUT: 1 - logger
    // sampler.SetProperty(EsrSidsSampler::OutputSocketType, ZMQ_PUB, 1);
    // sampler.SetProperty(EsrSidsSampler::OutputSndBufSize, 1000, 1);
    // sampler.SetProperty(EsrSidsSampler::OutputAddress, "tcp://*:5561", 1);

    sampler.ChangeState(EsrSidsSampler<TLoader>::SETOUTPUT);
    sampler.ChangeState(EsrSidsSampler<TLoader>::SETINPUT);
    
    
    //sampler.ChangeState(EsrSidsSampler<TLoader>::RUN);

    
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

#endif //__CINT__



ClassImp(SidsGui)
        
        
        