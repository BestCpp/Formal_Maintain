
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
using namespace std;
#include <pthread.h>
#include "block_queue.hpp"
#include "task.hpp"
#include <unistd.h>


// void* Productor(void* args)
// {
//     block_queue<int>* q = static_cast<block_queue<int>*>(args);
//     //生产者不断向阻塞队列写数据
//     while(true)
//     {
//         int in = rand() % 100;//0~100
//         q->push(in);
//         cout << "i am productor1,send data:" << in;
//         cout << " current size:" << q->size() << endl;
//         sleep(1);
//     }
// }

void* Productor(void* args)
{
    block_queue<task>* q = static_cast<block_queue<task>*>(args);
    // 我们想让生产者生产任务，然后将此任务放到阻塞队列里面去
    while(true)
    {
        int left = rand() % 100;
        int right = rand() % 20;
        int op = rand() % oper.size();
        task t(left,right,oper[op],task_math);
        q->push(t);
        cout << "productor send task:" << t.send_msg() << endl;
        sleep(1);
    }
}

template <class task,class save>
struct BlockQueue
{
    block_queue<save>* save_bq;
    block_queue<task>* task_bq;
};

void* Consumer(void* args)
{
    //block_queue<task>* q = static_cast<block_queue<task>*>(args);
    block_queue<task>* task_q = (static_cast<BlockQueue<task,save>*>(args))->task_bq;
    block_queue<save>* save_q = (static_cast<BlockQueue<task,save>*>(args))->save_bq;
    
    // 消费者不断从阻塞队列获取任务，然后执行
    while(true)
    {
        task t;
        task_q->pop(&t);
        string res = t();
        cout << "consumer running task:" << res << " ";

        save s(res,save_data);
        save_q->push(s);
        cout << "send sava task to Saver..." << endl;
    }
}

void* Saver(void* args)
{
    block_queue<save>* save_q = (static_cast<BlockQueue<task,save>*>(args))->save_bq;
    
    while(true)
    {
        save s;
        save_q->pop(&s);
        s();
        cout << "Saver save data success" << endl;
    }
}

// void* Consumer(void* args)
// {
//     block_queue<int>* q = static_cast<block_queue<int>*>(args);
//     //消费者不断从阻塞队列拿数据
//     while(true)
//     {
//         int out = 0;
//         q->pop(&out);
//         cout << "i am consumer,get data:" << out;
//         cout << " current size:" << q->size() << endl;
//         sleep(1);
    
//     }
// }



int main()
{
    srand((unsigned int)time(0));
    // 建立一个简单的生产者消费者模型
    pthread_t productor,consumer,saver;

    // 创建生产者、消费者线程
    BlockQueue<task,save> bq;

    block_queue<task>* task_bq = new block_queue<task>();
    block_queue<save>* save_bq = new block_queue<save>();
    bq.save_bq = save_bq;
    bq.task_bq = task_bq;

    pthread_create(&productor,nullptr,Productor,task_bq);
    pthread_create(&consumer,nullptr,Consumer,&bq);
    pthread_create(&saver,nullptr,Saver,&bq);

    pthread_join(productor,nullptr);
    pthread_join(consumer,nullptr);
    pthread_join(saver,nullptr);

    return 0;
}