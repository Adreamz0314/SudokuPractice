#pragma once
#include<iostream>
#include<string>

inline void message()
{
    std::cout<<std::endl;
}

template<typename T>
inline void message(const T&msg,bool lf=true)
{
    std::cout<<msg;
    if(lf) std::cout<<std::endl;

}

