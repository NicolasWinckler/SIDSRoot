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
#include "FairMQLogger.h"

using namespace std;



int main(int argc, char** argv) 
{

    int MinArg=6;
    int MaxArg=7;
    if (argc < MinArg || argc>MaxArg)
    {
        MQLOG(ERROR)<< "Number of argument incorrect. ("<<argc<<" instead of 6)";
        MQLOG(INFO) << "Script usage: startVisualAnalysis \t showfiles.sh command1 command2 \n";
        MQLOG(INFO) << "Binary usage: ShowFile treeName branch Directory Resultsfile command1 command2 \n"<<endl;
        return 1;
    }
    int i = 1;
    
    string treename(argv[i]);
    ++i;
    string branchname(argv[i]);
    ++i;
    string dirname(argv[i]);
    ++i;
    string Resultsfilename(argv[i]);
    ++i;
    string Command1(argv[i]);
    
    
    string Command2;
    if(argc>MinArg)
    {
        ++i;
        Command2=string(argv[i]);
    }
        
    
    EsrTree *DecayTree = new EsrTree(Resultsfilename,treename,branchname);
    std::vector<EsrInjData> DataList=DecayTree->GetEsrData();
    delete DecayTree;
    
    vector<string> analyzedFiles;
    for(unsigned int k(0); k<DataList.size(); k++)
    {
        string temp=DataList[k].GetFileName();
        analyzedFiles.push_back(temp);
    }
    
    int command=-666;
    if(Command1=="all" || Command1=="--all")
        command=SIDSFileManager::kPrintAll;
    
    if(Command1=="analyzed" || Command1=="--analyzed" || Command1=="done")
        command=SIDSFileManager::kPrintAnalyzed;
    
    if(Command1=="notanalyzed" || Command1=="--notanalyzed" || Command1=="left")
        command=SIDSFileManager::kPrintNotAnalyzed;
    
    
        
    
    SIDSFileManager fileListManager(dirname,analyzedFiles);
    
    
    
    switch(command)
    {
        case SIDSFileManager::kPrintAll :
        {
            fileListManager.PrintAll();
            break;
        }
        
        case SIDSFileManager::kPrintAnalyzed :
        {
            if(Command2=="--detail" || Command2=="detail")
                fileListManager.PrintAnalyzed(true);
            else
                fileListManager.PrintAnalyzed();
            break;
        }
        
        case SIDSFileManager::kPrintNotAnalyzed :
        {
            fileListManager.PrintNotAnalyzed();
            break;
        }
        
        default:
                MQLOG(ERROR) << "Command showfiles.sh "<< Command1 <<" not supported";
                MQLOG(INFO) << "Usage :";
                MQLOG(INFO) <<"\t showfiles.sh all";
                MQLOG(INFO) <<"\t showfiles.sh analyzed";
                MQLOG(INFO) <<"\t showfiles.sh analyzed detail";
                MQLOG(INFO) <<"\t showfiles.sh done";
                MQLOG(INFO) <<"\t showfiles.sh notanalyzed";
                MQLOG(INFO) <<"\t showfiles.sh left";
                
    }
    

    
    
    return 0;
}

