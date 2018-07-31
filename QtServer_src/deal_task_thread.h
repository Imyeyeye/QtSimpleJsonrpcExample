#ifndef DEAL_TASK_THREAD_H
#define DEAL_TASK_THREAD_H

#include <QRunnable>
#include <tuple>
#include <list>
#include <QGenericArgument>

#include "global.h"

class Deal_Task_Thread : public QRunnable
{
public:
    Deal_Task_Thread();
    void run() Q_DECL_OVERRIDE;

    /**
     * @brief Json_Match 从json字符串中提取字段
     * @param json_str
     * @return std::tuple<MethodName,MethodParam,id>
     */
    std::tuple<QByteArray,std::list<Param_Msg>,int> Json_Match(const QByteArray &json_str);
};

#endif // DEAL_TASK_THREAD_H
