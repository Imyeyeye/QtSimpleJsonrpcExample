#include "deal_task_thread.h"
#include <QThread>
#include <QDebug>
#include "global.h"
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define GET_ARG(Number) QGenericArgument Number;{QByteArray ParamType = ParamMap[useful_ParamList.front().Param_Name]; \
    if(ParamType.contains("int")){Number = Q_ARG(int,useful_ParamList.front().Value.toInt(&ok));} \
    else if(ParamType.contains("float")){Number = Q_ARG(float, useful_ParamList.front().Value.toFloat(&ok)); } \
    else if(ParamType.contains("QString")){Number = Q_ARG(QString, QString(useful_ParamList.front().Value)); } \
    useful_ParamList.pop_front();}

#define TRANSFORM_JUDGE if(!ok){ \
    QJsonObject object; \
    object.insert("error","Parameter information is incorrect"); \
    object.insert("result",QJsonObject()); \
    object.insert("id",id); \
    QMetaObject::invokeMethod(net_work,"Reply",Qt::QueuedConnection,Q_ARG(QByteArray,QString(QJsonDocument(object).toJson(QJsonDocument::Compact)).toLatin1()),Q_ARG(QByteArray,uuid)); \
    continue;} \

#define AFTER_REPLY  if(success){Result.insert("id",id); \
    QMetaObject::invokeMethod(net_work,"Reply",Qt::QueuedConnection,Q_ARG(QByteArray,QString(QJsonDocument(Result).toJson(QJsonDocument::Compact)).toLatin1()),Q_ARG(QByteArray,uuid)); \
    /*qDebug()<<"ID-Call-Success:"<<id;*/ \
    }else{ QJsonObject object;object.insert("error","Parameter information is incorrect");object.insert("result",QJsonObject());object.insert("id",id); \
    QMetaObject::invokeMethod(net_work,"Reply",Qt::QueuedConnection,Q_ARG(QByteArray,QString(QJsonDocument(object).toJson(QJsonDocument::Compact)).toLatin1()),Q_ARG(QByteArray,uuid)); } \

Deal_Task_Thread::Deal_Task_Thread()
{
    setAutoDelete(true);
}

void Deal_Task_Thread::run()
{
    Rpc_Call_Function *rpc = new Rpc_Call_Function;
    const QMetaObject object = rpc->StaticMetaObject();

    while(1)
    {
        QByteArray json_str,uuid;
        {
            QMutexLocker locker(&recv_mutex);
            if(!recv_msg.empty())
            {
               auto msg =  recv_msg.front();
               json_str = std::get<0>(msg);
               uuid = std::get<1>(msg);
                recv_msg.pop();


            }
        }
        if(json_str.isEmpty())
        {
            QThread::sleep(1);
            //qDebug()<<"json_str.isEmpty";
            continue;
        }

        std::tuple<QByteArray, std::list<Param_Msg>, int> useful_tuple;

        useful_tuple = Json_Match(json_str);

        int id = std::get<2>(useful_tuple);
        //qDebug()<<"Recv-Msg-ID:"<<id;
        QByteArray AllMethodName = std::get<0>(useful_tuple);
        QByteArrayList MethodNameList = AllMethodName.split('.');
        QByteArray MethodName = MethodNameList.size()==2?MethodNameList.at(1):"";

        int MethodIndex = m_Handle[MethodName];
        if(MethodIndex > 0)
        {
            //提取Param

            QMetaMethod Method = object.method(MethodIndex-1);

            QJsonObject Result;
            QGenericReturnArgument Return_Arg = Q_RETURN_ARG(QJsonObject,Result);

            std::list<Param_Msg> useful_ParamList = std::get<1>(useful_tuple);

            std::map<QByteArray,QByteArray> ParamMap= m_ParamHandle[MethodName];

            int MethodNumber = ParamMap.size();
            if(useful_ParamList.size()!=MethodNumber)
            {
                QJsonObject object;
                object.insert("error","Parameter information is incorrect");
                object.insert("result",QJsonObject());
                object.insert("id",id);
                QMetaObject::invokeMethod(net_work,"Reply",Qt::QueuedConnection,Q_ARG(QByteArray,QString(QJsonDocument(object).toJson(QJsonDocument::Compact)).toLatin1()),Q_ARG(QByteArray,uuid));
                continue;
            }

            //匹配参数个数
            switch (MethodNumber) {
            case 0:
            {
                bool success = Method.invokeOnGadget(rpc,Return_Arg);
                AFTER_REPLY
                }
                break;
            case 1:
            {

                bool ok = true;
                GET_ARG(arg1);TRANSFORM_JUDGE;
                bool success = Method.invokeOnGadget(rpc,Return_Arg,arg1);
                AFTER_REPLY
                }
                break;

            case 2:
            {
                bool ok = true;
                GET_ARG(arg1);TRANSFORM_JUDGE;
                GET_ARG(arg2);TRANSFORM_JUDGE;
                bool success = Method.invokeOnGadget(rpc,Return_Arg, arg1, arg2);
                AFTER_REPLY

                }
                break;

            case 3:
            {
                bool ok = true;
                GET_ARG(arg1);TRANSFORM_JUDGE;
                GET_ARG(arg2);TRANSFORM_JUDGE;
                GET_ARG(arg3);TRANSFORM_JUDGE;
                bool success = Method.invokeOnGadget(rpc,Return_Arg,arg1,arg2,arg3);
                AFTER_REPLY
                }
                break;
            case 4:
            {
                bool ok = true;
                GET_ARG(arg1);TRANSFORM_JUDGE;
                GET_ARG(arg2);TRANSFORM_JUDGE;
                GET_ARG(arg3);TRANSFORM_JUDGE;
                GET_ARG(arg4);TRANSFORM_JUDGE;
                bool success = Method.invokeOnGadget(rpc,Return_Arg,arg1,arg2,arg3,arg4);
                AFTER_REPLY
                }
                break;
            case 5:
            {
                bool ok = true;
                GET_ARG(arg1);TRANSFORM_JUDGE;
                GET_ARG(arg2);TRANSFORM_JUDGE;
                GET_ARG(arg3);TRANSFORM_JUDGE;
                GET_ARG(arg4);TRANSFORM_JUDGE;
                GET_ARG(arg5);TRANSFORM_JUDGE;
                bool success = Method.invokeOnGadget(rpc,Return_Arg,arg1,arg2,arg3,arg4,arg5);
                AFTER_REPLY
                }
                break;
            default:break;
            }

        }
        else
        {
            //没有找到对应函数
            QJsonObject object;
            object.insert("error","No corresponding method found");
            object.insert("result",QJsonObject());
            object.insert("id",id);
            QMetaObject::invokeMethod(net_work,"Reply",Qt::QueuedConnection,Q_ARG(QByteArray,QString(QJsonDocument(object).toJson(QJsonDocument::Compact)).toLatin1()),Q_ARG(QByteArray,uuid));

        }

    }

}


std::tuple<QByteArray, std::list<Param_Msg>, int> Deal_Task_Thread::Json_Match(const QByteArray &json_str)
{
    QByteArray MethodName;
    std::list<Param_Msg> Param_msg;
    int Id;

    QJsonDocument document = QJsonDocument::fromJson(json_str);
    if(!document.isEmpty())
    {
        if(document.isObject())
        {
            QJsonObject object = document.object();
            //提取method
            if(object.contains("method"))
            {
                QJsonValue method_value = object.value("method");
                QVariant var_method = method_value.toVariant();
                if(var_method.canConvert(QMetaType::QByteArray))
                {
                    MethodName = var_method.toByteArray();
                }
            }
            //提取params
            if(object.contains("params"))
            {
                QJsonValue params_value = object.value("params");
                if(params_value.isArray())
                {
                    QJsonArray array = params_value.toArray();
                    if(array.size()==1)
                    {
                        QJsonValue param_array = array.at(0);
                        if(param_array.isObject())
                        {
                            QJsonObject param_object = param_array.toObject();

                            QJsonObject::Iterator it;
                            for(it = param_object.begin();it!=param_object.end();++it)
                            {
                                Param_Msg param_msg;
                                param_msg.Param_Name = it.key().toLatin1();
                                param_msg.Value = it.value().toVariant().toByteArray();
                                Param_msg.push_back(param_msg);
                                // qDebug()<<"get:"<<it.key()<<","<<it.value().toVariant().toByteArray();

                            }
                        }
                    }
                }
            }
            //提取id
            if(object.contains("id"))
            {
                QJsonValue id_value = object.value("id");
                QVariant var_id = id_value.toVariant();
                if(var_id.canConvert(QMetaType::Int))
                {
                    Id = var_id.toInt();
                }
            }
        }
    }

    return std::make_tuple(MethodName,Param_msg,Id);
}



