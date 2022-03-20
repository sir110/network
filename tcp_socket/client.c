#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd = 0, res = 0;
    char buf[20];
    struct sockaddr_in servaddr;

    //创建套接字
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket error");
        exit(1);
    }

    //设置服务器地址结构
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    res = inet_aton("localhost", &servaddr.sin_addr);
    if (res < 0)
    {
        printf("inet_pton error\n");
        exit(1);
    }

    //发送连接请求
    res = connect(fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (res < 0)
    {
        perror("connect error");
        exit(1);
    }

    //写数据
    strcpy(buf, "Hello Server.");
    res = write(fd, buf, sizeof(buf));
    if (res < 0)
    {
        perror("write error");
        exit(1);
    }
    printf("client write len: %d, write msg: %s\n", res, buf);

    //读数据
    res = read(fd, buf, sizeof(buf));
    if (res < 0)
    {
        perror("read error");
        exit(1);
    }
    printf("client read len: %d, read msg: %s\n", res, buf);

    //关闭套接字
    res = close(fd);
    if (res < 0)
    {
        perror("close error");
        exit(1);
    }

    return 0;
}