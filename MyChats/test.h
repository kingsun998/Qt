﻿#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <event2/event.h>

extern class test test;
class test : public QObject
{
    Q_OBJECT
public:
    explicit test(QObject *parent = nullptr);


    const char *get_file_type(const char *name);

    int hexit(char c);

    void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
                     struct sockaddr *sa, int socklen, void *user_data);

    int response_http(struct bufferevent *bev, const char *method, char *path);

    int send_dir(struct bufferevent *bev,const char *dirname);

    int send_error(struct bufferevent *bev);

    int send_file_to_http(const char *filename, struct bufferevent *bev);

    int send_header(struct bufferevent *bev, int no, const char* desp, const char *type, long len);

    void signal_cb(evutil_socket_t sig, short events, void *user_data);

    void strdecode(char *to, char *from);

    void strencode(char* to, size_t tosize, const char* from);

signals:

public slots:
};

#endif // TEST_H


