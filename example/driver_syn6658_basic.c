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
 * @file      driver_syn6658_basic.c
 * @brief     driver syn6658 basic source file
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

#include "driver_syn6658_basic.h"

static syn6658_handle_t gs_handle;        /**< syn6658 handle */

/**
 * @brief     basic example init
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t syn6658_basic_init(syn6658_interface_t interface)
{
    uint8_t res;
    
    /* link interface function */
    DRIVER_SYN6658_LINK_INIT(&gs_handle, syn6658_handle_t);
    DRIVER_SYN6658_LINK_UART_INIT(&gs_handle, syn6658_interface_uart_init);
    DRIVER_SYN6658_LINK_UART_DEINIT(&gs_handle, syn6658_interface_uart_deinit);
    DRIVER_SYN6658_LINK_UART_READ(&gs_handle, syn6658_interface_uart_read);
    DRIVER_SYN6658_LINK_UART_WRITE(&gs_handle, syn6658_interface_uart_write);
    DRIVER_SYN6658_LINK_UART_FLUSH(&gs_handle, syn6658_interface_uart_flush);
    DRIVER_SYN6658_LINK_SPI_INIT(&gs_handle, syn6658_interface_spi_init);
    DRIVER_SYN6658_LINK_SPI_DEINIT(&gs_handle, syn6658_interface_spi_deinit);
    DRIVER_SYN6658_LINK_SPI_TRANSMIT(&gs_handle, syn6658_interface_spi_transmit);
    DRIVER_SYN6658_LINK_GPIO_READY_INIT(&gs_handle, syn6658_interface_gpio_ready_init);
    DRIVER_SYN6658_LINK_GPIO_READY_DEINIT(&gs_handle, syn6658_interface_gpio_ready_deinit);
    DRIVER_SYN6658_LINK_GPIO_READY_READ(&gs_handle, syn6658_interface_gpio_ready_read);
    DRIVER_SYN6658_LINK_DELAY_MS(&gs_handle, syn6658_interface_delay_ms);
    DRIVER_SYN6658_LINK_DEBUG_PRINT(&gs_handle, syn6658_interface_debug_print);
    
    /* set interface */
    res = syn6658_set_interface(&gs_handle, interface);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set interface failed.\n");
        
        return 1;
    }
    
    /* syn6658 init */
    res = syn6658_init(&gs_handle);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: init failed.\n");
        
        return 1;
    }
    
    /* set default text type */
    res = syn6658_set_text_type(&gs_handle, SYN6658_BASIC_DEFAULT_TEXT_TYPE);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set text type failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
#if SYN6658_BASIC_SEND_CONFIG != 0
    /* set defalut synthesis volume */
    res = syn6658_set_synthesis_volume(&gs_handle, SYN6658_BASIC_DEFAULT_SYNTHESIS_VOLUME);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set synthesis volume failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set defalut synthesis speed */
    res = syn6658_set_synthesis_speed(&gs_handle, SYN6658_BASIC_DEFAULT_SYNTHESIS_SPEED);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set synthesis speed failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set defalut synthesis speaker */
    res = syn6658_set_synthesis_speaker(&gs_handle, SYN6658_BASIC_DEFAULT_SYNTHESIS_SPEAKER);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set synthesis speaker failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
#endif
    
    return 0;
}

/**
 * @brief     basic example synthesis
 * @param[in] fmt is the format data
 * @return    status code
 *            - 0 success
 *            - 1 synthesis failed
 * @note      none
 */
uint8_t syn6658_basic_synthesis(const char *const fmt, ...)
{
    uint8_t res;
    syn6658_status_t status;

    /* get status */
    res = syn6658_get_status(&gs_handle, &status);
    if (res != 0)
    {
        return 1;
    }
    
    /* check status */
    if (status == SYN6658_STATUS_BUSY)
    {
        return 1;
    }
    
    /* synthesis text */
    if (syn6658_synthesis_text(&gs_handle, fmt) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example sync
 * @return status code
 *         - 0 success
 *         - 1 sync failed
 * @note   none
 */
uint8_t syn6658_basic_sync(void)
{
    uint8_t res;
    syn6658_status_t status;
    
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            return 1;
        }
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t syn6658_basic_deinit(void)
{
    /* deinit syn6658 */
    if (syn6658_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example stop
 * @return status code
 *         - 0 success
 *         - 1 stop failed
 * @note   none
 */
uint8_t syn6658_basic_stop(void)
{
    /* stop */
    if (syn6658_stop(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example pause
 * @return status code
 *         - 0 success
 *         - 1 pause failed
 * @note   none
 */
uint8_t syn6658_basic_pause(void)
{
    /* pause */
    if (syn6658_pause(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example resume
 * @return status code
 *         - 0 success
 *         - 1 resume failed
 * @note   none
 */
uint8_t syn6658_basic_resume(void)
{
    /* resume */
    if (syn6658_resume(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example standby
 * @return status code
 *         - 0 success
 *         - 1 standby failed
 * @note   none
 */
uint8_t syn6658_basic_standby(void)
{
    /* standby */
    if (syn6658_standby(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example wake up
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t syn6658_basic_wake_up(void)
{
    /* wake up */
    if (syn6658_wake_up(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

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
uint8_t syn6658_basic_save_text(uint8_t offset, char *text)
{
    /* save text */
    if (syn6658_save_text(&gs_handle, offset, text) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     basic example play text
 * @param[in] times is the play times
 * @param[in] type is the encode type
 * @return    status code
 *            - 0 success
 *            - 1 play text failed
 * @note      0 <= times <= 15
 */
uint8_t syn6658_basic_play_text(uint8_t times, syn6658_type_t type)
{
    /* play text */
    if (syn6658_play_text(&gs_handle, times, type) != 0)
    {
        return 1;
    }
    
    return 0;
}
