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
    if (argc != 13)
    {
        MQLOG(ERROR)<<"Number of argument incorrect.";
        MQLOG(INFO) <<"Script usage: startVisualAnalysis \t userName inputFile \n";
        MQLOG(INFO) << "Binary usage: EsrSidsSampler$dataFormat \tID inputFile \n"
             <<"\t\t userName treeName branch outputFile\n"
             << "\t\t eventRate numIoTreads\n"
             << "\t\toutputSocketType outputSndBufSize outputMethod outputAddress\n" << endl;
        return 1;
    }


    MQconfig SamplerConfig;

    int i = 1;
    SamplerConfig.SetStringValue("ID",string(argv[i]));
    ++i;
    
    SamplerConfig.SetStringValue("InputFile",string(argv[i]));
    ++i;
    
    SamplerConfig.SetStringValue("UserName",string(argv[i]));
    ++i;
    
    SamplerConfig.SetStringValue("TreeName",string(argv[i]));
    ++i;
    
    SamplerConfig.SetStringValue("Branch",string(argv[i]));
    ++i;
    
    SamplerConfig.SetStringValue("OutputFile",string(argv[i]));
    ++i;
    
    int eventRate;
    stringstream(argv[i]) >> eventRate;
    SamplerConfig.SetIntValue("EventRate",eventRate);
    ++i;

    int numIoThreads;
    stringstream(argv[i]) >> numIoThreads;
    SamplerConfig.SetIntValue("NumIoThreads",numIoThreads);
    ++i;
    
    SamplerConfig.SetIntValue("NumInputs",0);
    SamplerConfig.SetIntValue("NumOutputs",1);

    SamplerConfig.SetStringValue("OutputSocketType",string(argv[i]));
    ++i;
    int outputSndBufSize;
    stringstream(argv[i]) >> outputSndBufSize;
    SamplerConfig.SetIntValue("OutputSndBufSize",outputSndBufSize);

    ++i;
    SamplerConfig.SetStringValue("OutputMethod",string(argv[i]));
    ++i;
    SamplerConfig.SetStringValue("OutputAddress",string(argv[i]));
    ++i;


    
    SamplerConfig.SetIntValue("BinDistancePDfreq",52);
    SamplerConfig.SetIntValue("BinPWindow",10);
    SamplerConfig.SetIntValue("BinDWindow",10);
    SamplerConfig.SetIntValue("BinningTraces",10);
    SamplerConfig.SetIntValue("BinningFreq2dHisto",2);
    SamplerConfig.SetDoubleValue("BinSigmaPeak",10);
    SamplerConfig.SetDoubleValue("ThresholdPeak",10);
    
    
    TApplication app("App", &argc, argv);
    MQLOG(INFO)<<"Run start";
    SidsGui* fSidsAnalysisGui = new SidsGui(gClient->GetRoot(), 1000, 600, SamplerConfig);
    fSidsAnalysisGui->MapWindow();
    app.Run();
    

    return 0;
}


