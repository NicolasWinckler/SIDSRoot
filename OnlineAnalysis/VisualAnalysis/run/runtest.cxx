/* 
 * File:   runtest.cxx
 * Author: winckler
 *
 * Created on September 16, 2014, 3:57 PM
 */

#include <cstdlib>
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


using namespace std;



int RootFileManager(TFile* rootfile);

void SeekObject(TKey *key);

void ReadDir(TDirectory *dir);



/*
 * 
 */
int main(int argc, char** argv) {

    
    TFile* rootfile = new TFile("/data.local2/software/SIDSRoot/build/example.root");
    
    RootFileManager(rootfile);
    return 0;
}



int RootFileManager(TFile* rootfile)
{

    if (!rootfile->IsOpen()) 
    {
      std::cout<<"Cannot open file "<<std::endl;
      return 0;
    }

    TList* list = rootfile->GetListOfKeys() ;
    if (!list) 
    { 
          std::cout<<"No key found in file "<<std::endl;
          return 0;
    }
    
    
    TDirectory *CurrentDir=gDirectory;
    ReadDir(CurrentDir);
    return 1;
}

void SeekObject(TKey *key)
{
    TObject* obj ;
    const TGPicture *pic = 0;
    TGListTreeItem *item;
    //TH1F *h = (TH1F*)gDirectory->Get("myHist");
    obj = key->ReadObj();
    if ((strcmp(obj->IsA()->GetName(),"TProfile")!=0)
            && (!obj->InheritsFrom("TH2"))
            && (!obj->InheritsFrom("TH1"))  
            && (!obj->InheritsFrom("Header"))
            ) 
    {
        printf("<W> Object %s is not 1D or 2D histogram : "
           "will not be converted\n",obj->GetName()) ;
    }
    printf("Histo name:%s title:%s\n",obj->GetName(),obj->GetTitle());
    
    /// SEARCH FOR HEADERS
    if(obj->InheritsFrom("Header"))
    {
        Header* RsaHeader=NULL;
        RsaHeader=(Header*)gDirectory->Get(obj->GetName());
        //std::cout<<"Header name = "<<obj->GetName()<<std::endl;
        
        
        
        RsaHeader->Show();
    }
    
}

void ReadDir(TDirectory *dir) 
{
   //ndirs++;
   TDirectory *dirsav = gDirectory;
   TIter next(dir->GetListOfKeys());
   TKey *key;
   while ((key = (TKey*)next())) 
   {
      if (key->IsFolder()) 
      {
         dir->cd(key->GetName());
         TDirectory *subdir = gDirectory;
         ReadDir(subdir);
         dirsav->cd();
         continue;
      }
      else
          SeekObject(key);
   }
}