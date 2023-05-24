//
// Created by zyz on 2023/5/20.
//

#ifndef R32_R32RECORDVALUE_H
#define R32_R32RECORDVALUE_H

#include <QString>
#include <QVariant>

//serial_number, dateTime, sensor_id, software_version, adc_value, concentration

class R32RecordValue
{
public:
    QString serial_number;
    QString dateTime;
    QString sensor_id;
    QString software_version;
    int adc_value = -1;
    // 定义为float，存数据库会出错
    QString concentration;

    QString toString() {
        return QString("serial_number:%1, dateTime:%2, sensor_id:%3, "
                       "software_version:%4, adc_value:%5, concentration:%6")
                .arg(serial_number).arg(dateTime).arg(sensor_id)
                .arg(software_version).arg(QString::number(adc_value)).arg(concentration);
    }
};

class R32RecordValueDao
{
public:
    static bool createTable();
    static int datasCountByDateTime(quint32, quint32);
    static bool insertOrUpdate(const R32RecordValue &data);
    static int insert(const R32RecordValue &data);
    static bool update(const R32RecordValue &data);
    static QList<R32RecordValue> queryByDateTime(const QString &beginTime, const QString &endTime);
    static int deleteByDateTime(quint32);

    // sensor_id是否存在
    static bool isExist(const QString &sensor_id);

private:
    static R32RecordValue mapToConsistencylog(const QMap<QString, QVariant> &rowMap);
    static QMap<QString, QVariant> parmsValue(const R32RecordValue &data);
};

#endif //R32_R32RECORDVALUE_H
