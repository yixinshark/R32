//
// Created by zyz on 2023/5/31.
//

#ifndef R32_MCUCONSTANT_H
#define R32_MCUCONSTANT_H

#include <QString>

// 单片机常量
const char MCU_SEND_HEADER = 0x51;
const char MCU_RECV_HEADER = 0x52;

// 电磁阀常量...
const char MCU_CMD_VALVE = 0x47;
const char MCU_CMD_FAN = 0x57;
const char MCU_CMD_ND = 0x67;
const char MCU_CMD_CHANNEL = 0x77;
const char MCU_CMD_BOX = 0x87;
const char MCU_CMD_ALARM_LIGHT = 0x97;

const QString MCU_VALVE = "valve";
// 风扇
const QString MCU_FAN = "fan";
// 浓度
const QString MCU_ND = "nd";
// 通道
const QString MCU_SET_CHANNEL = "channel";
// 箱体开关
const QString MCU_BOX_SWITCH = "box_switch";
// 报警灯
const QString MCU_ALARM_LIGHT = "alarm_light";


#endif //R32_MCUCONSTANT_H
