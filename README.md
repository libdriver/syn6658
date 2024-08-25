[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SYN6658

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/syn6658/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

SYN6658 Chinese speech synthesis chip is a high-end speech synthesis chip with higher cost-effectiveness and more natural effects, newly launched by Beijing Yuyin Tianxia Technology Co., Ltd. in 2012. SYN6658 receives text data to be synthesized through UART or SPI interface communication, achieving text to speech (or TTS speech) conversion. The latest SYN6658 speech synthesis chip launched by the company inherits the excellent features of OSYNO6188 and SYN6288 speech chips: small-sized patch packaging, simple hardware interface, low power consumption, clear and round tone, and extremely high cost-effectiveness; In addition, SYN6658 has made text/number/string recognition more intelligent and accurate, and the naturalness of speech synthesis has been greatly improved. SYN6658 has achieved a qualitative leap in speech synthesis performance and intelligence, making it a truly high-end Chinese speech synthesis chip for industry applications.

LibDriver SYN6658 is the full function driver of SYN6658 launched by LibDriver.It provides text synthesis, sound synthesis and other functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver SYN6658 source files.

/interface includes LibDriver SYN6658 UART, SPI platform independent template.

/test includes LibDriver SYN6658 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver SYN6658 sample code.

/doc includes LibDriver SYN6658 offline document.

/datasheet includes SYN6658 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface UART, SPI platform independent template and finish your platform UART, SPI driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_syn6658_basic.h"

uint8_t res;
char text[129] = "你好";
uint8_t text0[] = {0xB4, 0xCB, 0xB9, 0xA6, 0xC4, 0xDC, 0xBF, 0xC9, 0xD2, 0xD4, 0xBD, 0xAB, 0xB1, 0xBB, 0x00};
uint8_t text1[] = {0xB4, 0xF2, 0xC2, 0xD2, 0xCB, 0xB3, 0xD0, 0xF2, 0xB5, 0xC4, 0xD2, 0xBB, 0xB6, 0xCE, 0xCE, 0xC4, 0x00};
uint8_t text2[] = {0xB1, 0xBE, 0xB0, 0xB4, 0xBF, 0xCD, 0xBB, 0xA7, 0xCF, 0xA3, 0xCD, 0xFB, 0xB5, 0xC4, 0xCB, 0xB3, 0xD0, 0xF2, 0xB7, 0xD6, 0x00};
uint8_t text3[] = {0xB6, 0xCE, 0xBB, 0xBA, 0xB4, 0xE6, 0xBA, 0xF3, 0xD2, 0xBB, 0xC6, 0xF0, 0xB2, 0xA5, 0xB7, 0xC5, 0xA1, 0xA3, 0x00};

/* basic init */
res = syn6658_basic_init(SYN6658_INTERFACE_UART);
if (res != 0)
{
    return 1;
}

...
    
/* delay 500ms */
syn6658_interface_delay_ms(500);

/* syn */
res = syn6658_basic_synthesis(text);
if (res != 0)
{
    (void)syn6658_basic_deinit();

    return 1;
}
(void)syn6658_basic_sync();

...

/* sound test */
res = syn6658_synthesis_text(&gs_handle, (char const *)SYN6658_SOUND_202);
if (res != 0)
{
    (void)syn6658_deinit(&gs_handle);

    return 1;
}
(void)syn6658_basic_sync();

...

/* text test */
res = syn6658_basic_save_text(0, text0);
if (res != 0)
{
    (void)syn6658_deinit(&gs_handle);

    return 1;
}
res = syn6658_basic_save_text(1, text1);
if (res != 0)
{
    (void)syn6658_deinit(&gs_handle);

    return 1;
}
res = syn6658_basic_save_text(2, text2);
if (res != 0)
{
    (void)syn6658_deinit(&gs_handle);

    return 1;
}
res = syn6658_basic_save_text(3, text3);
if (res != 0)
{
    (void)syn6658_deinit(&gs_handle);

    return 1;
}
res = syn6658_basic_play_text(1, SYN6658_TYPE_GB2312);
if (res != 0)
{
    (void)syn6658_deinit(&gs_handle);

    return 1;
}

...
    
/* basic deinit */
(void)syn6658_basic_deinit();
```

### Document

Online documents: [https://www.libdriver.com/docs/syn6658/index.html](https://www.libdriver.com/docs/syn6658/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.