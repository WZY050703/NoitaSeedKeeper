# Noita种子管理器
这是一个命令行工具，帮你记住noita有趣的种子。
### 使用
Options:   
    -D/-debug                       列出所有传入命令  
    -d/-delet [ID]                  根据ID删掉一条记录  
    -f/find [Model(1/2/3)] [Item]   找一条记录  
        Model 1: By ID              模式一：通过ID   
        Model 2: By seed            模式二：通过种子（必须完整输入）   
        Model 3: By description     模式三：通过描述（可以只输入词语）   
    -h/-help                        显示命令帮助（由于编码问题，help是英文界面）   
    -l/-list                        列出所有记录   
    -s/-save [seed] [description]   保存一个种子和相关描述（自动分配ID，描述可缺省）   
    -exit                           退出（在执行完所有命令以后）
### 安装
需要下载源码编译
```cmd
g++.exe -g NoitaSeedKeeper.cpp -o NSK.exe
```
### 更新内容
1. 描述缺省
2. 通过时间随机ID
3. 支持多次命令输入（直接打开不传入参数）
### 目前缺陷
1. 搜索：只会返回第一条符合的记录
2. *一次输入有多次保存操作时有可能会产生同一个ID（暂时无法复现出来）
### 更新方向
1. 完善，消除以上缺陷
2. 尝试加入更多功能，比如种子计算器，自动保存炼金配方等
### 开源
本项目使用MIT开源，欢迎二次开发
