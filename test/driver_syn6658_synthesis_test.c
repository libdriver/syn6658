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
 * @file      driver_syn6658_synthesis_test.c
 * @brief     driver syn6658 synthesis test source file
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

#include "driver_syn6658_synthesis_test.h"

static syn6658_handle_t gs_handle;        /**< syn6658 handle */

/**
 * @brief     synthesis test
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t syn6658_synthesis_test(syn6658_interface_t interface)
{
    uint8_t res;
    syn6658_info_t info;
    syn6658_status_t status;
    char s[32];
    uint8_t gb2312_text[] = {0xD3, 0xEE, 0xD2, 0xF4, 0xCC, 0xEC, 0xCF, 0xC2, 0x00};
    uint8_t gbk_text[] = {0xD3, 0xEE, 0xD2, 0xF4, 0xCC, 0xEC, 0xCF, 0xC2, 0x00};
    uint8_t big5_text[] = {0xA6, 0x74, 0xAD, 0xB5, 0xA4, 0xD1, 0xA4, 0x55, 0x00};
    uint8_t unicode_little_text[] = {0x87, 0x5B, 0xF3, 0x97, 0x29, 0x59, 0x0B, 0x4E, 0x00};
    uint8_t unicode_big_text[] = {0x5B, 0x87, 0x97, 0xF3, 0x59, 0x29, 0x4E, 0x0B, 0x00};
    uint8_t text0[] = {0xB4, 0xCB, 0xB9, 0xA6, 0xC4, 0xDC, 0xBF, 0xC9, 0xD2, 0xD4, 0xBD, 0xAB, 0xB1, 0xBB, 0x00};
    uint8_t text1[] = {0xB4, 0xF2, 0xC2, 0xD2, 0xCB, 0xB3, 0xD0, 0xF2, 0xB5, 0xC4, 0xD2, 0xBB, 0xB6, 0xCE, 0xCE, 0xC4, 0x00};
    uint8_t text2[] = {0xB1, 0xBE, 0xB0, 0xB4, 0xBF, 0xCD, 0xBB, 0xA7, 0xCF, 0xA3, 0xCD, 0xFB, 0xB5, 0xC4, 0xCB, 0xB3, 0xD0, 0xF2, 0xB7, 0xD6, 0x00};
    uint8_t text3[] = {0xB6, 0xCE, 0xBB, 0xBA, 0xB4, 0xE6, 0xBA, 0xF3, 0xD2, 0xBB, 0xC6, 0xF0, 0xB2, 0xA5, 0xB7, 0xC5, 0xA1, 0xA3, 0x00};
    
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
    
    /* get syn6658 information */
    res = syn6658_info(&info);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        syn6658_interface_debug_print("syn6658: chip is %s.\n", info.chip_name);
        syn6658_interface_debug_print("syn6658: manufacturer is %s.\n", info.manufacturer_name);
        syn6658_interface_debug_print("syn6658: interface is %s.\n", info.interface);
        syn6658_interface_debug_print("syn6658: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        syn6658_interface_debug_print("syn6658: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        syn6658_interface_debug_print("syn6658: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        syn6658_interface_debug_print("syn6658: max current is %0.2fmA.\n", info.max_current_ma);
        syn6658_interface_debug_print("syn6658: max temperature is %0.1fC.\n", info.temperature_max);
        syn6658_interface_debug_print("syn6658: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
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
    
    /* synthesis text test */
    syn6658_interface_debug_print("syn6658: synthesis text test.\n");
    
    /* gb2312 synthesis text */
    syn6658_interface_debug_print("syn6658: gb2312 synthesis text.\n");
    
    /* set text type gb2312 */
    res = syn6658_set_text_type(&gs_handle, SYN6658_TYPE_GB2312);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set text type failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* synthesis text */
    res = syn6658_synthesis_text(&gs_handle, (char const *)gb2312_text);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* wait for finished */
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    /* gbk synthesis text */
    syn6658_interface_debug_print("syn6658: gbk synthesis text.\n");
    
    /* set text type gbk */
    res = syn6658_set_text_type(&gs_handle, SYN6658_TYPE_GBK);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set text type failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* synthesis text */
    res = syn6658_synthesis_text(&gs_handle, (char const *)gbk_text);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* wait for finished */
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    /* big5 synthesis text */
    syn6658_interface_debug_print("syn6658: big5 synthesis text.\n");
    
    /* set text type big5 */
    res = syn6658_set_text_type(&gs_handle, SYN6658_TYPE_BIG5);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set text type failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* synthesis text */
    res = syn6658_synthesis_text(&gs_handle, (char const *)big5_text);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* wait for finished */
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    /* unicode little endian synthesis text */
    syn6658_interface_debug_print("syn6658: unicode little endian synthesis text.\n");
    
    /* set text type unicode little endian */
    res = syn6658_set_text_type(&gs_handle, SYN6658_TYPE_UNICODE_LITTLE);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set text type failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* synthesis text */
    res = syn6658_synthesis_text(&gs_handle, (char const *)unicode_little_text);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* wait for finished */
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    /* unicode big endian synthesis text */
    syn6658_interface_debug_print("syn6658: unicode big endian synthesis text.\n");
    
    /* set text type unicode big endian */
    res = syn6658_set_text_type(&gs_handle, SYN6658_TYPE_UNICODE_BIG);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set text type failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* synthesis text */
    res = syn6658_synthesis_text(&gs_handle, (char const *)unicode_big_text);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* wait for finished */
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    /* sound synthesis text */
    syn6658_interface_debug_print("syn6658: sound synthesis text.\n");
    
    /* set text type gb2312 */
    res = syn6658_set_text_type(&gs_handle, SYN6658_TYPE_GB2312);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set text type failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* sound text */
    res = syn6658_synthesis_text(&gs_handle, (char const *)SYN6658_SOUND_202);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* wait for finished */
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    /* message synthesis text */
    syn6658_interface_debug_print("syn6658: message synthesis text.\n");
    
    /* message text */
    res = syn6658_synthesis_text(&gs_handle, (char const *)SYN6658_MESSAGE_A);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* wait for finished */
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    if (interface == SYN6658_INTERFACE_UART)
    {
        /* synthesis control test */
        syn6658_interface_debug_print("syn6658: synthesis control test.\n");
        res = syn6658_synthesis_text(&gs_handle, (char *)gb2312_text);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
        syn6658_interface_delay_ms(500);
        res = syn6658_pause(&gs_handle);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: pause failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
        syn6658_interface_debug_print("syn6658: synthesis control pause.\n");
        syn6658_interface_delay_ms(5000);
        res = syn6658_resume(&gs_handle);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: resume failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
        syn6658_interface_debug_print("syn6658: synthesis control resume.\n");
        status = SYN6658_STATUS_BUSY;
        while (status == SYN6658_STATUS_BUSY)
        {
            syn6658_interface_delay_ms(500);
            res = syn6658_get_status(&gs_handle, &status);
            if (res != 0)
            {
                syn6658_interface_debug_print("syn6658: get status failed.\n");
                (void)syn6658_deinit(&gs_handle);
                
                return 1;
            }
        }
        res = syn6658_synthesis_text(&gs_handle, (char *)gb2312_text);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
        syn6658_interface_delay_ms(500);
        res = syn6658_stop(&gs_handle);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: stop failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
        syn6658_interface_debug_print("syn6658: synthesis control stop.\n");
        syn6658_interface_delay_ms(5000);
    }
    
    /* synthesis volume test */
    syn6658_interface_debug_print("syn6658: synthesis volume test.\n");
    res = syn6658_set_synthesis_volume(&gs_handle, 5);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set synthesis volume failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    syn6658_interface_debug_print("syn6658: set synthesis volume 5.\n");
    res = syn6658_synthesis_text(&gs_handle, (char *)gb2312_text);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    res = syn6658_set_synthesis_volume(&gs_handle, 10);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set synthesis volume failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* synthesis speed test */
    syn6658_interface_debug_print("syn6658: synthesis speed test.\n");
    res = syn6658_set_synthesis_speed(&gs_handle, 10);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set synthesis speed failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    syn6658_interface_debug_print("syn6658: set synthesis speed 10.\n");
    res = syn6658_synthesis_text(&gs_handle, (char *)gb2312_text);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    res = syn6658_set_synthesis_speed(&gs_handle, 5);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set synthesis speed failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* synthesis speaker test */
    syn6658_interface_debug_print("syn6658: synthesis speaker test.\n");
    res = syn6658_set_synthesis_speaker(&gs_handle, SYN6658_SPEAKER_XIAOLING);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set synthesis speaker failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    syn6658_interface_debug_print("syn6658: set synthesis speaker xiaoling.\n");
    res = syn6658_synthesis_text(&gs_handle, (char *)gb2312_text);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    /* set synthesis speaker yixiaojian */
    res = syn6658_set_synthesis_speaker(&gs_handle, SYN6658_SPEAKER_YIXIAOJIAN);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set synthesis speaker failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    syn6658_interface_debug_print("syn6658: set synthesis speaker yixiaojian.\n");
    res = syn6658_synthesis_text(&gs_handle, (char *)gb2312_text);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    /* set synthesis speaker yixiaoqiang */
    res = syn6658_set_synthesis_speaker(&gs_handle, SYN6658_SPEAKER_YIXIAOQIANG);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set synthesis speaker failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    syn6658_interface_debug_print("syn6658: set synthesis speaker yixiaoqiang.\n");
    res = syn6658_synthesis_text(&gs_handle, (char *)gb2312_text);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    /* set synthesis speaker yixiaoqiang */
    res = syn6658_set_synthesis_speaker(&gs_handle, SYN6658_SPEAKER_TIANPEIPEI);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set synthesis speaker failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    syn6658_interface_debug_print("syn6658: set synthesis speaker tianpeipei.\n");
    res = syn6658_synthesis_text(&gs_handle, (char *)gb2312_text);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    /* set synthesis speaker yixiaoqiang */
    res = syn6658_set_synthesis_speaker(&gs_handle, SYN6658_SPEAKER_TANGLAOYA);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set synthesis speaker failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    syn6658_interface_debug_print("syn6658: set synthesis speaker tanglaoya.\n");
    res = syn6658_synthesis_text(&gs_handle, (char *)gb2312_text);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    
    /* set synthesis speaker yixiaoqiang */
    res = syn6658_set_synthesis_speaker(&gs_handle, SYN6658_SPEAKER_XIAOYANZI);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set synthesis speaker failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    syn6658_interface_debug_print("syn6658: set synthesis speaker xiaoyanzi.\n");
    res = syn6658_synthesis_text(&gs_handle, (char *)gb2312_text);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    res = syn6658_set_synthesis_speaker(&gs_handle, SYN6658_SPEAKER_XIAOLING);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: set synthesis speaker failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    /* command test */
    syn6658_interface_debug_print("syn6658: command test.\n");
    syn6658_interface_debug_print("syn6658: set command 0.\n");
    memset(s, 0, sizeof(char) * 32);
    strncpy(s, "[b0]", 32);
    res = syn6658_synthesis_text(&gs_handle, s);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    memset(s, 0, sizeof(char) * 32);
    strncpy(s, "abc,def,ghi", 32);
    res = syn6658_synthesis_text(&gs_handle, s);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    syn6658_interface_debug_print("syn6658: set command 1.\n");
    memset(s, 0, sizeof(char) * 32);
    strncpy(s, "[b1]", 32);
    res = syn6658_synthesis_text(&gs_handle, s);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    memset(s, 0, sizeof(char) * 32);
    strncpy(s, "abc,def,ghi", 32);
    res = syn6658_synthesis_text(&gs_handle, s);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    status = SYN6658_STATUS_BUSY;
    while (status == SYN6658_STATUS_BUSY)
    {
        syn6658_interface_delay_ms(500);
        res = syn6658_get_status(&gs_handle, &status);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: get status failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
    }
    memset(s, 0, sizeof(char) * 32);
    strncpy(s, "[b0]", 32);
    res = syn6658_synthesis_text(&gs_handle, s);
    if (res != 0)
    {
        syn6658_interface_debug_print("syn6658: synthesis text failed.\n");
        (void)syn6658_deinit(&gs_handle);
        
        return 1;
    }
    
    if (interface == SYN6658_INTERFACE_UART)
    {
        /* save and play text test */
        syn6658_interface_debug_print("syn6658: save and play text test.\n");
        
        /* save text to buffer 3 */
        syn6658_interface_debug_print("syn6658: save text to buffer 3.\n");
        
        /* save text */
        res = syn6658_save_text(&gs_handle, 3, (char *)text3);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: save text failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
        
        /* save text to buffer 1 */
        syn6658_interface_debug_print("syn6658: save text to buffer 1.\n");
        
        /* save text */
        res = syn6658_save_text(&gs_handle, 1, (char *)text1);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: save text failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
        
        /* save text to buffer 0 */
        syn6658_interface_debug_print("syn6658: save text to buffer 0.\n");
        
        /* save text */
        res = syn6658_save_text(&gs_handle, 0, (char *)text0);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: save text failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
        
        /* save text to buffer 2 */
        syn6658_interface_debug_print("syn6658: save text to buffer 2.\n");
        
        /* save text */
        res = syn6658_save_text(&gs_handle, 2, (char *)text2);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: save text failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
        
        /* play text test */
        syn6658_interface_debug_print("syn6658: play text test.\n");
        
        /* play text */
        res = syn6658_play_text(&gs_handle, 1, SYN6658_TYPE_GB2312);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: play text failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
        status = SYN6658_STATUS_BUSY;
        while (status == SYN6658_STATUS_BUSY)
        {
            syn6658_interface_delay_ms(500);
            res = syn6658_get_status(&gs_handle, &status);
            if (res != 0)
            {
                syn6658_interface_debug_print("syn6658: get status failed.\n");
                (void)syn6658_deinit(&gs_handle);
                
                return 1;
            }
        }
        
        /* syn6658 standby test */
        syn6658_interface_debug_print("syn6658: syn6658 standby test.\n");
        
        /* standby */
        res = syn6658_standby(&gs_handle);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: standby failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
        
        /* standby ok */
        syn6658_interface_debug_print("syn6658: standby ok.\n");
        
        /* syn6658 wake up test */
        syn6658_interface_debug_print("syn6658: syn6658 wake up test.\n");
        
        /* wake up */
        res = syn6658_wake_up(&gs_handle);
        if (res != 0)
        {
            syn6658_interface_debug_print("syn6658: wake up failed.\n");
            (void)syn6658_deinit(&gs_handle);
            
            return 1;
        }
        
        /* wake up ok */
        syn6658_interface_debug_print("syn6658: wake up ok.\n");
    }
    
    /* finish synthesis test */
    syn6658_interface_debug_print("syn6658: finish synthesis test.\n");
    (void)syn6658_deinit(&gs_handle);
    
    return 0;
}
