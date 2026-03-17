#pragma once
#include<iostream>
#include<string>

inline void message(const std::string msg="",bool lf=true)
{
    std::cout<<msg;
    if(lf) std::cout<<std::endl;

}
