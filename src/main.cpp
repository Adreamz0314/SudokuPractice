#include<iostream>
#include "utility.inl"
#include "scene.h"

int main()
{
    SetSystemEnv();                                             //设置环境编码
    Cscene scene;                                               //声明成员
    IputLanguage();                                             //语言选择
    int EraseGridNumber=InputDifficuty();                       //难度选择
    scene.generate;                                             //数独界面生成
    scene.erase(EraseGridNumber);                               //按照难度擦除格子
    scene.play;                                                 //用户游玩交互

    return 0;
}
