
#include <iostream>
#include <map>
#include <string>
#include <mutex>
using namespace std;

// 设计一个不能被拷贝的类
// class NoCopy//C++98 风格
// {
// public:
//     NoCopy(){}
// private:
//     NoCopy(const NoCopy& nc);//拷贝构造声明为私有
//     NoCopy& operator=(const NoCopy& nc);//赋值重载声明为私有
// };

// class NoCopy//C++11 风格
// {
// public:
//     NoCopy(){}
//     NoCopy(const NoCopy& nc) = delete;//使用delete关键字
//     NoCopy& operator=(const NoCopy& nc) = delete;
// };

//////////////////////////////////////////////////////////////////////////
//设计一个只能在堆上创建对象的类
// class HeapOnly//方法一
// {
// public:
//     static HeapOnly* CreateHeap()
//     {
//         return new HeapOnly();
//     }
    
//     HeapOnly(const HeapOnly& hp) = delete;
//     HeapOnly& operator=(const HeapOnly& hp) = delete;

// private:
//     HeapOnly(){}
    
// };

// class HeapOnly//方法二
// {
// public:
//     HeapOnly(){}
//     HeapOnly(const HeapOnly& hp) = delete;
//     HeapOnly& operator=(const HeapOnly& hp) = delete;
// private:
//     ~HeapOnly(){}//析构作私有
// };

/////////////////////////////////////////////////////////////////////////
// 设计一个只能在栈上创建对象的类
// class StackOnly
// {
// public:
//     static StackOnly CreateStack()
//     {
//         return StackOnly();
//     }
// private:
//     StackOnly(){}//构造函数私有
// };

// int main()
// {
//     //NoCopy n1;
//     //NoCopy n2(n1);//禁止拷贝

//     //HeapOnly hp1;//不能在栈上创建了
//     //HeapOnly* hp = HeapOnly::CreateHeap();//在堆上创建

//     // HeapOnly* hp = new HeapOnly;//在堆上创建
//     // HeapOnly hp2;//因为出作用域需要调析构，方法二调不到，报错
    
//     //StackOnly* so = new StackOnly;//堆上就不能创建了
//     StackOnly so1 = StackOnly::CreateStack();//在栈上创建了
//     return 0;
// }


/////////////////////////////////////////////////////////////////////
// 单例模式
// class Singleton//饿汉模式,在main函数前就创建对象了
// {
// public:
//     static Singleton& Create()
//     {
//         return _sg;
//     }
    
//     void insert(const string& name,int money)
//     {
//         _m[name] = money;
//     }

//     void print()
//     {
//         for(auto&e :_m)
//         {
//             cout << e.first << ":" << e.second << endl;
//         }
//     }
// private:
//     //不能在堆、栈上创建对象
//     Singleton(){}
//     Singleton(const Singleton& sg) = delete;
//     Singleton& operator=(const Singleton& sg) = delete;

//     static Singleton _sg;
//     map<string,int> _m;
// };
// Singleton Singleton::_sg;//创建对象

class Singleton//懒汉模式,在main函数前就创建对象了
{
public:
    // static Singleton& Create()
    // {
    //     //需要注意线程安全问题
    //     if(_sg == nullptr)// 只需要上一次锁，都可以这样用
    //     {
    //         _mtx.lock();
    //         if(_sg == nullptr)//在第一次调用Create时创建对象
    //         {
    //             _sg = new Singleton;
    //         }
    //         _mtx.unlock();
    //     }
    //     return *_sg;
    // }

    static Singleton& Create()
    {
        //需要注意线程安全问题
        if(_sg == nullptr)// 只需要上一次锁，都可以这样用
        {
            lock_guard<mutex> lock(_mtx);
            if(_sg == nullptr)//在第一次调用Create时创建对象
            {
                _sg = new Singleton;
            }
        }
        return *_sg;
    }
    
    static void DelInstance()
    {
        // 假如对象需要销毁需要做一些工作
        // ...例如保存数据到文件......
        cout << "DelInstance()" << endl;
        lock_guard<mutex> lock(_mtx);
        if(_sg)
        {
            delete _sg;
            _sg = nullptr;
        }
    }

    class GC
    {
    public:
        ~GC()
        {
            if(_sg)
            {
                cout << "~GC()" << endl;
                DelInstance();
            }
            
        }
    };
    
    void insert(const string& name,int money)
    {
        _m[name] = money;
    }

    void print()
    {
        for(auto&e :_m)
        {
            cout << e.first << ":" << e.second << endl;
        }
    }
private:
    //不能在堆、栈上创建对象
    Singleton(){}
    Singleton(const Singleton& sg) = delete;
    Singleton& operator=(const Singleton& sg) = delete;

    static Singleton* _sg;
    map<string,int> _m;
    static mutex _mtx;
    static GC _gc;// Singleton销毁时_gc也会销毁
};
Singleton* Singleton::_sg = nullptr;//创建对象
mutex Singleton::_mtx;
Singleton::GC Singleton::_gc;


// class Singleton//懒汉模式,在main函数前就创建对象了
// {
// public:
//     static Singleton& Create()
//     {
//         // C++98不能保证这里是线程安全的
//         // 但是C++11之后可以
//         static Singleton _sg;
//         return _sg;
//     }
    
//     void insert(const string& name,int money)
//     {
//         _m[name] = money;
//     }

//     void print()
//     {
//         for(auto&e :_m)
//         {
//             cout << e.first << ":" << e.second << endl;
//         }
//     }
// private:
//     //不能在堆、栈上创建对象
//     Singleton(){}
//     Singleton(const Singleton& sg) = delete;
//     Singleton& operator=(const Singleton& sg) = delete;

//     static Singleton* _sg;
//     map<string,int> _m;
//     static mutex _mtx;
// };
//Singleton* Singleton::_sg = nullptr;//创建对象
//mutex Singleton::_mtx;

int main()
{
    Singleton::Create().insert("张三",12000);
    Singleton& sl = Singleton::Create();
    sl.insert("李四",20000);
    sl.print();

    //Singleton::Create().DelInstance();
    return 0;
}