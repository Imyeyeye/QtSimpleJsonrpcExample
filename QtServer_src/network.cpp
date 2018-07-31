#include "network.h"
#include "global.h"

#include <QUuid>

NetWork::NetWork(QObject *parent) : QObject(parent)
{
    tcp_server = new QTcpServer;
        tcp_server->listen(QHostAddress::Any,1234);
        QObject::connect(tcp_server,&QTcpServer::newConnection,[=]{

            auto uuid = QUuid::createUuid().toByteArray();
            auto tcp_Socket = tcp_server->nextPendingConnection();
            socket_list[uuid] = tcp_Socket;


            QObject::connect(tcp_Socket,&QTcpSocket::disconnected,[=]{
                tcp_Socket->close();
                tcp_Socket->deleteLater();
                socket_list.erase(uuid);
            });
            QObject::connect(tcp_Socket,&QTcpSocket::readyRead,[=]{
                QByteArray json_str = tcp_Socket->readLine();//tcp_Socket->readAll();
                QMutexLocker locker(&recv_mutex);
                recv_msg.push(std::make_pair(json_str,uuid));//json_str);
                //qDebug()<<"push:"<<json_str<<",id:"<<uuid;;
            });
        });

}

NetWork::Reply(QByteArray result,QByteArray uuid)
{
    if(socket_list[uuid]!=0)
    {
        socket_list[uuid]->write(result);
    }
}
