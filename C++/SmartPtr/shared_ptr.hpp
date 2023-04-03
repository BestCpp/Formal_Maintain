#pragma once

#include <mutex>
using namespace std;
namespace ly
{
    template <class T>
    class shared_ptr
    {
    public:
        shared_ptr(T* ptr = nullptr)
            :_ptr(ptr),_pcount(new int(1)),_pmtx(new mutex)
        {}

        shared_ptr(const shared_ptr<T>& sp)
            :_ptr(sp._ptr),_pcount(sp._pcount),_pmtx(sp._pmtx)
        {
            _pmtx->lock();
            ++(*_pcount);//拷贝构造指向同一资源，引用计数+1
            _pmtx->unlock();
        }

        shared_ptr<T>& operator=(const shared_ptr<T>& sp)
        {
            if(_ptr != sp._ptr)//保证自己不给自己赋值
            {
                release();
                _ptr = sp._ptr;
                _pcount = sp._pcount;
                _pmtx = sp._pmtx;
                _pmtx->lock();
                ++(*_pcount);//赋值指向同一资源，引用计数+1
                _pmtx->unlock();
            }
            return *this;
        }

        void release()
        {
            bool flag = false;
            _pmtx->lock();
            if(--(*_pcount) == 0)
            {
                //delete[] _ptr;//关键问题
                delete _ptr;
                delete _pcount;
                //cout << "释放资源" << endl;
                flag = true;
            }
            _pmtx->unlock();
            if(flag)
            {
                delete _pmtx;
            }
        }

        T* get() const
        {
            return _ptr;
        }

        ~shared_ptr()
        {
            release();
        }

        int use_count()
        {
            return *_pcount;
        }

        T& operator*()
        {
            return *_ptr;
        }

        T* operator->()
        {
            return _ptr;
        }

    private:
        T* _ptr;//指向资源
        int* _pcount;//引用计数
        mutex* _pmtx;
    };

    template <class T>
    class weak_ptr
    {
    public:
        weak_ptr(T* ptr = nullptr)
            :_ptr(ptr)
        {}

        // weak_ptr(const weak_ptr<T>& wp)
        //     :_ptr(wp._ptr)
        // {}

        weak_ptr(const shared_ptr<T>& sp)
            :_ptr(sp.get())
        {}

        weak_ptr<T>& operator=(const shared_ptr<T>& sp)
        {
            _ptr = sp.get();
            return *this;
        }

        T& operator*()
        {
            return *_ptr;
        }

        T* operator->()
        {
            return _ptr;
        }
    private:
        T* _ptr;
    };
}