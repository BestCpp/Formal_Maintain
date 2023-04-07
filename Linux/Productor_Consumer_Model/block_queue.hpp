#pragma once

// 阻塞队列
#include <pthread.h>
#include <queue>

template <class T>
class block_queue
{
public:
    block_queue(int maxcap = 5)
        :_maxcap(maxcap)
    {
        pthread_mutex_init(&_mtx,nullptr);
        pthread_cond_init(&_pcond,nullptr);
        pthread_cond_init(&_ccond,nullptr);
    }
    
    void push(const T& data)//生产者生产
    {
        pthread_mutex_lock(&_mtx);
        while(is_full())//如果容量已满，不能生产
        {
            //线程挂起等待时候会将锁释放，因为挂起的时候带着锁进去会导致消费者无法访问临界资源
            pthread_cond_wait(&_pcond,&_mtx);//生产者线程放入条件变量等待
            //线程被唤醒时会继续向下执行，这个时候就要保证与消费者的互斥，函数就会恢复锁
        }
        //到这里一定是生产者可以生产
        q.push(data);
        //生产者一旦生产数据，则表示消费者可以消费
        pthread_cond_signal(&_ccond);
        pthread_mutex_unlock(&_mtx);
    }

    void pop(T* data)//输出型参数
    {//消费者消费
        pthread_mutex_lock(&_mtx);
        if(is_empty())//如果容量为空，不能消费
        {
            pthread_cond_wait(&_ccond,&_mtx);//生产者线程放入条件变量等待
        }
        //走到这里一定是消费者可以消费
        *data = q.front();
        q.pop();
        //消费一个空间就多一个，生产者可以生产
        pthread_cond_broadcast(&_pcond);
        pthread_mutex_unlock(&_mtx);
    }
    ~block_queue()
    {
        pthread_mutex_destroy(&_mtx);
        pthread_cond_destroy(&_pcond);
        pthread_cond_destroy(&_ccond);
    }

    size_t size()
    {
        return q.size();
    }

private:
    std::queue<T> q;//阻塞队列需要队列，利用STL
    pthread_mutex_t _mtx;//保证321原则的锁
    pthread_cond_t _pcond;//生产者条件变量
    pthread_cond_t _ccond;//消费者条件变量
    int _maxcap;//阻塞队列最大容量

    bool is_empty()//如果为空消费者不能消费
    {
        return q.empty();
    }

    bool is_full()//如果满生产者不能生产
    {
        return q.size() == _maxcap;
    }
};