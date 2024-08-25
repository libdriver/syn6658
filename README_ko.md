[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SYN6658

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/syn6658/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

SYN6658 중국어 음성 합성 칩은 베이징 위인 톈샤 테크놀로지 주식회사가 2012년에 새롭게 출시한 하이엔드 음성 합성 칩으로, 비용 효율성이 높고 효과가 더 자연스럽습니다. SYN6658은 UART 또는 SPI 인터페이스 통신을 통해 합성할 텍스트 데이터를 수신하여 텍스트 음성(또는 TTS 음성) 변환을 실현합니다. 회사에서 출시한 최신 SYN6658 음성 합성 칩은 OSYNO6188 및 SYN6288 음성 칩의 뛰어난 기능을 계승했습니다. 즉, 소형 패치 패키징, 간단한 하드웨어 인터페이스, 낮은 전력 소모, 선명하고 둥근 톤, 매우 높은 비용 효율성입니다. 또한 SYN6658은 텍스트/숫자/문자열 인식을 보다 지능적이고 정확하게 만들었고 음성 합성의 자연스러움이 크게 향상되었습니다. SYN6658은 음성 합성 성능과 지능에서 질적 도약을 달성하여 산업 애플리케이션을 위한 진정한 하이엔드 중국어 음성 합성 칩이 되었습니다.

LibDriver SYN6658은 LibDriver가 출시한 SYN6658의 전체 기능 드라이버입니다. 텍스트 합성, 사운드 합성 및 기타 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver SYN6658의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver SYN6658용 플랫폼 독립적인 UART, SPI버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver SYN6658드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver SYN6658프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver SYN6658오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 SYN6658데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인UART, SPI버스 템플릿을 참조하여 지정된 플랫폼에 대한 UART, SPI버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/syn6658/index.html](https://www.libdriver.com/docs/syn6658/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.