#include <sys/socket.h>
#include <sys/un.h>
#include <plog/Log.h>
#include <netinet/in.h>
#include <arpa/inet.h>

///////////////////////////////////////////////
#include <openssl/ssl.h> // header of OpenSSL
#include <openssl/err.h>

#include <plog/Appenders/ConsoleAppender.h>
static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;

int main(int argc, char const *argv[])
{
    plog::init(plog::verbose, &consoleAppender);

    ///////////////////////////////////////////////
    PLOGI << "normal socket intialization";
    //---------------------------------------------
    int fd;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        PLOGE << "Error: create socket";
    }

    //---------------------------------------------
    struct sockaddr_in sock_ad = {0};
    sock_ad.sin_family = AF_INET;
    sock_ad.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock_ad.sin_port = htons(11111);

    //---------------------------------------------
    while (1)
    {
        if (connect(fd, (const sockaddr *)&sock_ad, sizeof(sock_ad)) < 0)
        {
            PLOGE << "Error: connect";
            sleep(1);
        }
        else
        {
            break;
        }
    }

    ///////////////////////////////////////////////
    PLOGI << "SSL sequence";
    int err;
    SSL *ssl;
    SSL_CTX *ctx;
    SSL_load_error_strings();
    SSL_library_init();

    ctx = SSL_CTX_new(SSLv23_client_method());
    SSL_CTX_set_cipher_list(ctx, "AES256+GCM+SHA384");
    int ret;

    char crt_file[] = "server.crt";
    char key_file[] = "server.key";

    ret = SSL_CTX_use_certificate_file(ctx, crt_file, SSL_FILETYPE_PEM);

    ssl = SSL_new(ctx);
    if (!(err = SSL_set_fd(ssl, fd)))
    {
        PLOGE << "Error: SSL_set_fd";
    }

    int ssl_eno;
    while (1)
    {
        if ((err = SSL_connect(ssl)) < 0)
        {
            PLOGE << "Error: SSL_connect";
            ssl_eno = SSL_get_error(ssl, err);
            ERR_load_crypto_strings();
            SSL_load_error_strings(); // just once
            char msg[1024];
            ERR_error_string_n(ERR_get_error(), msg, sizeof(msg));
            PLOGE << "Error: SSL_connect " << msg;
            sleep(1);
            continue;
        }
        else if (!err)
        {
            PLOGE << "Error: SSL_connect(0)";
            ssl_eno = SSL_get_error(ssl, err);
            ERR_load_crypto_strings();
            SSL_load_error_strings(); // just once
            char msg[1024];
            ERR_error_string_n(ERR_get_error(), msg, sizeof(msg));
            PLOGE << "Error: SSL_connect(0) " << msg;
            sleep(1);
            continue;
        }
        else
        {
            break;
        }
    }

    ///////////////////////////////////////////////
    PLOGI << "send msg";
    // //---------------------------------------------
    // const char *msg = "This is test msg";
    // if (SSL_write(ssl, msg, strlen(msg)) < 0)
    // {
    //     PLOGE << "Error: send";
    // }

    char buf[512] = {0};
    if (SSL_read(ssl, buf, sizeof(buf)) < 0)
    {
        PLOGE << "Error: recv";
    }

    PLOGI << "CLI:RCV " << buf;

    ///////////////////////////////////////////////
    PLOGI << "shutdown SSL.";
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    fd = SSL_get_fd(ssl);
    SSL_free(ssl);
    close(fd);

    return 0;
}
