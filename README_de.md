[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver SYN6658

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/syn6658/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der chinesische Sprachsynthesechip SYN6658 ist ein High-End-Sprachsynthesechip mit höherer Kosteneffizienz und natürlicheren Effekten, der 2012 von Beijing Yuyin Tianxia Technology Co., Ltd. auf den Markt gebracht wurde. SYN6658 empfängt zu synthetisierende Textdaten über die UART- oder SPI-Schnittstellenkommunikation und erreicht so eine Text-to-Speech-Konvertierung (oder TTS-Sprache). Der neueste von der Firma auf den Markt gebrachte Sprachsynthesechip SYN6658 übernimmt die hervorragenden Eigenschaften der Sprachchips OSYNO6188 und SYN6288: kleines Patch-Gehäuse, einfache Hardwareschnittstelle, geringer Stromverbrauch, klarer und runder Ton und extrem hohe Kosteneffizienz; Darüber hinaus hat SYN6658 die Text-/Zahlen-/Zeichenfolgenerkennung intelligenter und genauer gemacht und die Natürlichkeit der Sprachsynthese wurde erheblich verbessert. SYN6658 hat einen qualitativen Sprung in der Sprachsyntheseleistung und -intelligenz erreicht und ist damit ein wirklich hochwertiger chinesischer Sprachsynthesechip für Industrieanwendungen.

LibDriver SYN6658 ist der voll funktionsfähige Treiber von SYN6658, der von LibDriver eingeführt wurde. Er bietet Textsynthese, Tonsynthese und andere Funktionen. LibDriver ist MISRA-kompatibel.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver SYN6658-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver SYN6658 UART, SPI.

/test enthält den Testcode des LibDriver SYN6658-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver SYN6658-Beispielcode.

/doc enthält das LibDriver SYN6658-Offlinedokument.

/Datenblatt enthält SYN6658-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige UART, SPI-Schnittstellenvorlage und stellen Sie Ihren Plattform-UART, SPI-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/syn6658/index.html](https://www.libdriver.com/docs/syn6658/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.