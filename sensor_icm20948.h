/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-05-29     Lenovo       the first version
 */
/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __SENSOR_ICM20948_H__
#define __SENSOR_ICM20948_H__
/* Includes -----------------------------------------------------------------*/
#include "sensor.h"
/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/
/* Exported macro -----------------------------------------------------------*/
/* Exported functions -------------------------------------------------------*/
int rt_hw_icm20948_init(const char *name, struct rt_sensor_config *cfg);

#endif /*__SENSOR_ICM20948_H__*/
/* End of file****************************************************************/
