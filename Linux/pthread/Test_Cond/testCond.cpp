
#include <iostream>
#include <string>
#include <vector>
using namespace std;
#include <pthread.h>
#include <unistd.h>

int tickets = 1000;
pthread_mutex_t mt = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* start_routine(void* args)
{
    string name = static_cast<char*>(args);
    while(true)
    {
        pthread_mutex_lock(&mt);
        pthread_cond_wait(&cond,&mt);//任何一个线程都先放入条件变量
        if(tickets > 0)
        {
            cout << name << " -> " << tickets-- << endl;
            pthread_mutex_unlock(&mt);
        }
        else 
        {
            pthread_mutex_unlock(&mt);
            break;
        }
    }
    return nullptr;
}

int main()
{
    //还是两个线程抢票的逻辑，但是这把使用条件变量达到线程同步
    // pthread_t t1,t2;
    // pthread_create(&t1,nullptr,start_routine,(void*)"thread 1");
    // pthread_create(&t2,nullptr,start_routine,(void*)"thread 2");

    vector<pthread_t> vec(5);
    for(int i=0;i<vec.size();i++)
    {
        char* name = new char[64];
        snprintf(name,64,"thread %d",i+1);
        pthread_create(&vec[i],nullptr,start_routine,(void*)name);
    }

    while(true)
    {
        usleep(12356);
        int n = pthread_cond_broadcast(&cond);//从条件变量中唤醒一个线程
        cout << "main thread siganl....." << endl;
    }
    
    // pthread_join(t1,nullptr);
    // pthread_join(t2,nullptr);

    for(auto& e:vec)
    {
        pthread_join(e,nullptr);
    }
    return 0;
}