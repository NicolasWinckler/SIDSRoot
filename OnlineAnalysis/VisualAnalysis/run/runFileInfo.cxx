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

bool is_number(const std::string& s);

int main(int argc, char** argv) 
{

    int MinArg=6;
    int MaxArg=8;
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
    string Command3;
    if(argc>MinArg)
    {
        ++i;
        Command2=string(argv[i]);
        if(argc==MaxArg)
        {
            ++i;
            Command3=string(argv[i]);
        }
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
    
    int AnalysisNumber=0;
    bool DuplDetail=false;
    if(Command1=="duplicates" || Command1=="--duplicates" || Command1=="doublet")
    {
        command=SIDSFileManager::kDuplicates;
        if(Command2=="detail")
        {
            DuplDetail=true;
            if(!Command3.empty())
                if(is_number(Command3))
                {
                    AnalysisNumber=std::stoi(Command3);
                }
        }
        else
        {
            if(is_number(Command2))
            {
                AnalysisNumber=std::stoi(Command2);
            }

        }
    }
    
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
        
        case SIDSFileManager::kDuplicates :
        {
            fileListManager.PrintDuplicates(DuplDetail,AnalysisNumber);
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
                MQLOG(INFO) <<"\t showfiles.sh duplicates detail 2";
                
    }
    

    
    
    return 0;
}

bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

