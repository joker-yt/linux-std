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
    PLOGI << "bind sockaddr_xx to socket";
    //---------------------------------------------
    if (bind(fd, (const sockaddr *)&sock_ad, sizeof(sock_ad)) < 0)
    {
        PLOGE << "Error: bind";
    }

    ///////////////////////////////////////////////
    PLOGI << "listen <- preparing to accept connecting request.";
    //---------------------------------------------
    if (listen(fd, 5) < 0)
    {
        PLOGE << "listen";
    }

    ///////////////////////////////////////////////
    PLOGI << "accept";
    //---------------------------------------------
    int target_fd;
    struct sockaddr sock_ad_accepted;
    socklen_t sockaddr_len = sizeof(sock_ad_accepted);
    ssize_t sz;
    char buf[512];
    while (1)
    {
        memset(&sock_ad_accepted, 0, sockaddr_len);
        memset(buf, 0, sizeof(buf));
        if ((target_fd = accept(fd, &sock_ad_accepted, &sockaddr_len)) < 0)
        {
            PLOGE << "Error: accept";
            continue;
        }

        sz = recv(target_fd, buf, sizeof(buf), 0);
        PLOGI << "SRV:RCV " << buf;
        const char *res_msg = u8"ack from SRV";
        send(target_fd, res_msg, strlen(res_msg), 0);
    }

    close(fd);

    return 0;
}
