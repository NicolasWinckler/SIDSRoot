/* 
 * File:   MQconfig.cxx
 * Author: winckler
 * 
 * Created on August 14, 2014, 4:04 PM
 */


#include "MQconfig.h"

MQconfig::MQconfig() : fID(),
    fUserName(),
    fInputFile(),
    fOutputFile(),
    fTreeName(),
    fParameterFile(),
    fBranch(),
    fEventRate(0),
    fNumIoThreads(0),
    fNumInputs(0),
    fNumOutputs(0),
    fOutputSocketType(),
    fOutputSndBufSize(0),
    fOutputMethod(),
    fOutputAddress(),
    fPayloadFormat()
{
    
    /// Define string keys
    //file related
    fKeyStringValues.push_back("InputFile");
    fKeyStringValues.push_back("OutputFile");
    fKeyStringValues.push_back("ParameterFile");
    fKeyStringValues.push_back("Branch");
    fKeyStringValues.push_back("TreeName");
    
    //device related
    fKeyStringValues.push_back("ID");
    fKeyStringValues.push_back("OutputSocketType");
    fKeyStringValues.push_back("OutputMethod");
    fKeyStringValues.push_back("OutputAddress");
    fKeyStringValues.push_back("PayloadFormat");
    
    //experiment related
    fKeyStringValues.push_back("DetectorID");
    fKeyStringValues.push_back("DetectorSuffix");
    
    fKeyStringValues.push_back("KickerPrefix");
    fKeyStringValues.push_back("KickerSufffix");
    
    fKeyStringValues.push_back("UserName");
    
    
    
    /// Define int keys
    //device related
    fKeyIntValues.push_back("OutputSndBufSize");
    fKeyIntValues.push_back("EventRate");
    fKeyIntValues.push_back("NumIoThreads");
    fKeyIntValues.push_back("NumInputs");
    fKeyIntValues.push_back("NumOutputs");
    
    
    //exp related
    fKeyIntValues.push_back("BinDistancePDfreq");
    fKeyIntValues.push_back("BinPWindow");
    fKeyIntValues.push_back("BinDWindow");
    fKeyIntValues.push_back("BinningTraces");
    fKeyIntValues.push_back("BinningFreq2dHisto");
    
    
    /// Define double keys
    //exp related
    fKeyDoubleValues.push_back("BinSigmaPeak");
    fKeyDoubleValues.push_back("ThresholdPeak");
    
    
    /// create and init keys in maps
    for(auto p: fKeyDoubleValues)
        fParameterDoubleValues[p];
    
    for(auto p: fKeyStringValues)
        fParameterStringValues[p];
    
    for(auto p: fKeyFloatValues)
        fParameterFloatValues[p];
    
    for(auto p: fKeyIntValues)
        fParameterIntValues[p];
    
    
    
}

MQconfig::~MQconfig() {
}


    

void MQconfig::SetDoubleValue(string key, double Value)
{
    if(fParameterDoubleValues.count(key))
        fParameterDoubleValues[key]=Value;
    else
    {
        cout<<"[ERROR] given key "<< key <<" is not defined. "<<endl;
        PrintAvailKey(fParameterDoubleValues);
    }
}


void MQconfig::SetFloatValue(string key, float Value)
{
    if(fParameterFloatValues.count(key))
        fParameterFloatValues[key]=Value;
    else
    {
        cout<<"[ERROR] given key "<< key <<" is not defined. "<<endl;
        PrintAvailKey(fParameterFloatValues);
    }
}

void MQconfig::SetIntValue(string key, int Value)
{
    if(fParameterIntValues.count(key))
        fParameterIntValues[key]=Value;
    else
    {
        cout<<"[ERROR] given key "<< key <<" is not defined. "<<endl;
        PrintAvailKey(fParameterIntValues);
    }
}

void MQconfig::SetStringValue(string key, string Value)
{
    if(fParameterStringValues.count(key))
        fParameterStringValues[key]=Value;
    else
    {
        cout<<"[ERROR] given key "<< key <<" is not defined. "<<endl;
        PrintAvailKey(fParameterStringValues);
    }
    
}





double MQconfig::GetDoubleValue(string key)
{
    double val(0.);
    if(fParameterDoubleValues.count(key))
        val=fParameterDoubleValues.at(key);
    else
    {
        cout<<"[ERROR] given key "<< key <<" is not defined. "<<endl;
        PrintAvailKey(fParameterDoubleValues);
    }
    return val;
}

float MQconfig::GetFloatValue(string key)
{
    float val(0.);
    if(fParameterFloatValues.count(key))
        val=fParameterFloatValues.at(key);
    else
    {
        cout<<"[ERROR] given key "<< key <<" is not defined. "<<endl;
        PrintAvailKey(fParameterFloatValues);
    }
    return val;
}

int MQconfig::GetIntValue(string key)
{
    int val(0);
    if(fParameterIntValues.count(key))
        val=fParameterIntValues.at(key);
    else
    {
        cout<<"[ERROR] given key "<< key <<" is not defined. "<<endl;
        PrintAvailKey(fParameterIntValues);
    }
    return val;
}

string MQconfig::GetStringValue(string key)
{
    string val;
    if(fParameterStringValues.count(key))
        val=fParameterStringValues.at(key);
    else
    {
        cout<<"[ERROR] given key "<< key <<" is not defined. "<<endl;
        PrintAvailKey(fParameterStringValues);
    }
    return val;
}







