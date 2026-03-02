#ifndef BQ76920_REG_H
#define BQ76920_REG_H

#include "hal_data.h"

#define BQ76920_I2C_ADDR            (0x08) 

/* 寄存器地址定义 */
#define BQ76920_REG_SYS_STAT        0x00    // 系统状态
#define BQ76920_REG_CELLBAL1        0x01    // 均衡控制
#define BQ76920_REG_SYS_CTRL1       0x04    // 系统控制1
#define BQ76920_REG_SYS_CTRL2       0x05    // 系统控制2
#define BQ76920_REG_PROTECT1        0x06    // 保护1
#define BQ76920_REG_PROTECT2        0x07    // 保护2
#define BQ76920_REG_PROTECT3        0x08    // 保护3
#define BQ76920_REG_OV_TRIP         0x09    // 过压阈值
#define BQ76920_REG_UV_TRIP         0x0A    // 欠压阈值
#define BQ76920_REG_CC_CFG          0x0B    // 库仑计配置

/* 电压寄存器 */
#define BQ76920_REG_VC1_HI          0x0C
#define BQ76920_REG_VC1_LO          0x0D
#define BQ76920_REG_VC2_HI          0x0E
#define BQ76920_REG_VC2_LO          0x0F
#define BQ76920_REG_VC3_HI          0x10
#define BQ76920_REG_VC3_LO          0x11
#define BQ76920_REG_VC4_HI          0x12
#define BQ76920_REG_VC4_LO          0x13
#define BQ76920_REG_VC5_HI          0x14
#define BQ76920_REG_VC5_LO          0x15

/* 电池组电压寄存器 */
#define BQ76920_REG_BAT_HI          0x2A
#define BQ76920_REG_BAT_LO          0x2B

/* 电流寄存器 */
#define BQ76920_REG_CC_HI           0x32
#define BQ76920_REG_CC_LO           0x33

/* ADC增益和偏移 */
#define BQ76920_REG_ADCGAIN1        0x50
#define BQ76920_REG_ADCOFFSET       0x51
#define BQ76920_REG_ADCGAIN2        0x59

/* SYS_STAT 位定义 */
#define BQ76920_STAT_OCD            (1 << 0)  // 放电过流
#define BQ76920_STAT_SCD            (1 << 1)  // 短路
#define BQ76920_STAT_OV             (1 << 2)  // 过压
#define BQ76920_STAT_UV             (1 << 3)  // 欠压
#define BQ76920_STAT_OVRD_ALERT     (1 << 4)  // 过温警告
#define BQ76920_STAT_DEVICE_XREADY  (1 << 5)  // 设备未就绪
#define BQ76920_STAT_CC_READY       (1 << 7)  // 库仑计就绪

/* SYS_CTRL2 位定义 */
#define BQ76920_CTRL2_CHG_ON        (1 << 0)  // 充电FET
#define BQ76920_CTRL2_DSG_ON        (1 << 1)  // 放电FET
#define BQ76920_CTRL2_CC_EN         (1 << 6)  // 使能库仑计

/* 配置参数 */
#define BQ76920_MAX_CELLS           5         // BQ76920最多5节
#define BQ76920_SHUNT_RESISTOR_mOHM 1.0f      // 采样电阻 (mΩ)
#define BQ76920_ADC_GAIN            365.0f    // ADC增益 (μV)
#define BQ76920_ADC_OFFSET          0.0f      // ADC偏移 (mV)

/* BMS状态结构体 */
typedef struct {
    // I2C句柄
    void *i2c_handle;  // 指向 i2c_master_instance_t
    
    // 电池参数
    uint8_t cell_count;
    float cell_voltage[BQ76920_MAX_CELLS];  // 单体电压 (V)
    float pack_voltage;                      // 总电压 (V)
    float current;                           // 电流 (A), 正为充电
    float temperature;                       // 温度 (°C)
    
    // 保护状态
    struct {
        bool overvoltage;       // 过压
        bool undervoltage;      // 欠压
        bool overcurrent_charge;    // 充电过流
        bool overcurrent_discharge; // 放电过流
        bool short_circuit;         // 短路
        bool overtemperature;       // 过温
    } protection;
    
    // FET状态
    bool charge_fet_enabled;
    bool discharge_fet_enabled;
    
    // 均衡状态
    uint8_t balance_status;  // 位掩码，每位对应一个电池
    
    // SOC/SOH
    float soc;  // 电量百分比 (0-100%)
    float soh;  // 健康度 (0-100%)
    
    // 校准参数
    float adc_gain;
    float adc_offset;
    
} bq76920_t;

#endif