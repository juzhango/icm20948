/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-05-29     juzhango     the first version
 */
/* Includes -----------------------------------------------------------------*/
#include "icm20948_reg.h"
#include <sensor.h>
#include <stdint.h>
#define DBG_ENABLE
#define DBG_LEVEL DBG_INFO
#define DBG_SECTION_NAME "sensor.icm20948"
#define DBG_COLOR
#include <rtdbg.h>
/* Private typedef ----------------------------------------------------------*/
static rt_size_t icm20948_fetch_data(struct rt_sensor_device *sensor, void *buf, rt_size_t len);
static rt_err_t icm20948_control(struct rt_sensor_device *sensor, int cmd, void *args);

/* Private define -----------------------------------------------------------*/
#ifdef PKG_ICM20948_I2C_ADDR_TYPE_LOW
#define ICM20948_DEV_ADDR ICM20948_I2C_ADD_L
#else
#define ICM20948_DEV_ADDR ICM20948_I2C_ADD_H
#endif /* PKG_ICM20948_I2C_ADDR_TYPE_LOW */

#define ICM_LOG_D(fmt, ...) LOG_D("%s %d: " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define ICM_LOG_I(fmt, ...) LOG_I("%s %d: " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define ICM_LOG_W(fmt, ...) LOG_W("%s %d: " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define ICM_LOG_E(fmt, ...) LOG_E("%s %d: " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#ifdef LOG_HEX
#define ICM_DUMP(tag, mem, len) LOG_HEX(tag, 16, (rt_uint8_t *)(mem), len)
#else
#define ICM_DUMP(tag, mem, len)
#endif

/* ±2g ~ ±16g */
#define ICM20948_ACCE_RANG_MIN 2000
#define ICM20948_ACCE_RANG_MAX 16000

/* ±250 ~ ±2000 */
#define ICM20948_GYRO_RANG_MIN 250
#define ICM20948_GYRO_RANG_MAX 2000

/* Private variables --------------------------------------------------------*/
static icm20948_dev_ctx_t dev_ctx;
static struct rt_sensor_ops sensor_ops =
    {
        icm20948_fetch_data,
        icm20948_control};

/* Extern variables ---------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/

static int write_reg(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
    rt_uint8_t tmp = reg;
    struct rt_i2c_msg msgs[2];

    msgs[0].addr = ICM20948_DEV_ADDR; /* Slave address */
    msgs[0].flags = RT_I2C_WR;        /* Write flag */
    msgs[0].buf = &tmp;               /* Slave register address */
    msgs[0].len = 1;                  /* Number of bytes sent */

    msgs[1].addr = ICM20948_DEV_ADDR;            /* Slave address */
    msgs[1].flags = RT_I2C_WR | RT_I2C_NO_START; /* Read flag */
    msgs[1].buf = bufp;                          /* Read data pointer */
    msgs[1].len = len;                           /* Number of bytes read */

    if (rt_i2c_transfer((struct rt_i2c_bus_device *)handle, msgs, 2) != 2)
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}

static int read_reg(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
    rt_uint8_t tmp = reg;
    struct rt_i2c_msg msgs[2];

    msgs[0].addr = ICM20948_DEV_ADDR; /* Slave address */
    msgs[0].flags = RT_I2C_WR;        /* Write flag */
    msgs[0].buf = &tmp;               /* Slave register address */
    msgs[0].len = 1;                  /* Number of bytes sent */

    msgs[1].addr = ICM20948_DEV_ADDR; /* Slave address */
    msgs[1].flags = RT_I2C_RD;        /* Read flag */
    msgs[1].buf = bufp;               /* Read data pointer */
    msgs[1].len = len;                /* Number of bytes read */

    if (rt_i2c_transfer((struct rt_i2c_bus_device *)handle, msgs, 2) != 2)
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}

static rt_size_t icm20948_polling_fetch_data(rt_sensor_t sensor, struct rt_sensor_data *data)
{
    int16_t raw_data[3] = {0};
    rt_memset(data, 0, sizeof(struct rt_sensor_data));
    data->timestamp = rt_sensor_get_ts();
    if (sensor->info.type == RT_SENSOR_CLASS_ACCE)
    {
        data->type = RT_SENSOR_CLASS_ACCE;
        icm20948_accel_fs_sel_t full_scale;
        if (icm20948_accel_full_scale_get(&dev_ctx, &full_scale))
        {
            ICM_LOG_E("Read full scale failed!");
            return -RT_ERROR;
        }
        if (icm20948_acceleration_raw_get(&dev_ctx, raw_data))
        {
            ICM_LOG_E("Read accelerometer data failed! ");
            return -RT_ERROR;
        }
        ICM_DUMP("raw_data", raw_data, 6);

        data->data.acce.x = icm20948_accel_lsb_to_mg(raw_data[0], full_scale);
        data->data.acce.y = icm20948_accel_lsb_to_mg(raw_data[1], full_scale);
        data->data.acce.z = icm20948_accel_lsb_to_mg(raw_data[2], full_scale);

        data->timestamp = rt_sensor_get_ts();
        return 1;
    }
    if (sensor->info.type == RT_SENSOR_CLASS_GYRO)
    {
        data->type = RT_SENSOR_CLASS_GYRO;
        icm20948_gyro_fs_sel_t full_scale;
        if (icm20948_gyro_full_scale_get(&dev_ctx, &full_scale))
        {
            ICM_LOG_E("Read full scale failed!");
            return -RT_ERROR;
        }
        if (icm20948_angular_rate_raw_get(&dev_ctx, raw_data))
        {
            ICM_LOG_E("Read gyroscop data failed!");
            return -RT_ERROR;
        }
        ICM_DUMP("raw_data", raw_data, 6);

        data->data.gyro.x = icm20948_gyro_lsb_to_mdps(raw_data[0], full_scale);
        data->data.gyro.y = icm20948_gyro_lsb_to_mdps(raw_data[1], full_scale);
        data->data.gyro.z = icm20948_gyro_lsb_to_mdps(raw_data[2], full_scale);

        data->timestamp = rt_sensor_get_ts();
        return 1;
    }
    else
    {
        ICM_LOG_E("Unsupported sensor type: %d", sensor->info.type);
    }
    return 0;
}

static rt_size_t icm20948_fetch_data(struct rt_sensor_device *sensor, void *buf, rt_size_t len)
{
    RT_ASSERT(buf);
    if (sensor->config.mode == RT_SENSOR_MODE_POLLING)
    {
        return icm20948_polling_fetch_data(sensor, buf);
    }
    else
        return 0;
}

static rt_err_t icm20948_control(struct rt_sensor_device *sensor, int cmd, void *args)
{
    rt_err_t result = RT_EOK;

    switch (cmd)
    {
    case RT_SENSOR_CTRL_GET_ID:
        result = icm20948_device_id_get(&dev_ctx, args);
        break;
    default:
        return -RT_ERROR;
    }
    return result;
}

/* Public function prototypes -----------------------------------------------*/

static rt_err_t icm20948_init(struct rt_sensor_intf *intf)
{
    rt_uint8_t id;
    rt_uint32_t ts = 0;
    uint8_t rst = 1;
    struct rt_i2c_bus_device *i2c_bus_dev = (struct rt_i2c_bus_device *)rt_device_find(intf->dev_name);
    if (i2c_bus_dev == RT_NULL)
    {
        ICM_LOG_E("%s null", intf->dev_name);
        return -RT_ERROR;
    }

    dev_ctx.handle = i2c_bus_dev;
    dev_ctx.write_reg = write_reg;
    dev_ctx.read_reg = read_reg;

    if (icm20948_device_id_get(&dev_ctx, &id) == RT_EOK)
    {
        if (id != 0XEA)
        {
            ICM_LOG_E("This device(id=0x%X) is not LSM6DSM", id);
        }
    }
    ICM_LOG_I("ICM20948 ID: 0x%X", id);

    /* Restore default configuration */
    if (icm20948_reset_set(&dev_ctx, PROPERTY_ENABLE) != RT_EOK)
    {
        ICM_LOG_E("Reset ICM20948 failed");
        return -RT_ERROR;
    }
    ts = rt_tick_get_millisecond();
    do
    {
        icm20948_reset_get(&dev_ctx, &rst);
        if ((rt_tick_get_millisecond() - ts) > 1000)
        {
            ICM_LOG_W("Reset ICM20948 failed!");
            break;
        }
    } while (rst);
    icm20948_user_bank_set(&dev_ctx, ICM20948_SEL_USER_BANK_0);
    icm20948_sleep_set(&dev_ctx, ICM20948_RUN_MODE);
    rt_thread_mdelay(10);

    icm20948_user_bank_set(&dev_ctx, ICM20948_SEL_USER_BANK_2);
    icm20948_odr_align_set(&dev_ctx, 1);

    icm20948_gyro_smplrt_div_set(&dev_ctx, 0x7);
    icm20948_gyro_dlpfcfg_set(&dev_ctx, ICM20948_GYRO_DLPFCFG_6);
    icm20948_gyro_full_scale_set(&dev_ctx, ICM20948_GYRO_FS_SEL_500dps);
    icm20948_gyro_fchoice_set(&dev_ctx, ICM20948_ENABLE_GYRO_DLPF);

    icm20948_accel_smplrt_div_2_set(&dev_ctx, 0x7);
    icm20948_accel_dlpfcfg_set(&dev_ctx, ICM20948_ACCEL_DLPFCFG_6);
    icm20948_accel_full_scale_set(&dev_ctx, ICM20948_ACCEL_FS_SEL_2g);
    icm20948_accel_fchoice_set(&dev_ctx, ICM20948_ENABLE_ACCEL_DLPF);

    icm20948_user_bank_set(&dev_ctx, ICM20948_SEL_USER_BANK_0);
    rt_thread_mdelay(100);

    return RT_EOK;
}

int rt_hw_icm20948_init(const char *name, struct rt_sensor_config *cfg)
{
    int ret = RT_EOK;
    RT_ASSERT(cfg);

    static struct rt_sensor_device sensor_acce;
    sensor_acce.info.type = RT_SENSOR_CLASS_ACCE;
    sensor_acce.info.vendor = RT_SENSOR_VENDOR_INVENSENSE;
    sensor_acce.info.model = "icm20948_acc";
    sensor_acce.info.unit = RT_SENSOR_UNIT_MG;
    sensor_acce.info.intf_type = RT_SENSOR_INTF_I2C;
    sensor_acce.info.range_max = ICM20948_ACCE_RANG_MAX;
    sensor_acce.info.range_min = ICM20948_ACCE_RANG_MIN;
    sensor_acce.info.period_min = 10;
    sensor_acce.info.fifo_max = 0;
    if (cfg->range < ICM20948_ACCE_RANG_MIN)
    {
        cfg->range = ICM20948_ACCE_RANG_MIN;
    }
    rt_memcpy(&sensor_acce.config, cfg, sizeof(struct rt_sensor_config));
    sensor_acce.ops = &sensor_ops;

    ret = rt_hw_sensor_register(&sensor_acce, name, RT_DEVICE_FLAG_RDWR, RT_NULL);
    if (ret != RT_EOK)
    {
        ICM_LOG_E("Register ICM20948 accelerometer sensor failed,err: %d", ret);
    }

    static struct rt_sensor_device sensor_gyro;
    sensor_gyro.info.type = RT_SENSOR_CLASS_GYRO;
    sensor_gyro.info.vendor = RT_SENSOR_VENDOR_INVENSENSE;
    sensor_gyro.info.model = "icm20948_gyro";
    sensor_gyro.info.unit = RT_SENSOR_UNIT_MDPS;
    sensor_gyro.info.intf_type = RT_SENSOR_INTF_I2C;
    sensor_gyro.info.range_max = ICM20948_GYRO_RANG_MAX;
    sensor_gyro.info.range_min = ICM20948_GYRO_RANG_MIN;
    sensor_gyro.info.period_min = 10;
    sensor_gyro.info.fifo_max = 0;
    if (cfg->range < ICM20948_GYRO_RANG_MIN)
    {
        cfg->range = ICM20948_GYRO_RANG_MIN;
    }
    rt_memcpy(&sensor_gyro.config, cfg, sizeof(struct rt_sensor_config));
    sensor_gyro.ops = &sensor_ops;

    ret = rt_hw_sensor_register(&sensor_gyro, name, RT_DEVICE_FLAG_RDWR, RT_NULL);
    if (ret != RT_EOK)
    {
        ICM_LOG_E("Register ICM20948 gyroscope sensor failed,err: %d", ret);
    }

    ret = icm20948_init(&cfg->intf);
    if (ret != RT_EOK)
    {
        ICM_LOG_E("ICM20948 init failed,err: %d", ret);
    }
    else
    {
        ICM_LOG_I("sensor init success");
    }

    return ret;
}

/* End of file****************************************************************/
