/* 
 * File:   runFileInfo.cxx
 * Author: winckler
 *
 * Created on October 3, 2014, 10:22 PM
 */


#include <cstdlib>
#include <vector>
#include <map>
#include <string>

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

    
    SIDSFileManager fileman;
    fileman.SetDirectory(dir);
    fileman.SetInputList(analyzedFiles);
    fileman.Compare();
    
    return 0;
}

