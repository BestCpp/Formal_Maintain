
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;
#include <pthread.h>
#include "block_queue.hpp"
#include <unistd.h>


void* Productor1(void* args)
{
    block_queue<int>* q = static_cast<block_queue<int>*>(args);
    //生产者不断向阻塞队列写数据
    while(true)
    {
        int in = rand() % 100;//0~100
        q->push(in);
        cout << "i am productor1,send data:" << in;
        cout << " current size:" << q->size() << endl;
        sleep(1);
    }
}

void* Productor2(void* args)
{
    block_queue<int>* q = static_cast<block_queue<int>*>(args);
    //生产者不断向阻塞队列写数据
    while(true)
    {
        int in = rand() % 100;//0~100
        q->push(in);
        cout << "i am productor2,send data:" << in;
        cout << " current size:" << q->size() << endl;
        sleep(1);

    }
}

void* Consumer(void* args)
{
    block_queue<int>* q = static_cast<block_queue<int>*>(args);
    //消费者不断从阻塞队列拿数据
    while(true)
    {
        int out = 0;
        q->pop(&out);
        cout << "i am consumer,get data:" << out;
        cout << " current size:" << q->size() << endl;
        sleep(1);
    
    }
}

int main()
{
    srand((unsigned int)time(0));
    // 建立一个简单的生产者消费者模型
    pthread_t productor1,productor2,consumer;

    // 创建生产者、消费者线程
    block_queue<int>* q = new block_queue<int>();
    pthread_create(&productor1,nullptr,Productor1,q);
    pthread_create(&productor2,nullptr,Productor2,q);
    pthread_create(&consumer,nullptr,Consumer,q);

    pthread_join(productor1,nullptr);
    pthread_join(productor2,nullptr);
    pthread_join(consumer,nullptr);

    return 0;
}