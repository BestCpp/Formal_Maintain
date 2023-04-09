#pragma once

#include <functional>
#include <cstdio>

class task
{
    typedef std::function<int(int,int,char)> func_t;
public:
    task(){}
    task(int left,int right,int op,func_t func)
        :_left(left),_right(right),_op(op),_func(func)
    {}
    
    std::string operator()()
    {
        int res = _func(_left,_right,_op);
        char buffer[64];
        std::snprintf(buffer,sizeof(buffer),"%d %c %d = %d",_left,_op,_right,res);
        return buffer;
    }

    std::string send_msg()
    {
        char buffer[64];
        std::snprintf(buffer,sizeof(buffer),"%d %c %d = ?",_left,_op,_right);
        return buffer;
    }
private:
    int _left;//左操作数
    int _right;//右操作数
    char _op;//操作符
    func_t _func;//回调
};


class save
{
typedef std::function<void(std::string&)> func_t;
public:
    save(){}
    
    save(const std::string& data,func_t func)
        :_data(data),_func(func)
    {}

    void operator()()
    {
        _func(_data);
    }

private:
    std::string _data;
    func_t _func;//回调
};

string oper = "+-*/%";

int task_math(int x,int y,char op)
{
    switch(op)
    {
        case '+':
        return x + y;
        case '-':
        return x - y;
        case '*':
        return x * y;
        case '/':
        {
            if(y == 0)
            {
                cerr << "div zero error!" << endl;
                return -1;
            }
            return x / y;
        }
        case '%':
        if(y == 0)
        {
            cerr << "mod zero error!" << endl;
            return -1;
        }
        return x % y;
    }
    return -1;
}


void save_data(const std::string& s)
{
    FILE* fp = fopen("./log.txt","a+");
    if(!fp)
    {
        std::cerr << "fp is nullptr" << std::endl;
        return;
    }
    fprintf(fp,"%s\n",s.c_str());
    fclose(fp);
}