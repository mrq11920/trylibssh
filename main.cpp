#include <QCoreApplication>
#include <QDebug>
#include "libssh/libsshpp.hpp"
#include <iostream>

int show_remote_processes(ssh::Session& s)
{
    ssh::Channel channel(s);
    int rc;
    char buffer[256];
    int nbytes;

    channel.openSession();

    channel.requestExec("ps aux");

    nbytes = channel.read(buffer,sizeof(buffer),-1);
    while(nbytes > 0)
    {
        if(write(1,buffer,nbytes) != (unsigned int)nbytes)
        {
            channel.close();
            return false;
        }
        nbytes = channel.read(buffer,sizeof(buffer),-1);
    }
    if(nbytes < 0)
    {
        channel.close();
    }
    channel.sendEof();
    channel.close();
    return true;
}
int direct_tcp(ssh::Session& s)
{
    int rc;
    char *http_get = "GET / HTTP/1.1\nHost: www.google.com\n\n";
    int nbytes, nwritten;
    char buffer[256];
    ssh::Channel channel(s);
    rc = channel.openForward("www.google.com",443,"localhost",5555);
    nbytes =strlen(http_get);
    nwritten = channel.write(http_get,nbytes);
    if(nbytes != nwritten)
    {
        return false;
    }
    nbytes = channel.read(buffer,sizeof(buffer),-1);
    while(nbytes > 0)
    {
        if(write(1,buffer,nbytes) != (unsigned int)nbytes)
        {
            channel.close();
            return false;
        }
        nbytes = channel.read(buffer,sizeof(buffer),-1);
    }

    return true;
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    int verbosity = SSH_LOG_PROTOCOL;
    char* password;
    ssh::Session s;

    s.setOption(SSH_OPTIONS_HOST,"localhost");
    s.setOption(SSH_OPTIONS_LOG_VERBOSITY,verbosity);
    s.setOption(SSH_OPTIONS_PORT,22);
    s.connect();
    bool isKnown = s.isServerKnown();
    qDebug() << "isKnown --> " << isKnown;
    password = getpass("Password: ");

    auto auth = s.userauthPassword(password);
    qDebug() << "auth --> " << auth;
    if(auth == SSH_AUTH_SUCCESS)
    {
        direct_tcp(s);
//        show_remote_processes(s);
    }

    return a.exec();
}
