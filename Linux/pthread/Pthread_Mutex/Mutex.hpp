#pragma once

#include <pthread.h>


class MutexGuard//RAII风格的锁
{
public:
    MutexGuard(pthread_mutex_t* pmtx = nullptr)
        :_pmtx(pmtx)
    {
        if(_pmtx != nullptr)
        {
            pthread_mutex_lock(_pmtx);
        }
    }

    ~MutexGuard()
    {
        if(_pmtx != nullptr)
        {
            pthread_mutex_unlock(_pmtx);
        }
    }
private:
    pthread_mutex_t* _pmtx;
};