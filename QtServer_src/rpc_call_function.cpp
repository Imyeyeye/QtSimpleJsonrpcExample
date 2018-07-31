#include "rpc_call_function.h"
#include <QDebug>

#define NORMAL_RETURN(RESULT) QJsonObject object; \
    object.insert("error",QJsonValue(QJsonValue::Null)); \
    object.insert("result",RESULT); \
    return object;

#define ERROR_RETURN(ERROR) QJsonObject object,Result; \
    object.insert("error",ERROR); \
    Result.insert("Result",QJsonValue(QJsonValue::Null)); \
    object.insert("result",Result); \
    return object;


Rpc_Call_Function::Rpc_Call_Function()
{

}

Rpc_Call_Function::~Rpc_Call_Function()
{

}

const QMetaObject Rpc_Call_Function::StaticMetaObject()
{
    return this->staticMetaObject;
}

QJsonObject Rpc_Call_Function::Addition(const qint32 &A, const qint32 &B)
{
    int result = A + B ;

    QJsonObject Result;
    Result.insert("Result",result);

    NORMAL_RETURN(Result)
}

QJsonObject Rpc_Call_Function::Division(qint32 &A, qint32 &B)
{
    if(B == 0)
    {
        ERROR_RETURN("Divisor is not equal to 0")
    }
    qreal result = (qreal)A / (qreal)B;

    QJsonObject Result;
    Result.insert("Result",result);

    NORMAL_RETURN(Result)
}

