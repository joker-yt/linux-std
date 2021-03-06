#include <sys/socket.h>
#include <sys/un.h>
#include <plog/Log.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

///////////////////////////////////////////////
#include <openssl/ssl.h> // header of OpenSSL
#include <openssl/err.h>

#include <plog/Appenders/ConsoleAppender.h>
static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;

int main(int argc, char const *argv[])
{
    plog::init(plog::verbose, &consoleAppender);
    ///////////////////////////////////////////////
    PLOGI << "intialization for SSL (things to do once in sequence)";
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    SSL_load_error_strings();

    ///////////////////////////////////////////////
    PLOGI << "normal socket intialization";
    //---------------------------------------------
    int fd;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        PLOGE << "Error: create socket";
    }

    struct sockaddr_in sock_ad = {0};
    sock_ad.sin_family = AF_INET;
    sock_ad.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock_ad.sin_port = htons(11111);

    if (bind(fd, (const struct sockaddr *)&sock_ad, sizeof(sock_ad)) < 0)
    {
        PLOGE << "Error: bind";
    }

    PLOGI << "listen <- preparing to accept connecting request.";
    //---------------------------------------------
    if (listen(fd, 5) < 0)
    {
        PLOGE << "Error listen ";
    }

    ///////////////////////////////////////////////
    PLOGI << "accept";
    //---------------------------------------------
    int target_fd;
    struct sockaddr sock_ad_accepted;
    socklen_t sockaddr_len = sizeof(sock_ad_accepted);

    SSL *ssl;
    SSL_CTX *ctx;
    while (1)
    {
        memset(&sock_ad_accepted, 0, sockaddr_len);
        if ((target_fd = accept(fd, &sock_ad_accepted, &sockaddr_len)) < 0)
        {
            PLOGE << "Error: accept";
            continue;
        }

        while (1)
        {
            PLOGI << "SSL sequence";
            int ssl_eno;
            int ret;

            char crt_file[] = "server.crt";
            char key_file[] = "server.key";
            if ((ctx = SSL_CTX_new(SSLv23_server_method())) == nullptr)
            {
                PLOGE << "SSL_CTX_new";
                break;
            }

            if ((ret = SSL_CTX_use_certificate_file(ctx, crt_file, SSL_FILETYPE_PEM)) != 1)
            {
                PLOGE << "SSL_CTX_use_certificate_file";
                break;
            }

            if ((ret = SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM)) != 1)
            {
                PLOGE << "SSL_CTX_use_PrivateKey_file";
                break;
            }

            if ((ssl = SSL_new(ctx)) == nullptr)
            {
                PLOGE << "SSL_new";
                break;
            }

            if ((ret = SSL_set_fd(ssl, target_fd)) == 0)
            {
                PLOGE << "SSL_set_fd";
                break;
            }

            if ((ret = SSL_accept(ssl)) < 0)
            {
                ssl_eno = SSL_get_error(ssl, ret);
                char msg[256] = {0};
                ERR_error_string_n(ERR_get_error(), msg, sizeof(msg));
                PLOGE << "Error: SSL_accept " << msg;
            }
            else
            {
                break;
            }

            sleep(1);
        }

        ///////////////////////////////////////////////
        PLOGI << "receive msg";
        ssize_t sz;
        char buf[256] = {0};
        sz = SSL_read(ssl, buf, sizeof(buf));
        PLOGI << "SRV:RCV " << buf;
        const char *res_msg = u8"ack from SRV";
        SSL_write(ssl, res_msg, strlen(res_msg));
    }

    ///////////////////////////////////////////////

    PLOGI << "shutdown SSL.";
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    close(fd);

    return 0;
}
