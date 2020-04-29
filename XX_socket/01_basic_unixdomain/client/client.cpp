#include <sys/socket.h>
#include <sys/un.h>
#include <plog/Log.h>

#include <plog/Appenders/ConsoleAppender.h>
static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;

int main(int argc, char const *argv[])
{
    plog::init(plog::verbose, &consoleAppender);

    ///////////////////////////////////////////////
    PLOGI << "create socket";
    //---------------------------------------------
    int fd;
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        PLOGE << "Error: create socket";
    }

    ///////////////////////////////////////////////
    PLOGI << "set sockaddr_xx";
    //---------------------------------------------
    struct sockaddr_un sock_ad = {0};
    sock_ad.sun_family = AF_UNIX;
    strcpy(sock_ad.sun_path, "/tmp/afunix_test_socket");

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

    return 0;
}
