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
#endif //__CINT__


#include "EsrTree.h"

#include "MQconfig.h"

#include "EsrInjData.h"

#include "SidsQualityTagField.h"
#include "SidsDecayTxtField.h"
#include "header.h"


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
    
    SidsSummary(const TGWindow *p, int w, int h, MQconfig SamplerConfig, std::string Filename="");
    virtual ~SidsSummary();
    
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
    void EventInfo(Int_t event, Int_t px, Int_t py, TObject *selected);
    void ChangeMode(Int_t BoxID);
    
    
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
    double               fSliderScale;
    
    std::vector<TH1D*> f1DHisto;                // Input 1D-Histos
    std::vector<TH2D*> f2DHisto;                // Input 2D-Histos
    std::vector<Header*> fHeaders;              // Input Headers
    std::vector<EsrInjData> fDataList;
    
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

