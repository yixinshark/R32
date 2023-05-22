//
// Created by zyz on 23-5-11.
//

#ifndef R32_CONSTANT_H
#define R32_CONSTANT_H

#include <QString>
#include <QMap>

const char header0 = 0x47;
const char header1 = 0x4c;

//// 浓度标定, 0x02
//const char cmd_nd = 0x02;
//
//// 设置产品的ID号(唯一识别码)
//const char cmd_set_id = 0x06;

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

// 设置产品从机地址
const QString SLAVE_ADDRESS = "slave_address";
const QString STATIC_ADDRESS = "static_address";

const unsigned char static_address_value = 0xFF;

// 错误信息
const QString ERROR_MSG = "errorMsg";
const QString ERROR_MSG_UNKNOWN = "未知错误";

// 结果
const QString OPT_RESULT = "result";

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
const unsigned char NTC_CMD = 0x03;
const unsigned char R32_CMD = 0x04;
const unsigned char VER_CMD = 0x05;
const unsigned char SET_ID_CMD = 0x06;
const unsigned char READ_PRODUCT_ID_CMD = 0x07;
const unsigned char SET_SLAVE_ADDR_CMD = 0xFD;
const unsigned char READ_PRODUCT_ADDR_CMD = 0xFE;
const unsigned char READ_ERROR_CMD = 0xFF;

// 自定义，记录数据库
const unsigned char RECORD_DATA = 0x99;

const QString CMD1_OBJECT_NAME = "cmd1";
const QString CMD2_OBJECT_NAME = "cmd2";
const QString CMD3_OBJECT_NAME = "cmd3";
const QString CMD4_OBJECT_NAME = "cmd4";
const QString CMD5_OBJECT_NAME = "cmd5";
const QString CMD6_OBJECT_NAME = "cmd6";
const QString CMD7_OBJECT_NAME = "cmd7";
const QString CMD8_OBJECT_NAME = "cmd8";
const QString CMD_FD_OBJECT_NAME = "cmd_FD";
const QString CMD_FE_OBJECT_NAME = "cmd_FE";

// 按钮objectName与命令号映射
const QMap<QString, unsigned char> ObjectNameCmdMap = {
        {CMD1_OBJECT_NAME, LD_CMD},
        {CMD2_OBJECT_NAME, ND_CMD},
        {CMD3_OBJECT_NAME, NTC_CMD},
        {CMD4_OBJECT_NAME, R32_CMD},
        {CMD5_OBJECT_NAME, VER_CMD},
        {CMD6_OBJECT_NAME, SET_ID_CMD},
        {CMD_FD_OBJECT_NAME, SET_SLAVE_ADDR_CMD},
        {CMD_FE_OBJECT_NAME, READ_PRODUCT_ADDR_CMD},
        {CMD7_OBJECT_NAME, READ_PRODUCT_ID_CMD}
};

#endif //R32_CONSTANT_H
