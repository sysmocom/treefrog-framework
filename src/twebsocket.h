#ifndef TWEBSOCKET_H
#define TWEBSOCKET_H

#include <QTcpSocket>
#include <QList>
#include <QByteArray>
#include <TGlobal>
#include <THttpRequestHeader>
#include <atomic>
#include "tabstractwebsocket.h"

class TWebSocketFrame;
class TWebSocketWorker;
class TSession;


class T_CORE_EXPORT TWebSocket : public QTcpSocket, public TAbstractWebSocket
{
    Q_OBJECT
public:
    TWebSocket(int socketDescriptor, const QHostAddress &address, const THttpRequestHeader &header, QObject *parent = 0);
    virtual ~TWebSocket();

    const QByteArray &socketUuid() const { return uuid; }
    bool canReadRequest() const;
    void disconnect() Q_DECL_OVERRIDE;

public slots:
    void readRequest();
    void deleteLater();
    void releaseWorker();
    void sendRawData(const QByteArray &data);
    void close();

protected:
    void startWorkerForOpening(const TSession &session);
    void startWorkerForClosing();
    virtual qint64 writeRawData(const QByteArray &data) Q_DECL_OVERRIDE;
    virtual QList<TWebSocketFrame> &websocketFrames() Q_DECL_OVERRIDE { return frames; }
    QList<TWebSocketFrame> frames;

signals:
    void sendByWorker(const QByteArray &data);
    void disconnectByWorker();

private:
    void startWorker(TWebSocketWorker *worker);

    QByteArray uuid;
    THttpRequestHeader reqHeader;
    QByteArray recvBuffer;
    std::atomic<int> myWorkerCounter;
    std::atomic<bool> deleting;

    friend class TActionThread;
    Q_DISABLE_COPY(TWebSocket)
};

#endif // TWEBSOCKET_H
