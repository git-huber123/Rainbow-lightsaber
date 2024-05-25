/*!
 *  @file Adafruit_LSM6DS.h
 *
 * 	I2C Driver base for Adafruit LSM6DSxx 6-DoF Accelerometer and Gyroscope
 *      library
 *
 * 	Adafruit invests time and resources providing this open source code,
 *      please support Adafruit and open-source hardware by purchasing products
 *from Adafruit!
 *
 *	BSD license (see license.txt)
 */

#ifndef _ADAFRUIT_LSM6DS_H
#define _ADAFRUIT_LSM6DS_H

#define LSM6DS_I2CADDR_DEFAULT  0x6A    ///< LSM6DS default i2c address

#define LSM6DS_FUNC_CFG_ACCESS  0x01    ///< Enable embedded functions register
#define LSM6DS_INT1_CTRL        0x0D    ///< Interrupt control for INT 1
#define LSM6DS_INT2_CTRL        0x0E    ///< Interrupt control for INT 2
#define LSM6DS_WHOAMI           0x0F    ///< Chip ID register
#define LSM6DS_CTRL1_XL         0x10    ///< Main accelerometer config register
#define LSM6DS_CTRL2_G          0x11    ///< Main gyro config register
#define LSM6DS_CTRL3_C          0x12    ///< Main configuration register
#define LSM6DS_CTRL8_XL         0x17    ///< High and low pass for accel
#define LSM6DS_CTRL10_C         0x19    ///< Main configuration register
#define LSM6DS_WAKEUP_SRC       0x1B    ///< Why we woke up
#define LSM6DS_STATUS_REG       0x1E    ///< Status register
#define LSM6DS_OUT_TEMP_L       0x20    ///< First data register (temperature low)
#define LSM6DS_OUTX_L_G         0x22    ///< First gyro data register
#define LSM6DS_OUTX_L_A         0x28    ///< First accel data register
#define LSM6DS_STEPCOUNTER      0x4B    ///< 16-bit step counter
#define LSM6DS_TAP_CFG          0x58    ///< Tap/pedometer configuration
#define LSM6DS_WAKEUP_THS       0x5B    ///< Single and double-tap function threshold register
#define LSM6DS_WAKEUP_DUR       0x5C    ///< Free-fall, wakeup, timestamp and sleep mode duration
#define LSM6DS_MD1_CFG          0x5E    ///< Functions routing on INT1 register

/** The accelerometer data rate */
#define LSM6DS_RATE_SHUTDOWN    0x00
#define LSM6DS_RATE_12_5_HZ     0x10
#define LSM6DS_RATE_26_HZ       0x20
#define LSM6DS_RATE_52_HZ       0x30
#define LSM6DS_RATE_104_HZ      0x40
#define LSM6DS_RATE_208_HZ      0x50
#define LSM6DS_RATE_416_HZ      0x60
#define LSM6DS_RATE_833_HZ      0x70
#define LSM6DS_RATE_1_66K_HZ    0x80
#define LSM6DS_RATE_3_33K_HZ    0x90
#define LSM6DS_RATE_6_66K_HZ    0xA0

/** The accelerometer data range */
#define LSM6DS_ACCEL_RANGE_2_G      0b0000
#define LSM6DS_ACCEL_RANGE_16_G     0b0100
#define LSM6DS_ACCEL_RANGE_4_G      0b1000
#define LSM6DS_ACCEL_RANGE_8_G      0b1100

/** The gyro data range */
#define LSM6DS_GYRO_RANGE_125_DPS   0b0010
#define LSM6DS_GYRO_RANGE_250_DPS   0b0000
#define LSM6DS_GYRO_RANGE_500_DPS   0b0100
#define LSM6DS_GYRO_RANGE_1000_DPS  0b1000
#define LSM6DS_GYRO_RANGE_2000_DPS  0b1100

#endif
