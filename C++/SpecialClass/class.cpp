
#include <iostream>
#include <map>
#include <string>
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
    static Singleton& Create()
    {
        if(_sg == nullptr)//在第一次调用Create时创建对象
        {
            _sg = new Singleton;
        }
        return *_sg;
    }
    
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
};
Singleton* Singleton::_sg = nullptr;//创建对象


int main()
{
    Singleton::Create().insert("张三",12000);
    Singleton& sl = Singleton::Create();
    sl.insert("李四",20000);
    sl.print();

    return 0;
}