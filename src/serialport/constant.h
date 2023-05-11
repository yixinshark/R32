//
// Created by zyz on 23-5-11.
//

#ifndef R32_CONSTANT_H
#define R32_CONSTANT_H

#include <QString>
#include <QMap>

const char header0 = 0x47;
const char header1 = 0x4c;

// 零点标定, 0x01
const unsigned char cmd_ld[] = {header0, header1, 0x07, 0x00, 0x01, 0x00, 0x00};

// 浓度标定, 0x02
const unsigned char cmd_nd[] = {header0, header1, 0x0c, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 读取NTC的ADC值和温度, 0x03
const unsigned char cmd_ntc[] = {header0, header1, 0x07, 0x00, 0x03, 0x00, 0x00};

// 读取R32的ADC值和浓度
const unsigned char cmd_r32[] = {header0, header1, 0x07, 0x00, 0x04, 0x00, 0x00};

// 读取软件版本号
const unsigned char cmd_ver[] = {header0, header1, 0x07, 0x00, 0x05, 0x00, 0x00};

// 设置产品的ID号(唯一识别码)
const unsigned char cmd_set_id[] = {header0, header1, 0x0c, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 读取产品的ID号(唯一识别码)
const unsigned char cmd_read_id[] = {header0, header1, 0x07, 0x00, 0x07, 0x00, 0x00};

// 设置产品从机地址(广播)
const unsigned char cmd_set_addr[] = {header0, header1, 0x08, 0xFF, 0xFD, 0x00, 0x00, 0x00};

// 读取产品从机地址(广播)
const unsigned char cmd_read_addr[] = {header0, header1, 0x07, 0xFF, 0xFE, 0x00, 0x00};

// 回复错误消息
const unsigned char errorAck = 0xFF;

// 0x00:无错误
// 0x01:校验和错误
// 0x02:命令码错误
// 0x03:数据超范围
const QMap<char, QString> errorMsg = {
        {0x01, "校验和错误"},
        {0x02, "命令码错误"},
        {0x03, "数据超范围"}
};

#endif //R32_CONSTANT_H
