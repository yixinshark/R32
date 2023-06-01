//
// Created by zyz on 23-6-1.
//

#ifndef R32_HANDLEDATABASE_H
#define R32_HANDLEDATABASE_H

#include <QObject>

class HandleDataBase : public QObject
{
    Q_OBJECT

public:
    explicit HandleDataBase(QObject *parent = nullptr);
    ~HandleDataBase() override;

signals:
    void frameReceived(char cmd, const QVariantMap &info);
    void recvedFrameData(const QByteArray &frameData);

public:
    virtual QByteArray getSendData(char cmd, const QVariantMap &info);
    virtual void processReceivedData(const QByteArray &data) = 0;

protected:
    // 填充发送内容
    virtual void addContent(char cmd, const QVariantMap &info, QByteArray &data) = 0;
    virtual void addCheckSum(QByteArray &data);

    // 帧数据是否有效
    virtual bool frameIsValid(const QByteArray &frameData) = 0;

protected:
    QByteArray m_receivedData;
};

#endif //R32_HANDLEDATABASE_H
