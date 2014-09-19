/* 
 * File:   MQconfig.h
 * Author: winckler
 *
 * Created on August 14, 2014, 4:04 PM
 */

#ifndef MQCONFIG_H
#define	MQCONFIG_H

#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdlib.h>

using namespace std;

class MQconfig 
{
public:
    MQconfig();
    virtual ~MQconfig();
    
    
    
    string GetID() const { return fID;}
    void SetID(string ID) { fID=ID;}
    
    string GetUserName() const { return fUserName;}
    void SetUserName(string UserName) {fUserName=UserName;}
    
    string GetInputFile() const { return fInputFile;}
    void SetInputFile(string InputFile) {fInputFile=InputFile;}
    
    string GetOutputFile() const { return fOutputFile;}
    void SetOutputFile(string OutputFile) {fOutputFile=OutputFile;}
    
    string GetTreeName() const { return fTreeName;}
    void SetTreeName(string TreeName) {fTreeName=TreeName;}
    
    string GetParameterFile() const { return fParameterFile;}
    void SetParameterFile(string ParameterFile) { fParameterFile=ParameterFile;}
    
    string GetBranch() const { return fBranch;}
    void SetBranch(string Branch) { fBranch=Branch;}
    
    int GetEventRate() const { return fEventRate;}
    void SetEventRate(int EventRate) { fEventRate=EventRate;}
    
    int GetNumIoThreads() const { return fNumIoThreads;}
    void SetNumIoThreads(int NumIoThreads) { fNumIoThreads=NumIoThreads;}
    
    int GetNumInputs() const { return fNumInputs;}
    void SetNumInputs(int NumInputs) { fNumInputs=NumInputs;}
    
    int GetNumOutputs() const { return fNumOutputs;}
    void SetNumOutputs(int NumOutputs) { fNumOutputs=NumOutputs;}
    
    string GetOutputSocketType() const { return fOutputSocketType;}
    void SetOutputSocketType(string OutputSocketType) { fOutputSocketType=OutputSocketType;}
    
    int GetOutputSndBufSize() const { return fOutputSndBufSize;}
    void SetOutputSndBufSize(int OutputSndBufSize) { fOutputSndBufSize=OutputSndBufSize;}
    
    string GetOutputMethod() const { return fOutputMethod;}
    void SetOutputMethod(string OutputMethod) { fOutputMethod=OutputMethod;}
    
    string GetOutputAddress() const { return fOutputAddress;}
    void SetOutputAddress(string OutputAddress) { fOutputAddress=OutputAddress;}
    
    string GetPayloadFormat() const { return fPayloadFormat;}
    void SetPayloadFormat(string PayloadFormat) { fPayloadFormat=PayloadFormat;}
    
    
    
    double GetDoubleValue(string key);
    float GetFloatValue(string key);
    int GetIntValue(string key);
    string GetStringValue(string key);
    
    
    void SetDoubleValue(string key, double Value);
    void SetFloatValue(string key, float Value);
    void SetIntValue(string key, int Value);
    void SetStringValue(string key, string Value);
    
    
    
    
protected:
    string fID;
    string fUserName;
    string fInputFile;
    string fOutputFile;
    string fTreeName;
    string fParameterFile;
    string fBranch;
    int fEventRate;
    int fNumIoThreads;
    int fNumInputs;
    int fNumOutputs;
    string fOutputSocketType;
    int fOutputSndBufSize;
    string fOutputMethod;
    string fOutputAddress;
    string fPayloadFormat;
    
    map<string, int> fParameterIntValues;
    map<string, double> fParameterDoubleValues;
    map<string, float> fParameterFloatValues;
    map<string, string> fParameterStringValues;
    
    vector<string> fKeyIntValues;
    vector<string> fKeyDoubleValues;
    vector<string> fKeyFloatValues;
    vector<string> fKeyStringValues;
    
    
    template<class T> void PrintAvailKey(T mymap)
    {
        cout<<"[INFO] Available keys in map are:"<<endl;
        for(auto p : mymap)
            cout<<"[INFO] "<<p.first<<' '<<p.second<<endl;
    }
    
};

#endif	/* MQCONFIG_H */

