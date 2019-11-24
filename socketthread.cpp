#include "socketthread.h"

SocketThread::SocketThread(qintptr descriptor, QObject *parent)
    :  QThread(parent), m_descriptor(descriptor)
{
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    mDb = new Database();
}

SocketThread::~SocketThread()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
        m_socket->waitForDisconnected();
    }

    if (mDb != nullptr) {
        mDb = nullptr;
    }
}

void SocketThread::run()
{
    // qDebug() << Q_FUNC_INFO;
    m_socket = new QTcpSocket;
    m_socket->setSocketDescriptor(m_descriptor);
    connect(m_socket, SIGNAL(readyRead()),    this, SLOT(onReadyRead()), Qt::DirectConnection);
    // connect(m_socket, SIGNAL(connected()),    this, SLOT(onConnected()), Qt::DirectConnection);
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()), Qt::DirectConnection);
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
    exec();
}

void SocketThread::onConnected()
{
    qDebug() << "This is okay ====> " << __FUNCTION__;
}

void SocketThread::onReadyRead()
{
    QDataStream in(m_socket);
    // in.setVersion(QDataStream::Qt_5_5);

    QByteArray ba =  m_socket->readAll();
    QString uid = QTextCodec::codecForMib(106)->toUnicode(ba);
    uid = uid.trimmed();
    // qDebug() << DataAsString;

    // TDK JALAN
    // QString message;
    // in >> message;
    // qDebug() << message;

    // m_socket->disconnectFromHost();


    // Get person data
    mDb->getPersonData(uid);

    // Insert into sqlite
    RFIData data;
    data.type = ResultTypes::NewData;
    data.uid = uid;
    mDb->process(data);
    emit newData(data);
    // emitNewData(ResultTypes::NewData, uid, "");
}

void SocketThread::onDisconnected()
{
    RFIData data;
    data.type = ResultTypes::Disconnected;
    data.string = "Disconnected";
    //qDebug() << "Disconnected!";
    m_socket->close();
    // Exit event loop
    quit();
}

void SocketThread::onError(QAbstractSocket::SocketError error)
{
    QString string;
    switch (error) {
    case QAbstractSocket::ConnectionRefusedError            :       string = "ConnectionRefusedError"           ; break;
    case QAbstractSocket::RemoteHostClosedError             :       string = "RemoteHostClosedError"            ; break;
    case QAbstractSocket::HostNotFoundError                 :       string = "HostNotFoundError"                ; break;
    case QAbstractSocket::SocketAccessError                 :       string = "SocketAccessError"                ; break;
    case QAbstractSocket::SocketResourceError               :       string = "SocketResourceError"              ; break;
    case QAbstractSocket::SocketTimeoutError                :       string = "SocketTimeoutError"               ; break;
    case QAbstractSocket::DatagramTooLargeError             :       string = "DatagramTooLargeError"            ; break;
    case QAbstractSocket::NetworkError                      :       string = "NetworkError"                     ; break;
    case QAbstractSocket::AddressInUseError                 :       string = "AddressInUseError"                ; break;
    case QAbstractSocket::SocketAddressNotAvailableError    :       string = "SocketAddressNotAvailableError"   ; break;
    case QAbstractSocket::UnsupportedSocketOperationError   :       string = "UnsupportedSocketOperationError"  ; break;
    case QAbstractSocket::UnfinishedSocketOperationError    :       string = "UnfinishedSocketOperationError"   ; break;
    case QAbstractSocket::ProxyAuthenticationRequiredError  :       string = "ProxyAuthenticationRequiredError" ; break;
    case QAbstractSocket::SslHandshakeFailedError           :       string = "SslHandshakeFailedError"          ; break;
    case QAbstractSocket::ProxyConnectionRefusedError       :       string = "ProxyConnectionRefusedError"      ; break;
    case QAbstractSocket::ProxyConnectionClosedError        :       string = "ProxyConnectionClosedError"       ; break;
    case QAbstractSocket::ProxyConnectionTimeoutError       :       string = "ProxyConnectionTimeoutError"      ; break;
    case QAbstractSocket::ProxyNotFoundError                :       string = "ProxyNotFoundError"               ; break;
    case QAbstractSocket::ProxyProtocolError                :       string = "ProxyProtocolError"               ; break;
    case QAbstractSocket::OperationError                    :       string = "OperationError"                   ; break;
    case QAbstractSocket::SslInternalError                  :       string = "SslInternalError"                 ; break;
    case QAbstractSocket::SslInvalidUserDataError           :       string = "SslInvalidUserDataError"          ; break;
    case QAbstractSocket::TemporaryError                    :       string = "TemporaryError"                   ; break;
    case QAbstractSocket::UnknownSocketError                :       string = "UnknownSocketError"               ; break;
    }

    RFIData data;
    data.type   = ResultTypes::Error;
    data.string = string;
    emit newData(data);

    //qDebug() << "ERROR";'
    m_socket->reset();
}

//void SocketThread::emitNewData(ResultTypes type, QString uid, QString string)
//{
//    RFIData data;
//    data.type = type;
//    data.uid = uid;
//    data.string = string;
//    emit newData(data);
//}

