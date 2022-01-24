/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-01-05     jing       the first version
 */
/*
 * 程序清单：这是一个 I2C 设备使用例程
 * 例程导出了 i2c_aht10_sample 命令到控制终端
 * 命令调用格式：i2c_aht10_sample i2c1
 * 命令解释：命令第二个参数是要使用的I2C总线设备名称，为空则使用默认的I2C总线设备
 * 程序功能：通过 I2C 设备读取温湿度传感器 aht10 的温湿度数据并打印
*/

#include <rtthread.h>
#include <rtdevice.h>

//#define AHT10_I2C_BUS_NAME          "i2c1"  /* 传感器连接的I2C总线设备名称 */
//#define AHT10_ADDR                  0x38    /* 从机地址 */
//#define AHT10_CALIBRATION_CMD       0xE1    /* 校准命令 */
//#define AHT10_NORMAL_CMD            0xA8    /* 一般命令 */
//#define AHT10_GET_DATA              0xAC    /* 获取数据命令 */
#define MS5837_ADDR_WRITE         0xEC
//#define MS5837_ADDR_READ          0xED
//#define MS5837_RESET              0x1E
//#define MS5837_ADC_READ           0x00
//#define MS5837_PROM_READ          0xA0
//#define MS5837_CONVERT_D1_8192    0x4A
//#define MS5837_CONVERT_D2_8192    0x5A
#define MS5837_ADDR             0x76 //I2C address of the sensor
#define MS5837_RESET            0x1E //I2C Command: Reset
#define MS5837_ADC_READ     0x00 //I2C Command: Read
#define MS5837_PROM_READ    0xA0 //I2C Command: Read PROM
//The following are commands for getting D1 and D2 at different oversampling ratios (OCR).
//https://www.mouser.com/datasheet/2/418/5/NG_DS_MS5837-30BA_B1-1130109.pdf
//There are different computational times (delays) required for the different OCR Values
//which can be found in the datasheet. I am using the minimum value, for which dT=0.6ms,
//but the minimum delay in the code that is required is 2ms for some reason.
#define MS5837_CONVERT_D1_256   0x40//I2C Command: Request D1 Conversion @ OCR=256 (min)
#define MS5837_CONVERT_D2_256   0x50//I2C Command: Request D2 Conversion @ OCR=256 (min)
#define MS5837_CONVERT_D1_512   0x42
#define MS5837_CONVERT_D2_512   0x52
#define MS5837_CONVERT_D1_1024  0x44
#define MS5837_CONVERT_D2_1024  0x54
#define MS5837_CONVERT_D1_2048  0x46
#define MS5837_CONVERT_D2_2048  0x56
#define MS5837_CONVERT_D1_4096  0x48
#define MS5837_CONVERT_D2_4096  0x58
#define MS5837_CONVERT_D1_8192  0x4A//I2C Command: Request D1 Conversion @ OCR=8192 (max)
#define MS5837_CONVERT_D2_8192  0x5A//I2C Command: Request D2 Conversion @ OCR=8192 (max)

static struct rt_i2c_bus_device *i2c_bus = RT_NULL;     /* I2C总线设备句柄 */
static rt_bool_t initialized = RT_FALSE;                /* 传感器初始化状态 */



// 写 mpu6050 单个寄存器
//reg: 寄存器地址
//data: 数据
// 返回值: 0, 正常 / -1, 错误代码
rt_err_t write_reg(struct rt_i2c_bus_device *bus,rt_uint8_t reg, rt_uint8_t data)
{
    rt_uint8_t buf[2];

    buf[0] = reg;
    buf[1] = data;

    if (rt_i2c_master_send(bus, MS5837_ADDR_WRITE, 0, buf ,2) == 2)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

/* 读传感器寄存器数据 */
static rt_err_t read_regs(struct rt_i2c_bus_device *bus, rt_uint8_t len, rt_uint8_t *buf)
{
    struct rt_i2c_msg msgs;

    msgs.addr = MS5837_ADDR;
    msgs.flags = RT_I2C_RD;
    msgs.buf = buf;
    msgs.len = len;

    /* 调用I2C设备接口传输数据 */
    if (rt_i2c_transfer(bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

static void read_temp_humi(float *cur_temp, float *cur_humi)
{
    rt_uint8_t temp[6];

    write_reg(i2c_bus, MS5837_ADC_READ, 0);      /* 发送命令 */
    read_regs(i2c_bus, 6, temp);                /* 获取传感器数据 */

    /* 湿度数据转换 */
    *cur_humi = (temp[1] << 12 | temp[2] << 4 | (temp[3] & 0xf0) >> 4) * 100.0 / (1 << 20);
    /* 温度数据转换 */
    *cur_temp = ((temp[3] & 0xf) << 16 | temp[4] << 8 | temp[5]) * 200.0 / (1 << 20) - 50;
}

static void aht10_init()
{
    rt_uint8_t temp[2] = {0, 0};

    /* 查找I2C总线设备，获取I2C总线设备句柄 */
    i2c_bus = (struct rt_i2c_bus_device *)rt_i2c_bus_device_find("i2c1");
    if (i2c_bus == RT_NULL)
    {
        rt_kprintf("can't find iic device!\n");
    }
    else
    {
        int ret=write_reg(i2c_bus, MS5837_ADDR, MS5837_RESET);
        if (ret==RT_EOK)
        {
            rt_kprintf("init iic device success!\n");
        }
        else{
            rt_kprintf("init iic device fail !\n");
        }
        rt_thread_mdelay(400);
    }
}

void i2c_aht10_sample()
{
    float humidity, temperature;
    char name[RT_NAME_MAX];
    if (!initialized)
    {
        /* 传感器初始化 */
        aht10_init();
    }
    if (initialized)
    {
        /* 读取温湿度数据 */
        read_temp_humi(&temperature, &humidity);

        rt_kprintf("read aht10 sensor humidity   : %d.%d %%\n", (int)humidity, (int)(humidity * 10) % 10);
        rt_kprintf("read aht10 sensor temperature: %d.%d \n", (int)temperature, (int)(temperature * 10) % 10);
    }
    else
    {
        rt_kprintf("initialize sensor failed!\n");
    }
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(i2c_aht10_sample, i2c aht10 sample);
