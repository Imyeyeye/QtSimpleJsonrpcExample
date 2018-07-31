#include "global.h"

#include <QDebug>

std::map<QByteArray,int> m_Handle;

std::map<QByteArray,std::map<QByteArray,QByteArray> > m_ParamHandle;

NetWork *net_work;

QMutex recv_mutex;

std::queue<std::tuple<QByteArray,QByteArray> > recv_msg;

bool registered(QByteArray FunName)
{
    Rpc_Call_Function rpc;
    const QMetaObject object = rpc.StaticMetaObject();
    for(int i = object.methodOffset(); i < object.methodCount(); ++i)
    {
        if(object.method(i).methodSignature().contains(FunName))
        {
            m_Handle[FunName] = i+1;

            if(object.method(i).parameterCount()>0)
            {
                QList<QByteArray> parameterNames = object.method(i).parameterNames();
                QList<QByteArray> parameterTypes = object.method(i).parameterTypes();

                std::map<QByteArray,QByteArray> Param_Info_map;
                for(int index = 0 ; index < object.method(i).parameterCount();++index)
                {
                    Param_Info_map[parameterNames[index]] = parameterTypes[index];
                }
                m_ParamHandle[FunName] = Param_Info_map;
                //qDebug()<<m_ParamHandle[FunName];
            }
            return true;
        }
    }
    return false;
}


