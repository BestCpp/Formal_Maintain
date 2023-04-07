
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;
#include "Pthread.hpp"
#include "Mutex.hpp"
#include <unistd.h>

//全局锁
//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

class PthreadData
{
public:
    PthreadData(const string& args,pthread_mutex_t* pmtx)
        :_args(args),_pmtx(pmtx)
    {}

public:
    string _args;//想要传过去的字符串
    pthread_mutex_t* _pmtx;//指向锁的指针
};

int tickets = 10000;//票有10000张

void* start_routine(void* args)
{
    //string msg = (char*)args;
    PthreadData* pd = (PthreadData*)args;
    while(true)
    {
        //加锁的过程本身就是原子的，我们不必担心
        //申请锁成功的线程访问临界资源期间，其他线程只能阻塞等待(被迫串行)
        //在访问临界资源的过程中是可以被切换的(同时也会带走上下文，锁就是上下文)，其他线程只关心这个线程有没有申请锁、有没有释放锁
        //pthread_mutex_lock(pd->_pmtx);
        {
            MutexGuard mg(pd->_pmtx);
            if(tickets > 0)
            {
                usleep(1234);//先让线程阻塞
                //在线程阻塞的这段时间，tickets的值已经发生改变
                cout << pd->_args << " 抢票成功,第" << tickets-- << "张" << endl;
                pthread_mutex_unlock(pd->_pmtx);
            }
            else 
            {
                pthread_mutex_unlock(pd->_pmtx);
                break;
            }
        }
        //抢完票后还需要做其他工作，模拟一下
        usleep(1000);
    }
    return nullptr;
}

int main()
{
    pthread_mutex_t lock;//局部锁
    pthread_mutex_init(&lock,nullptr);
    vector<Pthread> vec(4);

    // for(int i=0;i<vec.size();i++)
    // {
    //     char buffer[64] = {0};
    //     snprintf(buffer,sizeof(buffer),"thread %d",i+1);
    //     PthreadData* pd = new PthreadData(buffer,&lock);
    //     pthread_create(&vec[i],nullptr,start_routine,(void*)pd);
    // }

    // for(auto& e:vec)
    // {
    //     pthread_join(e,nullptr);
    // }

    //如何把局部锁传递给线程函数？->结构体/类
    PthreadData* pd1 = new PthreadData("Teacher",&lock);
    unique_ptr<Pthread> up1(new Pthread("phtread 1",start_routine,(void*)pd1));

    PthreadData* pd2 = new PthreadData("Student",&lock);
    unique_ptr<Pthread> up2(new Pthread("phtread 2",start_routine,(void*)pd2));
    
    PthreadData* pd3 = new PthreadData("Doctor",&lock);
    unique_ptr<Pthread> up3(new Pthread("phtread 3",start_routine,(void*)pd3));
    
    PthreadData* pd4 = new PthreadData("Driver",&lock);
    unique_ptr<Pthread> up4(new Pthread("phtread 4",start_routine,(void*)pd4));

    
    up1->join();
    up2->join();

    up3->join();
    up4->join();

    pthread_mutex_destroy(&lock);//销毁
    return 0;
}



// #include "Pthread.hpp"
// #include <iostream>
// using namespace std;
// #include <unistd.h>

// void* start_routine(void* argc)
// {
//     while(true)
//     {
//         cout << (char*)argc << endl;
//         sleep(1);
//     }
// }
// int main()
// {
//     Pthread p1("thread 1",start_routine,(void*)"Hello Pthread");
//     Pthread p2("thread 2",start_routine,(void*)"Clinet Pthread");
//     Pthread p3("thread 3",start_routine,(void*)"Server Pthread");
//     cout << "create " << p1.name() << " success" << endl;
//     cout << "create " << p2.name() << " success" << endl;
//     cout << "create " << p3.name() << " success" << endl;

    
//     p1.join();
//     return 0;
// }


// #include <iostream>
// #include <cassert>
// #include <cstring>
// using namespace std;
// #include <pthread.h>
// #include <unistd.h>

// //int num = 100;//这种全局变量被所有线程共享
// __thread int num = 100;//线程局部变量，每个线程都有一个

// void* start_routine(void* argc)
// {
//     pthread_t tid = pthread_self();//获取自己的tid
//     int n = pthread_detach(tid);
//     assert(n == 0);
//     (void)n;

//     int cnt = 5;
//     while(cnt--)
//     {
//         cout << (char*)argc << " tid:" << tid  << " num:" << num++ << " &num:" << &num << endl;
//         sleep(1);
//     }
// }
// int main()
// {
//     pthread_t tid;
//     int n = pthread_create(&tid,nullptr,start_routine,(void*)"thread 1");
//     assert(n == 0);
//     (void)n;

//     while(true)
//     {
//         cout << "main pthread..." << "pthread 1 tid:" << tid << " num:" << num << " &num:" << &num << endl;
//         sleep(1);
//     }
//     //线程创建后调度是随机的，所以新线程没有执行线程分离，主线程已经开始阻塞式等待了
//     // sleep(2);
//     // n = pthread_join(tid,nullptr);
//     // if(n == 0)
//     // {
//     //     cout << "thread 1 quit success" << endl;
//     // }
//     // else 
//     // {
//     //     cout << "join fail:" << strerror(n) << endl; 
//     // }
//     // assert(n == 0);
//     // (void)n;

//     return 0;
// }



// #include <iostream>
// #include <thread>
// using namespace std;
// #include <unistd.h>

// //语言上的线程库本质就是对原生库的封装
// //在编译时也要指定pthread库
// int main()
// {
//     thread t1([]()
//     {
//         while(true)
//         {
//             cout << "新线程" << endl;
//             sleep(1);
//         }
//     });
    
//     while(true)
//     {
//         cout << "主线程" << endl;
//         sleep(1);
//     }


//     t1.join();
//     return 0;
// }


// #include <iostream>
// #include <cassert>
// #include <cstring>
// #include <vector>
// using namespace std;
// #include <unistd.h>
// #include <pthread.h>
// #include <signal.h>

// class Pthread
// {
// public:
//     pthread_t _tid;
//     char buffer[64];
//     int _nummber;
// };

// class ReturnMsg//还可以返回对象
// {
// public:
//     pthread_t _tid;
//     int _nummber;
// };

// void* start_routine(void* arg)//因为被多个执行流调用，所以可以看作是一个可重入函数
// {
//     //sleep(1);//休息一秒后，主线程的循环走完了

//     int cnt = 10;
//     while(cnt)
//     {
//         //证明了线程有自己的独立栈结构!
//         //cout << (char*)arg << " &cnt: " << &cnt << " cnt: " << cnt-- << endl;
//         cout << ((Pthread*)arg)->buffer << " &cnt: " << &cnt << " cnt: " << dec << cnt-- << endl;
        
//         sleep(1);
//         // if(cnt == 3)
//         // {
//         //     cout << (char*)arg << " exit!" << endl;
//         //     int* p = nullptr;
//         //     *p = 3;
//         // }
//     }
//     // return arg;//返回值是线程终止方式的一种
//     pthread_exit((void*)100);//通过函数也是一种方式，返回值放在pthread库中
//     // ReturnMsg* rt = new ReturnMsg();
//     // rt->_tid = ((Pthread*)arg)->_tid;
//     // rt->_nummber = ((Pthread*)arg)->_nummber;
//     // pthread_exit((void*)rt);
// }

// int main()
// {
//     //pthread_t tid;
//     //pthread_create(&tid,nullptr,start_routine,(void*)"thread one");
//     vector<Pthread*> vec;
// #define NUM 10
//     for(int i=0;i<NUM;++i)
//     {
//         //因为线程来不及执行，主线程就进入下一次循环，重新在以前的栈位置上
//         //重新开辟buffer，所以会发生覆盖,因为栈位置是相同的，所以buffer的地址也一样
//         //进而导致每个线程看到的内容都是覆盖后的内容
//         //最后一次覆盖的内容为9，所以所有线程看到的都是9
//         // char buffer[64] = {0};
//         // snprintf(buffer,sizeof(buffer),"%s:%d","pthread id",i);
//         // pthread_create(&tid,nullptr,start_routine,(void*)buffer);

//         //解决上面的问题，就是让每个进程有属于自己的信息
//         Pthread* pd = new Pthread();
//         snprintf(pd->buffer,sizeof(pd->buffer),"%s:%d","pthread id",i);
//         pd->_nummber = i;
//         pthread_create(&pd->_tid,nullptr,start_routine,(void*)pd);
//         vec.push_back(pd);
//     }
//     sleep(5);
//     //甚至我们可以主动的取消某个线程
//     for(int i=0;i<vec.size()/2;i++)//取消一半线程
//     {
//         pthread_cancel(vec[i]->_tid);
//         cout << "pthread num: " << vec[i]->_nummber<< " was canceled" << endl;
//     }
//     //线程终止之后，如何获取它的退出信息?
//     for(auto& e:vec)
//     {
//         void* ret = nullptr;
//         //因为线程的退出信息都通过pthread_create写在库中，所以需要获取它
//         pthread_join(e->_tid,(void**)&ret);//等待线程，获取它的退出信息
//         cout << "pthread num: " << e->_nummber << " exit success , code: " << (long long)ret << endl;
//         sleep(1);
//     }

//     // while(true)
//     // {
//     //     //cout << "main thread" << endl;
//     //     sleep(1);
//     // }
//     return 0;
// }


// #include <iostream>
// using namespace std;

// int main()
// {
//     cout << "hello Linux!" << endl;
//     return 0;
// }
