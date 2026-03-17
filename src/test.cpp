#include<iostream>
#include "utility.inl"

int main()
{
    message("Test1");
    message("Test2",1);
    message("Test3",true);
    message("Test4",0);
    message("Test5",false);

    return 0;
}
