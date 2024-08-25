[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SYN6658

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/syn6658/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

SYN6658中国語音声合成チップは、北京毓音天下科技有限公司が2012年に新たに発売した、コストパフォーマンスが高く、より自然な効果を持つハイエンドの音声合成チップです。SYN6658は、UARTまたはSPIインターフェース通信を介して合成するテキストデータを受信し、テキストから音声（またはTTS音声）への変換を実現します。同社が発売した最新のSYN6658音声合成チップは、OSYNO6188およびSYN6288音声チップの優れた機能を継承しています。パッチパッケージが小型、ハードウェアインターフェースがシンプル、消費電力が低い、トーンが明瞭で丸みがあり、コストパフォーマンスが非常に高いです。さらに、SYN6658はテキスト/数字/文字列の認識をよりインテリジェントかつ正確にし、音声合成の自然さが大幅に向上しました。SYN6658は、音声合成のパフォーマンスとインテリジェンスにおいて質的な飛躍を達成し、業界アプリケーション向けの真にハイエンドの中国語音声合成チップとなっています。

LibDriver SYN6658 は、LibDriver がリリースした SYN6658 のフル機能ドライバです。テキスト合成、サウンド合成などの機能を提供します。LibDriver は MISRA に準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver SYN6658 のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver SYN6658 用のプラットフォームに依存しないUART, SPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver SYN6658 ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver SYN6658 プログラミング例が含まれています。

/ docディレクトリには、LibDriver SYN6658 オフラインドキュメントが含まれています。

/ datasheetディレクトリには、SYN6658 データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないUART, SPIバステンプレートを参照して、指定したプラットフォームのUART, SPIバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/syn6658/index.html](https://www.libdriver.com/docs/syn6658/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。