/* 
 * File:   MQconfig.cxx
 * Author: winckler
 * 
 * Created on August 14, 2014, 4:04 PM
 */


#include "MQconfig.h"

MQconfig::MQconfig() 
{
    /////////////////////////////////////////////////////////////////////
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
    fKeyStringValues.push_back("InputDirectory");
    fKeyStringValues.push_back("OutputDirectory");
    
    /////////////////////////////////////////////////////////////////////
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
    fKeyIntValues.push_back("BinZoomTH2Window");
    fKeyIntValues.push_back("BinningTraces");
    fKeyIntValues.push_back("BinningFreqTH2");
    
    
    fKeyIntValues.push_back("Binning");
    
    
    /////////////////////////////////////////////////////////////////////
    /// Define double keys
    //exp related
    fKeyDoubleValues.push_back("BinSigmaPeak");
    fKeyDoubleValues.push_back("ThresholdPeak");
    fKeyDoubleValues.push_back("Zmax");
    fKeyDoubleValues.push_back("ZSliderScale");
    
    
    // fit par
    fKeyDoubleValues.push_back("x_min");
    fKeyDoubleValues.push_back("x_max");
    
    fKeyDoubleValues.push_back("LambdaInit");
    fKeyDoubleValues.push_back("Lambda_Max");
    fKeyDoubleValues.push_back("Lambda_Min");
    
    fKeyDoubleValues.push_back("NormFactInit");
    fKeyDoubleValues.push_back("NormFact_Max");
    fKeyDoubleValues.push_back("NormFact_Min");
    
    fKeyDoubleValues.push_back("AmpInit");
    fKeyDoubleValues.push_back("Amp_Max");
    fKeyDoubleValues.push_back("Amp_Min");
    
    fKeyDoubleValues.push_back("OmegaInit");
    fKeyDoubleValues.push_back("Omega_Max");
    fKeyDoubleValues.push_back("Omega_Min");
    
    fKeyDoubleValues.push_back("PhiInit");
    fKeyDoubleValues.push_back("Phi_Max");
    fKeyDoubleValues.push_back("Phi_Min");
    
    
    
    
    /////////////////////////////////////////////////////////////////////
    /// create and init keys in maps
    
    for(auto p: fKeyIntValues)
        fContainer[p];
    for(auto p: fKeyDoubleValues)
        fContainer[p];
    for(auto p: fKeyStringValues)
        fContainer[p];
    
}

MQconfig::~MQconfig() 
{   
}

bool MQconfig::is_empty(const any & operand)
{
    return operand.empty();
}

bool MQconfig::is_int(const any & operand)
{
    return operand.type() == typeid(int);
}

bool MQconfig::is_double(const any & operand)
{
    return operand.type() == typeid(double);
}

bool MQconfig::is_float(const any & operand)
{
    return operand.type() == typeid(float);
}

bool MQconfig::is_char_ptr(const any & operand)
{
    try
    {
        any_cast<const char *>(operand);
        return true;
    }
    catch(const boost::bad_any_cast &)
    {
        return false;
    }
}

bool MQconfig::is_string(const any & operand)
{
    return any_cast<string>(&operand);
}

