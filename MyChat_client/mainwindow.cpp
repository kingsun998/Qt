#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  <sys/types.h>
#include  <socketapi.h>
#include  <errno.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <socketapi.h>
#include  <stdio.h>
#include  <string.h>
#include  <stdlib.h>
#include  <stdlib.h>
#include  <event2/event.h>
#include  <event2/util.h>
#include  <signal.h>
#include <QDebug>
#pragma comment (lib,"ws2_32.lib")
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


int tcp_connect_server(const char* server_ip, int port);

void cmd_msg_cb(int fd, short events, void* arg);
void socket_read_cb(int fd, short events, void *arg);

void run(){
       WSADATA Ws;
       //Init Windows Socket
       if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
       {
         return;
       }
       int sockfd;
       char buffer[1024];
       struct sockaddr_in server_addr;
       struct hostent *host;
       int portnumber, nbytes;

       if ((host = gethostbyname("127.0.0.1")) == NULL)
       {
         fprintf(stderr, "Gethostname error");
         qDebug()<<stderr;
         return;
       }

       if ((portnumber = atoi("9999"))<0)
       {
         fprintf(stderr, "Usage:9999 hostname portnumber");
         qDebug()<<stderr;
         return;
       }

       /* 客户程序开始建立 sockfd描述符 */
       if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
       {
         fprintf(stderr, "Socket Error:%s\a\n", strerror(errno));
         qDebug()<<stderr;
         return;
       }

       /* 客户程序填充服务端的资料    */
       memset(&server_addr,0, sizeof(server_addr));
       server_addr.sin_family = AF_INET;
       server_addr.sin_port = htons(portnumber);
       server_addr.sin_addr = *((struct in_addr *)host->h_addr);

       /* 客户程序发起连接请求     */
       if (connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
      {
         fprintf(stderr, "Connect Error:%s\a\n", strerror(errno));
         exit(1);
       }

       while (true)
       {
         char MESSAGE[] = "hello server..\n";
         //bufferevent_write(buf_ev,MESSAGE,strlen(MESSAGE));
         //
         if (-1 == (::send(sockfd, MESSAGE, strlen(MESSAGE), 0)))
         {
           printf("the net has a error occured..");
           break;
         }

         if ((nbytes = recv(sockfd, buffer, 1024,0)) == -1)
        {
           fprintf(stderr, "read error:%s\n", strerror(errno));
           exit(1);
         }

         buffer[nbytes] = '\0';
         printf("I have received:%s\n", buffer);
         memset(buffer, 0, 1024);

         Sleep(2);

       }
       /* 结束通讯   */
       closesocket(sockfd);
       exit(0);

       return 0;
     }
}

void cmd_msg_cb(int fd, short events, void* arg)
{
    char msg[1024];

    int ret = recv(fd, msg, sizeof(msg));
    if( ret <= 0 )
    {
        perror("read fail ");
        exit(1);
    }

    int sockfd = *((int*)arg);

    //把终端的消息发送给服务器端
    //为了简单起见，不考虑写一半数据的情况
    write(sockfd, msg, ret);
}


void socket_read_cb(int fd, short events, void *arg)
{
    char msg[1024];

    //为了简单起见，不考虑读一半数据的情况
    int len =  recv(msg,sizeof (msg),1, sizeof(msg)-1));
    if( len <= 0 )
    {
        perror("read fail ");
        exit(1);
    }

    msg[len] = '\0';

    printf("recv %s from server\n", msg);
}

typedef struct sockaddr SA;
int tcp_connect_server(const char* server_ip, int port)
{
    int sockfd, status, save_errno;
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr) );

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    status = inet_aton(server_ip, &server_addr.sin_addr);

    if( status == 0 ) //the server_ip is not valid value
    {
        errno = EINVAL;
        return -1;
    }

    sockfd = ::socket(PF_INET, SOCK_STREAM, 0);
    if( sockfd == -1 )
        return sockfd;


    status = ::connect(sockfd, (SA*)&server_addr, sizeof(server_addr) );

    if( status == -1 )
    {
        save_errno = errno;
        ::close(sockfd);
        errno = save_errno; //the close may be error
        return -1;
    }

    evutil_make_socket_nonblocking(sockfd);

    return sockfd;
}

void MainWindow::on_pushButton_clicked()
{

}
