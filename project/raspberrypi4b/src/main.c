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
 * @file      main.c
 * @brief     main source file
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
#include "driver_syn6658_synthesis_test.h"
#include <getopt.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief     syn6658 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t syn6658(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"text", required_argument, NULL, 1},
        {"index", required_argument, NULL, 2},
        {"times", required_argument, NULL, 3},
        {"interface", required_argument, NULL, 4},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint8_t text_flag = 0;
    uint8_t index = 0;
    uint8_t times = 1;
    char text[129] = {0};
    syn6658_interface_t interface = SYN6658_INTERFACE_UART;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* text */
            case 1 :
            {
                /* set the test */
                memset(text, 0, sizeof(char) * 129);
                strncpy(text, optarg, 128);
                text_flag = 1;
                
                break;
            }
            
            /* index */
            case 2 :
            {
                /* set the index */
                index = atoi(optarg);
                
                break;
            }
            
            /* times */
            case 3 :
            {
                /* set the times */
                times = atoi(optarg);
                
                break;
            }
            
            /* interface */
            case 4 :
            {
                if (strcmp("uart", optarg) == 0)
                {
                    interface = SYN6658_INTERFACE_UART;
                }
                else if (strcmp("spi", optarg) == 0)
                {
                    interface = SYN6658_INTERFACE_SPI;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_syn", type) == 0)
    {
        /* run syn test */
        if (syn6658_synthesis_test(interface) != 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (strcmp("e_syn", type) == 0)
    {
        uint8_t res;
        
        /* check the flag */
        if (text_flag != 1)
        {
            return 5;
        }
        
        /* basic init */
        res = syn6658_basic_init(interface);
        if (res != 0)
        {
            return 1;
        }
        
        /* delay 500ms */
        syn6658_interface_delay_ms(500);
        
        /* output */
        syn6658_interface_debug_print("syn6658: synthesis %s.\n", text);
        
        /* syn */
        res = syn6658_basic_synthesis(text);
        if (res != 0)
        {
            (void)syn6658_basic_deinit();
            
            return 1;
        }
        
        /* basic deinit */
        (void)syn6658_basic_sync();
        (void)syn6658_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_save", type) == 0)
    {
        uint8_t res;
        
        /* check the flag */
        if (text_flag != 1)
        {
            return 5;
        }
        
        /* basic init */
        res = syn6658_basic_init(interface);
        if (res != 0)
        {
            return 1;
        }
        
        /* delay 500ms */
        syn6658_interface_delay_ms(500);
        
        /* output */
        syn6658_interface_debug_print("syn6658: save index %d text %s.\n", index, text);
        
        /* save text */
        res = syn6658_basic_save_text(index, text);
        if (res != 0)
        {
            (void)syn6658_basic_deinit();
            
            return 1;
        }
        
        /* basic deinit */
        (void)syn6658_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_play", type) == 0)
    {
        uint8_t res;
        
        /* basic init */
        res = syn6658_basic_init(interface);
        if (res != 0)
        {
            return 1;
        }
        
        /* delay 500ms */
        syn6658_interface_delay_ms(500);
        
        /* output */
        syn6658_interface_debug_print("syn6658: play %d times.\n", times);
        
        /* play text */
        res = syn6658_basic_play_text(times, SYN6658_TYPE_GB2312);
        if (res != 0)
        {
            (void)syn6658_basic_deinit();
            
            return 1;
        }
        
        /* basic deinit */
        (void)syn6658_basic_sync();
        (void)syn6658_basic_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        syn6658_interface_debug_print("Usage:\n");
        syn6658_interface_debug_print("  syn6658 (-i | --information)\n");
        syn6658_interface_debug_print("  syn6658 (-h | --help)\n");
        syn6658_interface_debug_print("  syn6658 (-p | --port)\n");
        syn6658_interface_debug_print("  syn6658 (-t syn | --test=syn) --interface=<uart | spi>\n");
        syn6658_interface_debug_print("  syn6658 (-e syn | --example=syn) --interface=<uart | spi> --text=<gb2312>\n");
        syn6658_interface_debug_print("  syn6658 (-e save| --example=save) --interface=<uart | spi> --index=<index> --text=<gb2312>\n");
        syn6658_interface_debug_print("  syn6658 (-e play | --example=play) --interface=<uart | spi> --times=<times>\n");
        syn6658_interface_debug_print("\n");
        syn6658_interface_debug_print("Options:\n");
        syn6658_interface_debug_print("  -e <syn | save | play>, --example=<syn | save | play>\n");
        syn6658_interface_debug_print("                          Run the driver example.\n");
        syn6658_interface_debug_print("  -h, --help              Show the help.\n");
        syn6658_interface_debug_print("  -i, --information       Show the chip information.\n");
        syn6658_interface_debug_print("      --interface=<uart | spi>\n");
        syn6658_interface_debug_print("                          Set the chip interface.([default: uart])\n");
        syn6658_interface_debug_print("      --index=<index>     Set saved index it's rang is 0 - 15.([default: 0])\n");
        syn6658_interface_debug_print("  -p, --port              Display the pin connections of the current board.\n");
        syn6658_interface_debug_print("  -t <syn>, --test=<syn>  Run the driver test.\n");
        syn6658_interface_debug_print("      --times=<times>     Set the play times and it's rang is 1 - 15.([default: 1])\n");
        syn6658_interface_debug_print("      --text=<gb2312>     Set the synthesis text and it's format is GB2312.\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        syn6658_info_t info;
        
        /* print syn6658 info */
        syn6658_info(&info);
        syn6658_interface_debug_print("syn6658: chip is %s.\n", info.chip_name);
        syn6658_interface_debug_print("syn6658: manufacturer is %s.\n", info.manufacturer_name);
        syn6658_interface_debug_print("syn6658: interface is %s.\n", info.interface);
        syn6658_interface_debug_print("syn6658: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        syn6658_interface_debug_print("syn6658: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        syn6658_interface_debug_print("syn6658: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        syn6658_interface_debug_print("syn6658: max current is %0.2fmA.\n", info.max_current_ma);
        syn6658_interface_debug_print("syn6658: max temperature is %0.1fC.\n", info.temperature_max);
        syn6658_interface_debug_print("syn6658: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        syn6658_interface_debug_print("syn6658: TX connected to GPIO15(BCM).\n");
        syn6658_interface_debug_print("syn6658: RX connected to GPIO14(BCM).\n");
        syn6658_interface_debug_print("syn6658: SCK connected to GPIO11(BCM).\n");
        syn6658_interface_debug_print("syn6658: MISO connected to GPIO9(BCM).\n");
        syn6658_interface_debug_print("syn6658: MOSI connected to GPIO10(BCM).\n");
        syn6658_interface_debug_print("syn6658: CS connected to GPIO8(BCM).\n");
        syn6658_interface_debug_print("syn6658: READY connected to GPIO17(BCM).\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = syn6658(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        syn6658_interface_debug_print("syn6658: run failed.\n");
    }
    else if (res == 5)
    {
        syn6658_interface_debug_print("syn6658: param is invalid.\n");
    }
    else
    {
        syn6658_interface_debug_print("syn6658: unknown status code.\n");
    }

    return 0;
}
