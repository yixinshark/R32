//
// Created by zyz on 23-5-11.
//

#ifndef R32_HANDLEDATA_H
#define R32_HANDLEDATA_H

#include <QObject>

class HandleData : public QObject
{
    Q_OBJECT
public:
    explicit HandleData(QObject *parent = nullptr);
    ~HandleData() override;

    void sendData(int cmd, const QVariantMap &info);

// handle send data
private:
    void addContent(int cmd, const QVariantMap &info, QByteArray &data);
    void addCheckSum(QByteArray &data);

    // 添加浓度标定命令的数据
    bool addCmd_nd(const QVariantMap &info, QByteArray &data);
    // 添加设置产品ID号的命令数据
    bool addCmd_set_id(const QVariantMap &info, QByteArray &data);

// handle recv data
private:
    void processReceivedData(const QByteArray &data);
    bool frameIsValid(const QByteArray &frameData);

private:
    unsigned char m_address = 0x00;
    QByteArray m_receivedData;
};


#endif //R32_HANDLEDATA_H
