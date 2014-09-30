/* 
 * File:   runEsrSampler.cxx
 * Author: winckler
 *
 * Created on August 13, 2014, 2:59 PM
 */


#include <cstdlib>
#include <vector>
#include <iostream>
#include <csignal>



#include "TApplication.h"

#include "SidsGui.h"
#include "FairMQLogger.h"

#include "MQconfig.h"

using std::cout;
using std::cin;
using std::endl;
using std::stringstream;

int main(int argc, char** argv)
{
    if (argc != 24)
    {
        MQLOG(ERROR)<<"Number of argument incorrect. ("<<argc<<" instead of 24)";
        MQLOG(INFO) <<"Script usage: startVisualAnalysis \t userName inputFile \n";
        MQLOG(INFO) << "Binary usage: EsrSidsSampler$dataFormat \tID inputFile \n"
             << "\t\t userName treeName branch outputFile\n"
             << "\t\t eventRate numIoTreads\n"
             << "\t\toutputSocketType outputSndBufSize outputMethod outputAddress\n"
             << "\t\t binDistancePDfreq binPWindow binDWindow binningTraces binningFreqTH2\n" 
             << "\t\t binSigmaPeak thresholdPeak detectorID detectorSuffix kickerPrefix binZoomTH2Window\n"   
                << endl;
        return 1;
    }


    MQconfig ConfigParameter;

    int i = 1;
    ConfigParameter.SetValue("ID",string(argv[i]));
    ++i;
    
    ConfigParameter.SetValue("InputFile",string(argv[i]));
    ++i;
    
    ConfigParameter.SetValue("UserName",string(argv[i]));
    ++i;
    
    ConfigParameter.SetValue("TreeName",string(argv[i]));
    ++i;
    
    ConfigParameter.SetValue("Branch",string(argv[i]));
    ++i;
    
    ConfigParameter.SetValue("OutputFile",string(argv[i]));
    ++i;
    
    int eventRate;
    stringstream(argv[i]) >> eventRate;
    ConfigParameter.SetValue("EventRate",eventRate);
    ++i;

    int numIoThreads;
    stringstream(argv[i]) >> numIoThreads;
    ConfigParameter.SetValue("NumIoThreads",numIoThreads);
    ++i;
    
    ConfigParameter.SetValue("NumInputs",0);
    ConfigParameter.SetValue("NumOutputs",1);

    ConfigParameter.SetValue("OutputSocketType",string(argv[i]));
    ++i;
    int outputSndBufSize;
    stringstream(argv[i]) >> outputSndBufSize;
    ConfigParameter.SetValue("OutputSndBufSize",outputSndBufSize);

    ++i;
    ConfigParameter.SetValue("OutputMethod",string(argv[i]));
    ++i;
    ConfigParameter.SetValue("OutputAddress",string(argv[i]));
    ++i;


    //13
    int BinDistancePDfreq;
    stringstream(argv[i]) >> BinDistancePDfreq;
    ConfigParameter.SetValue("BinDistancePDfreq",BinDistancePDfreq);
    ++i;
    
    int BinPWindow;
    stringstream(argv[i]) >> BinPWindow;
    ConfigParameter.SetValue("BinPWindow",BinPWindow);
    ++i;
    
    int BinDWindow;
    stringstream(argv[i]) >> BinDWindow;
    ConfigParameter.SetValue("BinDWindow",BinDWindow);
    ++i;
    
    int BinningTraces;
    stringstream(argv[i]) >> BinningTraces;
    ConfigParameter.SetValue("BinningTraces",BinningTraces);
    ++i;
    
    int BinningFreqTH2;
    stringstream(argv[i]) >> BinningFreqTH2;
    ConfigParameter.SetValue("BinningFreqTH2",BinningFreqTH2);
    ++i;
    
    double BinSigmaPeak;
    stringstream(argv[i]) >> BinSigmaPeak;
    ConfigParameter.SetValue("BinSigmaPeak",BinSigmaPeak);
    ++i;
    
    double ThresholdPeak;
    stringstream(argv[i]) >> ThresholdPeak;
    ConfigParameter.SetValue("ThresholdPeak",ThresholdPeak);
    ++i;
    
    ConfigParameter.SetValue("DetectorID",string(argv[i]));
    ++i;
    
    ConfigParameter.SetValue("DetectorSuffix",string(argv[i]));
    ++i;
    
    ConfigParameter.SetValue("KickerPrefix",string(argv[i]));
    ++i;
    
    int BinZoomTH2Window;
    stringstream(argv[i]) >> BinZoomTH2Window;
    ConfigParameter.SetValue("BinZoomTH2Window",BinZoomTH2Window);
    ++i;
    
    
    
    string outputfilename=ConfigParameter.GetValue<string>("OutputFile");
    string treename=ConfigParameter.GetValue<string>("TreeName");
    string branchname=ConfigParameter.GetValue<string>("Branch");
    string inputFileName=ConfigParameter.GetValue<string>("InputFile");
    string alreadyAnalyzedFilename;
    //fInputFile = new  TFile(fFileName);
    EsrTree *DecayTree = new EsrTree(outputfilename,treename,branchname);
    std::vector<EsrInjData> DataList=DecayTree->GetEsrData();
    delete DecayTree;
    
    int countDoublet=0;
    bool alreadyAnalyzed=false;
    bool startgui=true;
    for(unsigned int i(0); i<DataList.size(); i++)
    {
        alreadyAnalyzedFilename=DataList[i].GetFileName();
        size_t found = inputFileName.find(alreadyAnalyzedFilename);
        if(found!=std::string::npos)
        {
            countDoublet++;
            alreadyAnalyzed=true;
        }
    }
    if(alreadyAnalyzed)
    {
        cout<<"[WARNING] File "<<alreadyAnalyzedFilename<<" has already been analyzed ";
        if(countDoublet==1) 
            cout<<" once."<<endl;
        if(countDoublet==2) 
            cout<<" twice."<<endl;
        if(countDoublet>2) 
            cout<< countDoublet <<" times."<<endl;
        cout<<"[WARNING] Do you want to analyze this file again?"<<endl;
        cout<<"1) yes (analyzed data results will be duplicated)"<<endl;
        cout<<"2) no"<<endl;
        string reply;
        cin>>reply;
        
        if(reply =="1" || reply=="y" || reply=="Y" || reply=="yes" || reply=="YES" || reply=="Yes")
        {
            startgui=true;
            cout<<"[INFO] Start analysis"<<endl;
        }
        
        if(reply =="2" || reply=="n" || reply=="N" || reply=="no" || reply=="NO" || reply=="No")
        {
            startgui=false;
            cout<<"[INFO] Analysis stop."<<endl;
        }
        
    }
    
    
    if(startgui)
    {
        TApplication app("App", &argc, argv);
        MQLOG(INFO)<<"Run start";
        SidsGui* fSidsAnalysisGui = new SidsGui(gClient->GetRoot(), 1000, 600, ConfigParameter);
        fSidsAnalysisGui->MapWindow();
        app.Run();
    }

    return 0;
}


