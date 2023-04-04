#pragma once

#include <string>
#include <pthread.h>
#include <functional>
#include <assert.h>

class Pthread;

class Context//上下文
{
public:
    Pthread* _this;
    void* _args;
};

class Pthread
{
public:
    typedef std::function<void*(void*)> func_t;
    Pthread(const std::string& name,const func_t& func,void* args)
        :_name(name),_func(func),_args(args)
    {
        Context* ctt = new Context();
        ctt->_this = this;
        ctt->_args = _args;

        int n = pthread_create(&_tid,nullptr,start_routine,(void*)ctt);
        assert(n == 0);
        (void)n;
    }

    Pthread(){}

    //void* start_routine(void* args)//这么写不行，有this指针
    //{}

    static void* start_routine(void* args)
    {
        Context* pc = (Context*)args;
        return pc->_this->run(pc->_args);
    }

    void join()
    {
        int n = pthread_join(_tid,nullptr);
        assert(n == 0);
        (void)n;
    }

    std::string& name()
    {
        return _name;
    }
private:
    std::string _name;//线程名字
    void* _args;//参数
    pthread_t _tid;
    func_t _func;//函数

    void* run(void* args)
    {
        _func(args);
        return nullptr;
    }
};