/* 
 * File:   runtest.cxx
 * Author: winckler
 *
 * Created on September 16, 2014, 3:57 PM
 */

#include <iostream>
#include <type_traits>




namespace MyData
{

    struct StandardLayout1 {};
    struct Base
    {
      public:
          Base()= default;
        double fa1;
        double fa2;
    };

    struct Derived1 :  Base
    {
      public:
        int fX1;
        int fY1;
        int fZ1;
    };

    struct Derived2 :  Base
    {
        Derived2()= default;
        int fIdx;
        double fX2;
        double fY2;
        double fZ2;
    };
    
    
    struct Derived3
    {
        //Derived3()= default;
        int fIdx;
        double fX3;
        double fY3;
        double fZ3;
        Base B3;
    };
}



int main(int argc, char** argv) 
{
/*
    if(std::is_pod<MyData::Base>::value)
    {
        std::cout<<"Base is POD" << std::endl;
    }
    
    if(std::is_trivially_copyable<MyData::Derived1>::value)
    {
        std::cout<<"Derived1 is trivial" << std::endl;
    }
    
    if(std::is_pod<MyData::Derived2>::value)
    {
        std::cout<<"Derived2 is POD" << std::endl;
    }
    
    if(std::is_pod<MyData::Derived3>::value)
    {
        std::cout<<"Derived3 is POD" << std::endl;
    }
    
    */
    
    return 0;
}

