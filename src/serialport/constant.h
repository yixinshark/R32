//
// Created by zyz on 23-5-11.
//

#ifndef R32_CONSTANT_H
#define R32_CONSTANT_H

#include <QString>
#include <QMap>

const char header0 = 0x47;
const char header1 = 0x4c;

// 浓度标定, 0x02
const char cmd_nd = 0x02;

// 设置产品的ID号(唯一识别码)
const char cmd_set_id = 0x06;

// 浓度
const QString CONCENTRATION = "concentration";
// 温度
const QString TEMPERATURE = "temperature";

// 产品类型, 产品ID
const QString PRODUCT_TYPE = "product_type";
const QString PRODUCT_ID = "product_id";

// ADC值
const QString ADC_VALUE = "adc_value";
const QString NTC_STATUS_2 = "NTC探头开路";
const QString NTC_STATUS_3 = "NTC探头短路";

// 软件版本号
const QString SOFTWARE_VERSION = "software_version";

// 错误信息
const QString ERROR_MSG = "errorMsg";
const QString ERROR_MSG_UNKNOWN = "未知错误";

// 0x00:无错误
// 0x01:校验和错误
// 0x02:命令码错误
// 0x03:数据超范围
const QMap<char, QString> errorMsg = {
        {0x01, "校验和错误"},
        {0x02, "命令码错误"},
        {0x03, "数据超范围"}
};

// 命令号
const unsigned char LD_CMD = 0x01;
const unsigned char ND_CMD = 0x02;
const unsigned char NCT_CMD = 0x03;
const unsigned char R32_CMD = 0x04;
const unsigned char VER_CMD = 0x05;
const unsigned char SET_ID_CMD = 0x06;
const unsigned char READ_PRODUCT_CMD = 0x07;
const unsigned char SET_PRODUCT_ADDR_CMD = 0xFD;
const unsigned char READ_PRODUCT_ADDR_CMD = 0xFE;
const unsigned char READ_ERROR_CMD = 0xFF;




// 零点标定, 0x01
const unsigned char cmd_ld[] = {header0, header1, 0x07, 0x00, 0x01, 0x00, 0x00};

// 浓度标定, 0x02
//const unsigned char cmd_nd[] = {header0, header1, 0x0c, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 读取NTC的ADC值和温度, 0x03
const unsigned char cmd_ntc[] = {header0, header1, 0x07, 0x00, 0x03, 0x00, 0x00};

// 读取R32的ADC值和浓度
const unsigned char cmd_r32[] = {header0, header1, 0x07, 0x00, 0x04, 0x00, 0x00};

// 读取软件版本号
const unsigned char cmd_ver[] = {header0, header1, 0x07, 0x00, 0x05, 0x00, 0x00};

// 设置产品的ID号(唯一识别码)
//const unsigned char cmd_set_id[] = {header0, header1, 0x0c, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 读取产品的ID号(唯一识别码)
const unsigned char cmd_read_id[] = {header0, header1, 0x07, 0x00, 0x07, 0x00, 0x00};

// 设置产品从机地址(广播)
const unsigned char cmd_set_addr[] = {header0, header1, 0x08, 0xFF, 0xFD, 0x00, 0x00, 0x00};

// 读取产品从机地址(广播)
const unsigned char cmd_read_addr[] = {header0, header1, 0x07, 0xFF, 0xFE, 0x00, 0x00};

// 回复错误消息
const unsigned char errorAck = 0xFF;



#endif //R32_CONSTANT_H
