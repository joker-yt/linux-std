#ifndef __RECEIVER_H__
#define __RECEIVER_H__

#include <mqueue.h>
#include "../mymsg.h"

class Receiver
{
public:
    explicit Receiver() : _q(std::string{"Receiver"}, 0)
    {
    }
    ~Receiver()
    {
    }

    void Receive()
    {
        _msg = _q.Receive();
    }

    std::string Message() const
    {
        return _msg;
    }

private:
    MyMessageQueue _q;
    std::string _msg;
};

#endif // !__RECEIVER_H__
