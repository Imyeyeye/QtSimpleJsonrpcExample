#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <map>
#include <queue>
#include <QMetaMethod>
#include <QMetaObject>
#include <QTcpSocket>
#include <QMutex>

#include "rpc_call_function.h"
#include "network.h"

//参数信息
struct Param_Msg{
    QByteArray Param_Name;  //参数名字
    QByteArray Value;  //参数值
};

//rpc调用方法和索引集 <MethodName,MethodIndex>
extern std::map<QByteArray,int> m_Handle;

//对应方法的参数名和参数信息 <MethodName,std::map<Param_Name,Param_Type> >
extern std::map<QByteArray,std::map<QByteArray,QByteArray> > m_ParamHandle;

extern NetWork *net_work;

extern QMutex recv_mutex;

//json_str,Quuid->QByteArray
extern std::queue<std::tuple<QByteArray,QByteArray> > recv_msg;//接收的jsonrpc字符串

extern bool registered(QByteArray FunName);//注册rpc可调用函数


#endif // GLOBAL_H
