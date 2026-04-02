#include "scene.h"

#include <memory.h>

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "common.h"
#include "display_symbol.h"
#include "i18n.h"
#include "utility.inl"
#include "color.h"

CScene::CScene(int index)
    : _max_column(static_cast<int>(pow(index, 2)))
    , _cur_point({0, 0})
{
    init();
}

CScene::~CScene()                                                       //析构函数 结束时自动执行 销毁指针（构造于scene.h） 释放内存
{                                                                       //KeyMap *keyMap{};  KeyMap构造于common.h
    if(keyMap) delete keyMap;
}

void CScene::show() const                                               //绘制完整场景
{
    cls();                                                              //适用于不同系统的清屏命令  utility.inl

    printUnderline();                                                   //打印水平分格线

    // 获取光标位置的数字值（若光标在有效位置）
    int highlighted_num = UNSELECTED;                                   //已经提前定义为整数0
    if (_cur_point.y >= 0 && _cur_point.y < _max_column) {
        const CBlock& cursor_block = _row_block[_cur_point.y];
        highlighted_num = cursor_block.getNumberValue(_cur_point.x);            //判断是否为有效位置
    }

    for (int row = 0; row < _max_column; ++row)
    {
        CBlock block = _row_block[row];
        if(_cur_point.y == row) block.print(_cur_point.x, highlighted_num);
        else block.print(-1, highlighted_num);
        printUnderline(row);
    }
}

void CScene::setMode(KeyMode mode)                                              //选择按键操作模式
{
    switch (mode)
    {
    case KeyMode::NORMAL:
        keyMap = new Normal;                                                    //创建指向Normal的指针
        break;

    case KeyMode::VIM:
        keyMap = new Vim;
        break;
    }
}

void CScene::printUnderline(int line_no) const
{                                                                               //打印分格线，以及光标选中时的下划线箭头提示
    auto is_curline = (_cur_point.y == line_no);
    for (int colunm = 0; colunm < 9; ++colunm)
    {                                                                           //此处colunm拼写错误，应该改为column
        if((colunm%3) == 0 || line_no == -1 || (line_no+1)%3 == 0) {
            std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED) << CORNER << Color::Modifier();
        } else {
            std::cout <<  CORNER;                                               //CORNER定义于display_symbol.h,表示十字角点符号
        }
        auto third_symbol = (is_curline && _cur_point.x == colunm) ? ARROW : LINE;
        if(line_no == -1 || (line_no+1)%3 == 0) {
            std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED) << LINE << third_symbol << LINE << Color::Modifier();
        } else {
            std::cout << LINE << third_symbol << LINE;
        }
    }
    std::cout << Color::Modifier(Color::BOLD, Color::BG_DEFAULT, Color::FG_RED) << CORNER << Color::Modifier()<< std::endl;
}

void CScene::init()
{
    memset(_map, UNSELECTED, sizeof(_map));

    int col = 0;
    int row = 0;

    for(col = 0; col < _max_column; ++col)
    {
        CBlock column_block;

        for(row = 0; row < _max_column; ++row)
        {
            column_block.push_back(_map + row * _max_column + col);
        }

        _column_block[col] = column_block;
    }

    for(row = 0; row < _max_column; ++row)
    {
        CBlock row_block;

        for(col = 0; col < _max_column; ++col)
        {
            row_block.push_back(_map + row * _max_column + col);
        }

        _row_block[row] = row_block;
    }


    for(row = 0; row < _max_column; ++row)
    {
        for(col = 0; col < _max_column; ++col)
        {
            _xy_block[row / 3][col / 3].push_back(_map + row * _max_column + col);
        }
    }

    return;
}

bool CScene::setCurValue(const int nCurValue, int &nLastValue)
{
    auto point = _map[_cur_point.x + _cur_point.y * 9];
    if (point.state == State::ERASED)
    {
        nLastValue = point.value;
        setValue(nCurValue);
        return true;
    }
    else
        return false;
}

void CScene::setValue(const point_t& p, const int value)
{
    _map[p.x + p.y * 9].value = value;
}

void CScene::setValue(const int value)
{
    auto p = _cur_point;
    this->setValue(p, value);
}


// 选择count个格子清空
void CScene::eraseRandomGrids(const int count)                      //随机选取一定数量的格子，擦除
{
    point_value_t p = {UNSELECTED, State::ERASED};

    std::vector<int> v(81);
    for (int i = 0; i < 81; ++i) {
        v[i] = i;
    }

    for (int i = 0; i < count; ++i) {
        int r = random(0, static_cast<int>(v.size() - 1));
        _map[v[r]] = p;
        v.erase(v.begin() + r);
    }
}


bool CScene::isComplete()
{
    // 任何一个block未被填满，则肯定未完成
    for (size_t i = 0; i < 81; ++i)
    {
        if (_map[i].value == UNSELECTED)
            return false;
    }

    // 同时block里的数字还要符合规则
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            if (!_row_block[row].isValid() || 
                !_column_block[col].isValid() || 
                !_xy_block[row / 3][col / 3].isValid())
                return false;
        }
    }

    return true;
}

bool CScene::save(const char *filename) {
  auto filepath = std::filesystem::path(filename);
  if (std::filesystem::exists(filepath)) {
    return false;
  }

    std::fstream fs;
    fs.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

    // save _map
    for (int i = 0; i < 81; i++) {
        fs << _map[i].value << ' ' << static_cast<int>(_map[i].state) << std::endl;
    }

    // save _cur_point
    fs << _cur_point.x << ' ' << _cur_point.y << std::endl;

    // save _vCommand
    fs << _vCommand.size() << std::endl;
    for (CCommand command : _vCommand) {
        point_t point = command.getPoint();
        fs << point.x << ' ' << point.y << ' '
           << command.getPreValue() << ' '
           << command.getCurValue() << std::endl;
    }

    fs.close();
    return true;
}

bool CScene::load(const char *filename) {
  auto filepath = std::filesystem::path(filename);
  if (!std::filesystem::exists(filepath)) {
    return false;
  }

    std::fstream fs;
    fs.open(filename, std::fstream::in | std::fstream::out | std::fstream::app);

    // load _map
    for (int i = 0; i < 81; i++) {
        int tmpState;
        fs >> _map[i].value >> tmpState;
        _map[i].state = static_cast<State>(tmpState);
    }

    // load _cur_point
    fs >> _cur_point.x >> _cur_point.y;

    // load _vCommand
    int commandSize;
    fs >> commandSize;
    for (int i = 0; i < commandSize; i++) {
        point_t point;
        int preValue, curValue;
        fs >> point.x >> point.y >> preValue >> curValue;
        _vCommand.emplace_back(this, point, preValue, curValue);
    }
    return true;
}


void CScene::play()                             //玩家交互部分
{
    show();

    char key = '\0';
    while (1)
    {
        key = static_cast<char>(_getch());
        if (key >= '0' && key <= '9')
        {
            CCommand oCommand(this);
            if (!oCommand.execute(key - '0'))
            {
                std::cout << "this number can't be modified." << std::endl;
            }
            else
            {
                _vCommand.push_back(std::move(oCommand));  // XXX: move without move constructor
                show();
                continue;
            }
        }
        if (key == keyMap->ESC)
        {
            message(I18n::Instance().Get(I18n::Key::ASK_QUIT));
            std::string strInput;
            std::cin >> strInput;
            if (strInput[0] == 'y' || strInput[0] == 'Y')
            {
                message(I18n::Instance().Get(I18n::Key::ASK_SAVE));
                std::cin >> strInput;
                if (strInput[0] == 'y' || strInput[0] == 'Y') {
                  do {
                    message(I18n::Instance().Get(I18n::Key::ASK_SAVE_PATH));
                    std::cin >> strInput;
                    if (!save(strInput.c_str())) {
                      message(I18n::Instance().Get(I18n::Key::FILE_EXIST_ERROR));
                    } else {
                      break;
                    }
                  } while (true);
                }
                exit(0);
            } else {
              message(I18n::Instance().Get(I18n::Key::CONTINUE));
            }
        }
        else if (key == keyMap->U)
        {
          if (_vCommand.empty()) {
            message(I18n::Instance().Get(I18n::Key::UNDO_ERROR));
          } else {
            CCommand &oCommand = _vCommand.back();
            oCommand.undo();
            _vCommand.pop_back();
            show();
          }
        }
        else if (key == keyMap->LEFT)
        {
            _cur_point.x = (_cur_point.x - 1) < 0 ? 0 : _cur_point.x - 1;
            show();
        }
        else if (key == keyMap->RIGHT)
        {
            _cur_point.x = (_cur_point.x + 1) > 8 ? 8 : _cur_point.x + 1;
            show();
        }
        else if (key == keyMap->DOWN)
        {
            _cur_point.y = (_cur_point.y + 1) > 8 ? 8 : _cur_point.y + 1;
            show();
        }
        else if (key == keyMap->UP)
        {
            _cur_point.y = (_cur_point.y - 1) < 0 ? 0 : _cur_point.y - 1;
            show();
        }
        else if (key == keyMap->ENTER)
        {
          if (isComplete()) {
            message(I18n::Instance().Get(I18n::Key::CONGRATULATION));
            getchar();
            exit(0);
          } else {
            message(I18n::Instance().Get(I18n::Key::NOT_COMPLETED));
          }
        }
    }
}











// 一个场景可以多次被初始化
void CScene::generate()                                                     //场景生成
{
    std::vector<std::vector<int>> matrix;
    for (int i = 0; i < 9; i++)
        matrix.push_back(std::vector<int>(9, 0));                           //创建一个9x9的空矩阵

    // 初始化三个nuit
    // 2 6 5 | 0 0 0 | 0 0 0
    // 3 4 1 | 0 0 0 | 0 0 0
    // 8 9 7 | 0 0 0 | 0 0 0
    // ---------------------
    // 0 0 0 | 1 9 4 | 0 0 0
    // 0 0 0 | 8 3 6 | 0 0 0
    // 0 0 0 | 5 2 7 | 0 0 0
    // ---------------------
    // 0 0 0 | 0 0 0 | 3 4 5
    // 0 0 0 | 0 0 0 | 9 6 2
    // 0 0 0 | 0 0 0 | 7 8 1
    for (int num = 0; num < 3; num++)                                       //循环三次，生成三个初始区块
    {
        std::vector<int> unit = shuffle_unit();                             //来源 utility.inl
        int start_index = num * 3;                                          //用于确定matrix[i][j]的位置
        for (int i = start_index; i < start_index+3; i++)
            for (int j = start_index; j < start_index+3; j++)
            {
                matrix[i][j] = unit.back();                                //把unit数组中最后一个元素输入到matrix[i][j]中
                unit.pop_back();                                           //清除unit数组的最后一个元素（已经输入）
            }
    }

    // 统计空格数量
    std::vector<std::tuple<int, int>> box_list;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (matrix[i][j] == 0)
                box_list.push_back(std::make_tuple(i, j));               //记录空格坐标

    // 逐个填充空格
    std::map<std::string, std::vector<int>> available_num {};
    int full_num = 0;
    int empty_num = static_cast<int>(box_list.size());
    while (full_num < empty_num)
    {
        std::tuple<int, int> position = box_list[full_num];
        int row = std::get<0>(position);
        int col = std::get<1>(position);
        std::vector<int> able_unit;
        std::string key = std::to_string(row) + "x" + std::to_string(col);
        if (available_num.find(key) == available_num.end())                    //如果没有找到指定的key
        {
            // 九宫格
            able_unit = get_unit();                                            //来自utility.inl,包含从1到9的vector
            for(int i=row/3*3; i<row/3*3+3; i++){                              //0,1,2；3,4,5；6,7,8. 循环三次
                for(int j=col/3*3; j<col/3*3+3; j++){
                    able_unit.erase(std::remove(able_unit.begin(), able_unit.end(), matrix[i][j]), able_unit.end());
                }
            }
            // 行
            for (int i = 0; i < 9; i++)
                if (matrix[row][i] != 0)
                    able_unit.erase(std::remove(able_unit.begin(), able_unit.end(), matrix[row][i]), able_unit.end());                                          //擦除操作
            // 列
            for (int i = 0; i < 9; i++)
                if (matrix[i][col] != 0)
                    able_unit.erase(std::remove(able_unit.begin(), able_unit.end(), matrix[i][col]), able_unit.end());
            available_num[key] = able_unit;
        }
        else
        {
            able_unit = available_num[key];
        }

        // 如果没有可用的数字，则回溯
        if (available_num[key].size() <= 0)
        {
            full_num -= 1;
            if (available_num.find(key) != available_num.end())
                available_num.erase(key);
            matrix[row][col] = 0;
            continue;
        }
        else
        {
            matrix[row][col] = available_num[key].back();
            available_num[key].pop_back();
            full_num += 1;
        }

    }

    // 填入场景
    for (int row = 0; row < 9; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            point_t point = {row, col};
            setValue(point, matrix[row][col]);
        }
    }

    assert(isComplete());

    return;
}

                                                        //generate部分结束








bool CScene::setPointValue(const point_t &stPoint, const int nValue)
{
    auto point = _map[stPoint.x + stPoint.y * 9];
    if (State::ERASED == point.state)
    {
        _cur_point = stPoint;
        setValue(nValue);
        return true;
    }
    else
        return false;
}

point_t CScene::getCurPoint()
{
    return _cur_point;
}
