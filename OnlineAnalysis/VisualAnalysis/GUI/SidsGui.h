/* 
 * File:   SidsGui.h
 * Author: winckler
 *
 * Created on August 7, 2014, 5:01 PM
 */

#ifndef SIDSGUI_H
#define	SIDSGUI_H

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
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <memory>

#ifdef NANOMSG
#include "FairMQTransportFactoryNN.h"
#else
#include "FairMQTransportFactoryZMQ.h"
#endif

#include "EsrSamplerTask.h"
#include "EsrSidsSampler.h"
#endif //__CINT__


#include "EsrTree.h"

#include "MQconfig.h"

#include "EsrInjData.h"

#include "SidsQualityTagField.h"
#include "SidsDecayTxtField.h"
#include "header.h"

enum EMyMessageTypes {
   M_FILE_OPEN,
   M_FILE_BROWSE,
   M_FILE_NEWCANVAS,
   M_FILE_CLOSEWIN,
   M_FILE_EXIT,
   M_HELP_ABOUT
};


//----------------------------------------------------------------------

class SidsGui : public TGMainFrame 
{

public:
    SidsGui(const TGWindow *p, int w, int h, MQconfig SamplerConfig, std::string Filename="");
    virtual ~SidsGui();
    
    /// buttons & gui stuffs
    void HandleMenu(Int_t);
    void DataDropped(TGListTreeItem* item, TDNDData* data);
    void ResetStatus();
    void DoDraw();
    void DoCloseWindow();
    void DoExit();
    void DoValidate();
    void AddDecay();
    void RemoveDecay();
    void DoDoubleClick(Int_t event, Int_t px, Int_t py, TObject *);
    void EventInfo(Int_t event, Int_t px, Int_t py, TObject *selected);
    void ChangeMode(Int_t BoxID);
    void DoSlider();
    void InitSlider();

    void DoSaveFigure()
    {
        string dir=fParConfig.GetValue<string>("InputDirectory");
        dir+="/go2014figures/Traces/";

        string inputname=dir+fFileName.Data();
        int lastindex = inputname.find_last_of("."); 
        string figfile = inputname.substr(0, lastindex);
        figfile+=".pdf";
        fCanvas2->SaveAs(figfile.c_str());
    }
protected:
    TRootEmbeddedCanvas  *fEc;                  // embedded canvas (left))
    TRootEmbeddedCanvas  *fEc2;                 // embedded canvas (right)
    TGTextButton         *fButtonExit;          // "Exit" text button
    TGTextButton         *fButtonValidate;      // "Validate" text button
    SidsQualityTagField  *fFileQualityTag;      // File Quality&comment button
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
    TH1D                 *fParentTrace;         // parent proj
    TH1D                 *fDaughterTrace;       // daughter proj
    TH1I                 *fNEC;                 // number of EC History.
    TH1F                 *fPfreq;               // Parent freq History.
    TH2D                 *fCurrentHisto;
    TGDoubleVSlider      *fVslider1;
    TGVerticalFrame      *fControlFrame;        // main button frame
    TString              fFileName;             // file name
    TGFileInfo           fFileInfo;             // file info
    EsrInjData           fDecayData;            // Data to fill and store
    string               fDetectorID;           // DetectorID string
    float                fParentFreq;           // Current Parent freq.;
    MQconfig             fParConfig;            // Parameter container
    TFile                *fInputFile;           // Input root file
    double               fSliderScale;
    
    std::vector<SidsDecayTxtField*> fDecayField;// Decay field buttons
    std::vector<TH1D*> f1DHisto;                // Input 1D-Histos
    std::vector<TH2D*> f2DHisto;                // Input 2D-Histos
    std::vector<Header*> fHeaders;              // Input Headers

    
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
    
    /// Analysis specific
    void FindTraces(
            TH2D* hist2d, 
            Int_t BinPWindow=10, Int_t BinDWindow=10, Int_t BinDist=52, 
            Double_t sigma = 4., Double_t threshold = 0.2, 
            Option_t* option = "");

    /// MQ stuffs
    void StartSampler();
    bool ReadyToSend() const {return fReadyToSend;}
    void SetSampler(bool active=false){fSampler=active;}
   
private:
    int fDecayCounter;
    int fHisto1DCounter;
    int fHisto2DCounter;
    int fHeaderCounter;
    bool fReadyToSend;
    bool fSampler;
    
   ClassDef(SidsGui, 0); 
};

#endif	/* SIDSGUI_H */

