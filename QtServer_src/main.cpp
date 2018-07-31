#include <QCoreApplication>
#include <QThreadPool>

#include "global.h"
#include "deal_task_thread.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<QPair<QVariant,QVariant> >();
    qSetMessagePattern("%{time yyyy-MM-dd h:mm:ss.zzz} %{message}");

    QCoreApplication a(argc, argv);

    registered("Addition");
    registered("Division");

    net_work = new NetWork(&a);

    QThreadPool pool(&a);
    pool.setMaxThreadCount(20);
    for (int i = 1; i <= 20; ++i) {
            Deal_Task_Thread *task = new Deal_Task_Thread();
            pool.start(task);
    }

    a.exec();

    system("taskkill /im QtSimpleJsonrpcExample.exe /f");

    return 0;
}
