#include <memory>
#include <unistd.h>

#include "sender.h"

int main(int argc, char *argv[])
{
    std::unique_ptr<Sender> sender = std::make_unique<Sender>();

    sleep(3);

    int i = 5;
    while (i--)
    {
        std::cout << "message?" << std::endl;
        std::string msg;
        std::cin >> msg;

        std::cout << __FUNCTION__ << ": send message[" << msg << "]" << std::endl;
        sender->Send(msg);
        sleep(2);
    }

    return 0;
}