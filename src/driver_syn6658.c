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
 * @file      driver_syn6658.c
 * @brief     driver syn6658 source file
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

#include "driver_syn6658.h"
#include <stdarg.h>

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Voicetx SYN6658"       /**< chip name */
#define MANUFACTURER_NAME         "Voicetx"               /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        3.1f                    /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        4.5f                    /**< chip max supply voltage */
#define MAX_CURRENT               53.0f                   /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                  /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                   /**< chip max operating temperature */
#define DRIVER_VERSION            1000                    /**< driver version */

/**
 * @brief     check busy
 * @param[in] *handle points to a syn6658 handle structure
 * @param[in] timeout is the timeout in ms
 * @return    status code
 *            - 0 success
 *            - 1 timeout or gpio read failed
 * @note      none
 */
static uint8_t a_check_busy(syn6658_handle_t *handle, uint32_t timeout)
{
    uint8_t value;
    uint32_t i;

    for (i = 0; i < timeout; i++)                        /* loop all */
    {
        if (handle->gpio_ready_read(&value) != 0)        /* read ready level */
        {
            return 1;                                    /* return error */
        }
        if (value == 0)                                  /* check value */
        {
            return 0;                                    /* return ready */
        }
        handle->delay_ms(1);                             /* delay 1ms */
    }

    return 1;                                            /* return timeout */
}

/**
 * @brief     write data
 * @param[in] *handle points to a syn6658 handle structure
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_write(syn6658_handle_t *handle, uint8_t *buf, uint16_t len)
{
    uint8_t res;
    uint16_t i;

    if (handle->uart_spi != 0)                              /* spi interface */
    {
        for (i = 0; i < len; i++)                           /* write all */
        {
            uint8_t rx;
            
            res = handle->spi_transmit(&buf[i], &rx, 1);    /* write one byte */
            if (res != 0)                                   /* check result */
            {
                return 1;                                   /* return error */
            }
            handle->delay_ms(1);                            /* delay 1ms */
        }
    }
    else                                                    /* uart interface */
    {
        res = handle->uart_flush();                         /* uart flush */
        if (res != 0)                                       /* check result */
        {
            return 1;                                       /* return error */
        }
        res = handle->uart_write(buf, len);                 /* uart write */
        if (res != 0)                                       /* check result */
        {
            return 1;                                       /* return error */
        }
    }

    return 0;                                               /* success return 0 */
}

/**
 * @brief      read data
 * @param[in]  *handle points to a syn6658 handle structure
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint16_t a_read(syn6658_handle_t *handle, uint8_t *buf, uint16_t len)
{
    uint8_t res;
    uint16_t i;

    if (handle->uart_spi != 0)                             /* spi interface */
    {
        for (i = 0; i < len; i++)                          /* loop all */
        {
            uint8_t tx = 0xFF;
            
            res = handle->spi_transmit(&tx, &buf[i], 1);   /* read one byte */
            if (res != 0)                                  /* check result */
            {
                return 1;                                  /* return error */
            }
            handle->delay_ms(1);                           /* delay 1ms */
        }
    }
    else
    {
        if (handle->uart_read(buf, len) != len)            /* read data */
        {
            return 1;                                      /* return error */
        }
    }

    return len;                                            /* success return 0 */
}

/**
 * @brief     set the chip interface
 * @param[in] *handle points to a syn6658 handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t syn6658_set_interface(syn6658_handle_t *handle, syn6658_interface_t interface)
{
    if (handle == NULL)                          /* check handle */
    {
        return 2;                                /* return error */
    }

    handle->uart_spi = (uint8_t)interface;       /* set interface */

    return 0;                                    /* success return 0 */
}

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to a syn6658 handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t syn6658_get_interface(syn6658_handle_t *handle, syn6658_interface_t *interface)
{
    if (handle == NULL)                                         /* check handle */
    {
        return 2;                                               /* return error */
    }

    *interface = (syn6658_interface_t)(handle->uart_spi);       /* get interface */

    return 0;                                                   /* success return 0 */
}

/**
 * @brief     initialize the chip
 * @param[in] *handle points to a syn6658 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 uart or spi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 gpio ready init failed
 * @note      none
 */
uint8_t syn6658_init(syn6658_handle_t *handle)
{
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->debug_print == NULL)                                   /* check debug_print */
    {
        return 3;                                                      /* return error */
    }
    if (handle->uart_init == NULL)                                     /* check uart_init */
    {
        handle->debug_print("syn6658: uart_init is null.\n");          /* uart_init is null */

        return 3;                                                      /* return error */
    }
    if (handle->uart_deinit == NULL)                                   /* check uart_deinit */
    {
        handle->debug_print("syn6658: uart_deinit is null.\n");        /* uart_deinit is null */

        return 3;                                                      /* return error */
    }
    if (handle->uart_read == NULL)                                     /* check uart_read */
    {
        handle->debug_print("syn6658: uart_read is null.\n");          /* uart_read is null */

        return 3;                                                      /* return error */
    }
    if (handle->uart_write == NULL)                                    /* check uart_write */
    {
        handle->debug_print("syn6658: uart_write is null.\n");         /* uart_write is null */

        return 3;                                                      /* return error */
    }
    if (handle->uart_flush == NULL)                                    /* check uart_flush */
    {
        handle->debug_print("syn6658: uart_flush is null.\n");         /* uart_flush is null */

        return 3;                                                      /* return error */
    }
    if (handle->spi_init == NULL)                                      /* check spi_init */
    {
        handle->debug_print("syn6658: spi_init is null.\n");           /* spi_init is null */

        return 3;                                                      /* return error */
    }
    if (handle->spi_deinit == NULL)                                    /* check spi_deinit */
    {
        handle->debug_print("syn6658: spi_deinit is null.\n");         /* spi_deinit is null */

        return 3;                                                      /* return error */
    }
    if (handle->spi_transmit == NULL)                                  /* check spi_transmit */
    {
        handle->debug_print("syn6658: spi_transmit is null.\n");       /* spi_transmit is null */

        return 3;                                                      /* return error */
    }
    if (handle->gpio_ready_init == NULL)                               /* check gpio_ready_init */
    {
        handle->debug_print("syn6658: gpio_ready_init is null.\n");    /* gpio_ready_init is null */

        return 3;                                                      /* return error */
    }
    if (handle->gpio_ready_deinit == NULL)                             /* check gpio_ready_deinit */
    {
        handle->debug_print("syn6658: gpio_ready_deinit is null.\n");  /* gpio_ready_deinit is null */

        return 3;                                                      /* return error */
    }
    if (handle->gpio_ready_read == NULL)                               /* check gpio_ready_read */
    {
        handle->debug_print("syn6658: gpio_ready_read is null.\n");    /* gpio_ready_read is null */

        return 3;                                                      /* return error */
    }
    if (handle->delay_ms == NULL)                                      /* check delay_ms */
    {
        handle->debug_print("syn6658: delay_ms is null.\n");           /* delay_ms is null */

        return 3;                                                      /* return error */
    }

    if (handle->uart_spi != 0)                                         /* spi interface */
    {
        if (handle->spi_init() != 0)                                   /* spi init */
        {
            handle->debug_print("syn6658: spi init failed.\n");        /* spi init failed */

            return 1;                                                  /* return error */
        }
    }
    else                                                               /* uart interface */
    {
        if (handle->uart_init() != 0)                                  /* uart init */
        {
            handle->debug_print("syn6658: uart init failed.\n");       /* uart init failed */

            return 1;                                                  /* return error */
        }
    }
    if (handle->gpio_ready_init() != 0)                                /* gpio ready init */
    {
        if (handle->uart_spi != 0)                                     /* spi interface */
        {
            (void)handle->spi_deinit();                                /* spi deinit */
        }
        else                                                           /* uart interface */
        {
            (void)handle->uart_deinit();                               /* uart deinit */
        }
        handle->debug_print("syn6658: gpio ready init failed.\n");     /* gpio ready init failed */

        return 4;                                                      /* return error */
    }
    handle->inited = 1;                                                /* flag finish initialization */

    return 0;                                                          /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle points to a syn6658 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 uart or spi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 gpio ready deinit failed
 * @note      none
 */
uint8_t syn6658_deinit(syn6658_handle_t *handle)
{
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }

    if (handle->uart_spi != 0)                                         /* spi interface */
    {
        if (handle->spi_deinit() != 0)                                 /* spi deinit */
        {
            handle->debug_print("syn6658: spi deinit failed.\n");      /* spi deinit failed */

            return 1;                                                  /* return error */
        }
    }
    else                                                               /* uart interface */
    {
        if (handle->uart_deinit() != 0)                                /* uart deinit */
        {
            handle->debug_print("syn6658: uart deinit failed.\n");     /* uart deinit failed */

            return 1;                                                  /* return error */
        }
    }
    if (handle->gpio_ready_deinit() != 0)                              /* gpio ready deinit */
    {
        handle->debug_print("syn6658: gpio ready deinit failed.\n");   /* gpio ready deinit failed */

        return 4;                                                      /* return error */
    }
    handle->inited = 0;                                                /* flag closed */

    return 0;                                                          /* success return 0 */
}

/**
 * @brief      get the current status
 * @param[in]  *handle points to a syn6658 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t syn6658_get_status(syn6658_handle_t *handle, syn6658_status_t *status)
{
    uint8_t res;
    uint8_t value;
    uint8_t times = 3;
    uint16_t len;
    uint8_t temp[1];
    uint8_t cmd[4];

    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    if (handle->uart_spi != 0)                                                    /* spi interface */
    {
        if (handle->gpio_ready_read(&value) != 0)                                 /* read ready level */
        {
            return 1;                                                             /* return error */
        }
        if (value != 0)                                                           /* check value */
        {
            *status = (syn6658_status_t)(1);                                      /* set status */
        }
        else
        {
            *status = (syn6658_status_t)(0);                                      /* set status */
        }

        return 0;                                                                 /* success return 0 */
    }
    else                                                                          /* uart interface */
    {
        cmd[0] = 0xFD;                                                            /* frame header */
        cmd[1] = 0x00;                                                            /* length msb */
        cmd[2] = 0x01;                                                            /* length lsb */
        cmd[3] = 0x21;                                                            /* command */
        while (1)                                                                 /* loop */
        {
            res = handle->uart_flush();                                           /* uart flush */
            if (res != 0)                                                         /* check result */
            {
                handle->debug_print("syn6658: uart flush failed.\n");             /* uart flush failed */

                return 1;                                                         /* return error */
            }
            res = handle->uart_write((uint8_t *)cmd, 4);                          /* uart write */
            if (res != 0)                                                         /* check result */
            {
                handle->debug_print("syn6658: uart write failed.\n");             /* uart write failed */

                return 1;                                                         /* return error */
            }
            handle->delay_ms(100);                                                /* delay 100 ms */
            memset(temp, 0, sizeof(uint8_t) * 1);                                 /* clear the buffer */
            len = handle->uart_read((uint8_t *)temp, 1);                          /* uart read */
            if (len != 1)                                                         /* check result */
            {
                handle->debug_print("syn6658: uart read failed.\n");              /* uart read failed */

                return 1;                                                         /* return error */
            }
            if (temp[0] == 0x4F)                                                  /* check frame */
            {
                *status = (syn6658_status_t)(0);                                  /* set status */

                return 0;                                                         /* success return 0 */
            }
            else if (temp[0] == 0x4E)                                             /* check frame */
            {
                *status = (syn6658_status_t)(1);                                  /* set status */

                return 0;                                                         /* success return 0 */
            }
            else
            {
                if (times != 0)                                                   /* check times */
                {
                    times--;                                                      /* retry times-- */
                    handle->delay_ms(100);                                        /* delay 100 ms */

                    continue;                                                     /* continue */
                }
                handle->debug_print("syn6658: command receive failed.\n");        /* command receive failed */

                return 1;                                                         /* return error */
            }
        }
    }
}

/**
 * @brief     stop the chip
 * @param[in] *handle points to a syn6658 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 stop failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 spi interface can't use this function
 * @note      none
 */
uint8_t syn6658_stop(syn6658_handle_t *handle)
{
    uint8_t res;
    uint16_t len;
    uint8_t temp;
    uint8_t cmd[4];

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    if (handle->uart_spi != 0)                                            /* check interface */
    {
        handle->debug_print("syn6658: spi interface \
                            can't use this function.\n");                 /* spi interface can't use this function */

        return 4;                                                         /* return error */
    }
    
    cmd[0] = 0xFD;                                                        /* frame header */
    cmd[1] = 0x00;                                                        /* length msb */
    cmd[2] = 0x01;                                                        /* length lsb */
    cmd[3] = 0x02;                                                        /* command */
    res = a_write(handle, (uint8_t *)cmd, 4);                             /* write data */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("syn6658: uart write failed.\n");             /* uart write failed */

        return 1;                                                         /* return error */
    }
    handle->delay_ms(100);                                                /* delay 100 ms */
    len = a_read(handle, (uint8_t *)&temp, 1);                            /* read data */
    if (len != 1)                                                         /* check result */
    {
        handle->debug_print("syn6658: uart read failed.\n");              /* uart read failed */

        return 1;                                                         /* return error */
    }
    if (temp == 0x41)                                                     /* check return */
    {
        return 0;                                                         /* success return 0 */
    }
    else
    {
        handle->debug_print("syn6658: command receive failed.\n");        /* command receive failed */

        return 1;                                                         /* return error */
    }
}

/**
 * @brief     pause the chip
 * @param[in] *handle points to a syn6658 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 pause failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 spi interface can't use this function
 * @note      none
 */
uint8_t syn6658_pause(syn6658_handle_t *handle)
{
    uint8_t res;
    uint16_t len;
    uint8_t temp;
    uint8_t cmd[4];

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    if (handle->uart_spi != 0)                                            /* check interface */
    {
        handle->debug_print("syn6658: spi interface \
                            can't use this function.\n");                 /* spi interface can't use this function */

        return 4;                                                         /* return error */
    }
    
    cmd[0] = 0xFD;                                                        /* frame header */
    cmd[1] = 0x00;                                                        /* length msb */
    cmd[2] = 0x01;                                                        /* length lsb */
    cmd[3] = 0x03;                                                        /* command */
    res = a_write(handle, (uint8_t *)cmd, 4);                             /* write data */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("syn6658: uart write failed.\n");             /* uart write failed */

        return 1;                                                         /* return error */
    }
    handle->delay_ms(100);                                                /* delay 100 ms */
    len = a_read(handle, (uint8_t *)&temp, 1);                            /* read data */
    if (len != 1)                                                         /* check result */
    {
        handle->debug_print("syn6658: uart read failed.\n");              /* uart read failed */

        return 1;                                                         /* return error */
    }
    if (temp == 0x41)                                                     /* check return */
    {
        
        return 0;                                                         /* success return 0 */
    }
    else
    {
        handle->debug_print("syn6658: command receive failed.\n");        /* command receive failed */

        return 1;                                                         /* return error */
    }
}

/**
 * @brief     resume the chip
 * @param[in] *handle points to a syn6658 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 resume failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 spi interface can't use this function
 * @note      none
 */
uint8_t syn6658_resume(syn6658_handle_t *handle)
{
    uint8_t res;
    uint16_t len;
    uint8_t temp;
    uint8_t cmd[4];

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    if (handle->uart_spi != 0)                                            /* check interface */
    {
        handle->debug_print("syn6658: spi interface \
                            can't use this function.\n");                 /* spi interface can't use this function */

        return 4;                                                         /* return error */
    }
    
    cmd[0] = 0xFD;                                                        /* frame header */
    cmd[1] = 0x00;                                                        /* length msb */
    cmd[2] = 0x01;                                                        /* length lsb */
    cmd[3] = 0x04;                                                        /* command */
    res = a_write(handle, (uint8_t *)cmd, 4);                             /* write data */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("syn6658: uart write failed.\n");             /* uart write failed */

        return 1;                                                         /* return error */
    }
    handle->delay_ms(100);                                                /* delay 100 ms */
    len = a_read(handle, (uint8_t *)&temp, 1);                            /* read data */
    if (len != 1)                                                         /* check result */
    {
        handle->debug_print("syn6658: uart read failed.\n");              /* uart read failed */

        return 1;                                                         /* return error */
    }
    if (temp == 0x41)                                                     /* check return */
    {
        return 0;                                                         /* success return 0 */
    }
    else
    {
        handle->debug_print("syn6658: command receive failed.\n");        /* command receive failed */

        return 1;                                                         /* return error */
    }
}

/**
 * @brief     set the chip to standby mode
 * @param[in] *handle points to a syn6658 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 standby failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 spi interface can't use this function
 * @note      none
 */
uint8_t syn6658_standby(syn6658_handle_t *handle)
{
    uint8_t res;
    uint16_t len;
    uint8_t temp;
    uint8_t cmd[4];

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    if (handle->uart_spi != 0)                                            /* check interface */
    {
        handle->debug_print("syn6658: spi interface \
                            can't use this function.\n");                 /* spi interface can't use this function */

        return 4;                                                         /* return error */
    }
    
    cmd[0] = 0xFD;                                                        /* frame header */
    cmd[1] = 0x00;                                                        /* length msb */
    cmd[2] = 0x01;                                                        /* length lsb */
    cmd[3] = 0x22;                                                        /* command */
    res = a_write(handle, (uint8_t *)cmd, 4);                             /* write data */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("syn6658: uart write failed.\n");             /* uart write failed */

        return 1;                                                         /* return error */
    }
    handle->delay_ms(100);                                                /* delay 100 ms */
    len = a_read(handle, (uint8_t *)&temp, 1);                            /* read data */
    if (len != 1)                                                         /* check result */
    {
        handle->debug_print("syn6658: uart read failed.\n");              /* uart read failed */

        return 1;                                                         /* return error */
    }
    if (temp == 0x41)                                                     /* check return */
    {
        return 0;                                                         /* success return 0 */
    }
    else
    {
        handle->debug_print("syn6658: command receive failed.\n");        /* command receive failed */

        return 1;                                                         /* return error */
    }
}

/**
 * @brief     wake up the chip
 * @param[in] *handle points to a syn6658 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 wake up failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 spi interface can't use this function
 * @note      none
 */
uint8_t syn6658_wake_up(syn6658_handle_t *handle)
{
    uint8_t res;
    uint16_t len;
    uint8_t temp;
    uint8_t cmd[4];

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }
    if (handle->uart_spi != 0)                                            /* check interface */
    {
        handle->debug_print("syn6658: spi interface \
                            can't use this function.\n");                 /* spi interface can't use this function */

        return 4;                                                         /* return error */
    }
    
    cmd[0] = 0xFD;                                                        /* frame header */
    cmd[1] = 0x00;                                                        /* length msb */
    cmd[2] = 0x01;                                                        /* length lsb */
    cmd[3] = 0xFF;                                                        /* command */
    res = a_write(handle, (uint8_t *)cmd, 4);                             /* write data */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("syn6658: uart write failed.\n");             /* uart write failed */

        return 1;                                                         /* return error */
    }
    handle->delay_ms(100);                                                /* delay 100 ms */
    res = a_write(handle, (uint8_t *)cmd, 4);                             /* write data */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("syn6658: uart write failed.\n");             /* uart write failed */

        return 1;                                                         /* return error */
    }
    handle->delay_ms(100);                                                /* delay 100 ms */
    len = a_read(handle, (uint8_t *)&temp, 1);                            /* read data */
    if (len != 1)                                                         /* check result */
    {
        handle->debug_print("syn6658: uart read failed.\n");              /* uart read failed */

        return 1;                                                         /* return error */
    }
    if (temp == 0x41)                                                     /* check return */
    {
        return 0;                                                         /* success return 0 */
    }
    else
    {
        handle->debug_print("syn6658: command receive failed.\n");        /* command receive failed */

        return 1;                                                         /* return error */
    }
}

/**
 * @brief     set the chip text type
 * @param[in] *handle points to a syn6658 handle structure
 * @param[in] type is the chip text type
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t syn6658_set_text_type(syn6658_handle_t *handle, syn6658_type_t type)
{
    if (handle == NULL)                  /* check handle */
    {
        return 2;                        /* return error */
    }
    if (handle->inited != 1)             /* check handle initialization */
    {
        return 3;                        /* return error */
    }

    handle->type = (uint8_t)type;        /* set type */

    return 0;                            /* success return 0 */
}

/**
 * @brief      get the chip text type
 * @param[in]  *handle points to a syn6658 handle structure
 * @param[out] *type points to a chip text type
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t syn6658_get_text_type(syn6658_handle_t *handle, syn6658_type_t *type)
{
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    if (handle->inited != 1)                      /* check handle initialization */
    {
        return 3;                                 /* return error */
    }

    *type = (syn6658_type_t)(handle->type);       /* get type */

    return 0;                                     /* success return 0 */
}

/**
 * @brief     synthesis the test
 * @param[in] *handle points to a syn6658 handle structure
 * @param[in] fmt is the format data
 * @return    status code
 *            - 0 success
 *            - 1 synthesis text failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 *            - 5 uart read failed
 * @note      none
 */
uint8_t syn6658_synthesis_text(syn6658_handle_t *handle, const char *const fmt, ...)
{
    va_list args;
    uint8_t res;
    uint8_t temp;
    uint16_t l;
    uint16_t len;
    uint32_t timeout = SYN6658_BUSY_TIMEOUT;

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    memset((char *)(handle->buf + 5), 0, sizeof(char) * 4091);            /* clear buffer */
    va_start(args, fmt);                                                  /* var start */
    (void)vsnprintf((char *)(handle->buf + 5), 4091, 
                   (char const *)fmt, args);                              /* print to buffer */
    va_end(args);                                                         /* var end */
    len = (uint16_t)strlen((const char *)(handle->buf + 5));              /* get length of txt */
    handle->buf[0] = 0xFD;                                                /* frame header */
    handle->buf[1] = (uint8_t)((len + 2) / 256);                          /* length msb */
    handle->buf[2] = (len + 2) % 256;                                     /* length lsb */
    handle->buf[3] = 0x01;                                                /* command */
    handle->buf[4] = handle->type;                                        /* command type */
    res = a_check_busy(handle, timeout);
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("syn6658: chip is busy.\n");                  /* chip is busy */

        return 4;                                                         /* return error */
    }
    res = a_write(handle, handle->buf, len + 5);                          /* write data */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("syn6658: write failed.\n");                  /* write failed */

        return 1;                                                         /* return error */
    }
    if (handle->uart_spi == 0)                                            /* uart interface */
    {
        handle->delay_ms(100);                                            /* delay 100 ms */
    }
    l = a_read(handle, (uint8_t *)&temp, 1);                              /* read data */
    if (l != 1)                                                           /* check result */
    {
        handle->debug_print("syn6658: uart read failed.\n");              /* uart read failed */

        return 5;                                                         /* return error */
    }
    if (temp == 0x41)                                                     /* check return */
    {
        if (handle->uart_spi != 0)                                        /* spi interface */
        {
            handle->delay_ms(200);                                        /* delay 200 ms */
        }
        
        return 0;                                                         /* success return 0 */
    }
    else
    {
        handle->debug_print("syn6658: command receive failed.\n");        /* command receive failed */

        return 5;                                                         /* return error */
    }
}

/**
 * @brief     set the chip synthesis volume
 * @param[in] *handle points to a syn6658 handle structure
 * @param[in] volume is the synthesis volume
 * @return    status code
 *            - 0 success
 *            - 1 set synthesis volume failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 volume is invalid
 * @note      0 <= volume <= 10
 */
uint8_t syn6658_set_synthesis_volume(syn6658_handle_t *handle, uint8_t volume)
{
    char cmd[8];

    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    if (volume > 10)                                                                             /* check volume */
    {
        handle->debug_print("syn6658: volume is invalid.\n");                                    /* volume is invalid */

        return 4;                                                                                /* return error */
    }

    memset((char *)cmd, 0, sizeof(char) * 8);                                                    /* memory set 0 */
    (void)snprintf((char *)cmd, 8, "[v%d]", (int16_t)volume);                                    /* set command */
    if (syn6658_set_command_with_arg(handle, 0x01, 0x00, cmd, SYN6658_BUSY_TIMEOUT) != 0)        /* write command */
    {
        return 1;                                                                                /* return error */
    }

    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief     set the synthesis speed
 * @param[in] *handle points to a syn6658 handle structure
 * @param[in] speed is the synthesis speed
 * @return    status code
 *            - 0 success
 *            - 1 set synthesis speed failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 speed is invalid
 * @note      0 <= speed <= 10
 */
uint8_t syn6658_set_synthesis_speed(syn6658_handle_t *handle, uint8_t speed)
{
    char cmd[8];

    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    if (speed > 10)                                                                              /* check volume */
    {
        handle->debug_print("syn6658: speed is invalid.\n");                                     /* speed is invalid */

        return 4;                                                                                /* return error */
    }

    memset((char *)cmd, 0, sizeof(char) * 8);                                                    /* memory set 0 */
    (void)snprintf((char *)cmd, 8, "[s%d]", (int16_t)speed);                                     /* set command */
    if (syn6658_set_command_with_arg(handle, 0x01, 0x00, cmd, SYN6658_BUSY_TIMEOUT) != 0)        /* write command */
    {
        return 1;                                                                                /* return error */
    }

    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief     set the synthesis speaker
 * @param[in] *handle points to a syn6658 handle structure
 * @param[in] speaker is the synthesis speaker
 * @return    status code
 *            - 0 success
 *            - 1 set synthesis speed failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t syn6658_set_synthesis_speaker(syn6658_handle_t *handle, syn6658_speaker_t speaker)
{
    char cmd[8];

    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }

    memset((char *)cmd, 0, sizeof(char) * 8);                                                    /* memory set 0 */
    (void)snprintf((char *)cmd, 8, "[m%d]", (int16_t)speaker);                                   /* set command */
    if (syn6658_set_command_with_arg(handle, 0x01, 0x00, cmd, SYN6658_BUSY_TIMEOUT) != 0)        /* write command */
    {
        return 1;                                                                                /* return error */
    }

    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief     save text to the buffer
 * @param[in] *handle points to a syn6658 handle structure
 * @param[in] offset is the buffer offset
 * @param[in] *text points to a text buffer
 * @return    status code
 *            - 0 success
 *            - 1 save text failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 offset is invalid
 *            - 5 text length is invalid
 *            - 6 spi interface can't use this function
 * @note      0 <= offset <= 15
 *            text length <= ((16 - offset) * 256)
 */
uint8_t syn6658_save_text(syn6658_handle_t *handle, uint8_t offset, char *text)
{
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    if (offset > 15)                                                                             /* check offset */
    {
        handle->debug_print("syn6658: offset is invalid, 0<= offset <= 15.\n");                  /* offset is invalid */

        return 4;                                                                                /* return error */
    }
    if ((uint16_t)strlen(text) > (uint16_t)(256 * (16 - offset)))                                /* check text length  */
    {
        handle->debug_print("syn6658: text length is invalid.\n");                               /* text length is invalid */

        return 5;                                                                                /* return error */
    }
    if (handle->uart_spi != 0)                                                                   /* check interface */
    {
        handle->debug_print("syn6658: spi interface can't use this function.\n");                /* spi interface can't use this function */

        return 6;                                                                                /* return error */
    }

    if (syn6658_set_command_with_arg(handle, 0x31, offset, text, SYN6658_BUSY_TIMEOUT) != 0)     /* write command */
    {
        return 1;                                                                                /* return error */
    }

    return 0;                                                                                    /* success return 0 */
}

/**
 * @brief     play text
 * @param[in] *handle points to a syn6658 handle structure
 * @param[in] times is the play times
 * @param[in] type is the encode type
 * @return    status code
 *            - 0 success
 *            - 1 play text failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 times is invalid
 *            - 5 spi interface can't use this function
 * @note      1 <= times <= 15
 */
uint8_t syn6658_play_text(syn6658_handle_t *handle, uint8_t times, syn6658_type_t type)
{
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    if ((times > 15) || (times < 1))                                                      /* check times */
    {
        handle->debug_print("syn6658: times is invalid, 1<= times <= 15.\n");             /* times is invalid */

        return 4;                                                                         /* return error */
    }
    if (handle->uart_spi != 0)                                                            /* check interface */
    {
        handle->debug_print("syn6658: spi interface can't use this function.\n");         /* spi interface can't use this function */

        return 5;                                                                         /* return error */
    }
    
    if (syn6658_set_command_with_arg(handle, 0x32, (times << 4) | (uint8_t)(type),
                                    (char *)"", SYN6658_BUSY_TIMEOUT) != 0)               /* write command */
    {
        return 1;                                                                         /* return error */
    }

    return 0;                                                                             /* success return 0 */
}

/**
 * @brief     send the command with arg to the chip
 * @param[in] *handle points to a syn6658 handle structure
 * @param[in] command is the set command
 * @param[in] param is the set param
 * @param[in] *txt points to a txt buffer
 * @param[in] timeout is the timeout in ms
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 txt is too long
 *            - 5 chip is busy
 *            - 6 uart read failed
 * @note      none
 */
uint8_t syn6658_set_command_with_arg(syn6658_handle_t *handle, uint8_t command, uint8_t param, char *txt, uint32_t timeout)
{
    uint8_t res;
    uint8_t temp;
    uint16_t l;
    uint16_t len;

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    len = (uint16_t)strlen(txt);                                          /* get length of txt */
    if (len > 4091)                                                       /* check result */
    {
        handle->debug_print("syn6658: txt is too long.\n");               /* txt is too long */

        return 4;                                                         /* return error */
    }
    handle->buf[0] = 0xFD;                                                /* frame header */
    handle->buf[1] = (uint8_t)((len + 2) / 256);                          /* length msb */
    handle->buf[2] = (len + 2) % 256;                                     /* length lsb */
    handle->buf[3] = command;                                             /* command */
    handle->buf[4] = param;                                               /* command param */
    strncpy((char *)&handle->buf[5], txt, len);                           /* copy command */
    res = a_check_busy(handle, timeout);                                  /* check busy */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("syn6658: chip is busy.\n");                  /* chip is busy */

        return 5;                                                         /* return error */
    }
    res = a_write(handle, handle->buf, len + 5);                          /* write data */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("syn6658: write failed.\n");                  /* write failed */

        return 1;                                                         /* return error */
    }
    if (handle->uart_spi == 0)                                            /* uart interface */
    {
        handle->delay_ms(100);                                            /* delay 100 ms */
    }
    l = a_read(handle, (uint8_t *)&temp, 1);                              /* read data */
    if (l != 1)                                                           /* check result */
    {
        handle->debug_print("syn6658: uart read failed.\n");              /* uart read failed */

        return 6;                                                         /* return error */
    }
    if (temp == 0x41)                                                     /* check return */
    {
        if (handle->uart_spi != 0)                                        /* spi interface */
        {
            handle->delay_ms(200);                                        /* delay 200 ms */
        }
        
        return 0;                                                         /* success return 0 */
    }
    else
    {
        handle->debug_print("syn6658: command receive failed.\n");        /* command receive failed */

        return 6;                                                         /* return error */
    }
}

/**
 * @brief     send the command to the chip
 * @param[in] *handle points to a syn6658 handle structure
 * @param[in] command is the set command
 * @param[in] timeout is the timeout in ms
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 *            - 5 uart read failed
 * @note      none
 */
uint8_t syn6658_set_command(syn6658_handle_t *handle, uint8_t command, uint32_t timeout)
{
    uint8_t res;
    uint8_t temp;
    uint16_t l;

    if (handle == NULL)                                                   /* check handle */
    {
        return 2;                                                         /* return error */
    }
    if (handle->inited != 1)                                              /* check handle initialization */
    {
        return 3;                                                         /* return error */
    }

    handle->buf[0] = 0xFD;                                                /* frame header */
    handle->buf[1] = 0x00;                                                /* length msb */
    handle->buf[2] = 0x01;                                                /* length lsb */
    handle->buf[3] = command;                                             /* command */
    res = a_check_busy(handle, timeout);                                  /* check busy */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("syn6658: chip is busy.\n");                  /* chip is busy */

        return 4;                                                         /* return error */
    }
    res = a_write(handle, handle->buf, 4);                                /* write data */
    if (res != 0)                                                         /* check result */
    {
        handle->debug_print("syn6658: write failed.\n");                  /* write failed */

        return 1;                                                         /* return error */
    }
    if (handle->uart_spi == 0)                                            /* uart interface */
    {
        handle->delay_ms(100);                                            /* delay 100 ms */
    }
    l = a_read(handle, (uint8_t *)&temp, 1);                              /* read data */
    if (l != 1)                                                           /* check result */
    {
        handle->debug_print("syn6658: uart read failed.\n");              /* uart read failed */

        return 5;                                                         /* return error */
    }
    if (temp == 0x41)                                                     /* check return */
    {
        if (handle->uart_spi != 0)                                        /* spi interface */
        {
            handle->delay_ms(200);                                        /* delay 200 ms */
        }
        
        return 0;                                                         /* success return 0 */
    }
    else
    {
        handle->debug_print("syn6658: command receive failed.\n");        /* command receive failed */

        return 5;                                                         /* return error */
    }
}

/**
 * @brief      get chip's information
 * @param[out] *info points to a syn6658 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t syn6658_info(syn6658_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }

    memset(info, 0, sizeof(syn6658_info_t));                        /* initialize syn6658 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "UART SPI", 16);                       /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */

    return 0;                                                       /* success return 0 */
}
