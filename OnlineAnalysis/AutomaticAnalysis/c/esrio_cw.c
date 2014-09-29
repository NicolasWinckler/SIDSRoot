/************************************************************************** 
 * PV_ESRIO
 * PV-WAVE interface to ESRIO
 *
 * Author:
 *	Paul Buehler
 *	Stefan Meyer Institute
 *	AT-1090 Vienna
 *	paul.buehler@oeaw.ac.at
 *
 * Revisions:
 *
 *	02-09-2014	running under UNIX					pb
 *
 * Functions:
 *
 *
 *************************************************************************/
/* Includes */

#include "esrio.h"
#include "EsrInjData.h"
#include "EsrTree.h"

#include "TROOT.h"
#include "TRint.h"
#include "TClass.h"
#include <TFile.h>
#include <TKey.h>

/*************************************************************************/
EXPORT long getnumdata_cw(int argc, char *argv[])
{

  long    *ndata;
  
  char		**outfname;
  
/* is right number of arguments ? */
  if (argc != 2) return(esrio_badnumarg_err);
    
/* associate input parameters with C-variables */
  outfname  = ((char  ***) argv)[0];
  ndata     = ((long   **) argv)[1];
    
  // get number of InjData
  TFile f(*outfname,"READ");
  TTree* t = (TTree*)f.Get("automatic");
  *ndata = t->GetEntries();
  f.Close();
  
  return(0);
      
}

/**************************************************************************/
EXPORT long writedata_cw(int argc, char *argv[])
{

  long    *ndecays;
  long    ii=0;
  
  long		*qq;
  double	*fmd;
  double	*dtb, *dfb, *foff;
  double	*numions;
  double	*decdata;
  
  char		**outfname;
  char		**datafname;
  
/* is right number of arguments ? */
  if (argc != 10) return(esrio_badnumarg_err);
    
/* associate input parameters with C-variables */
  outfname  = ((char  ***) argv)[0];
  datafname = ((char  ***) argv)[1];
  dtb       = ((double **) argv)[2];
  dfb       = ((double **) argv)[3];
  foff      = ((double **) argv)[4];
  qq        = ((long   **) argv)[5];
  fmd       = ((double **) argv)[6];
  numions   = ((double **) argv)[7];
  ndecays   = ((long   **) argv)[8];
  decdata   = ((double **) argv)[9];
    
  // define EsrInjData item
  EsrInjData DecayData(*datafname,SIDS::kRSA51,*dtb,*dfb,*foff);
  DecayData.SetUserName("automatic");
  DecayData.SetQualityTag(*qq);
  DecayData.SetCoolParentFreq(*(fmd+0));
  DecayData.SetCoolDaughterFreq(*(fmd+1));
  DecayData.SetNParentInit(*(numions+0));
  DecayData.SetNParentEnd(*(numions+1));
  DecayData.SetNDaughterEnd(*(numions+2));

  // add all decay times
  DecayData.SetNEC(*ndecays);
  for (ii=0; ii<*ndecays; ii++) {
  
    EsrDecayEvent Event(SIDS::kECDecay);
    Event.SetDecayTime(decdata[4*ii]);
    Event.SetDecayFreq(decdata[4*ii+1]);
    Event.SetArrivalTime(decdata[4*ii+2]);
    Event.SetArrivalFreq(decdata[4*ii+3]);
    DecayData.AddEvent(Event);
    
  }
    
  // save the InjData to the file
  EsrTree DecayTree(*outfname,"automatic","EsrInjData.");
  DecayTree.UpdateTree(DecayData);
  
  // print out number of InjData
  TFile f(*outfname,"READ");
  TTree* t = (TTree*)f.Get("automatic");
  fprintf(stderr,"Number of InjData: %i\n",t->GetEntries());
  f.Close();
  
  return(0);
      
}

/**************************************************************************/
EXPORT long readdata_cw(int argc, char *argv[])
{

    
  char		**outfname;
  char		**filenames;
  char		**users;
  
  long    *decaytype;
  long		*qq;
	double  *fmd;
	double  *dtbs,*dfbs,*foffs;
  double  *numions;
  double  *decdata;

  long    ddpos = 0;
  TString allusers("//");
  TString allfilenames("//");
  EsrInjData* DecayData = NULL;
   
/* is right number of arguments ? */
  if (argc != 11) return(esrio_badnumarg_err);
    
/* associate input parameters with C-variables
   decaytype:
   0: UnknownDecay, 
   1: ECDecay, 
   2: BetaDecay, 
   3: LossDecay,
   4: Scattering
*/
  outfname  = ((char  ***) argv)[ 0];
  decaytype = ((long   **) argv)[ 1];
  users     = ((char  ***) argv)[ 2];
  filenames = ((char  ***) argv)[ 3];
  dtbs      = ((double **) argv)[ 4];
  dfbs      = ((double **) argv)[ 5];
  foffs     = ((double **) argv)[ 6];
  qq        = ((long   **) argv)[ 7];
  fmd       = ((double **) argv)[ 8];
  numions   = ((double **) argv)[ 9];
  decdata   = ((double **) argv)[10];
  
  // open file and get tree
  TFile f(*outfname,"READ");
  TTree* t = (TTree*)f.Get("automatic");
  
  // if file is ok ...
  if (&f) {
    t->SetBranchAddress("EsrInjData.",&DecayData);
   
    // loop over all InjData
    for (int ii = 0; ii < t->GetEntries(); ii++) {
      t->GetEntry(ii);
      EsrInjData Data_i = *DecayData;
      
      // users
      allusers.Append(Data_i.GetUserName().c_str());
      allusers.Append("//");
     
      // filenames
      allfilenames.Append(Data_i.GetFileName().c_str());
      allfilenames.Append("//");
     
      // find decays of type decaytype
      vector<EsrDecayEvent>decaylist;
      switch ( *decaytype ) {
        case 1 : {
          decaylist = Data_i.GetECData();
          // fprintf(stderr,"Number of ECdecays: %i\n",decaylist.size());
          break;
        }
        case 2 : {
          decaylist = Data_i.GetBetaData();
          // fprintf(stderr,"Number of BETAdecays: %i\n",decaylist.size());
          break;
        }
        case 3 : {
          decaylist = Data_i.GetLossData();
          // fprintf(stderr,"Number of LOSSdecays: %i\n",decaylist.size());
          break;
        }
        default: {
          decaylist = Data_i.GetUnknownData();
          // fprintf(stderr,"Number of UNKNOWNdecays: %i\n",decaylist.size());
          break;
        }
      }
            
      // update dtb,dfb,foff,numions
      *dtbs    = Data_i.GetTimeResolution();    dtbs++;
      *dfbs    = Data_i.GetFreqResolution();    dfbs++;
      *foffs   = Data_i.GetFreqOffset();        foffs++;
      *qq      = Data_i.GetQualityTag();        qq++;
      *(fmd+0) = Data_i.GetCoolParentFreq();
      *(fmd+1) = Data_i.GetCoolDaughterFreq();  fmd += 2;
      *(numions+0) = Data_i.GetNParentInit();
      *(numions+1) = Data_i.GetNParentEnd();
      *(numions+2) = Data_i.GetNDaughterEnd();  numions += 3;
      
      // fill decaylist into decdata
      // decdata is supposed to be an array of (4,esrio_maxnumdecays,n),
      // where n is the number of events
      // a maximum of esrio_maxnumdecays decays per event can thus be
      // copied to decdata
      int n2c = decaylist.size();
      if (decaylist.size() > esrio_maxnumdecays) {
        fprintf(stderr,"<ATTENTION> There are %i decays. Can read maximum %i values only!\n",decaylist.size(),esrio_maxnumdecays);
        n2c = esrio_maxnumdecays;
      }
      for (int jj=0;jj<n2c;jj++) {
        *(decdata+0) = decaylist[jj].GetDecayTime();
        *(decdata+1) = decaylist[jj].GetDecayFreq();
        *(decdata+2) = decaylist[jj].GetArrivalTime();
        *(decdata+3) = decaylist[jj].GetArrivalFreq();
        decdata += 4;
      }
      decdata += 4*(esrio_maxnumdecays-n2c);
    }
  }
  f.Close();
  
  // copy string arrays: allusers,allfilenames
  strncpy(*users,allusers.Data(),allusers.Length());
  strncpy(*filenames,allfilenames.Data(),allfilenames.Length());

  return(0);
      
}

/**************************************************************************/
