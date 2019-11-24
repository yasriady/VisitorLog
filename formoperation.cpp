#include "formoperation.h"
#include "ui_formoperation.h"

FormOperation::FormOperation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormOperation)
{
    ui->setupUi(this);
    initUi();

    mServer = new Server(this);
    connect(mServer, SIGNAL(newData(RFIData)), this, SLOT(onNewData(RFIData)));

    mDbFetcher = new DbRefFetcher(this);
    connect(mDbFetcher, SIGNAL(memberDataReady(Member)), this, SLOT(onMemberDataReady(Member)));
}

FormOperation::~FormOperation()
{
    if (mServer != nullptr) {
        mServer->close();
        mServer = nullptr;
    }
    delete ui;
}

void FormOperation::initUi()
{
    ui->frame->hide();
}

void FormOperation::updateUi(const Member member)
{
    ui->name->setText(member.getNama());
    ui->idNumber->setText(member.getNim());
    ui->uid->setText(member.getUid());
}

void FormOperation::onNewData(RFIData data)
{
    switch (data.type) {

    case ResultTypes::Disconnected:
        ui->msg->setText(data.string);
        break;
    case ResultTypes::Error:
        ui->msg->setText(data.string);
        break;
    case ResultTypes::IncomingConnection:
        ui->msg->setText(data.string);
        break;
    case ResultTypes::NewData:
        // ui->uid->setText(data.uid);
        processUid(data.uid);
        break;
    }
}

void FormOperation::onMemberDataReady(Member member)
{
    updateUi(member);
}

void FormOperation::processUid(const QString uid)
{
    /**
     * Get data with RESTAPI
     * Wait incoming rest data
    */
    mDbFetcher->process(uid);
}




