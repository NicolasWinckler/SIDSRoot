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

SIDSFileManager::~SIDSFileManager()
{
    
}


void SIDSFileManager::SetInputList(const vector<string> &fileList )
{
    fInputList=fileList;
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
            cout << entry->d_name << endl;
            fDirFileList.push_back(string(entry->d_name));
        }
    }

    closedir(dir);
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
        cout<<p<<endl;
        int counduplicates=0;
        for(auto q : list)
        {
            if(p==q)
            {
                counduplicates++;
                fAnalyzedFiles[p]=counduplicates;
            }
        }
    }
    
    
    //*
    for(auto r : fAnalyzedFiles)
    {
        cout<<"[INFO] "<<r.first<<" analyzed "<<r.second << " times." <<endl;
    }
    //*/
    
}

void SIDSFileManager::Compare()
{
    vector<string> NonAnalyzedFiles;
    
    for(auto p : fDirFileList)
    {
        if(fAnalyzedFiles.count(p)==0)
            NonAnalyzedFiles.push_back(p);
    }
    cout<<"Files not analized:"<<endl;
    for(auto p : NonAnalyzedFiles)
    {
        cout<<p<<endl;
    }
}
