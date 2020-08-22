#include <memory>
#include <unistd.h>

#include "receiver.h"

// http://d.hatena.ne.jp/s-kita/20080713/1215940088

int main(int argc, char const *argv[])
{
    std::unique_ptr<Receiver> receiver = std::make_unique<Receiver>();

    int i = 5;
    while (i--)
    {
        receiver->Receive();
        std::cout << receiver->Message() << std::endl;
    }

    std::cout << __FUNCTION__ << ": end" << std::endl;
}
