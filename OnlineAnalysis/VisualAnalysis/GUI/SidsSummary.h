/* 
 * File:   SidsSummary.h
 * Author: winckler
 *
 * Created on October 4, 2014, 3:09 AM
 */

#ifndef SIDSSUMMARY_H
#define	SIDSSUMMARY_H

//root
#include "TROOT.h"
#include "TApplication.h"
#include "TSystem.h"
#include "TGFrame.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGMenu.h"
#include "TGFileDialog.h"
#include "TBrowser.h"
#include "TRootEmbeddedCanvas.h"
#include "TRootHelpDialog.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF2.h"
#include "TGraph.h"
#include "TImage.h"
#include "TRandom.h"
#include "TGMsgBox.h"
#include "TGPicture.h"
#include "TGListTree.h"
#include "TObjString.h"
#include "TMessage.h"
#include "TTimer.h"
#include "TGDNDManager.h"
#include "TString.h"
#include "TFile.h"
#include "TClass.h"
#include "TKey.h"
#include "TSpectrum.h"
#include <TGComboBox.h>
#include "TGStatusBar.h"
#include "TVirtualPadEditor.h"
#include <TGClient.h>
#include <TFrame.h>
#include "TGTripleSlider.h"

#include "TMath.h"
#include "TPaveText.h"
#include "RooRealVar.h" 
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooGenericPdf.h"
#include "RooMinuit.h"
#include "RooWorkspace.h"
#include "RooRandom.h"
#include "RooUniform.h"
#include "RooGaussian.h"
#include "RooStats/ModelConfig.h"
#include "RooNumIntConfig.h"
#include "RooRealProxy.h"
#include "RooAbsPdf.h"
#include "RooChi2Var.h"

//std
#include <stdlib.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <csignal>
//#include <libgen.h>
#include <cmath>
// boost and fairroot
#ifndef __CINT__
#include <boost/filesystem.hpp>
#include <memory>
#include <map>
#include "SIDSFileManager.h"
#endif //__CINT__


#include "EsrTree.h"

#include "MQconfig.h"

#include "EsrInjData.h"

#include "SidsRooOscModel.h"
//#include "RooMypdf.h"
#include "SidsQualityTagField.h"
#include "SidsDecayTxtField.h"
#include "header.h"
//#include "RooMyPdfDict.h"
#include "SidsFitButtons.h"

using namespace RooFit;
using namespace RooStats;

class SidsRooOscModel;





enum MessageTypes {
   FILE_OPEN,
   FILE_BROWSE,
   FILE_NEWCANVAS,
   FILE_CLOSEWIN,
   FILE_EXIT,
   HELP_ABOUT
};

//----------------------------------------------------------------------

class SidsSummary : public TGMainFrame 
{

public:
    
    enum FitType
    {
        kUnbinnedLikelihood,
        kBinnedLikelihood,
        kChi2,
        kPNLL,
        kNoFit
    };
    
    SidsSummary(const TGWindow *p, int w, int h, MQconfig SamplerConfig, std::string Filename="");
    virtual ~SidsSummary();
    
    /// buttons & gui stuffs
    void HandleMenu(Int_t);
    void DataDropped(TGListTreeItem* item, TDNDData* data);
    void ResetStatus();
    void DoDraw(Int_t BoxID);
    void DoCloseWindow();
    void DoExit();
    void EventInfo(Int_t event, Int_t px, Int_t py, TObject *selected);
    void UnbinnedLikelihoodFit(bool Draw);
    void UpdateHistData(double tmin=0., double tmax=70.);
    void UpdateFitPar();
    void DoFitUpdate(Int_t BoxID);
    void DoFitUpdate();
    
    static double Chi2FitM0(double *t,double *par);
    static double Chi2FitM1(double *t,double *par);
    void DoChi2Fit(bool Draw);
    void DoPNLL(bool Draw);
    void DoNothing(bool Draw);
    void DoSaveFigure();
protected:
    TRootEmbeddedCanvas  *fEc;                  // embedded canvas (left))
    TRootEmbeddedCanvas  *fEc2;                 // embedded canvas (right)
    TGTextButton         *fButtonExit;          // "Exit" text button
    TGMenuBar            *fMenuBar;             // main menu bar
    TGPopupMenu          *fMenuFile;            // "File" popup menu entry
    TGPopupMenu          *fMenuHelp;            // "Help" popup menu entry
    TCanvas              *fCanvas1;             // canvas1 (left)
    TCanvas              *fCanvas2;             // canvas (right)
    TGListTree           *fListTree;            // left list tree
    TGListTreeItem       *fBaseLTI;             // base (root) list tree item
    TGLabel              *fStatus;              // label used to display status
    TGStatusBar          *fStatusBar;           // status bar
    TGraph               *fGraph;               // TGraph object
    TH1D                 *fHisto_px;            // freq proj
    TH1D                 *fECDecayTimes;         // parent proj
    TH1D                 *fDaughterTrace;       // daughter proj
    TH1I                 *fNEC;                 // number of EC History.
    TH1F                 *fPfreq;               // Parent freq History.
    TH1D                 *fECFreq;
    TGDoubleVSlider      *fVslider1;
    TGVerticalFrame      *fControlFrame;        // main button frame
    TString              fFileName;             // file name
    TGFileInfo           fFileInfo;             // file info
    EsrInjData           fDecayData;            // Data to fill and store
    string               fDetectorID;           // DetectorID string
    float                fParentFreq;           // Current Parent freq.;
    MQconfig             fParConfig;            // Parameter container
    TFile                *fInputFile;           // Input root file
    int                  fBinning;
    int                  fFitID;
    
    SidsFitButtons* fNormFactframe;
    SidsFitButtons* fBinningframe;
    SidsFitButtons* fObsframe;
    SidsFitButtons* flambdaframe;
    SidsFitButtons* fampframe;
    SidsFitButtons* fOmegaframe;
    SidsFitButtons* fPhiframe;
    
    // fit par
    double fx_min;
    double fx_max;
    
    double fNormFactInit;
    double fNormFact_Max;
    double fNormFact_Min;
    
    double flambdaInit;
    double flambda_Max;
    double flambda_Min;
    
    double fampInit;
    double famp_Max;
    double famp_Min;
    
    double fOmegaInit;
    double fOmega_Max;
    double fOmega_Min;
    
    double fPhiInit;
    double fPhi_Max;
    double fPhi_Min;
    string fFigureName;
    
    RooRealVar* fx;
    RooRealVar* fNorm0;
    RooRealVar* fNorm1;
    RooRealVar* flambda0;
    RooRealVar* flambda1;
    RooRealVar* famp1;
    RooRealVar* fomega1;
    RooRealVar* fphi1;
    
    RooGenericPdf* fpdfH0;
    SidsRooOscModel* fpdfH1;
    RooGenericPdf* fChi2pdfH0;
    RooGenericPdf* fChi2pdfH1;
    RooDataSet* fECdata;
    
    
    RooFitResult* fFitResultH0;
    RooFitResult* fFitResultH1;
    TF1 *fexp1;
    TF1 *fosc1;
    
    std::vector<TH1D*> f1DHisto;                // Input 1D-Histos
    std::vector<TH2D*> f2DHisto;                // Input 2D-Histos
    std::vector<Header*> fHeaders;              // Input Headers
    std::vector<EsrInjData> fDataList;
    
    map<string,vector<int> > fDuplicatesIdx;
    map<string, int> fDataToPlotIdx;
    
    /// init constructor
    void SetupGUI();            // use once in constructor
    void InitParameters();      // use once in constructor
    
    /// handle files, histos etc.
    void OpenRootFile();
    int RootFileManager(TFile* file=NULL, const TString & filename="");
    void SeekObject(TKey *key);
    int AddToListTree(TObject* obj);
    void AddToRootFile(TObject* obj, const string & outputFileName, const string &  fileOption="NEW");
    void SaveHisto(const string & outputFileName);
    void ReadDir(TDirectory *dir);
    void ReInitRooFitPar();
    
   
private:
    int fDecayCounter;
    int fHisto1DCounter;
    int fHisto2DCounter;
    int fHeaderCounter;
    bool fReadyToSend;
    bool fSampler;
    
   ClassDef(SidsSummary, 0); 
};




#endif	/* SIDSSUMMARY_H */

