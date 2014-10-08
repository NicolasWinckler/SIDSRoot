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
    int MinArg=6;
    int MaxArg=8;
    if (argc < MinArg || argc>MaxArg)
    {
        MQLOG(ERROR)<< "Number of argument incorrect. ("<<argc<<" instead of 6)";
        MQLOG(INFO) << "Script usage: startVisualAnalysis \t showfiles.sh command1 command2 \n";
        MQLOG(INFO) << "Binary usage: ShowStatus treeName branch Directory Resultsfile OutputDir command1 command2 \n"<<endl;
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
    string OutputDir(argv[i]);
    ++i;
    
    MQconfig ConfigParameter;
    ConfigParameter.SetValue("InputDirectory",dirname);
    ConfigParameter.SetValue("InputFile",Resultsfilename);
    ConfigParameter.SetValue("TreeName",treename);
    ConfigParameter.SetValue("Branch",branchname);
    ConfigParameter.SetValue("OutputDirectory",OutputDir);
    
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
    
    
    
    
    int Binning=100;
    ConfigParameter.SetValue("Binning",Binning);
    
    double fx_min=6.0;
    double fx_max=60.0;
    ConfigParameter.SetValue("x_min",fx_min);
    ConfigParameter.SetValue("x_max",fx_max);
    
    double flambdaInit=0.0120;
    double flambda_Max=1.5*flambdaInit;
    double flambda_Min=0.;
    ConfigParameter.SetValue("LambdaInit",flambdaInit);
    ConfigParameter.SetValue("Lambda_Max",flambda_Max);
    ConfigParameter.SetValue("Lambda_Min",flambda_Min);
    
    
    double fampInit=0.1;
    double famp_Max=0.3;
    double famp_Min=-0.3;
    ConfigParameter.SetValue("AmpInit",fampInit);
    ConfigParameter.SetValue("Amp_Max",famp_Max);
    ConfigParameter.SetValue("Amp_Min",famp_Min);
    
    
    double fOmegaInit=0.88;
    double fOmega_Max=7.0;
    double fOmega_Min=0.;
    ConfigParameter.SetValue("OmegaInit",fOmegaInit);
    ConfigParameter.SetValue("Omega_Max",fOmega_Max);
    ConfigParameter.SetValue("Omega_Min",fOmega_Min);
    
    
    double fPhiInit=0.;
    double fPhi_Max=3.14;
    double fPhi_Min=-3.14;
    ConfigParameter.SetValue("PhiInit",fPhiInit);
    ConfigParameter.SetValue("Phi_Max",fPhi_Max);
    ConfigParameter.SetValue("Phi_Min",fPhi_Min);
    
    double fNormFactInit=5.e+3;
    double fNormFact_Max=1.e+10;
    double fNormFact_Min=0.;
    
    ConfigParameter.SetValue("NormFactInit",fNormFactInit);
    ConfigParameter.SetValue("NormFact_Max",fNormFact_Max);
    ConfigParameter.SetValue("NormFact_Min",fNormFact_Min);
    
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


