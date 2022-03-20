#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct addrinfo hint, *result;
    struct sockaddr remote;
    int res, listen_fd, conn_fd;
    char buf[100];

    memset(&hint, 0, sizeof(hint));
    hint.ai_family   = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = 0;
    hint.ai_flags    = AI_PASSIVE;

    res = getaddrinfo(NULL, "8080", &hint, &result);
    if (res != 0)
    {
        perror("error : cannot get socket address!\n");
        exit(1);
    }

    //初始化监听套接字
    listen_fd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listen_fd == -1)
    {
        perror("error : cannot get socket file descriptor!\n");
        exit(1);
    }
    
    //绑定监听套接字和端口
    res = bind(listen_fd, result->ai_addr, result->ai_addrlen);
    if (res == -1)
    {
        perror("error : cannot bind the socket with the given address!\n");
        exit(1);
    }

    //监听客户端请求
    res = listen(listen_fd, SOMAXCONN);
    if (res == -1)
    {
        perror("error : cannot listen at the given socket!\n");
        exit(1);
    }

    //接受客户端请求
    while (1)
    {
        int len = sizeof(struct sockaddr);
        conn_fd = accept(listen_fd, &remote, &len);

        res = read(conn_fd, buf, sizeof(buf));
        printf("server read len: %d, read data : %s\n", res, buf);

        //模拟阻塞处理过程
        sleep(2);        

        strcpy(buf, "Hello Client");
        res = write(conn_fd, buf, sizeof(buf));
        printf("server write len: %d, write data: %s\n", res, buf);
    }
    
    //关闭监听套接字
    close(listen_fd);
}