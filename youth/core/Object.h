#ifndef OBJECT_H
#define OBJECT_H

namespace youth
{

class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

class copyable
{
protected:
    copyable() = default;
    ~copyable() = default;
};

}

#endif // OBJECT_H
