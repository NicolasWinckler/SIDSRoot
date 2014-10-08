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
        fParConfig(SamplerConfig), fInputFile(NULL), fBinning(100),fFitID(kUnbinnedLikelihood),
        fNormFactframe(NULL),
        fBinningframe(NULL),
        fObsframe(NULL),
        flambdaframe(NULL),
        fampframe(NULL),
        fOmegaframe(NULL),
        fPhiframe(NULL),
        fDecayCounter(0), fHisto1DCounter(0), fHisto2DCounter(0), fHeaderCounter(0),
        fReadyToSend(false), fSampler(false),
        fx_min(0.), fx_max(0.),
        fNormFactInit(0.),
        fNormFact_Max(0.),
        fNormFact_Min(0.),
        flambdaInit(0.), flambda_Max(0.), flambda_Min(0.),
        fampInit(0.), famp_Max(0.), famp_Min(0.),
        fOmegaInit(0.), fOmega_Max(0.), fOmega_Min(0.),
        fPhiInit(0.), fPhi_Max(0.), fPhi_Min(0.), fFigureName(),
        fx(NULL), flambda0(NULL),
        flambda1(NULL), famp1(NULL), fomega1(NULL), fphi1(NULL),
        fpdfH0(NULL), fpdfH1(NULL), fChi2pdfH0(NULL), fChi2pdfH1(NULL),fECdata(NULL), 
        fFitResultH0(NULL), fFitResultH1(NULL), fexp1(NULL), fosc1(NULL)

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
   combo->AddEntry("Unbinned Likelihood fit", kUnbinnedLikelihood);
   combo->AddEntry("Binned Likelihood fit", kBinnedLikelihood);
   combo->AddEntry("Unbinned Likelihood Ratio Profile", kPNLL);
   combo->AddEntry("Binned Chi2 fit", kChi2);
   combo->AddEntry("Binned Data only (No Fit)", kNoFit);
   //combo->Connect("Selected(Int_t)", "SidsSummary", this, "ChangeMode(Int_t)");
   //combo->Connect("Selected(Int_t)", "SidsSummary", this, "DoDraw(Int_t)");
   combo->Connect("Selected(Int_t)", "SidsSummary", this, "DoFitUpdate(Int_t)");
   //combo->Select(SIDS::kRSA51);
   hfrmDrawNBox->AddFrame(combo, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
   combo->Resize(100, 20);
   // */
   fControlFrame->AddFrame(hfrmDrawNBox, new TGLayoutHints(kLHintsExpandX,2,2,2,5));
   
   /// ADD PAR fields
   fObsframe = new SidsFitButtons(fControlFrame,3,"Time","Min : ","Max : ", "Binning : ");
   fControlFrame->AddFrame(fObsframe, new TGLayoutHints(kLHintsLeft,2,2,2,5));
   
   fObsframe->SetEntry1(fx_min);
   fObsframe->SetEntry2(fx_max);
   fObsframe->SetEntry3((double)fBinning);
   
   /// ADD PAR fields
   fNormFactframe = new SidsFitButtons(fControlFrame,3,"Normalization Factor","Min : ","Max : ", "Init : ");
   fControlFrame->AddFrame(fNormFactframe, new TGLayoutHints(kLHintsExpandX,2,2,2,5));
   
   fNormFactframe->SetEntry1(fNormFact_Min);
   fNormFactframe->SetEntry2(fNormFact_Max);
   fNormFactframe->SetEntry3(fNormFactInit);
   
   
   /// ADD PAR fields
   flambdaframe = new SidsFitButtons(fControlFrame,3,"Lambda","Min : ","Max : ", "Init : ");
   fControlFrame->AddFrame(flambdaframe, new TGLayoutHints(kLHintsExpandX,2,2,2,5));
   
   flambdaframe->SetEntry1(flambda_Min);
   flambdaframe->SetEntry2(flambda_Max);
   flambdaframe->SetEntry3(flambdaInit);
   
   /// ADD PAR fields
   fampframe = new SidsFitButtons(fControlFrame,3,"amplitude","Min : ","Max : ", "Init : ");
   fControlFrame->AddFrame(fampframe, new TGLayoutHints(kLHintsExpandX,2,2,2,5));
   
   fampframe->SetEntry1(famp_Min);
   fampframe->SetEntry2(famp_Max);
   fampframe->SetEntry3(fampInit);
   
   /// ADD PAR fields
   fOmegaframe = new SidsFitButtons(fControlFrame,3,"Omega","Min : ","Max : ", "Init : ");
   fControlFrame->AddFrame(fOmegaframe, new TGLayoutHints(kLHintsExpandX,2,2,2,5));
   
   fOmegaframe->SetEntry1(fOmega_Min);
   fOmegaframe->SetEntry2(fOmega_Max);
   fOmegaframe->SetEntry3(fOmegaInit);
   
   /// ADD PAR fields
   fPhiframe = new SidsFitButtons(fControlFrame,3,"Phi","Min : ","Max : ", "Init : ");
   fControlFrame->AddFrame(fPhiframe, new TGLayoutHints(kLHintsExpandX,2,2,2,5));
   
   fPhiframe->SetEntry1(fPhi_Min);
   fPhiframe->SetEntry2(fPhi_Max);
   fPhiframe->SetEntry3(fPhiInit);
   
   TGTextButton *draw = new TGTextButton(fControlFrame,"&Update Fit");
   draw->Connect("Clicked()","SidsSummary",this,"DoFitUpdate()");
   
   
   fControlFrame->AddFrame(draw, new TGLayoutHints(kLHintsCenterX,2,2,2,5));
   
   
   TGTextButton *SaveFig = new TGTextButton(fControlFrame,"&Save main Figure");
   SaveFig->Connect("Clicked()","SidsSummary",this,"DoSaveFigure()");
   fControlFrame->AddFrame(SaveFig, new TGLayoutHints(kLHintsCenterX,2,2,2,5));
   
   
   hfrm->AddFrame(fControlFrame, new TGLayoutHints(kLHintsRight | kLHintsExpandY));
   AddFrame(hfrm, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   
   
   
   ////////////////////////////////////////////////////// 2nd layer
   //TGHorizontalFrame *hf3 = new TGHorizontalFrame(this, 10, 10);
   TGHorizontalFrame *hf3 = new TGHorizontalFrame(this, 10, 10);
   //hf3 = new TGVerticalFrame(this, 0, 0, 0);

   combo->Select(kUnbinnedLikelihood);
   //combo->Select(kChi2);
   
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
    fBinning=fParConfig.GetValue<int>("Binning");
    
    fx_min=fParConfig.GetValue<double>("x_min");
    fx_max=fParConfig.GetValue<double>("x_max");
    
    fNormFactInit=fParConfig.GetValue<double>("NormFactInit");
    fNormFact_Max=fParConfig.GetValue<double>("NormFact_Max");
    fNormFact_Min=fParConfig.GetValue<double>("NormFact_Min");
    
    flambdaInit=fParConfig.GetValue<double>("LambdaInit");
    flambda_Max=fParConfig.GetValue<double>("Lambda_Max");
    flambda_Min=fParConfig.GetValue<double>("Lambda_Min");
    
    fampInit=fParConfig.GetValue<double>("AmpInit");
    famp_Max=fParConfig.GetValue<double>("Amp_Max");
    famp_Min=fParConfig.GetValue<double>("Amp_Min");
    
    fOmegaInit=fParConfig.GetValue<double>("OmegaInit");
    fOmega_Max=fParConfig.GetValue<double>("Omega_Max");
    fOmega_Min=fParConfig.GetValue<double>("Omega_Min");
    
    fPhiInit=fParConfig.GetValue<double>("PhiInit");
    fPhi_Max=fParConfig.GetValue<double>("Phi_Max");
    fPhi_Min=fParConfig.GetValue<double>("Phi_Min");
    
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
    
    
    // search for index of duplicates and fill non duplicates first
    for(unsigned int k(0);k<fDataList.size() ;k++)
    {
        string temp=fDataList[k].GetFileName();
        if(duplicates.count(temp))
        {
            fDuplicatesIdx[temp].push_back(k);
        }
        else
        {
            fDataToPlotIdx[temp]=k;
        }
    }
    
    
    // count number of duplicates and add to main list the wanted index (here last added duplicates)
    int sumduplicate=0;
    for(auto p : fDuplicatesIdx)
    {
        sumduplicate+=p.second.size();
        fDataToPlotIdx[p.first]=p.second.back();
    }
    
    /*
    for(auto p : fDuplicatesIdx)
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
    
    
    
    
    
    fx = new RooRealVar("x","x",tmin,tmax);
    flambda0 = new RooRealVar("#lambda_{0}","#lambda_{0}",flambdaInit,flambda_Min,flambda_Max);
    fNorm0 = new RooRealVar("N_{0}","N_{0}",fNormFactInit,fNormFact_Min,fNormFact_Max);

    //Define alt parameters
    flambda1 = new RooRealVar("#lambda_{1}","#lambda_{1}",flambdaInit,flambda_Min,flambda_Max);
    fNorm1 = new RooRealVar("N_{1}","N_{1}",fNormFactInit,fNormFact_Min,fNormFact_Max);
    famp1 = new RooRealVar("a_{1}","a_{1}",fampInit,famp_Min,famp_Max);
    fomega1 = new RooRealVar("#omega_{1}","#omega_{1}",fOmegaInit,fOmega_Min,fOmega_Max);
    fphi1 = new RooRealVar("#phi_{1}","#phi_{1}",fPhiInit,fPhi_Min,fPhi_Max);
    
    fpdfH0 = new RooGenericPdf("H0","#lambda_{0}*exp(-#lambda_{0}*x)",RooArgSet(*fx,*flambda0));
    fpdfH1 = new SidsRooOscModel("H1","H1",*fx,*flambda1,*famp1,*fomega1,*fphi1);
    
    fChi2pdfH0 = new RooGenericPdf("Chi2H0","N_{0}*exp(-#lambda_{0}*x)",RooArgSet(*fx,*fNorm0,*flambda0));
    //fChi2pdfH1 = new RooGenericPdf("Chi2H1","N_{1}*(1+a_{1}*cos(#omega_{1}*x+#phi{1}))exp(-#lambda_{1}*x)",RooArgSet(*fx,*fNorm1,*flambda1,*famp1,*fomega1,*fphi1));
    fChi2pdfH1 = new RooGenericPdf("Chi2H1","N_{1}*#lambda_{1}*(#lambda_{1}*#lambda_{1}+#omega_{1}*#omega_{1})/(#lambda_{1}*#lambda_{1}*(1+a_{1})+#omega_{1}*#omega_{1})*(1+a_{1}*cos(#omega_{1}*x+#phi_{1}))*exp(-#lambda_{1}*x)"
		,RooArgSet(*fx,*fNorm1,*flambda1,*famp1,*fomega1,*fphi1));
    
    fECdata= new RooDataSet("DataSet","DataSet",*fx);
    
    
    
    
    
    int histIdx=0;
    for(auto p : fDataToPlotIdx)// loop chronological (files sorted)  including last duplicates only
    {
        int TreeIndex=p.second;
        int NumbEC=fDataList[TreeIndex].GetNEC();
        float freq=fDataList[TreeIndex].GetCoolParentFreq();
        double f_Dcool=(double)freq+1584.;
        vector<EsrDecayEvent> ECDecayEvents=fDataList[TreeIndex].GetECData();
        
        // set hist content for each file
        fNEC->SetBinContent(histIdx+1,NumbEC);
        fPfreq->SetBinContent(histIdx+1,freq);
        fPfreq->GetYaxis()->SetRangeUser(244.90e+6,244.93e+6);
        for(unsigned int j(0);j<ECDecayEvents.size();j++)
        {
            double t_ec=(double)ECDecayEvents[j].GetDecayTime();
            double f_ec=(double)ECDecayEvents[j].GetDecayFreq()-f_Dcool;
            fECDecayTimes->Fill(t_ec);
            fECFreq->Fill(f_ec);
            *fx=t_ec;
            
            //if(t_ec>fx_min && t_ec<fx_max)
            fECdata->add(*fx);
        }
        histIdx++;
    }
    
}


void SidsSummary::DoFitUpdate(Int_t BoxID)
{
    fFitID=BoxID;
    UpdateFitPar();
    UpdateHistData(fx_min, fx_max);
    ReInitRooFitPar();
    DoDraw(fFitID);
}


void SidsSummary::DoFitUpdate()
{
    UpdateFitPar();
    UpdateHistData(fx_min, fx_max);
    ReInitRooFitPar();
    DoDraw(fFitID);
}

void SidsSummary::UpdateFitPar()
{
   fx_min=fObsframe->GetVal1();
   fx_max=fObsframe->GetVal2();
   fBinning=(int)fObsframe->GetVal3();
   
   flambda_Min=flambdaframe->GetVal1();
   flambda_Max=flambdaframe->GetVal2();
   flambdaInit=flambdaframe->GetVal3();
   
   fNormFact_Min=fNormFactframe->GetVal1();
   fNormFact_Max=fNormFactframe->GetVal2();
   fNormFactInit=fNormFactframe->GetVal3();
   
   famp_Min=fampframe->GetVal1();
   famp_Max=fampframe->GetVal2();
   fampInit=fampframe->GetVal3();
   
   fOmega_Min=fOmegaframe->GetVal1();
   fOmega_Max=fOmegaframe->GetVal2();
   fOmegaInit=fOmegaframe->GetVal3();
   
   fPhi_Min=fPhiframe->GetVal1();
   fPhi_Max=fPhiframe->GetVal2();
   fPhiInit=fPhiframe->GetVal3();
}

void SidsSummary::UpdateHistData(double tmin, double tmax)
{
    fECDecayTimes->Reset();
    fECFreq->Reset();
    fECDecayTimes->SetBins(fBinning,tmin,tmax);
    
    //int histIdx=0;
    for(auto p : fDataToPlotIdx)// loop chronological (files sorted)  including last duplicates only
    {
        int TreeIndex=p.second;
        float freq=fDataList[TreeIndex].GetCoolParentFreq();
        double f_Dcool=(double)freq+1584.;
        vector<EsrDecayEvent> ECDecayEvents=fDataList[TreeIndex].GetECData();
        
        
        for(unsigned int j(0);j<ECDecayEvents.size();j++)
        {
            double t_ec=(double)ECDecayEvents[j].GetDecayTime();
            double f_ec=(double)ECDecayEvents[j].GetDecayFreq()-f_Dcool;
            
            if(t_ec>tmin && t_ec<tmax)
            {
                fECDecayTimes->Fill(t_ec);
                fECFreq->Fill(f_ec);
            }
        }
        //histIdx++;
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
void SidsSummary::DoDraw(Int_t BoxID) 
{
    fCanvas2->SetCrosshair();
    fCanvas2->cd();
    
    switch(BoxID)
    {
        case kUnbinnedLikelihood :
        {
            MQLOG(INFO)<<"*********************************************";
            MQLOG(INFO)<<"************* Unbinned Likelihood Fit method ***************"<<endl;
            UnbinnedLikelihoodFit(true);
            break;
        }
        
        case kBinnedLikelihood :
        {
            MQLOG(ERROR)<<"Function not implemented yet";
            break;
        }
        
        case kChi2 :
        {
            MQLOG(INFO)<<"*********************************************";
            MQLOG(INFO)<<"************* Chi2 Fit method ***************"<<endl;
            DoChi2Fit(true);
            break;
        }
    
        case kPNLL :
        {
            MQLOG(INFO)<<"*********************************************";
            MQLOG(INFO)<<"********** Negative Likelihood Ratio Profile method ************"<<endl;
            DoPNLL(true);
            break;
        }
        
        case kNoFit :
        {
            MQLOG(INFO)<<"*********************************************";
            MQLOG(INFO)<<"********** Plot Data only ************"<<endl;
            DoNothing(true);// :)
            break;
        }
    }
    
    //fECDecayTimes->Draw();
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
    
    MQLOG(INFO)<<"Number of analyzed EC-decay in range : "<<fECDecayTimes->GetSum();
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



void SidsSummary::ReInitRooFitPar()
{
    if(fx_max<fx_min)
    {
        double temp=fx_min;
        fx_min=fx_max;
        fx_max=temp;
    }

    /// OBSERVABLE
    if(fx)
        fx->setRange(fx_min,fx_max);
    
    
    // M0 & M1 PARAMETERS
    if(flambda0)
    {
        flambda0->setVal(flambdaInit);
        flambda0->setRange(flambda_Min,flambda_Max);
    }
    
    if(flambda1)
    {
        flambda1->setVal(flambdaInit);
        flambda1->setRange(flambda_Min,flambda_Max);
    }
    
    if(famp1)
    {
        famp1->setVal(fampInit);
        famp1->setRange(famp_Min,famp_Max);
    }
    
    
    if(fomega1)
    {
        fomega1->setVal(fOmegaInit);
        fomega1->setRange(fOmega_Min,fOmega_Max);
    }
    
    
    if(fphi1)
    {
        fphi1->setVal(fPhiInit);
        fphi1->setRange(fPhi_Min,fPhi_Max);
    }
    
    /// PDF
    if(fpdfH0)
    {
        delete fpdfH0;
        fpdfH0=NULL;
        fpdfH0 = new RooGenericPdf("H0","#lambda_{0}*exp(-#lambda_{0}*x)",RooArgSet(*fx,*flambda0));
    }
    else
        fpdfH0 = new RooGenericPdf("H0","#lambda_{0}*exp(-#lambda_{0}*x)",RooArgSet(*fx,*flambda0));

    if(fpdfH1)
    {
        delete fpdfH1;
        fpdfH1=NULL;
        fpdfH1 = new SidsRooOscModel("H1","H1",*fx,*flambda1,*famp1,*fomega1,*fphi1);
    }
    else{
        fpdfH1 = new SidsRooOscModel("H1","H1",*fx,*flambda1,*famp1,*fomega1,*fphi1);}
}

void SidsSummary::UnbinnedLikelihoodFit(bool Draw)
{
    
    /// //               error band option description
    /// //        RooFit::VisualizeError(*r,1),FillColor(kOrange)
    // Visualize 1-sigma error encoded in fit result 'r' as orange band using linear error propagation
    // This results in an error band that is by construction symmetric
    //
    // The linear error is calculated as
    // error(x) = Z* F_a(x) * Corr(a,a') F_a'(x)
    //
    // where     F_a(x) = [ f(x,a+da) - f(x,a-da) ] / 2, 
    // 
    //         with f(x) = the plotted curve 
    //              'da' = error taken from the fit result
    //        Corr(a,a') = the correlation matrix from the fit result
    //                Z = requested significance 'Z sigma band'
    //
    // The linear method is fast (required 2*N evaluations of the curve, where N is the number of parameters), 
    // but may not be accurate in the presence of strong correlations (~>0.9) and at Z>2 due to linear and 
    // Gaussian approximations made


    /// //        RooFit::VisualizeError(*r,1,kFALSE),DrawOption("L"),LineWidth(2),LineColor(kRed)
    // Calculate error using sampling method and visualize as dashed red line. 
    //
    // In this method a number of curves is calculated with variations of the parameter values, as sampled 
    // from a multi-variate Gaussian p.d.f. that is constructed from the fit results covariance matrix. 
    // The error(x) is determined by calculating a central interval that capture N% of the variations 
    // for each valye of x, where N% is controlled by Z (i.e. Z=1 gives N=68%). The number of sampling curves 
    // is chosen to be such that at least 100 curves are expected to be outside the N% interval, and is minimally 
    // 100 (e.g. Z=1->Ncurve=356, Z=2->Ncurve=2156)) Intervals from the sampling method can be asymmetric, 
    // and may perform better in the presence of strong correlations, but may take (much) longer to calculate

    
    
    string strXmin;
    ostringstream ossTemp1;
    ossTemp1 << fx_min;
    strXmin = ossTemp1.str();

    string strXmax;
    ostringstream ossTemp2;
    ossTemp2 << fx_max;
    strXmax = ossTemp2.str();
    
    string CutRange="x>"+strXmin;
    CutRange+=" && x<";
    CutRange+=strXmax;
    ReInitRooFitPar();
    
    
    RooDataSet* ReducedDataSet = (RooDataSet*) fECdata->reduce(*fx,CutRange.c_str()) ;
    
    string strNEC;
    int NumbEC=(int)ReducedDataSet->sumEntries();
    ostringstream ossTemp3;
    ossTemp3 << NumbEC;
    strNEC = ossTemp3.str();

    string strBinning;
    ostringstream ossTemp4;
    ossTemp4 << fBinning;
    strBinning = ossTemp4.str();

    string Datatitle("Binned data and fitting functions in range [");
    Datatitle+=strXmin;
    Datatitle+=",";
    Datatitle+=strXmax;
    Datatitle+="] seconds (N_{EC}=";
    Datatitle+=strNEC;
    Datatitle+=", Binning=";
    Datatitle+=strBinning;
    Datatitle+=") ";
    
    
     
    
    if(Draw)
    {
        fFitResultH0=fpdfH0->fitTo(*ReducedDataSet,RooFit::Save());
        fFitResultH1=fpdfH1->fitTo(*ReducedDataSet,RooFit::Save());

        RooPlot* xframe0 = fx->frame(RooFit::Title(Datatitle.c_str()));
        ReducedDataSet->plotOn(xframe0,RooFit::Binning(fBinning));
        

        RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);
        fpdfH0->plotOn(xframe0,RooFit::VisualizeError(*fFitResultH0,1),RooFit::FillColor(kOrange));
        fpdfH1->plotOn(xframe0,RooFit::VisualizeError(*fFitResultH1,1),RooFit::FillColor(kOrange));
        fpdfH0->plotOn(xframe0,RooFit::VisualizeError(*fFitResultH0,1,kFALSE),RooFit::DrawOption("L"),RooFit::LineWidth(1),RooFit::LineColor(kRed),RooFit::LineStyle(kDashed));
        fpdfH1->plotOn(xframe0,RooFit::VisualizeError(*fFitResultH1,1,kFALSE),RooFit::DrawOption("L"),RooFit::LineWidth(1),RooFit::LineColor(kBlue),RooFit::LineStyle(kDashed));
        fpdfH0->plotOn(xframe0,RooFit::LineColor(kRed));
        fpdfH1->plotOn(xframe0,RooFit::LineColor(kBlue));
        RooMsgService::instance().setGlobalKillBelow(RooFit::INFO);
        
        xframe0->Draw();
   
    }
    
    fFigureName="Go2014_UnbinnedLikelihoodFit_Range_";
    fFigureName+=strXmin;
    fFigureName+="_";
    fFigureName+=strXmax;
    fFigureName+="_Binning_";
    fFigureName+=strBinning;
    fFigureName+=".pdf";
  }


void SidsSummary::DoSaveFigure()
{
    string dir=fParConfig.GetValue<string>("InputDirectory");
    dir+="/go2014figures/Results/";
    
    string figfile=dir+fFigureName;
    fCanvas2->SaveAs(figfile.c_str());
}

void SidsSummary::DoNothing(bool Draw)
{
    
    
    
    string strXmin;
    ostringstream ossTemp1;
    ossTemp1 << fx_min;
    strXmin = ossTemp1.str();

    string strXmax;
    ostringstream ossTemp2;
    ossTemp2 << fx_max;
    strXmax = ossTemp2.str();
    
    string CutRange="x>"+strXmin;
    CutRange+=" && x<";
    CutRange+=strXmax;
    ReInitRooFitPar();
    
    
    RooDataSet* ReducedDataSet = (RooDataSet*) fECdata->reduce(*fx,CutRange.c_str()) ; 
    
    string strNEC;
    int NumbEC=(int)ReducedDataSet->sumEntries();
    ostringstream ossTemp3;
    ossTemp3 << NumbEC;
    strNEC = ossTemp3.str();

    string strBinning;
    ostringstream ossTemp4;
    ossTemp4 << fBinning;
    strBinning = ossTemp4.str();

    string Datatitle("Binned data in range [");
    Datatitle+=strXmin;
    Datatitle+=",";
    Datatitle+=strXmax;
    Datatitle+="] seconds (N_{EC}=";
    Datatitle+=strNEC;
    Datatitle+=", Binning=";
    Datatitle+=strBinning;
    Datatitle+=")";
    RooPlot* xframe1 = fx->frame(RooFit::Title(Datatitle.c_str()));
    ReducedDataSet->plotOn(xframe1,RooFit::Binning(fBinning));
    xframe1->Draw();
    fFigureName="Go2014_DataPlot_Range_";
    fFigureName+=strXmin;
    fFigureName+="_";
    fFigureName+=strXmax;
    fFigureName+="_Binning_";
    fFigureName+=strBinning;
    fFigureName+=".pdf";
}

double SidsSummary::Chi2FitM1(double *t,double *par)
{
    	//p0=N, p1=Lambda_EC, p2=a, p3=w, p4=phi, p5=lambda_tot
	double fitval = par[0]*(1+par[1]*TMath::Cos(par[2]*t[0]+par[3]))*TMath::Exp(-par[4]*t[0]);
	return fitval;
}

double SidsSummary::Chi2FitM0(double *t,double *par)
{
    double fitval = par[0]*TMath::Exp(-par[1]*t[0]);
    return fitval;
}




void SidsSummary::DoPNLL(bool Draw)
{
    string strXmin;
    ostringstream ossTemp1;
    ossTemp1 << fx_min;
    strXmin = ossTemp1.str();

    string strXmax;
    ostringstream ossTemp2;
    ossTemp2 << fx_max;
    strXmax = ossTemp2.str();
    
    string CutRange="x>"+strXmin;
    CutRange+=" && x<";
    CutRange+=strXmax;
    ReInitRooFitPar();
    
    string strBinning;
    ostringstream ossTemp4;
    ossTemp4 << fBinning;
    strBinning = ossTemp4.str();
    
    
    RooDataSet* ReducedDataSet = (RooDataSet*) fECdata->reduce(*fx,CutRange.c_str()) ; 
    //fomega1->setVal(3.7);
    fpdfH1->fitTo(*ReducedDataSet);
    RooAbsReal* NLL= fpdfH1->createNLL(*ReducedDataSet);
    //double NegLogL=NLL->getValV();
    RooAbsReal *pnll = NLL->createProfile(*fomega1);

    // * Double_t findRoot(RooRealVar& x, Double_t xmin, Double_t xmax, Double_t yval)
    // * Return value of x (in range xmin,xmax) at which function equals yval.
    // * (Calculation is performed with Brent root finding algorithm)


    RooPlot* fr1 = fomega1->frame(Bins(fBinning), 
            //Range(mu-Ns*sigma,mu+Ns*sigma),
            Range(fOmega_Min,fOmega_Max),
            Title("Unbinned Likelihood Ratio Profile"));
    pnll->plotOn(fr1,LineColor(kBlue),ShiftToZero());
    //NLL->plotOn(fr1,ShiftToZero(),LineStyle(kDashed),LineColor(kBlue)) ;
    fr1->Draw();

    fFigureName="Go2014_LikelihoodRatioProfile_Range_";
    fFigureName+=strXmin;
    fFigureName+="_";
    fFigureName+=strXmax;
    fFigureName+="_Binning_";
    fFigureName+=strBinning;
    fFigureName+=".pdf";
}

void SidsSummary::DoChi2Fit(bool Draw)
{
    string Method("RooFit");
    
    
    if(Method=="ROOT")
    {
//*
        if(fexp1)
        {
            delete fexp1;
            fexp1=NULL;
            fexp1 = new TF1("fexp1",Chi2FitM0,fx_min,fx_max,2);
        }
        else 
            fexp1 = new TF1("fexp1",Chi2FitM0,fx_min,fx_max,2);	
        fexp1->SetParameters(fNormFactInit,flambdaInit);
        fexp1->SetParName(0,"N_0");
        fexp1->SetParName(1,"#lambda");

        if(fosc1)
        {
            delete fosc1;
            fosc1=NULL;
            fosc1 = new TF1("fosc1",Chi2FitM1,fx_min,fx_min,5);
        }
        else
            fosc1 = new TF1("fosc1",Chi2FitM1,fx_min,fx_min,5);
        fosc1->SetParameters(fNormFactInit,fampInit,fOmegaInit,fPhiInit,flambdaInit);	//p0=N,, p2=a, p3=w, p4=phi, p5=lambda_tot
        fosc1->SetParName(0,"N_0");
        fosc1->SetParName(1,"a");
        fosc1->SetParName(2,"#omega");
        fosc1->SetParName(3,"#phi");
        fosc1->SetParName(4,"#lambda");

        fexp1->SetRange(fx_min,fx_max);
        fexp1->SetParameters(fNormFactInit,flambdaInit);
        fexp1->SetParLimits(0,fNormFact_Min,fNormFact_Max);
        fexp1->SetParLimits(1,flambda_Min,flambda_Max); 

        fosc1->SetRange(fx_min,fx_max);
        fosc1->SetParameters(fNormFactInit,fampInit,fOmegaInit,fPhiInit,flambdaInit);
        fosc1->SetParLimits(0,fNormFact_Min,fNormFact_Max); 
        fosc1->SetParLimits(1,flambda_Min,flambda_Max); 
        fosc1->SetParLimits(2,famp_Min,famp_Max); 
        fosc1->SetParLimits(3,fOmega_Min,fOmega_Max); 
        fosc1->SetParLimits(4,fPhi_Min,fPhi_Max); 

        fECDecayTimes->Fit(fexp1, "RB","",fx_min,fx_max);
        fECDecayTimes->Fit(fosc1, "RB","",fx_min,fx_max);


        fCanvas2->cd();
        fECDecayTimes->Draw();
        fexp1->SetLineColor(kRed);
        fosc1->SetLineColor(kBlue);

        fexp1->Draw("SAME");
        fosc1->Draw("SAME");
    }
   // */
    
    
    if(Method=="RooFit")
    {
        RooDataHist ReducedDataSet("BinnedData","BinnedData",*fx,fECDecayTimes);


        RooChi2Var Chi20("Chi2M0","Chi2M0",*fpdfH0,ReducedDataSet,true);
        RooMinuit m1(Chi20);
        m1.migrad();
        // m1.hesse();
        RooFitResult* r1 = m1.save() ;

        fpdfH0->fitTo(ReducedDataSet,RooFit::Range(fx_min,fx_max));
        fpdfH1->fitTo(ReducedDataSet,RooFit::Range(fx_min,fx_max));

        RooChi2Var Chi21("Chi2M1","Chi2M1",*fpdfH1,ReducedDataSet,true);
        RooMinuit m2(Chi21);
        m2.migrad();
        // m2.hesse();
        RooFitResult* r2 = m2.save() ;

        //Plotting Bussiness
        RooPlot* xframe = fx->frame(RooFit::Title("Binned data and Chi2 fit"));
        ReducedDataSet.plotOn(xframe, RooFit::Name("BinnedData"));
        fpdfH0->plotOn(xframe,RooFit::Name("Chi2H0"));
        fpdfH1->plotOn(xframe,RooFit::Name("Chi2H1"));

        fpdfH0->plotOn(xframe,RooFit::LineColor(kRed));
        fpdfH1->plotOn(xframe,RooFit::LineColor(kBlue));
        xframe->Draw();

        //Goodness of Fit
        double chi2ReducedM0 = xframe->chiSquare("Chi2H0","BinnedData",2);
        double chi2M0=Chi20.getVal();
        //double chi2ReducedM0 = xframe->chiSquare("Chi2H0","BinnedData");
        double ndoff = xframe->GetNbinsX();
        //cout<<"chi2/ndoff (M0): "<<chi2M0<<"/"<<ndoff<<'\t'<<chi2ReducedM0<<endl;


        double chi2M1=Chi21.getVal();
        double chi2ReducedM1 = xframe->chiSquare("Chi2H1","BinnedData",5);
        //double chi2ReducedM1 = xframe->chiSquare("Chi2H1","BinnedData");
        double ndoffM1 = xframe->GetNbinsX();
        //cout<<"chi2/ndoff (M1): "<<chi2M1<<"/"<<ndoffM1<<'\t'<<chi2ReducedM1<<endl;

        MQLOG(INFO)<<"chi2/ndoff (M0): "<<chi2ReducedM0;
        MQLOG(INFO)<<"chi2/ndoff (M1): "<<chi2ReducedM1;
    
    }
    
    string strXmin;
    ostringstream ossTemp1;
    ossTemp1 << fx_min;
    strXmin = ossTemp1.str();

    string strXmax;
    ostringstream ossTemp2;
    ossTemp2 << fx_max;
    strXmax = ossTemp2.str();
    
    string strBinning;
    ostringstream ossTemp4;
    ossTemp4 << fBinning;
    strBinning = ossTemp4.str();
    fFigureName="Go2014_LikelihoodRatioProfile_Range_";
    fFigureName+=strXmin;
    fFigureName+="_";
    fFigureName+=strXmax;
    fFigureName+="_Binning_";
    fFigureName+=strBinning;
    fFigureName+=".pdf";
    /*
    //RooDataSet* ReducedDataSet = (RooDataSet*) fECdata->reduce(*fx,CutRange.c_str()) ;
    RooPlot* xframe0 = fx->frame(RooFit::Title("Binned data and Chi2 fit"));
    ReducedDataSet->plotOn(xframe0,RooFit::Binning(fBinning), Name("data_chi2"), DataError( RooAbsData::SumW2 ));
    fpdfH0->plotOn(xframe0,RooFit::LineColor(kRed));
    fpdfH1->plotOn(xframe0,RooFit::LineColor(kBlue));

    
    
    
    // to get a chi2 value we need to plot the complete pdf, for this as long as we never draw it we are ok.
    RooPlot *p = MASS->frame();
    SignalData->plotOn(p, Binning(bins), Name("data_chi2"), DataError( RooAbsData::SumW2 )); 
    pdf_total->plotOn(p, Range("fitRange"), Normalization(1.0,RooAbsReal::RelativeExpected), Name("curve_chi2"));
    // There is a method within RooPlot called chiSquare(). This returns the reduced chi2 which when multiplied out
    // by the number of bins gives the actual chi2 fit between the .
    chi2 = p->chiSquare( "curve_chi2", "data_chi2") * bins;
    // The total number of degrees of freedom is defined as the number of bins subtracted by the free floating parameters
    // in the fit. We already know the number of bins as we specified it, so the free params (we also know) but can be
    // obtained using:-
    Int_t floated = static_cast<Int_t>(fitResult->floatParsFinal().getSize());
 
    std::cout << "Chi2 = " << chi2 << "  (for "<< bins << " bins and " << floated << " floating params)" << std::endl;
    std::cout<<" Prob(chi2,ndof) of above = " << TMath::Prob(chi2, bins-floated) << std::endl;    
    */

}




ClassImp(SidsSummary)
        
        
        