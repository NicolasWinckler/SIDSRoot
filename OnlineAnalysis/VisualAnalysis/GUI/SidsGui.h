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

#include "TGStatusBar.h"
#include "TVirtualPadEditor.h"
#include <TGClient.h>
#include <TFrame.h>

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

class SidsGui : public TGMainFrame {

protected:
   TRootEmbeddedCanvas  *fEc;          // embedded canvas
   TRootEmbeddedCanvas  *fEc2;          // embedded canvas
   TGTextButton         *fButtonExit;  // "Exit" text button
   TGTextButton         *fButtonValidate;    // "Validate" text button
   SidsQualityTagField  *fFileQualityTag;
   TGMenuBar            *fMenuBar;     // main menu bar
   TGPopupMenu          *fMenuFile;    // "File" popup menu entry
   TGPopupMenu          *fMenuHelp;    // "Help" popup menu entry
   TCanvas              *fCanvas1;      // canvas
   TCanvas              *fCanvas2;      // canvas
   TGListTree           *fListTree;    // left list tree
   TGListTreeItem       *fBaseLTI;     // base (root) list tree item
   TGLabel              *fStatus;      // label used to display status
   TGStatusBar          *fStatusBar;
   TGraph               *fGraph;       // TGraph object
   TH1F                 *fHist1D;      // 1D histogram
   TH2F                 *fHist2D;      // 2D histogram
   TGVerticalFrame      *fControlFrame;
   TGHorizontalFrame    *fAddRemoveButtonsFrm;
   TH2D                 *fSidsHisto;
   TString              fFileName;
   TGFileInfo           fFileInfo;
   EsrInjData           fDecayData;
   //TGCompositeFrame   *fEditorFrame;
   //TVirtualPadEditor  *fEditor;
   
   std::vector<SidsDecayTxtField*> fDecayField;
   
   std::vector<TH1D*> f1DHisto;
   std::vector<TH2D*> f2DHisto;
   std::vector<Header*> fHeaders;
   
   
   int RootFileManager(TFile* rootfile);
   void SeekObject(TKey *key);
   
public:
   SidsGui(const TGWindow *p, int w, int h, MQconfig SamplerConfig, std::string Filename="");
   virtual ~SidsGui();
   void              DoDraw();
   void              DoCloseWindow();
   void              HandleMenu(Int_t);
   TObject          *GetObject(const char *obj);
   void              OpenRootFile();//TGFileInfo fileInfo);
   void              DataDropped(TGListTreeItem* item, TDNDData* data);
   void              ResetStatus();
   
   void              DoExit();
   void              DoValidate();
   void              AddDecay();
   void              RemoveDecay();
   void              StartSampler();
   
   void DoDoubleClick(Int_t event, Int_t px, Int_t py, TObject *);
   
   bool ReadyToSend() const {return fReadyToSend;}
   void SetSampler(bool active=false){fSampler=active;}
   
void EventInfo(Int_t event, Int_t px, Int_t py, TObject *selected);
void ReadDir(TDirectory *dir);
private:
    int fDecayCounter;
    int fHisto1DCounter;
    int fHisto2DCounter;
    int fHeaderCounter;
    bool fReadyToSend;
    bool fSampler;
    MQconfig fSamplerConfig;
   ClassDef(SidsGui, 0); // Mainframe for Drag and Drop demo
};

/*
#ifndef __CINT__
static void s_signal_handler(int signal);
static void s_catch_signals(void);
#endif //__CINT__
*/

#endif	/* SIDSGUI_H */

