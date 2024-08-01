#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// 功能函数
bool if_file_exist()
{
    fstream f;
    f.open("seeds.ini");
    if (!f.good()) // 检查存在
    {
        // cout<<"Not exit"<<endl;
        f.close();
        return false;
    }
    f.close();
    return true;
}

string reverse(int len, string str)
{
    string newstr;
    for (int i = len - 1; i >= 0; i--)
    {
        newstr = newstr + str[i];
    }
    return newstr;
}

string filesearch(string model, string item)
{
    fstream f;
    string data;
    f.open("seeds.ini", ios::in);
    f >> data;
    f.close();
    if (model == "1")
    { // 编号模式
        int i = data.find(";" + item + ",") + 1;
        if (i - 1 == string::npos)
        {
            // cout << "Str not exit." << endl;
            return "FINDERR:-1";
        }
        string backdata;
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
        if (n - 1 == string::npos)
        {
            // cout << "Str not exit." << endl;
            return "FINDERR:-1";
        }
        string output_b, output_a; // before,after
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
        if (n - 1 == string::npos)
        {
            // cout << "Str not exit." << endl;
            return "FINDERR:-1";
        }
        string output_b, output_a; // before,after
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
    bool filex = if_file_exist();
    int number = 0;
    fstream f;
    if (filex) // 找编号
    {
        f.open("seeds.ini", ios::in);
        string data;
        f >> data;
        int i = data.size() - 2;
        while (i >= 0) // 倒着跑
        {
            if (';' == data[i]) // 读到分隔符号
            {
                i = i + 1;
                string tmp;
                while (data[i] != ',')
                {
                    tmp = tmp + data[i];
                    i = i + 1;
                }
                // cout<<tmp<<endl;
                number = stoi(tmp);
                // cout << tmp << ";" << number << endl;
                break;
            }
            i = i - 1;
        }

        // cout<<data;
        f.close();
    }
    else
    {
        f.open("seeds.ini", ios::app);
        f << ";";
        f.close();
    }
    return number;
}

// 接口实现函数↓
void help()
{
    cout << "NoitaSeedKeeper v0.1.0" << endl;
    cout<<"Options:\n    -D/-debug                       list all argvargument count\n    -d/-delet [ID]                  delet a seed by it's ID\n    -f/find [Model(1/2/3)] [Item]   find a seed\n        Model 1: By ID\n        Model 2: By seed            (Must be complete)\n        Model 3: By description     (Support word search)\n    -h/-help                        out put this page\n    -l/-list                        list all seeds\n    -s/-save [seed] [description]   save a seed(Auto-configuration ID)"<<endl;
}

void save(string seed, string config)
{
    int number = save_init(); // 输出文件初始化和获取编号

    if (filesearch("2", seed) != "FINDERR:-1")
    {
        cout << "seed exsit." << endl;
        return;
    }

    fstream f;
    f.open("seeds.ini", ios::app);
    f << number + 1 << "," << seed << "," << config << ";";
    f.close();
    cout << "seed ID." << number + 1 << " has been saved" << endl;
}

void find(string model, string item)
{
    if (!if_file_exist())
    {
        cout << "ERROR:File ont exist" << endl;
        return;
    }
    if (model == "1" or model == "2" or model == "3")
    {
        string ret = filesearch(model, item);
        if ("FINDERR:-1" == ret)
            cout << "Not exist." << endl;
        else
            cout << ret << endl;
    }
    else
        cout << "ERROR:Don't have this model: " + model << endl;
}

void list()
{
    if (!if_file_exist())
    {
        cout << "ERROR:File ont exist" << endl;
        return;
    }
    int len = 0;
    fstream f;
    string data;

    f.open("seeds.ini", ios::in);
    f >> data;
    len = data.size();
    for (int i = 1; i < len; i++) // 跳过第一个分隔符
    {
        cout << data[i];
        if (';' == data[i])
        {
            cout << endl;
        }
    }
    f.close();
}

void delet(string num)
{
    if (!if_file_exist())
    {
        cout << "ERROR:File ont exist" << endl;
        return;
    }

    fstream f;
    string data;
    f.open("seeds.ini", ios::in);
    f >> data;
    f.close();
    int n = data.find(";" + num + ",") + 1, len = data.size();
    if (n - 1 == string::npos)
        cout << "Seed ID." + num + " not exit." << endl;

    int j = 0;
    string backdata;
    for (j = 0; j < n; j++)
        backdata = backdata + data[j];
    while (data[j] != ';')
        j = j + 1;
    j = j + 1;
    for (j; j < len; j++)
        backdata = backdata + data[j];
    f.open("seeds.ini", ios::out);
    f << backdata;
    f.close();
    cout << "Seed ID." + num + " has been delet." << endl;
}

// 主函数
int main(int argc, char **argv)
{
    cout << "start" << endl;
    if (argc <= 1) // 没输入
        help();
    // cout<<"1"<<endl;
    for (int i = 0; i < argc; i++) // 读选项
    {
        // cout<<"i:"<<i<<endl;
        if (argv[i] == string("-D") or argv[i] == string("-debug"))
        {
            // cout<<"deb"<<endl;
            for (int i = 0; i < argc; ++i) // 检查输入
                cout << i << "," << argv[i] << endl;
        }
        if (argv[i] == string("-h") or argv[i] == string("-help"))
            help();
        if (argv[i] == string("-s") or argv[i] == string("-save"))
        {
            // cout<<"se"<<endl;
            i = i + 1;
            save(argv[i], argv[i + 1]);
            i = 1 + i;
        }
        if (argv[i] == string("-l") or argv[i] == string("-list"))
            list();
        if (argv[i] == string("-f") or argv[i] == string("-find"))
        {
            i = i + 1;
            find(argv[i], argv[i + 1]);
            i = i + 1;
        }
        if (argv[i] == string("-d") or argv[i] == string("-delet"))
        {
            i = i + 1;
            delet(argv[i]);
        }
    }
    return 0;
}