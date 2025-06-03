/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-05-29     juzhango     the first version
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SENSOR_REG_H__
#define _SENSOR_REG_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <math.h>

#ifdef __GNUC__
#define PACKED __attribute__((packed))
#else
#define PACKED
#endif

#ifndef DRV_BYTE_ORDER
#ifndef __BYTE_ORDER__

#define DRV_LITTLE_ENDIAN 1234
#define DRV_BIG_ENDIAN 4321

/** if _BYTE_ORDER is not defined, choose the endianness of your architecture
 * by uncommenting the define which fits your platform endianness
 */
// #define DRV_BYTE_ORDER    DRV_BIG_ENDIAN
#define DRV_BYTE_ORDER DRV_LITTLE_ENDIAN

#else /* defined __BYTE_ORDER__ */

#define DRV_LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
#define DRV_BIG_ENDIAN __ORDER_BIG_ENDIAN__
#define DRV_BYTE_ORDER __BYTE_ORDER__

#endif /* __BYTE_ORDER__*/
#endif /* DRV_BYTE_ORDER */

#ifndef MEMS_SHARED_TYPES
#define MEMS_SHARED_TYPES

typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
    uint8_t bit0 : 1;
    uint8_t bit1 : 1;
    uint8_t bit2 : 1;
    uint8_t bit3 : 1;
    uint8_t bit4 : 1;
    uint8_t bit5 : 1;
    uint8_t bit6 : 1;
    uint8_t bit7 : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
    uint8_t bit7 : 1;
    uint8_t bit6 : 1;
    uint8_t bit5 : 1;
    uint8_t bit4 : 1;
    uint8_t bit3 : 1;
    uint8_t bit2 : 1;
    uint8_t bit1 : 1;
    uint8_t bit0 : 1;
#endif /* DRV_BYTE_ORDER */
} bitwise_t;

#define PROPERTY_DISABLE (0U)
#define PROPERTY_ENABLE (1U)

typedef int (*temp_dev_write_ptr)(void *, uint8_t, uint8_t *, uint16_t);
typedef int (*temp_dev_read_ptr)(void *, uint8_t, uint8_t *, uint16_t);

typedef struct
{
    /** Component mandatory fields **/
    temp_dev_write_ptr write_reg;
    temp_dev_read_ptr read_reg;
    /** Customizable optional pointer **/
    void *handle;
} icm20948_dev_ctx_t;

#endif /* MEMS_SHARED_TYPES */

/** I2C Device Address if SA0=0 -> 68 if SA0=1 -> 69 **/
#define ICM20948_I2C_ADD_L 0x68U
#define ICM20948_I2C_ADD_H 0x69U

/** Device Identification (Who am I) **/
#define ICM20948_ID 0xEAU

#define ICM20948_WHO_AM_I 0x00U

#define ICM20948_REG_BANK_SEL 0x7FU
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
    uint8_t reserved_0 : 4;
    uint8_t user_bank : 2;
    uint8_t reserved_1 : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
#endif
} PACKED icm20948_reg_bank_sel_t;

/* Bank 0 */
#define ICM20948_B0_PWR_MGMT_1 0x06U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
    uint8_t clksel : 3;
    uint8_t temp_dis : 1;
    uint8_t reserved : 1;
    uint8_t lp_en : 1;
    uint8_t sleep : 1;
    uint8_t device_reset : 1;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
#endif
} PACKED icm20948_pwr_mgmt_1_t;
#define ICM20948_B0_ACCEL_XOUT_H 0x2DU
#define ICM20948_B0_ACCEL_XOUT_L 0x2EU
#define ICM20948_B0_ACCEL_YOUT_H 0x2FU
#define ICM20948_B0_ACCEL_YOUT_L 0x30U
#define ICM20948_B0_ACCEL_ZOUT_H 0x31U
#define ICM20948_B0_ACCEL_ZOUT_L 0x32U
#define ICM20948_B0_GYRO_XOUT_H 0x33U
#define ICM20948_B0_GYRO_XOUT_L 0x34U
#define ICM20948_B0_GYRO_YOUT_H 0x35U
#define ICM20948_B0_GYRO_YOUT_L 0x36U
#define ICM20948_B0_GYRO_ZOUT_H 0x37U
#define ICM20948_B0_GYRO_ZOUT_L 0x38U

/* Bank 1 */
#define ICM20948_B1_XA_OFFS_H 0x14U
#define ICM20948_B1_XA_OFFS_L 0x15U
#define ICM20948_B1_YA_OFFS_H 0x16U
#define ICM20948_B1_YA_OFFS_L 0x17U
#define ICM20948_B1_ZA_OFFS_H 0x18U
#define ICM20948_B1_ZA_OFFS_L 0x19U

/* Bank 2 */
#define ICM20948_B2_GYRO_SMPLRT_DIV 0x00U
#define ICM20948_B2_GYRO_CONFIG_1 0x01U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
    uint8_t gyro_fchoice : 1;
    uint8_t gyro_fs_sel : 2;
    uint8_t gyro_dlpfcfg : 3;
    uint8_t reserved : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
#endif
} PACKED icm20948_gyro_config_1_t;
#define ICM20948_B2_GYRO_CONFIG_2 0x02U

#define ICM20948_B2_XG_OFFS_USRH 0x03U
#define ICM20948_B2_XG_OFFS_USRL 0x04U
#define ICM20948_B2_YG_OFFS_USRH 0x05U
#define ICM20948_B2_YG_OFFS_USRL 0x06U
#define ICM20948_B2_ZG_OFFS_USRH 0x07U
#define ICM20948_B2_ZG_OFFS_USRL 0x08U
#define ICM20948_B2_ODR_ALIGN_EN 0x09U
#define ICM20948_B2_ACCEL_SMPLRT_DIV_2 0x11U
#define ICM20948_B2_ACCEL_CONFIG 0x14U
typedef struct
{
#if DRV_BYTE_ORDER == DRV_LITTLE_ENDIAN
    uint8_t accel_fchoice : 1;
    uint8_t accel_fs_sel : 2;
    uint8_t accel_dlpfcfg : 3;
    uint8_t reserved : 2;
#elif DRV_BYTE_ORDER == DRV_BIG_ENDIAN
#endif
} PACKED icm20948_accel_config_t;

/************************************************************/

int32_t icm20948_read_reg(icm20948_dev_ctx_t *ctx, uint8_t reg, uint8_t *data, uint16_t len);
int32_t icm20948_write_reg(icm20948_dev_ctx_t *ctx, uint8_t reg, uint8_t *data, uint16_t len);

/************************************************************/
typedef enum
{
    ICM20948_SEL_USER_BANK_0 = 0,
    ICM20948_SEL_USER_BANK_1 = 1,
    ICM20948_SEL_USER_BANK_2 = 2,
    ICM20948_SEL_USER_BANK_3 = 3,
} PACKED icm20948_user_bank_t;
int32_t icm20948_user_bank_set(icm20948_dev_ctx_t *ctx, icm20948_user_bank_t val);
int32_t icm20948_user_bank_get(icm20948_dev_ctx_t *ctx, icm20948_user_bank_t *val);

int32_t icm20948_device_id_get(icm20948_dev_ctx_t *ctx, uint8_t *buff);

int32_t icm20948_reset_set(icm20948_dev_ctx_t *ctx, uint8_t val);
int32_t icm20948_reset_get(icm20948_dev_ctx_t *ctx, uint8_t *val);

typedef enum
{
    ICM20948_RUN_MODE = 0,
    ICM20948_SLEEP_MODE = 1,
} PACKED icm20948_sleep_t;
int32_t icm20948_sleep_set(icm20948_dev_ctx_t *ctx, icm20948_sleep_t val);

typedef enum
{
    ICM20948_CLKSEL_INTERNAL_20MHz = 0,
    ICM20948_CLKSEL_AUTO_SEL_BEST = 1,
    ICM20948_CLKSEL_STOP = 7,
} PACKED icm20948_clksel_t;
int32_t icm20948_clksel_set(icm20948_dev_ctx_t *ctx, icm20948_clksel_t val);

int32_t icm20948_gyro_smplrt_div_set(icm20948_dev_ctx_t *ctx, uint8_t val);

typedef enum
{
    ICM20948_GYRO_DLPFCFG_0 = 0,
    ICM20948_GYRO_DLPFCFG_1 = 1,
    ICM20948_GYRO_DLPFCFG_2 = 2,
    ICM20948_GYRO_DLPFCFG_3 = 3,
    ICM20948_GYRO_DLPFCFG_4 = 4,
    ICM20948_GYRO_DLPFCFG_5 = 5,
    ICM20948_GYRO_DLPFCFG_6 = 6,
    ICM20948_GYRO_DLPFCFG_7 = 7,
} PACKED icm20948_gyro_dlpfcfg_t;
int32_t icm20948_gyro_dlpfcfg_set(icm20948_dev_ctx_t *ctx, icm20948_gyro_dlpfcfg_t val);

typedef enum
{
    ICM20948_GYRO_FS_SEL_250dps = 0,
    ICM20948_GYRO_FS_SEL_500dps = 1,
    ICM20948_GYRO_FS_SEL_1000dps = 2,
    ICM20948_GYRO_FS_SEL_2000dps = 3,
} PACKED icm20948_gyro_fs_sel_t;
int32_t icm20948_gyro_full_scale_set(icm20948_dev_ctx_t *ctx, icm20948_gyro_fs_sel_t val);
int32_t icm20948_gyro_full_scale_get(icm20948_dev_ctx_t *ctx, icm20948_gyro_fs_sel_t *val);

typedef enum
{
    ICM20948_BYPASS_GYRO_DLPF = 0,
    ICM20948_ENABLE_GYRO_DLPF = 1,
} PACKED icm20948_gyro_fchoice_t;
int32_t icm20948_gyro_fchoice_set(icm20948_dev_ctx_t *ctx, icm20948_gyro_fchoice_t val);

int32_t icm20948_odr_align_set(icm20948_dev_ctx_t *ctx, uint8_t val);

int32_t icm20948_accel_smplrt_div_2_set(icm20948_dev_ctx_t *ctx, uint8_t val);
typedef enum
{
    ICM20948_ACCEL_DLPFCFG_0 = 0,
    ICM20948_ACCEL_DLPFCFG_1 = 1,
    ICM20948_ACCEL_DLPFCFG_2 = 2,
    ICM20948_ACCEL_DLPFCFG_3 = 3,
    ICM20948_ACCEL_DLPFCFG_4 = 4,
    ICM20948_ACCEL_DLPFCFG_5 = 5,
    ICM20948_ACCEL_DLPFCFG_6 = 6,
    ICM20948_ACCEL_DLPFCFG_7 = 7,
} PACKED icm20948_accel_dlpfcfg_t;
int32_t icm20948_accel_dlpfcfg_set(icm20948_dev_ctx_t *ctx, icm20948_accel_dlpfcfg_t val);

typedef enum
{
    ICM20948_ACCEL_FS_SEL_2g = 0,
    ICM20948_ACCEL_FS_SEL_4g = 1,
    ICM20948_ACCEL_FS_SEL_8g = 2,
    ICM20948_ACCEL_FS_SEL_16g = 3,
} PACKED icm20948_accel_fs_sel_t;
int32_t icm20948_accel_full_scale_set(icm20948_dev_ctx_t *ctx, icm20948_accel_fs_sel_t val);
int32_t icm20948_accel_full_scale_get(icm20948_dev_ctx_t *ctx, icm20948_accel_fs_sel_t *val);

typedef enum
{
    ICM20948_BYPASS_ACCEL_DLPF = 0,
    ICM20948_ENABLE_ACCEL_DLPF = 1,
} PACKED icm20948_accel_fchoice_t;
int32_t icm20948_accel_fchoice_set(icm20948_dev_ctx_t *ctx, icm20948_accel_fchoice_t val);

int32_t icm20948_acceleration_raw_get(icm20948_dev_ctx_t *ctx, int16_t *val);
int32_t icm20948_angular_rate_raw_get(icm20948_dev_ctx_t *ctx, int16_t *val);

// int32_t icm20948_gyro_calibration(icm20948_dev_ctx_t *ctx);
// int32_t icm20948_accel_calibration(icm20948_dev_ctx_t *ctx);

float_t icm20948_accel_lsb_to_mg(int16_t lsb, icm20948_accel_fs_sel_t fs_sel);
float_t icm20948_gyro_lsb_to_mdps(int16_t lsb, icm20948_gyro_fs_sel_t fs_sel);
#endif
