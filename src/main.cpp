#include <cstring>
#include <iostream>

#include "i18n.h"
#include "input.h"
#include "scene.h"
#include "system_env.hpp"
#include "utility.inl"

static void printHelp() {                           //帮助提示
    std::cout << std::endl;
    std::cout << "sudoku - a little game in command line" << std::endl
              << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "\t sudoku [-l <progressFile>]" << std::endl << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t -l <path> \t specify path of progress file to load, optional." << std::endl
              << std::endl;
}

int main(int argc, char **argv)
{
  SetSystemEnv();                                   //Windows环境下修改控制台编码为utf8防止乱码

  CScene scene;                                     // 声明一个名为scence的CScene类

  if (argc == 1)                                    //只运行未传入参数，即未加载存档
  {
    InputLanguage();                                //加载语言
    int eraseGridNumber = inputDifficulty();        //难度选择
    scene.generate();                               //生成数独程序界面，主要
    scene.eraseRandomGrids(eraseGridNumber);        //随机选取一定数量的格子，擦除
  }
    else if (argc == 3 && !strcmp(argv[1], "-l")) {
    // load saved game progress
    if (!scene.load(argv[2])) {
      message(I18n::Instance().Get(I18n::Key::LOAD_PROGRESS_FAIL));
      return 0;
    }
    InputLanguage();
  }
  else
  {
    printHelp();
    return 0;
  }

  scene.setMode(inputKeyMode());                    //选择按键操作模式

  scene.play();                                     //玩家交互部分

  return 0;
}
