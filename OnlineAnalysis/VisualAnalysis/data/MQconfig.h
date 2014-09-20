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
#include <list>

using namespace std;
#ifndef __CINT__
#include <boost/any.hpp>
using boost::any_cast;
typedef boost::any any;
typedef map<string,any> many;
#endif




class MQconfig 
{
public:
    MQconfig();
    virtual ~MQconfig();
    
    
#ifndef __CINT__
    void SetValue(const string & key, const any & Value)
    {
        if(fContainer.count(key))
            fContainer[key]=Value;
        else
        {
            cout<<"[ERROR] given key "<< key <<" is not defined. "<<endl;
            PrintAvailKeys(fContainer);
        }
    }
    
    template<class T> T GetValue(const string & key)
    {
        T val;
        if(!is_empty(fContainer[key]))
        {
            if(is_int(fContainer[key]) 
                    || is_double(fContainer[key])
                    || is_float(fContainer[key])
                    || is_char_ptr(fContainer[key])
                    || is_string(fContainer[key]))
                val=Convert<T>(key);
            else
            {
                cout<<"[ERROR] type not supported"<<endl;
               
            }
        }
        else
            cout<<"[ERROR] key "<<key <<" is empty"<<endl;
        return val;
    }
    
    
    string GetStringVal(const string & key)
    {
        string val;
        if(is_string(fContainer[key]))
            val=Convert<string>(key);
        return val;
    }
    
    int GetIntVal(const string & key)
    {
        int val=0;
        if(is_int(fContainer[key]))
            val=Convert<int>(key);
        return val;
    }
    
    double GetDoubleVal(const string & key)
    {
        double val=0.0;
        
        if(is_double(fContainer[key]))
            val=Convert<double>(key);
        return val;
    }
    
#endif
    
protected:
    
    
    vector<string> fKeyIntValues;
    vector<string> fKeyDoubleValues;
    vector<string> fKeyFloatValues;
    vector<string> fKeyStringValues;
    
    
    template<class U> void PrintAvailKey(U mymap)
    {
        cout<<"[INFO] Available keys in map are:"<<endl;
        for(auto p : mymap)
            cout<<"[INFO] "<<p.first<<' '<<p.second<<endl;
    }
    
    
#ifndef __CINT__
    many fContainer;
    void PrintAvailKeys(many mymap)
    {
        cout<<"[INFO] Available keys in map are:"<<endl;
        for(auto p : mymap)
        {
            cout<<"[INFO] ";
            
            string key=any_cast<string>(p.first);
            cout<<key<<" = ";
            
            if(is_int(p.second))
                cout<<any_cast<int>(p.second);
            
            if(is_string(p.second))
                cout<<any_cast<string>(p.second);
            
            if(is_double(p.second))
                cout<<any_cast<double>(p.second);
            
            
            cout<<endl;
        }
    }
    
    template<class T> T Convert(const string & key)
    {
        T val;
        if(fContainer.count(key))
        {
            val=any_cast<T>(fContainer[key]);
            //cout<<"Key "<<key<<" has value "<<val<<endl;
        }
        else
        {
            cout<<"[ERROR] given key "<< key <<" is not defined. "<<endl;
            PrintAvailKeys(fContainer);
        }
        
        return val;
    }
    
    
    bool is_empty(const any & operand)
    {
        return operand.empty();
    }

    bool is_int(const any & operand)
    {
        return operand.type() == typeid(int);
    }

    bool is_double(const any & operand)
    {
        return operand.type() == typeid(double);
    }
    
    bool is_float(const any & operand)
    {
        return operand.type() == typeid(float);
    }
    
    bool is_char_ptr(const any & operand)
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
    
    bool is_string(const any & operand)
    {
        return any_cast<string>(&operand);
    }
    
#endif//cint
};

#endif	/* MQCONFIG_H */

