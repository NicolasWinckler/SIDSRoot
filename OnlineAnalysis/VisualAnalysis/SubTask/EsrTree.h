/* 
 * File:   EsrTree.h
 * Author: winckler
 *
 * Created on August 19, 2014, 10:22 AM
 */

#ifndef ESRTREE_H
#define	ESRTREE_H


#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include <string>
#include "EsrInjData.h"
#include <vector>
#include <memory>

class EsrTree 
{
public:
    EsrTree(const string &filename, const string &treename, const string &branchname);
    virtual ~EsrTree();
    
    void UpdateTree(EsrInjData EsrData);
    std::vector<EsrInjData> GetEsrData();
    #ifndef __CINT__
    std::shared_ptr<EsrInjData> GetInjectionData(Long64_t inj);
    #endif //__CINT__
protected:
    string fFileName;
    string fTreeName;
    string fBranchName;
    TFile* fOutFile;
    TTree* fTree;
    EsrInjData* fEsrData;

    ClassDef(EsrTree, 0);
};

#endif	/* ESRTREE_H */

