#include <QCoreApplication>
#include <QDebug>
#include "libssh/libsshpp.hpp"
#include <iostream>

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

    }

    return a.exec();
}
