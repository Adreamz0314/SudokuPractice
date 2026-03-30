#include <iostream>
#include <string>

#include "common.h"
#include "i18n.h"
#include "utility.inl"

// return number of grids to be erased
int inputDifficulty()                                                            //难度选择
{
    cls();                                                                       //根据系统，清除终端内容

    std::string cmd;
    int need_erase_grids = 0;
    while (true)                                                                  //写死循环
    {
        message(I18n::Instance().Get(I18n::Key::ASK_DIFFICULTY));                //输出提示

        std::cin >> cmd;

        try
        {
            Difficulty difficulty = static_cast<Difficulty>(std::stoi(cmd));
            switch (difficulty)
            {
            case Difficulty::EASY:
                need_erase_grids = 20;
                break;
            case Difficulty::NORMAL:
                need_erase_grids = 35;
                break;
            case Difficulty::HARD:
                need_erase_grids = 50;
                break;
            }
        }
        catch(...) 
        {
            need_erase_grids = 0;
        }

        if (need_erase_grids > 0)                                                   //仅当输入结果合法时跳出循环
            break;

        message(I18n::Instance().Get(I18n::Key::INPUT_ERROR));
    }

    return need_erase_grids;                                                    //返回int型擦除格子数
}

KeyMode inputKeyMode() {                                                        //返回keymode类型，按键模式输入
  std::string mode;
  do {
    message(I18n::Instance().Get(I18n::Key::ASK_KEY_MAP));

    std::cin >> mode;

    try {
      KeyMode kmd = static_cast<KeyMode>(std::stoi(mode));
      return kmd;
    } catch (...) {
    }

    message(I18n::Instance().Get(I18n::Key::INPUT_ERROR));
  } while (true);
}

void InputLanguage()                                                            //语言选择
{
  std::string language;
  do
  {
    message("1English 2中文");                                                  //message作用类似于"cout"
    std::cin >> language;

    try
    {
      auto l = static_cast<Language>(std::stoul(language) - 1);
      if (l < Language::MAX) {
        I18n::Instance().SetLanguage(l);
        return;
      }
    }
    catch (...)
    {
    }

    message(I18n::Instance().Get(I18n::Key::INPUT_ERROR));
    //获取当前语言对应的 “输入错误” 提示文本，并通过 message 函数展示给用户；引用多个自定义函数，定义位置未知;


  }

  while (true);
}
