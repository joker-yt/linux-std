#ifndef __SENDER_H__
#define __SENDER_H__

#include <string>
#include "../mymsg.h"

class Sender
{
public:
    explicit Sender() : _q(std::string{"Sender"}, 1)
    {
    }

    ~Sender()
    {
    }

    void Send(const std::string &msg)
    {
        _q.Send(msg);
    }

private:
    MyMessageQueue _q;
};

#endif // __SENDER_H__