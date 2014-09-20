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
    if (argc != 23)
    {
        MQLOG(ERROR)<<"Number of argument incorrect. ("<<argc<<" instead of 23)";
        MQLOG(INFO) <<"Script usage: startVisualAnalysis \t userName inputFile \n";
        MQLOG(INFO) << "Binary usage: EsrSidsSampler$dataFormat \tID inputFile \n"
             << "\t\t userName treeName branch outputFile\n"
             << "\t\t eventRate numIoTreads\n"
             << "\t\toutputSocketType outputSndBufSize outputMethod outputAddress\n"
             << "\t\t binDistancePDfreq binPWindow binDWindow binningTraces binningFreq2dHisto\n" 
             << "\t\t binSigmaPeak thresholdPeak detectorID detectorSuffix kickerPrefix\n"   
                << endl;
        return 1;
    }


    MQconfig SamplerConfig;

    int i = 1;
    SamplerConfig.SetValue("ID",string(argv[i]));
    ++i;
    
    SamplerConfig.SetValue("InputFile",string(argv[i]));
    ++i;
    
    SamplerConfig.SetValue("UserName",string(argv[i]));
    ++i;
    
    SamplerConfig.SetValue("TreeName",string(argv[i]));
    ++i;
    
    SamplerConfig.SetValue("Branch",string(argv[i]));
    ++i;
    
    SamplerConfig.SetValue("OutputFile",string(argv[i]));
    ++i;
    
    int eventRate;
    stringstream(argv[i]) >> eventRate;
    SamplerConfig.SetValue("EventRate",eventRate);
    ++i;

    int numIoThreads;
    stringstream(argv[i]) >> numIoThreads;
    SamplerConfig.SetValue("NumIoThreads",numIoThreads);
    ++i;
    
    SamplerConfig.SetValue("NumInputs",0);
    SamplerConfig.SetValue("NumOutputs",1);

    SamplerConfig.SetValue("OutputSocketType",string(argv[i]));
    ++i;
    int outputSndBufSize;
    stringstream(argv[i]) >> outputSndBufSize;
    SamplerConfig.SetValue("OutputSndBufSize",outputSndBufSize);

    ++i;
    SamplerConfig.SetValue("OutputMethod",string(argv[i]));
    ++i;
    SamplerConfig.SetValue("OutputAddress",string(argv[i]));
    ++i;


    //13
    int BinDistancePDfreq;
    stringstream(argv[i]) >> BinDistancePDfreq;
    SamplerConfig.SetValue("BinDistancePDfreq",BinDistancePDfreq);
    ++i;
    
    int BinPWindow;
    stringstream(argv[i]) >> BinPWindow;
    SamplerConfig.SetValue("BinPWindow",BinPWindow);
    ++i;
    
    int BinDWindow;
    stringstream(argv[i]) >> BinDWindow;
    SamplerConfig.SetValue("BinDWindow",BinDWindow);
    ++i;
    
    int BinningTraces;
    stringstream(argv[i]) >> BinningTraces;
    SamplerConfig.SetValue("BinningTraces",BinningTraces);
    ++i;
    
    int BinningFreq2dHisto;
    stringstream(argv[i]) >> BinningFreq2dHisto;
    SamplerConfig.SetValue("BinningFreq2dHisto",BinningFreq2dHisto);
    ++i;
    
    double BinSigmaPeak;
    stringstream(argv[i]) >> BinSigmaPeak;
    SamplerConfig.SetValue("BinSigmaPeak",BinSigmaPeak);
    ++i;
    
    double ThresholdPeak;
    stringstream(argv[i]) >> ThresholdPeak;
    SamplerConfig.SetValue("ThresholdPeak",ThresholdPeak);
    ++i;
    SamplerConfig.SetValue("DetectorID",string(argv[i]));
    ++i;
    SamplerConfig.SetValue("DetectorSuffix",string(argv[i]));
    ++i;
    SamplerConfig.SetValue("KickerPrefix",string(argv[i]));
    ++i;
    
    
    TApplication app("App", &argc, argv);
    MQLOG(INFO)<<"Run start";
    SidsGui* fSidsAnalysisGui = new SidsGui(gClient->GetRoot(), 1000, 600, SamplerConfig);
    fSidsAnalysisGui->MapWindow();
    app.Run();
    

    return 0;
}


