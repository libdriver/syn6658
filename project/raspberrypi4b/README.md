### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

UART Pin: TX/RX GPIO14/GPIO15.

SPI Pin: SCLK/MOSI/MISO/CS GPIO11/GPIO10/GPIO9/GPIO8.

READY Pin: GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```
#### 2.2 Configuration

Enable serial port.

Disable serial console.

#### 2.3 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.4 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(syn6658 REQUIRED)
```

#### 2.5 Command Problem

Each command will sent a sleep command to the chip, but raspberry can't run wake up command, so you should power down and power on SYN6658 every time before your any new command.

### 3. SYN6658

#### 3.1 Command Instruction

1. Show syn6658 chip and driver information.

   ```shell
   syn6658 (-i | --information)
   ```

2. Show syn6658 help.

   ```shell
   syn6658 (-h | --help)
   ```

3. Show syn6658 pin connections of the current board.

   ```shell
   syn6658 (-p | --port)
   ```

5. Run syn6658 synthesis test.

   ```shell
   syn6658 (-t syn | --test=syn) --interface=<uart | spi> 
   ```

6. Run syn6658 synthesis text function, gb2312 format is gb2312.

   ```shell
   syn6658 (-e syn | --example=syn) --interface=<uart | spi> --text=<gb2312>
   ```

7. Run syn6658 save text function, index is the saved index and it's range is 0 - 15, gb2312 format is gb2312.

   ```shell
   syn6658 (-e save| --example=save) --interface=<uart | spi> --index=<index> --text=<gb2312>
   ```

7. Run syn6658 play text function, times is the play times and it's range is 1 - 15.

   ```shell
   syn6658 (-e play | --example=play) --interface=<uart | spi> --times=<times>
   ```

#### 3.2 Command Example

```shell
./syn6658 -i

syn6658: chip is Voicetx SYN6658.
syn6658: manufacturer is Voicetx.
syn6658: interface is UART SPI.
syn6658: driver version is 1.0.
syn6658: min supply voltage is 3.1V.
syn6658: max supply voltage is 4.5V.
syn6658: max current is 53.00mA.
syn6658: max temperature is 85.0C.
syn6658: min temperature is -40.0C.
```

```shell
./syn6658 -p

syn6658: TX connected to GPIO15(BCM).
syn6658: RX connected to GPIO14(BCM).
syn6658: SCK connected to GPIO11(BCM).
syn6658: MISO connected to GPIO9(BCM).
syn6658: MOSI connected to GPIO10(BCM).
syn6658: READY connected to GPIO17(BCM).
```

```shell
./syn6658 -t syn --interface=uart

syn6658: chip is Voicetx SYN6658.
syn6658: manufacturer is Voicetx.
syn6658: interface is UART SPI.
syn6658: driver version is 1.0.
syn6658: min supply voltage is 3.1V.
syn6658: max supply voltage is 4.5V.
syn6658: max current is 53.00mA.
syn6658: max temperature is 85.0C.
syn6658: min temperature is -40.0C.
syn6658: synthesis text test.
syn6658: gb2312 synthesis text.
syn6658: gbk synthesis text.
syn6658: big5 synthesis text.
syn6658: unicode little endian synthesis text.
syn6658: unicode big endian synthesis text.
syn6658: sound synthesis text.
syn6658: message synthesis text.
syn6658: synthesis control test.
syn6658: synthesis control pause.
syn6658: synthesis control resume.
syn6658: synthesis control stop.
syn6658: synthesis volume test.
syn6658: set synthesis volume 5.
syn6658: synthesis speed test.
syn6658: set synthesis speed 10.
syn6658: synthesis speaker test.
syn6658: set synthesis speaker xiaoling.
syn6658: set synthesis speaker yixiaojian.
syn6658: set synthesis speaker yixiaoqiang.
syn6658: set synthesis speaker tianpeipei.
syn6658: set synthesis speaker tanglaoya.
syn6658: set synthesis speaker xiaoyanzi.
syn6658: command test.
syn6658: set command 0.
syn6658: set command 1.
syn6658: save and play text test.
syn6658: save text to buffer 3.
syn6658: save text to buffer 1.
syn6658: save text to buffer 0.
syn6658: save text to buffer 2.
syn6658: play text test.
syn6658: syn6658 standby test.
syn6658: standby ok.
syn6658: syn6658 wake up test.
syn6658: wake up ok.
syn6658: finish synthesis test.
```

```shell
./syn6658 -e syn --interface=uart --text=你好

syn6658: synthesis 你好.
```

```shell
./syn6658 -e save --interface=uart --index=0 --text=开始

syn6658: save index 0 text 开始.

./syn6658 -e save --interface=uart --index=1 --text=播放

syn6658: save index 1 text 播放.

./syn6658 -e save --interface=uart --index=2 --text=测试

syn6658: save index 2 text 测试.

./syn6658 -e save --interface=uart --index=3 --text=语音

syn6658: save index 3 text 语音.
```

```shell
./syn6658 -e play --interface=uart --times=3

syn6658: play 3 times.
```

```shell
./syn6658 -h

Usage:
  syn6658 (-i | --information)
  syn6658 (-h | --help)
  syn6658 (-p | --port)
  syn6658 (-t syn | --test=syn) --interface=<uart | spi>
  syn6658 (-e syn | --example=syn) --interface=<uart | spi> --text=<gb2312>
  syn6658 (-e save| --example=save) --interface=<uart | spi> --index=<index> --text=<gb2312>
  syn6658 (-e play | --example=play) --interface=<uart | spi> --times=<times>

Options:
  -e <syn | save | play>, --example=<syn | save | play>
                          Run the driver example.
  -h, --help              Show the help.
  -i, --information       Show the chip information.
      --interface=<uart | spi>
                          Set the chip interface.([default: uart])
      --index=<index>     Set saved index it's rang is 0 - 15.([default: 0])
  -p, --port              Display the pin connections of the current board.
  -t <syn>, --test=<syn>  Run the driver test.
      --times=<times>     Set the play times and it's rang is 1 - 15.([default: 1])
      --text=<gb2312>     Set the synthesis text and it's format is GB2312.
```

