#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <memory>

// 功能函数
std::string IDGet_time() // 根据时间的ID生成器
{
    auto seedtime = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(seedtime); // 随机引擎
    std::string ID;
    for (int i = 0; i < 5; i++)
    {
        int tmp = rng() % 36;
        if (tmp < 10)
        {
            ID = ID + char(48 + tmp);
        }
        else
        {
            ID = ID + char(97 + tmp - 10);
        }
    }
    return ID;
}

bool if_file_exist()
{
    std::fstream f;
    f.open("seeds.ini");
    if (!f.good()) // 检查存在
    {
        // std::cout<<"Not exit"<<std::endl;
        f.close();
        return false;
    }
    f.close();
    return true;
}

std::string reverse(int len, std::string str) // 字符串反转
{
    std::string newstr;
    for (int i = len - 1; i >= 0; i--)
    {
        newstr = newstr + str[i];
    }
    return newstr;
}

std::string filesearch(std::string model, std::string item)
{
    std::fstream f;
    std::string data;
    f.open("seeds.ini", std::ios::in);
    f >> data;
    f.close();
    if (model == "1")
    { // ID模式
        int i = data.find(";" + item + ",") + 1;
        if (i - 1 == std::string::npos)
        {
            // std::cout << "Str not exit." << std::endl;
            return "FINDERR:-1";
        }
        std::string backdata;
        while (data[i] != ';')
        {
            backdata = backdata + data[i];
            i = i + 1;
        }
        return backdata;
    }
    if (model == "2")
    { // 种子搜索
        int n = data.find("," + item + ",") + 1;
        if (n - 1 == std::string::npos)
        {
            // std::cout << "Str not exit." << std::endl;
            return "FINDERR:-1";
        }
        std::string output_b, output_a; // before,after
        int i = n;
        while (data[i] != ';')
        {
            output_a = output_a + data[i];
            i = i + 1;
        }
        int j = n - 1;
        while (data[j] != ';')
        {
            output_b = output_b + data[j];
            j = j - 1;
        }
        reverse(output_b.size(), output_b);
        return reverse(output_b.size(), output_b) + output_a;
    }
    if (model == "3")
    { // 描述搜索
        int n = data.find(item);
        if (n - 1 == std::string::npos)
        {
            // std::cout << "Str not exit." << std::endl;
            return "FINDERR:-1";
        }
        std::string output_b, output_a; // before,after
        int i = n;
        while (data[i] != ';')
        {
            output_a = output_a + data[i];
            i = i + 1;
        }
        int j = n - 1;
        while (data[j] != ';')
        {
            output_b = output_b + data[j];
            j = j - 1;
        }
        return reverse(output_b.size(), output_b) + output_a;
    }
    return "FINDERR:-1";
}

int save_init()
{
    if (!if_file_exist()) // 不存在就创建
    {
        std::fstream f;
        f.open("seeds.ini", std::ios::app);
        f << ";";
        f.close();
        return 1;
    }
    return 0;
}

std::unique_ptr<std::string[]> instaer(int &argc) // 输入格式化
{
    // 输入
    std::string input;
    std::cout << "NSK>";
    std::getline(std::cin, input);
    if (!input.empty() && '\n' == input.back())
    {
        input.pop_back();
    }
    // 数个数
    int num = 0, len = input.size();
    bool ischar = false;
    for (int i = 0; i < len; i++)
    {
        int ch = int(input[i]);
        if ((int('A') <= ch && ch <= int('Z')) ||
            (int('a') <= ch && ch <= int('z')) ||
            (int('0') <= ch && ch <= int('9')))
        {
            ischar = true;
        }
        if (' ' == input[i] && true == ischar)
        {
            num++;
            ischar = false;
        }
    }
    if (ischar)
    {
        num++;
        ischar = false;
    }
    if (num == 0)
    {
        std::unique_ptr<std::string[]> re(new std::string[1]);
        re[0] = "-1";
        return re;
    }
    // 分割
    int floor = 0;
    // std::string *commands = new std::string[num];
    std::unique_ptr<std::string[]> commands(new std::string[num]); // 使用智能指针自动回收
    for (int i = 0; i < len; i++)
    {
        int ch = int(input[i]);
        if ((int('A') <= ch && ch <= int('Z')) ||
            (int('a') <= ch && ch <= int('z')) ||
            (int('0') <= ch && ch <= int('9')) ||
            int('-') == ch)
        {
            commands[floor] += input[i];
            ischar = true;
        }
        if (' ' == input[i] && true == ischar)
        {
            floor++;
            ischar = false;
        }
    }
    argc = num;
    return commands;
}
// 接口实现函数↓
void help()
{
    std::cout << "NoitaSeedKeeper v0.1.0" << std::endl;
    std::cout << "Options:\n    -D/-debug                       list all argvargument count\n    -d/-delet [ID]                  delet a seed by it's ID\n    -f/find [Model(1/2/3)] [Item]   find a seed\n        Model 1: By ID\n        Model 2: By seed            (Must be complete)\n        Model 3: By description     (Support word search)\n    -h/-help                        out put this page\n    -l/-list                        list all seeds\n    -s/-save [seed] [description]   save a seed(Auto-configuration ID)" << std::endl;
}

void save(std::string seed, std::string config)
{
    save_init();                   // 输出文件初始化
    std::string ID = IDGet_time(); // 获取ID

    if (filesearch("2", seed) != "FINDERR:-1")
    {
        std::cout << "seed exsit." << std::endl;
        return;
    }

    std::fstream f;
    f.open("seeds.ini", std::ios::app);
    f << ID << "," << seed << "," << config << ";";
    f.close();
    std::cout << "seed ID." << ID << " has been saved" << std::endl;
}

void find(std::string model, std::string item)
{
    if (!if_file_exist())
    {
        std::cout << "ERROR:File ont exist" << std::endl;
        return;
    }
    if (model == "1" or model == "2" or model == "3")
    {
        std::string ret = filesearch(model, item);
        if ("FINDERR:-1" == ret)
            std::cout << "Not exist." << std::endl;
        else
            std::cout << ret << std::endl;
    }
    else
        std::cout << "ERROR:Don't have this model: " + model << std::endl;
}

void list()
{
    if (!if_file_exist())
    {
        std::cout << "ERROR:File ont exist" << std::endl;
        return;
    }
    int len = 0;
    std::fstream f;
    std::string data;

    f.open("seeds.ini", std::ios::in);
    f >> data;
    len = data.size();
    for (int i = 1; i < len; i++) // 跳过第一个分隔符
    {
        std::cout << data[i];
        if (';' == data[i])
        {
            std::cout << std::endl;
        }
    }
    f.close();
}

void delet(std::string num)
{
    if (!if_file_exist())
    {
        std::cout << "ERROR:File ont exist" << std::endl;
        return;
    }

    std::fstream f;
    std::string data;
    f.open("seeds.ini", std::ios::in);
    f >> data;
    f.close();
    int n = data.find(";" + num + ",") + 1, len = data.size();
    if (n - 1 == std::string::npos)
    {
        std::cout << "Seed ID." + num + " not exit." << std::endl;
        return;
    }
    int j = 0;
    std::string backdata;
    for (j = 0; j < n; j++)
        backdata = backdata + data[j];
    while (data[j] != ';')
        j = j + 1;
    j = j + 1;
    for (j; j < len; j++)
        backdata = backdata + data[j];
    f.open("seeds.ini", std::ios::out);
    f << backdata;
    f.close();
    std::cout << "Seed ID." + num + " has been delet." << std::endl;
}

// 进程循环
int RedCommand(int argc, std::unique_ptr<std::string[]> argv) // 公用的命令解析器
{
    bool isexit = false;
    for (int i = 0; i < argc; i++) // 读选项
    {
        if (argv[i] == std::string("-D") or argv[i] == std::string("-debug"))
        {
            std::cout << "---Debug---\n";
            for (int i = 0; i < argc; ++i) // 检查输入
                std::cout << i << "," << argv[i] << std::endl;
        }
        if (argv[i] == std::string("-exit"))
            isexit = true;
        if (argv[i] == std::string("-h") or argv[i] == std::string("-help"))
            help();
        if (argv[i] == std::string("-l") or argv[i] == std::string("-list"))
            list();
        if (i < argc - 1 && (argv[i] == std::string("-s") or argv[i] == std::string("-save")))
        {
            if (argv[i + 1].find("-") == std::string::npos)
            {
                i = i + 1;
                if (i + 1 < argc && argv[i + 1].find("-") == std::string::npos)
                {
                    save(argv[i], argv[i + 1]);
                    i = 1 + i;
                }
                else
                    save(argv[i], "none");
            }
        }
        if (i < argc - 2 && (argv[i] == std::string("-f") or argv[i] == std::string("-find")))
        {
            if (argv[i + 1].find("-") == std::string::npos && argv[i + 2].find("-") == std::string::npos)
            {
                find(argv[i + 1], argv[i + 2]);
                i = i + 2;
            }
        }
        if (i < argc - 1 && (argv[i] == std::string("-d") or argv[i] == std::string("-delet")))
        {
            if (argv[i + 1].find("-") == std::string::npos)
            {
                i = i + 1;
                delet(argv[i]);
            }
        }
    }
    if (isexit) // 通过返回值判断是否退出
        return -1;
    else
        return 0;
}

int RunLoop()
{
    int reexit = 0;
    while (reexit == 0) // 主循环
    {
        int argc;
        auto argv = instaer(argc);
        reexit = RedCommand(argc, std::move(argv)); // 通过返回值判断是否退出
    }
    return 0;
}

// 主函数
int main(int argc, char **argv)
{
    std::cout << "start" << std::endl;
    if (argc <= 1) // 没初始命令输入
        RunLoop();
    else
    {
        std::unique_ptr<std::string[]> commands(new std::string[argc]); // 格式化
        for (int i = 0; i < argc; i++)
        {
            commands[i] = argv[i];
        }
        RedCommand(argc, std::move(commands));
    }
    return 0;
}