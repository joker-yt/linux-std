#ifndef __MYMSG_H__
#define __MYMSG_H__

#include <iostream>
#include <mqueue.h>
#include <errno.h>
#include <string>
#include <string.h>

class MyMessageQueue
{
public:
    explicit MyMessageQueue(std::string &&owner, bool b) : _owner(owner), _qname("/test")
    {
        int f = b ? O_WRONLY : O_RDONLY;
        _mqd = mq_open(_qname.c_str(), f | O_CREAT | O_EXCL, 0666, NULL);
        if (_mqd < 0)
            _mqd = mq_open(_qname.c_str(), f, 0666, NULL);

        if (_mqd < 0)
        {
            std::cerr << __FUNCTION__ << ": mq_open error -> " << strerror(errno) << std::endl;
        }

        mq_getattr(_mqd, &_attr);
        std::cout << __FUNCTION__ << ": msgmax -> " << _attr.mq_maxmsg << " msgsize ->  "
                  << _attr.mq_msgsize << std::endl;
    }
    ~MyMessageQueue()
    {
        std::cout << __FUNCTION__ << ": start unlink " << std::endl;
        if (mq_unlink(_qname.c_str()))
        {
            std::cerr << __FUNCTION__ << ": unlink..err " << strerror(errno) << std::endl;
        }
        std::cout << __FUNCTION__ << ": unlink..end " << std::endl;
    }

    void Send(const std::string &msg)
    {
        if (mq_send(_mqd, msg.c_str(), _attr.mq_msgsize, 0))
        {
            std::cerr << __FUNCTION__ << ": mq_send error -> " << strerror(errno) << std::endl;
        }
    }

    std::string Receive()
    {
        // char buf[32]; too small! it caused "Bad Address".
        char *buf = (char *)malloc(sizeof(char) * _attr.mq_msgsize);
        memset(buf, 0, sizeof(char) * _attr.mq_msgsize);
        ssize_t sz = mq_receive(_mqd, buf, _attr.mq_msgsize, 0);
        if (sz < 0)
        {
            std::cerr << __FUNCTION__ << ": receive error -> " << strerror(errno) << std::endl;
        }
        std::cout << __FUNCTION__ << ": msg(" << sz << ")[" << buf << "]" << std::endl;

        std::string msg{buf};
        return std::move(msg);
    }

private:
    const std::string _owner;
    const std::string _qname;
    mqd_t _mqd;
    struct mq_attr _attr;
};

#endif // !__MYMSG_H__