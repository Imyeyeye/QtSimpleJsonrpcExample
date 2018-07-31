#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <map>

class NetWork : public QObject
{
    Q_OBJECT
public:
    explicit NetWork(QObject *parent = nullptr);
    Q_INVOKABLE Reply(QByteArray result,QByteArray uuid);
private:
    QTcpServer *tcp_server;
    std::map<QByteArray,QTcpSocket*> socket_list;//Quuid->QByteArray,QTcpSocket*
signals:

public slots:
};

#endif // NETWORK_H
