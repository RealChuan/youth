#ifndef WEAKCALLBACK_H
#define WEAKCALLBACK_H

#include <functional>
#include <memory>

namespace youth {

namespace core {

// A barely usable WeakCallback

template<typename CLASS, typename... ARGS>
class WeakCallback
{
public:
    WeakCallback(const std::weak_ptr<CLASS> &object,
                 const std::function<void(CLASS *, ARGS...)> &function)
        : object_(object)
        , function_(function)
    {}

    ~WeakCallback() {}

    // Default dtor, copy ctor and assignment are okay
    void operator()(ARGS &&...args) const
    {
        std::shared_ptr<CLASS> ptr(object_.lock());
        if (ptr) {
            function_(ptr.get(), std::forward<ARGS>(args)...);
        }
        // else
        // {
        //   LOG_TRACE << "expired";
        // }
    }

private:
    std::weak_ptr<CLASS> object_;
    std::function<void(CLASS *, ARGS...)> function_;
};

template<typename CLASS, typename... ARGS>
WeakCallback<CLASS, ARGS...> makeWeakCallback(const std::shared_ptr<CLASS> &object,
                                              void (CLASS::*function)(ARGS...))
{
    return WeakCallback<CLASS, ARGS...>(object, function);
}

template<typename CLASS, typename... ARGS>
WeakCallback<CLASS, ARGS...> makeWeakCallback(const std::shared_ptr<CLASS> &object,
                                              void (CLASS::*function)(ARGS...) const)
{
    return WeakCallback<CLASS, ARGS...>(object, function);
}

} // namespace core

} // namespace youth

#endif // WEAKCALLBACK_H
