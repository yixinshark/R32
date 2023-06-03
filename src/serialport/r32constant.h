//
// Created by yixin on 2023/5/28.
//

#ifndef R32_R32CONSTANT_H
#define R32_R32CONSTANT_H

#include <QString>

const quint8 SEND_FRAME_LENGTH = 8;
const quint8 RECV_FRAME_LENGTH = 7;

const quint8 SEND_HEADER = 0x65;
const quint8 RECV_HEADER = 0x66;

// 标定浓度命令码 or 设置模块地址
const quint8 CMD_01 = 0x01;
// 标定浓度完成命令码 or 读取模块地址
const quint8 CMD_02 = 0x02;
// 查询标定状态命令码
const quint8 CMD_ND_STATUS_03 = 0x03;
// 读取标定的零点阻值R0
const quint8 CMD_READ_R0_04 = 0x04;
// 读取标定参数1
const quint8 CMD_READ_PARAM1_05 = 0x05;
// 读取标定参数2
const quint8 CMD_READ_PARAM2_06 = 0x06;
// 读取1000ppm的Rs/R0
const quint8 CMD_READ_1000PPM_07 = 0x07;
// 读取5000ppm的Rs/R0
const quint8 CMD_READ_5000PPM_08 = 0x08;

// 检测温度和湿度 0x21
const quint8 CMD_READ_TEMP_HUM_21 = 0x21;
// 读取ADC值 0x22
const quint8 CMD_READ_ADC_22 = 0x22;
// 读取气体探头的阻值 0x23
const quint8 CMD_READ_RESISTANCE_23 = 0x23;
// 读取温度补偿后的气体探头阻值 0x24
const quint8 CMD_READ_COMPENSATION_RESISTANCE_24 = 0x24;
// 读取气体浓度ppm和报警状态 0x25
const quint8 CMD_READ_GAS_CONCENTRATION_25 = 0x25;

// 读取固件版本号 0x41
const quint8 CMD_READ_FIRMWARE_VERSION_41 = 0x41;
// 打开或者关闭周期性打印数据 0x42
const quint8 CMD_OPEN_CLOSE_PRINT_42 = 0x42;
// 查询产品的故障状态 0x43
const quint8 CMD_QUERY_FAULT_STATUS_43 = 0x43;
// 设置报警阈值
const quint8 CMD_SET_ALARM_THRESHOLD_44 = 0x44;
// 读取报警阈值
const quint8 CMD_READ_ALARM_THRESHOLD_45 = 0x45;

// 常量定义
// -------------------------send--------------------------------
// 标定的点
const QString SEND_CAL_POINT = "cal_point";
// 标定浓度
const QString SEND_CAL_CONCENTRATION = "cal_concentration";
// 使能位
const QString SEND_PRINT_ENABLE = "print_enable";
// 报警阈值
const QString SEND_ALARM_THRESHOLD = "alarm_threshold";

// --------------------------ack--------------------------------
const QString ACK_RESULT = "result";
const QString ACK_ERROR = "error";
const QString ACK_FLOAT_VALUE = "float_value";

// 标定的零点阻值R0
const QString ACK_R0_VALUE = "R0_value";
// 标定参数1
const QString ACK_PARAM1_VALUE = "param1_value";
// 标定参数2
const QString ACK_PARAM2_VALUE = "param2_value";
// 1000ppm的Rs/R0
const QString ACK_1000PPM_VALUE = "1000ppm_value";
// 5000ppm的Rs/R0
const QString ACK_5000PPM_VALUE = "5000ppm_value";
// 温度
const QString ACK_TEMPERATURE = "temperature";
// 湿度
const QString ACK_HUMIDITY = "humidity";


// ADC值
const QString ACK_ADC_VALUE = "adc_value";
// 气体探头的阻值
const QString ACK_RESISTANCE_VALUE = "resistance_value";
// 4.读取温度补偿后的气体探头阻值
const QString ACK_COMPENSATION_RESISTANCE_VALUE = "compensation_resistance_value";
// 气体浓度ppm
const QString ACK_GAS_CONCENTRATION = "gas_concentration";
// 报警状态值
const QString ACK_ALARM_STATUS = "alarm_status";
// 报警阈值
const QString ACK_ALARM_THRESHOLD = "alarm_threshold";

// 固件版本号
const QString ACK_FIRMWARE_VERSION = "firmware_version";
// 副版本号
const QString ACK_FIRMWARE_SUB_VERSION = "firmware_sub_version";

// 打开或者关闭周期性打印数据 0x42
const QString ACK_PRINT_STATUS = "print_status";

// 是模块地址
const QString MODULE_ADDRESS = "module_address";
// 设置模块地址
const QString SET_MODULE_ADDRESS = "set_module_address";
// 读取模块地址
const QString READ_MODULE_ADDRESS = "read_module_address";

#endif //R32_R32CONSTANT_H
