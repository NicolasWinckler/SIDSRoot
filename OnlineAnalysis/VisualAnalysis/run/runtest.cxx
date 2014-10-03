/* 
 * File:   runtest.cxx
 * Author: winckler
 *
 * Created on September 16, 2014, 3:57 PM
 */

#include <cstdlib>
#include <vector>
#include <map>
#include <string>

#include "TKey.h"
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
#include "Riostream.h"
#include "header.h"

#include "EsrTree.h"

#include "dirent.h"
#include <unordered_set>
#include <iterator>
#include <algorithm>

#include "SIDSFileManager.h"

using namespace std;




/*
 * 
 */
int main(int argc, char** argv) 
{

    
    string treename="SIDSdecayData";
    string branchname="EsrInjData.";
    //string outputfilename="/Users/winckler/fair/exp/SIDSRoot/AnalysisOutput/Osc/Online/VisualAnalysis/SidsVisualDecayResults.root";
    string outputfilename="/Users/winckler/fair/exp/SIDSRoot/AnalysisOutput/Osc/Online/VisualAnalysis/SIDSDecayData.root";

    string alreadyAnalyzedFilename;
    //fInputFile = new  TFile(fFileName);
    EsrTree *DecayTree = new EsrTree(outputfilename,treename,branchname);
    std::vector<EsrInjData> DataList=DecayTree->GetEsrData();
    delete DecayTree;
    
    vector<string> analyzedFiles;
    for(unsigned int k(0); k<DataList.size(); k++)
    {
        alreadyAnalyzedFilename=DataList[k].GetFileName();
        analyzedFiles.push_back(alreadyAnalyzedFilename);
    }
    
    string dir="/Users/winckler/fair/exp/SIDSRoot/build";

    
    SIDSFileManager fileman(dir,analyzedFiles);
    return 0;
}

