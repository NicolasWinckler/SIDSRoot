/* 
 * File:   runtest.cxx
 * Author: winckler
 *
 * Created on September 16, 2014, 3:57 PM
 */

#include <cstdlib>
#include <vector>
#include <map>
#include <string>

#include "TKey.h"
#include "TROOT.h"
#include "TApplication.h"
#include "TSystem.h"
#include "TGFrame.h"
#include "TGButton.h"
#include "TGLabel.h"
#include "TGMenu.h"
#include "TGFileDialog.h"
#include "TBrowser.h"
#include "TRootEmbeddedCanvas.h"
#include "TRootHelpDialog.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF2.h"
#include "TGraph.h"
#include "TImage.h"
#include "TRandom.h"
#include "TGMsgBox.h"
#include "TGPicture.h"
#include "TGListTree.h"
#include "TObjString.h"
#include "TMessage.h"
#include "TTimer.h"
#include "TGDNDManager.h"
#include "TString.h"
#include "TFile.h"
#include "TClass.h"
#include "Riostream.h"
#include "header.h"

#include "EsrTree.h"

#include "dirent.h"
#include <unordered_set>
#include <iterator>
#include <algorithm>

#include "SIDSFileManager.h"

using namespace std;




/*
 * 
 */
int main(int argc, char** argv) 
{

    int i=666;
    
    int* ptr=&i;
    shared_ptr<int> shptr(ptr) ;
    
    
    int *ptr2=shptr.get();
    //*ptr2=999;
    
    cout<<"I="<<i<<endl;
    return 0;
}

