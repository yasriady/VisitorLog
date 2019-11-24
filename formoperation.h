#ifndef FORMOPERATION_H
#define FORMOPERATION_H

#include "server.h"
#include "rfidata.h"
#include <QWidget>
#include <db/dbreffetcher.h>

namespace Ui
{
class FormOperation;
}

class FormOperation : public QWidget
{
    Q_OBJECT
public:
    explicit FormOperation(QWidget *parent = nullptr);
    ~FormOperation();

private:
    Ui::FormOperation *ui;
    Server *mServer;
    void initUi();
    DbRefFetcher *mDbFetcher;
    void updateUi(const Member member);

    void processUid(const QString uid);
    // void requestData(const QString uid);

public slots:
    void onNewData(RFIData data);
    void onMemberDataReady(Member member);


};

#endif // FORMOPERATION_H
