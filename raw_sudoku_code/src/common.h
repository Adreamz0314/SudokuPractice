#ifndef _SUDOKU_COMMON_H_
#define _SUDOKU_COMMON_H_

static const unsigned int UNSELECTED = 0; // 本文件定义未选择为整数0

enum class Difficulty : int              // 强类型枚举  难度分级
{
    EASY = 1,
    NORMAL,
    HARD
};

enum class State : int                  // 强类型枚举  输入状态
{
    INITED = 0,                         // 输入
    ERASED,                             // 擦除
};

enum class KeyMode : int                // 按键模式
{
    NORMAL = 1,
    VIM
};

struct KeyMap                           // 结构体 为按键赋值十六进制数
{
    const char ESC = 0x1B;
    const char U = 0x75;
    char UP;
    char LEFT;
    char DOWN;
    char RIGHT;
    const char ENTER = 0x0D;
};

struct Normal : KeyMap                  // 结构体 为normal按键模式下的按键赋值，继承前一个结构体KeyMap
{
    Normal()
    {
        UP = 0x77;
        LEFT = 0x61;
        DOWN = 0x73;
        RIGHT = 0x64;
    }
};

struct Vim : KeyMap {                   // 结构体 为vim按键模式下的按键赋值，继承前一个结构体KeyMap
    Vim()
    {
        UP = 0x6B;
        LEFT = 0x68;
        DOWN = 0x6A;
        RIGHT = 0x6C;
    }
};

using point_t = struct point_t {        // 定义一个新的结构体，同时为他赋予别名（然而别名本名相同不知何意）
    int x;
    int y;
};

using point_value_t = struct point_value_t {
    int value;
    State state;
};

class CPointSort                        // 结构体 用于判断两点坐标是否相同
{
  public:
    bool operator()(const point_t &lhs, const point_t &rhs) const
    {
        if ((lhs.x == rhs.x) && (lhs.y == rhs.y))
            return false;
        else
            return true;
    }
};

#endif
