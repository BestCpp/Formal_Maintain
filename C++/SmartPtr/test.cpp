
#include <iostream>
#include <thread>
#include <memory>
using namespace std;

#include "shared_ptr.hpp"
using namespace ly;

void test1()//测试基本功能
{
    ly::shared_ptr<int> sp1(new int(3));
    cout << *sp1 << endl;
    ly::shared_ptr<int> sp2(sp1);
    cout << *sp2 << endl;
}

void test2()//线程安全问题 -> 加锁之后保证了shared_ptr内部安全
{
    ly::shared_ptr<int> sp1(new int(3));
    
    int n = 100000;

    std::thread p1([&]()
    {
        for(int i=0;i<n;i++)
        {
            ly::shared_ptr<int> sp2(sp1);
        }
    });
    
    std::thread p2([&]()
    {
        for(int i=0;i<n;i++)
        {
            ly::shared_ptr<int> sp3(sp1);
        }
    });
    p1.join();
    p2.join();
    cout << sp1.use_count() << endl;
}

struct Test
{
    int x = 0;
    int y = 0;
    int z = 0;
};

void test3()//shared_ptr外部没有安全
{
    ly::shared_ptr<Test> sp1(new Test());
    int n = 100000;
    mutex mtx;
    std::thread p1([&]()
    {
        for(int i=0;i<n;i++)
        {
            ly::shared_ptr<Test> sp2(sp1);

            mtx.lock();
            sp2->x++;
            sp2->y++;
            sp2->z++;
            mtx.unlock();
        }
    });
    
    std::thread p2([&]()
    {
        for(int i=0;i<n;i++)
        {
            ly::shared_ptr<Test> sp3(sp1);
            mtx.lock();
            sp3->x++;
            sp3->y++;
            sp3->z++;
            mtx.unlock();
        }
    });

    p1.join();
    p2.join();

    std::cout << sp1.use_count() << endl;//这个没有问题

    //未加锁之前，根被没加够20万次
    //加锁之后，没有资源冲突问题
    std::cout << sp1->x << endl;
    std::cout << sp1->y << endl;
    std::cout << sp1->z << endl;

}


struct Node
{
    // Node* _next;
    // Node* _prev;
    // ly::shared_ptr<Node> _next;
    // ly::shared_ptr<Node> _prev;

    //使用weak_prt单纯的拿到资源，不增加引用计数
    ly::weak_ptr<Node> _next;
    ly::weak_ptr<Node> _prev;

    ~Node()
    {
        cout << "释放资源" << endl;
    }
};

void test4()//循环引用问题
{
    // Node* n1 = new Node();
    // Node* n2 = new Node();

    ly::shared_ptr<Node> n1(new Node());
    ly::shared_ptr<Node> n2(new Node());

    n1->_next = n2;
    n2->_prev = n1;

    cout << n1.use_count() << endl;
    cout << n2.use_count() << endl;

    // delete n1;
    // delete n2;
}

template <class T>
struct DelAll
{
    void operator()(T* ptr)
    {
        delete[] ptr;
        cout << "delete [] " << ptr << endl; 
    }  
};

template <>
struct DelAll<FILE>
{
    void operator()(FILE* ptr)
    {
        fclose(ptr);
        cout << "delete File" << ptr << endl;
    }
};


void test5()//定制删除器
{
    ly::shared_ptr<int,DelAll<int>> sp1(new int[10]);
    ly::shared_ptr<FILE,DelAll<FILE>> sp2(fopen("hello.txt","r"));
    
}

int main()
{
    test5();
    return 0;
}