/* 
 * File:   runSummary.cxx
 * Author: winckler
 *
 * Created on October 4, 2014, 12:14 PM
 */


#include <cstdlib>
#include <vector>
#include <iostream>
#include <csignal>



#include "TApplication.h"

#include "SidsSummary.h"
#include "FairMQLogger.h"

#include "MQconfig.h"

using std::cout;
using std::cin;
using std::endl;
using std::stringstream;

int main(int argc, char** argv)
{
    int MinArg=5;
    int MaxArg=7;
    if (argc < MinArg || argc>MaxArg)
    {
        MQLOG(ERROR)<< "Number of argument incorrect. ("<<argc<<" instead of 6)";
        MQLOG(INFO) << "Script usage: startVisualAnalysis \t showfiles.sh command1 command2 \n";
        MQLOG(INFO) << "Binary usage: ShowStatus treeName branch Directory Resultsfile command1 command2 \n"<<endl;
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
    
    MQconfig ConfigParameter;

    
    ConfigParameter.SetValue("InputFile",Resultsfilename);
    ++i;
    
    ConfigParameter.SetValue("TreeName",treename);
    ++i;
    
    ConfigParameter.SetValue("Branch",branchname);
    ++i;
    
    string detID("RSA51");
    ConfigParameter.SetValue("DetectorID",detID);
    
    
    string Command1;    
    string Command2;
    if(argc>MinArg)
    {
        ++i;
        Command1=string(argv[i]);
        
        if(argc>MinArg+1)
        {
            ++i;
            Command2=string(argv[i]);
        }
        
    }
    
    bool startgui=true;
    
    if(startgui)
    {
        TApplication app("App", &argc, argv);
        MQLOG(INFO)<<"Run start";
        SidsSummary* fSidsResults = new SidsSummary(gClient->GetRoot(), 1000, 600, ConfigParameter);
        fSidsResults->MapWindow();
        app.Run();
    }
    
    return 0;
}


