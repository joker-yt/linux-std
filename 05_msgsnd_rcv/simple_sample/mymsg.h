#ifndef __MYMSG_H__
#define __MYMSG_H__

#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string>
#include <string.h>

class MyMessageQueue
{
public:
    explicit MyMessageQueue(std::string &&owner, bool b) : _owner(owner), _qname("/tmp/yqueue")
    {
        key_t key = ftok(_qname.c_str(), 'z');
        _qid = msgget(key, IPC_CREAT | 0666);
    }
    ~MyMessageQueue()
    {
    }

    void Send(const std::string &msg)
    {
        char buf[_MSG_SIZ];
        msg.copy(buf, msg.length());
        int ret = msgsnd(_qid, buf, _MSG_SIZ - 1, MSG_NOERROR);
        if (ret < 0)
        {
            std::cout << "MyMessageQueue::" << __FUNCTION__ << " " << strerror(errno) << std::endl;
        }
    }

    std::string Receive()
    {
        char buf[_MSG_SIZ] = {0};
        int ret = msgrcv(_qid, &buf, _MSG_SIZ - 1, 0, MSG_NOERROR);
        if (ret < 0)
        {
            std::cout << "MyMessageQueue::" << __FUNCTION__ << " " << strerror(errno) << std::endl;
        }

        std::string msg{buf};
        return std::move(msg);
    }

private:
    int _qid;
    const std::string _owner;
    const std::string _qname;
    const int _MSG_SIZ = 512;
};

#endif // !__MYMSG_H__