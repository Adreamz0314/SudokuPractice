#include<iostream>
#include "utility.inl"
#include "scene.h"

int main()
{

    std::cout<<std::endl;
    message("Test1");
    message("Test2",1);
    message("Test3",true);
    message("Test4",0);
    message("Test5",false);

    CScene test;
      for(int i=0; i<test.size(); i++)
      {
        for(int j=0; j<test[i].size(); j++)
        {
            std::cout << test[i][j] << " ";
        }
      }
        std::cout << std::endl;

    return 0;
}
