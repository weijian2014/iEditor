# 特点

- 使用 VSCode + Cmake
- 纯代码编写界面
- 不使用系统 API，跨平台

# 演示

![alt text](doc/images/demo.gif)

# 想法

- 使用 cmake 管理 C C++工程
- 国际化，实现中英文切换
- 完善 C/C++的 IDE 还需要投入大量时间，做出来也不会比 VSCode 好，转为 Markdown 编辑器？

# 版本

## v1.1.1

- 更新 Qt 版本 4.8.4 至 6.6.2
- 修复大量 BUG
- 去掉 project 管理
- 使用 C++11，包含 std::mutex, std::shared_ptr, std::call_once，lambda 函数等使用

## v1.1.0

- 2013 年 6 月 1 日： 基于毕业设计发布 ieditor v1.1.0

# QT 开发注意事项

- QT 的资源文件会编译成为二进制存储在可执行文件中，它是只读的
- 设置了 parent 的 QObject 对象在 parent 释放时会先主动释放 QObject 对象
- new 出来的 QBoxLayout 系列的类对象会自动析构
