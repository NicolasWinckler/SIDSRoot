/* 
 * File:   SIDSFileManager.h
 * Author: winckler
 *
 * Created on October 3, 2014, 10:14 PM
 */

#ifndef SIDSFILEMANAGER_H
#define	SIDSFILEMANAGER_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "FairMQLogger.h"
#include "dirent.h"
#include <unordered_set>
#include <iterator>
#include <algorithm>

using namespace std;

class show_copies {
    std::unordered_set<std::string> existing;
public:
    bool operator()(std::string const &in) {
        return existing.insert(in).second;
    }
};



class SIDSFileManager
{
public:
    enum
    {
        kPrintAll,
        kPrintAnalyzed,
        kPrintNotAnalyzed,
        kDuplicates,
        kDetail
    };
    
    SIDSFileManager();
    SIDSFileManager(const string &dirname, const vector<string> &fileList);
    virtual ~SIDSFileManager();
    
    void PrintAll(bool detail=false);
    void PrintAnalyzed(bool detail=false);
    void PrintNotAnalyzed(bool detail=false);
    void PrintDuplicates(bool detail=false, int anaNr=1);
    
    map<string,int> GetDuplicatesList()
    {
        return fDuplicatesList;
    }
    
protected:
    string fDirName;
    vector<string> fDirFileList;
    vector<string> fInputList;
    map<string,int> fAnalyzedFiles;
    vector<string> fNonAnalyzedFiles;
    map<string,int> fDuplicatesList;
    
    void SetInputList(const vector<string> &fileList );
    int SetDirectory(const string &dir);
    void GetNonAnalyzedFiles();
    bool has_suffix(const string& s, const string& suffix);
    void CountDuplicates(const vector<string> &fileList);
};

#endif	/* SIDSFILEMANAGER_H */

