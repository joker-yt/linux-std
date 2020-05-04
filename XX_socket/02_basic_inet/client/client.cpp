#include <sys/socket.h>
#include <sys/un.h>
#include <plog/Log.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <plog/Appenders/ConsoleAppender.h>
static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;

int main(int argc, char const *argv[])
{
    plog::init(plog::verbose, &consoleAppender);

    ///////////////////////////////////////////////
    PLOGI << "create socket";
    //---------------------------------------------
    int fd;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        PLOGE << "Error: create socket";
    }

    ///////////////////////////////////////////////
    PLOGI << "set sockaddr_xx";
    //---------------------------------------------
    struct sockaddr_in sock_ad = {0};
    sock_ad.sin_family = AF_INET;
    sock_ad.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock_ad.sin_port = htons(11111);

    ///////////////////////////////////////////////
    PLOGI << "connect";
    //---------------------------------------------
    if (connect(fd, (const sockaddr *)&sock_ad, sizeof(sock_ad)) < 0)
    {
        PLOGE << "Error: connect";
    }

    ///////////////////////////////////////////////
    PLOGI << "send msg";
    //---------------------------------------------
    const char *msg = "This is test msg";
    if (send(fd, msg, strlen(msg), 0) < 0)
    {
        PLOGE << "Error: send";
    }

    char buf[512] = {0};
    if (recv(fd, buf, sizeof(buf), 0) < 0)
    {
        PLOGE << "Error: recv";
    }

    PLOGI << "CLI:RCV " << buf;

    close(fd);

    return 0;
}
