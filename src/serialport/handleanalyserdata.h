//
// Created by yixin on 2023/6/12.
//

#ifndef R32_HANDLEANALYSERDATA_H
#define R32_HANDLEANALYSERDATA_H

#include "handledatabase.h"

/*!
 * @brief R32分析仪器数据
 */
class HandleAnalyserData : public HandleDataBase
{
    Q_OBJECT
public:
    explicit HandleAnalyserData(QObject *parent = nullptr);
    ~HandleAnalyserData() override;

public:
    void setAddress(char address);
    void processReceivedData(const QByteArray &data) override;

protected:
    void addContent(char cmd, const QVariantMap &info, QByteArray &data) override;
    bool frameIsValid(const QByteArray &frameData) override;

private:
    void handleContent(const QByteArray &content);

private:
    char m_address = 0x00;
};

#endif //R32_HANDLEANALYSERDATA_H
