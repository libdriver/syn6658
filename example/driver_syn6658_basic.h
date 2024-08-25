/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_syn6658_basic.h
 * @brief     driver syn6658 basic header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2024-08-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2024/08/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_SYN6658_BASIC_H
#define DRIVER_SYN6658_BASIC_H

#include "driver_syn6658_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup syn6658_example_driver syn6658 example driver function
 * @brief    syn6658 example driver modules
 * @ingroup  syn6658_driver
 * @{
 */

/**
 * @brief syn6658 basic send command configure
 */
#ifndef SYN6658_BASIC_SEND_CONFIG
    #define SYN6658_BASIC_SEND_CONFIG 0        /**< don't send */
#endif

/**
 * @brief syn6658 basic example default definition
 */
#define SYN6658_BASIC_DEFAULT_TEXT_TYPE           SYN6658_TYPE_GB2312              /**< gb2312 type */
#define SYN6658_BASIC_DEFAULT_SYNTHESIS_VOLUME    10                               /**< synthesis volume 10 */
#define SYN6658_BASIC_DEFAULT_SYNTHESIS_SPEED     5                                /**< synthesis speed 5 */
#define SYN6658_BASIC_DEFAULT_SYNTHESIS_SPEAKER   SYN6658_SPEAKER_XIAOLING         /**< xiaoling */

/**
 * @brief     basic example init
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t syn6658_basic_init(syn6658_interface_t interface);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t syn6658_basic_deinit(void);

/**
 * @brief     basic example synthesis
 * @param[in] fmt is the format data
 * @return    status code
 *            - 0 success
 *            - 1 synthesis failed
 * @note      none
 */
uint8_t syn6658_basic_synthesis(const char *const fmt, ...);

/**
 * @brief  basic example sync
 * @return status code
 *         - 0 success
 *         - 1 sync failed
 * @note   none
 */
uint8_t syn6658_basic_sync(void);

/**
 * @brief  basic example stop
 * @return status code
 *         - 0 success
 *         - 1 stop failed
 * @note   none
 */
uint8_t syn6658_basic_stop(void);

/**
 * @brief  basic example pause
 * @return status code
 *         - 0 success
 *         - 1 pause failed
 * @note   none
 */
uint8_t syn6658_basic_pause(void);

/**
 * @brief  basic example resume
 * @return status code
 *         - 0 success
 *         - 1 resume failed
 * @note   none
 */
uint8_t syn6658_basic_resume(void);

/**
 * @brief  basic example standby
 * @return status code
 *         - 0 success
 *         - 1 standby failed
 * @note   none
 */
uint8_t syn6658_basic_standby(void);

/**
 * @brief  basic example wake up
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t syn6658_basic_wake_up(void);

/**
 * @brief     basic example save text
 * @param[in] offset is the buffer offset
 * @param[in] *text points to a text buffer
 * @return    status code
 *            - 0 success
 *            - 1 save text failed
 * @note      0 <= offset <= 15
 *            text length <= ((16 - offset) * 256)
 */
uint8_t syn6658_basic_save_text(uint8_t offset, char *text);

/**
 * @brief     basic example play text
 * @param[in] times is the play times
 * @param[in] type is the encode type
 * @return    status code
 *            - 0 success
 *            - 1 play text failed
 * @note      0 <= times <= 15
 */
uint8_t syn6658_basic_play_text(uint8_t times, syn6658_type_t type);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
