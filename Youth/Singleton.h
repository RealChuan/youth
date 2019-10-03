#ifndef SINGLETON_H
#define SINGLETON_H

#include <assert.h>

#include "Thread.h"

namespace youth {

template <typename T>
class Singleton {
public:
    static T& getInstance(); // 获取单例的唯一实例对象

    Singleton(const Singleton &other) = delete;
    Singleton<T>& operator=(const Singleton &other) = delete;

private:
    static void init(){
        MutexLock lock(mutex);
        if(nullptr == instance.get())
            instance.reset(new T());
    }
    static pthread_once_t ponce_;
    static std::shared_ptr<T> instance;
    static Mutex mutex;
};

template <typename T> pthread_once_t Singleton<T>::ponce_;
template <typename T> std::shared_ptr<T> Singleton<T>::instance;
template <typename T> Mutex Singleton<T>::mutex;
template <typename T>
T& Singleton<T>::getInstance() {
    pthread_once(&ponce_, &Singleton::init);
    assert(nullptr != instance.get());
    return *instance.get();
}

#define SINGLETON(Class)                           \
private:                                           \
    Class();                                       \
    Class(const Class &other) = delete;            \
    Class& operator=(const Class &other) = delete; \
    friend class  Singleton<Class>;                \
                                                   \
public:                                            \
    ~Class();                                      \
    static Class& instance() {                     \
        return Singleton<Class>::getInstance();    \
    }                                              \

}

#endif // SINGLETON_H
