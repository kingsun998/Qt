#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QDebug"

#include "event2/event.h"
#include "event2/bufferevent.h"
#include "event2/buffer.h"
#include "event2/listener.h"
#include "event2/util.h"
#include "event2/http.h"

#include "signal.h"

#pragma comment(lib, "ws2_32")
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







static const char MESSAGE[] = "Hello, World!\n";

static const int PORT = 9995;
static void listener_cb(struct evconnlistener *, evutil_socket_t,
    struct sockaddr *, int socklen, void *);
static void signal_cb(evutil_socket_t, short, void *);
static void conn_writecb(struct bufferevent *, void *);
static void conn_eventcb(struct bufferevent *, short, void *);


void MainWindow::init(){
    struct event_base* base;
    struct evconnlistener *listener;
    struct event *signal_event;
    // 该结构体解决了sockaddr的缺陷，把port和addr 分开储存在两个变量中：
    // 用来处理网络通信的地址。
    struct sockaddr_in sin;

    WSADATA wsa_data;
    WSAStartup(0x0201,&wsa_data);

    base = event_base_new();
    if(!base){
        qDebug()<<"cound not initialize libevent";
    }

    memset(&sin, 0, sizeof (sin));
    sin.sin_family=AF_INET;
    //htons是将整型变量从主机字节顺序转变成网络字节顺序，
    //就是整数在地址空间存储方式变为高位字节存放在内存的低地址处。
    sin.sin_port = htons(PORT);
    // 绑定监听器至event_base,更高级的封装
    // 封装了socket，bind，listen，accept
    // base事件根节点
    // listener_cb 回调，每当有新连接的时候调用
    // (void*)base 回调参数
    // flags：
    //      LEV_OPT_LEAVE_SOCKETS_BLOCKING 文件描述符为阻塞
    //      LEV_OPT_CLOSE_ON_FREE 关闭时自动释放
    //      LEV_OPT_REUSEABLE 端口复用
    //      LEV_OPT_THREADSAFE 奉陪锁，线程安全
    //      技巧：如何传入多个参数，这里是使用多个参数，将参数定义为 2^n,这样与可以获得组合， 3=1+2，9=8+1
    // -1 为backlog，-1表示自动选择一个合适的值
    // sin是服务器需要绑定的ip端口信息
    // sizeof(sin) 表示地址的长度
    listener = evconnlistener_new_bind(base,listener_cb,(void *)base,
                                       LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,
                                       -1,
                                       (struct sockaddr*)&sin,
                                       sizeof (sin));
    if(!listener){
        qDebug()<<"Could not create a listener";
    }
    // 创建信号，base为根事件，
    qDebug()<<"create signal ...";
    signal_event=evsignal_new(base,SIGINT,signal_cb,(void *)base);

    if (!signal_event || event_add(signal_event, NULL)<0) {
        qDebug()<<"Could not create/add a signal event!";
        return;
    }
    //在libevent中，Reactor 框架组件表现为event_base结构体
    //循环将运行event_base，知道没有等待的或活动的时间
    qDebug()<<"exit loop";
    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_free(signal_event);
    event_base_free(base);

    qDebug()<<"done";
}
//在Windows中，libevent定义evutil_socket_t类型为整型指针，
//可以处理socket()或者accept()的输出，而没有指针截断的风险。
static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
    qDebug()<<"receive message ...";

    struct event_base *base = (event_base*)user_data;
    // bufferevent专门为封装成带有缓冲区的socket套接字。
    // 当有数据到来时，我们只需要在回调函数里面通过封装函数bufferevent_read读取数据即可，
    // 根本不需要自己处理一些细节，以及缓存的问题。
    struct bufferevent *bev;
    // 创建bufferenvt,绑定套接字
    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        qDebug()<<"Error constructing bufferevent!";
        // 退出event_base_loop
        event_base_loopbreak(base);
        return;
    }
    // 该函数的作用主要是赋值，把该函数后面的参数，赋值给第一个参数struct bufferevent *bufev定义的变量
    // 设置读写对应的回调函数,这里将读的对应函数设置为 NULL
    bufferevent_setcb(bev, NULL, conn_writecb, conn_eventcb, NULL);
    // bufferevent_enable调用event_add将读写事件加入到事件监听队列中。
    bufferevent_enable(bev, EV_WRITE);
    bufferevent_disable(bev, EV_READ);

    // 写入信息
    qDebug()<<"writing message";
    bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
    qDebug()<<"writed message";
}
static void conn_writecb(struct bufferevent *bev, void *user_data)
{
    struct evbuffer *output = bufferevent_get_output(bev);
    qDebug()<<"触发 write call back";
    if (evbuffer_get_length(output) == 0) {
        qDebug()<<"flushed answer\n";
        bufferevent_free(bev);
    }
}

static void conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
    if (events & BEV_EVENT_EOF) {
        printf("Connection closed.\n");
    } else if (events & BEV_EVENT_ERROR) {
        printf("Got an error on the connection: %s\n",
            strerror(errno));/*XXX win32*/
    }
    /* None of the other events can happen here, since we haven't enabled
     * timeouts */
    bufferevent_free(bev);
}

static void signal_cb(evutil_socket_t sig, short events, void *user_data)
{
    struct event_base *base = (event_base *)user_data;
    struct timeval delay = { 2, 0 };

    qDebug()<<"Caught an interrupt signal; exiting cleanly in two seconds";
    //退出循环
    event_base_loopexit(base, &delay);
}

void MainWindow::on_pushButton_clicked()
{
    init();
}
