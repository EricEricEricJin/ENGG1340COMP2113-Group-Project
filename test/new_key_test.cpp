#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
using namespace std;

char GetInput()
{
    // fd_set 为long型数组
    //其每个元素都能和打开的文件句柄建立联系
    fd_set rfds;
    struct timeval tv;
    char c = '\0';

    //将　rfds数组清零
    FD_ZERO(&rfds);
    //将rfds的第0位置为１，这样fd=1的文件描述符就添加到了rfds中
    //最初　rfds为00000000,添加后变为10000000
    FD_SET(0, &rfds);
    tv.tv_sec = 1;
    tv.tv_usec = 0; //设置等待超时时间

    //检测键盘是否有输入
    //由内核根据io状态修改rfds的内容，来判断执行了select的进程哪个句柄可读
    if (select(2, &rfds, NULL, NULL, &tv) > 0)
    {
        c = getchar();
        cout << (int)c << endl;
    }
        // return c;

    //没有数据返回n
    return 'n';
}

int main()
{
    //关闭缓存区，使从终端接收一个字符不用按回车
    system("stty -icanon");
    while (1)
    {

        char c = GetInput();

        if ('n' == c)
        {
            cout << "no data" << endl;
        }
        else
        {
            // cout << endl
            //     << "c  = " << c << endl;
        }
    }

    return 0;
}
