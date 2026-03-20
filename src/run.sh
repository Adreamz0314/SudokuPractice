#!/bin/bash
# 脚本名称：run.sh
# 功能：一键编译并运行SudokuPractice项目

# 第一步：定义变量（方便后续修改文件名）
# 可执行文件名称
OUTPUT="test"
# 需要编译的源文件列表（空格分隔）
SOURCE_FILES="test.cpp scene.cpp"
# 编译选项（-std=c++11 是指定C++11标准，避免兼容性问题）
COMPILE_OPTS="-std=c++11 -Wall"

# 第二步：执行编译命令
echo "===== 开始编译项目 ====="
g++ $SOURCE_FILES $COMPILE_OPTS -o $OUTPUT

# 第三步：检查编译是否成功
# $? 是上一条命令的返回值，0表示成功，非0表示失败
if [ $? -eq 0 ]; then
    echo "===== 编译成功！开始运行程序 ====="
    # 运行生成的可执行文件
    ./$OUTPUT
else
    echo "===== 编译失败！请检查代码错误 ====="
    # 退出脚本，返回非0状态码
    exit 1
fi
