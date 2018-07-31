#ifndef RPC_CALL_FUNCTION_H
#define RPC_CALL_FUNCTION_H

#include <QObject>
#include <QJsonObject>

class Rpc_Call_Function
{
    Q_GADGET
public:
    //rpc调用函数
    Q_INVOKABLE QJsonObject Addition(const qint32 &A,const qint32 &B);//加法
    Q_INVOKABLE QJsonObject Division(qint32 &A,qint32 &B);//除法
public:
    Rpc_Call_Function();
    ~Rpc_Call_Function();
    const QMetaObject StaticMetaObject();
};


#endif // RPC_CALL_FUNCTION_H
