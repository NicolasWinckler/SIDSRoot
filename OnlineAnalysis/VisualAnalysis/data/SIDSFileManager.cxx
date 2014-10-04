/* 
 * File:   SIDSFileManager.cxx
 * Author: winckler
 * 
 * Created on October 3, 2014, 10:14 PM
 */


#include "SIDSFileManager.h"


SIDSFileManager::SIDSFileManager() : fDirName()
{
    
}

SIDSFileManager::SIDSFileManager(const string &dirname, const vector<string> &fileList)
{
    SetDirectory(dirname);
    SetInputList(fileList);
    GetNonAnalyzedFiles();
}

SIDSFileManager::~SIDSFileManager()
{
    
}


void SIDSFileManager::SetInputList(const vector<string> &fileList )
{
    fInputList=fileList;
    sort(fInputList.begin(), fInputList.end());
    CountDuplicates(fInputList);
}

int SIDSFileManager::SetDirectory(const string &dirname)
{
    fDirName=dirname;
    if(fDirFileList.size()>0)
        fDirFileList.clear();
    DIR *dir = opendir(fDirName.c_str());
    if(!dir)
    {
        return 1;
    }
    
    dirent *entry;
    while(entry = readdir(dir))
    {
        if(has_suffix(entry->d_name, ".root"))
        {
            //cout << entry->d_name << endl;
            fDirFileList.push_back(string(entry->d_name));
        }
    }

    closedir(dir);
    sort(fDirFileList.begin(), fDirFileList.end());
    return 0;
}


bool SIDSFileManager::has_suffix(const string& s, const string& suffix)
{
    return (s.size() >= suffix.size()) && equal(suffix.rbegin(), suffix.rend(), s.rbegin());    
}


void SIDSFileManager::CountDuplicates(const vector<string> &fileList)
{
    vector<string> list=fileList;
    sort(list.begin(), list.end());
    std::vector<string> UniqueFileInList (list.size()); 
    std::vector<string>::iterator it;
    it=std::unique_copy (list.begin(),list.end(),UniqueFileInList.begin());
    UniqueFileInList.resize( std::distance(UniqueFileInList.begin(),it) );
    
    
    for(auto p : UniqueFileInList)
    {
        int counduplicates=0;
        for(auto q : list)
        {
            if(p==q)
            {
                
                
                counduplicates++;
                fAnalyzedFiles[p]=counduplicates;
                if(counduplicates>1)
                    fDuplicatesList[p]=counduplicates;
            }
            
        }
    }
    
}

void SIDSFileManager::GetNonAnalyzedFiles()
{    
    for(auto p : fDirFileList)
    {
        if(fAnalyzedFiles.count(p)==0)
            fNonAnalyzedFiles.push_back(p);
    }
    
}


void SIDSFileManager::PrintAll(bool detail)
{
    MQLOG(INFO)<<"*********************************";
    MQLOG(INFO)<<"***** LIST OF ALL FILES *****";
    MQLOG(INFO)<<"Root files found in directory: "<< fDirName;
    MQLOG(INFO)<<" ";
    for(auto p : fDirFileList)
    {
        MQLOG(INFO)<<p;
    }
    MQLOG(INFO)<<" ";
    MQLOG(INFO)<<"Number of files :"<<fDirFileList.size();
    MQLOG(INFO)<<"*********************************";
}

void SIDSFileManager::PrintAnalyzed(bool detail)
{
    MQLOG(INFO)<<"*********************************";
    MQLOG(INFO)<<"***** LIST OF ALREADY ANALYZED FILES *****";
    MQLOG(INFO)<<" ";
    for(auto p : fAnalyzedFiles)
    {
        if(detail)
            MQLOG(INFO)<<p.second<< " analysis for file "<<p.first ;
        else
            MQLOG(INFO)<<p.first ;
    }
    MQLOG(INFO)<<" ";
    MQLOG(INFO)<<"Number of files :"<<fAnalyzedFiles.size();
    MQLOG(INFO)<<"*********************************";
}

void SIDSFileManager::PrintNotAnalyzed(bool detail)
{
    MQLOG(INFO)<<"*********************************";
    MQLOG(INFO)<<"***** LIST OF NOT YET ANALYZED FILES *****";
    MQLOG(INFO)<<" ";
    for(auto p : fNonAnalyzedFiles)
    {
        MQLOG(INFO)<<p;
    }
    MQLOG(INFO)<<" ";
    MQLOG(INFO)<<"Number of files :"<<fNonAnalyzedFiles.size();
    MQLOG(INFO)<<"*********************************";
}


void SIDSFileManager::PrintDuplicates(bool detail, int anaNr)
{
    if(anaNr<1)
        anaNr=1;
    
    MQLOG(INFO)<<"*********************************";
    MQLOG(INFO)<<"***** LIST FILES ANALYZED MORE THAN "<< anaNr <<" TIMES *****";
    MQLOG(INFO)<<" ";
    for(auto p : fDuplicatesList)
    {
        if(p.second>anaNr)
        {
            if(detail)
                MQLOG(INFO)<<p.second<< " analysis for file "<<p.first ;
            else
                MQLOG(INFO)<<p.first ;
        }
    }
    MQLOG(INFO)<<" ";
    MQLOG(INFO)<<"Number of files :"<<fDuplicatesList.size();
    MQLOG(INFO)<<"*********************************";
    
}





