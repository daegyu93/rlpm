
# RLPM FIFO IP core

## Description, general use cases

RLPM(Realtime Laser Pulse Monitoring) module:

## Generic arhitecture

레이져 펄스의 실시간 감시를 위해서 고속 ADC를 사용하여 ns 단위로 레이져 펄스를 
Capture하고, Capture된 ADC Data를 저장하는 FIFO 이다. 저장된 ADC Data는 DMA를 통하여
CPU로 전달 된다. Analog device의 data_offload IP를 이에 맞게 수정하였다.

RLPM mode에 따라 레이져 펄스의 waveform을 그대로 전송할 수도 있고, 또는 Fmax, FWHM 계산
결과만 DMA를 통하여 CPU로 전송할 수도 있다.

## Parameters

|      NAME            |   TYPE      |  DEFAULT   |            DESCRIPTION      |
|----------------------|:-----------:|:----------:|:---------------------------:|
|ID                    |  integer    |    0       | Instance ID number | 
|MEM_TYPE              |  [ 0:0]     |    0       | Define the used storage type: FPGA RAM - 0; external DDR - 1 |
|MEM_SIZE              |  [31:0]     |   1024     | Define the size of the storage element |
|MEMC_UIF_DATA_WIDTH   |  [ 0:0]     |   512      | The valid data depends on the DDRx memory controller IP. |
|TX_OR_RXN_PATH        |  [ 0:0]     |    1       | If set TX path enabled, otherwise RX |
|SRC_DATA_WIDTH        |  integer    |    64      | The data width of the source interface |
|SRC_RAW_DATA_EN       |  [ 0:0]     |     0      | Enable if the data path does extend samples to 16 bits | 
|SRC_ADDR_WIDTH        |  integer    |    8       | The address width of the source interface, should be defined relative to the MEM_SIZE (MEM_SIZE/SRC_DATA_WIDTH/8) |
|DST_ADDR_WIDTH        |  integer    |    7       | The address width of the source interface, should be defined relative to the MEM_SIZE (MEM_SIZE/DST_DATA_WIDTH/8) |
|DST_DATA_WIDTH        |  integer    |    64      | The data width of the destination interface |
|DST_RAW_DATA_EN       |  [ 0:0]     |     0      | Enable if the data path does extend samples to 16 bits | 
|DST_CYCLIC_EN         |  [ 0:0]     |     0      | Enables CYCLIC mode for destinations like DAC | 
|AUTO_BRINUP           |  [ 0:0]     |     0      | If enabled the IP runs automatically after bootup | 

## Interfaces

### AXI4 Lite Memory Mapped Slave (S_AXI4_LITE)

This interface is used to access the register map.

```verilog
// interface clock -- system clock -- 100 MHz
input                   s_axi_aclk
// interface resetn -- synchronous reset active low
input                   s_axi_aresetn

/* write address channel */

// validates the address on the bus
input                   s_axi_awvalid
// write address
input       [15:0]      s_axi_awaddr
// protection type -- not used in the core
input       [ 2:0]      s_axi_awprot
// write ready, indicates that the slave can accept the address
output                  s_axi_awready

/* write data channel */

// validate the data on the bus
input                   s_axi_wvalid
// write data
input       [31:0]      s_axi_wdata
// write strobe, indicates which byte lanes to update
input       [ 3:0]      s_axi_wstrb
// write ready, indicates that the slave can accept the data
output                  s_axi_wready

/* write response channel */

// validates the write response of the slave
output                  s_axi_bvalid
// write response, indicate the status of the transfer
output      [ 1:0]      s_axi_bresp
// response ready, indicates that the master can accept the data
input                   s_axi_bready

/* read address channel */

// validates the address on the bus
input                   s_axi_arvalid
// read address
input       [15:0]      s_axi_araddr
// protection type -- not used in the core
input       [ 2:0]      s_axi_arprot
// read ready, indicates that the slave can accept the address
output                  s_axi_arready

/* read data channel */

// validate the data on the bus
output                  s_axi_rvalid
// read response, indicate the status of the transfer
output      [ 1:0]      s_axi_rresp
// read data drivers by the slave
output      [31:0]      s_axi_rdata
// read ready, indicates that the master can accept the data
input                   s_axi_rready
```

### Supported data interfaces

**NOTE**: To simplify the design both the source and destination data interface is
an AXI4 streaming interface. A FIFO write (ADC) interface can be treated as AXI4
stream where only the master controles the data rate (s_axis_ready is always asserted). 

#### AXI4 Stream interface (S_AXIS | M_AXIS)

* The AXI Stream Slave (S_AXIS) interface is used to receive AXI stream from
the transmit DMA or ADC device.

* The AXI Stream Master (M_AXIS) interface is used to transmit AXI stream
to receive DMA or DAC device

```verilog
// NOTE: this reference is a master interface

// interface clock -- can be device/core clock or DMA clock
input                        m_axis_aclk
// interface resetn -- synchronous reset with the system clock
input                        m_axis_resetn
// indicates that the slave can accept a transfer in the current cycle (in case of an ADC core, this will control the stream)
input                        m_axis_ready
// indicates that the master is driving a valid transfer
output                       m_axis_valid
// primary payload
output [DATA_WIDTH-1:0]      m_axis_data
// indicates the boundary of a packet
output                       m_axis_last
// byte qualifier, we need this so we can have different DMA and device data widths
output [(DATA_WIDTH/8)-1:0]  m_axis_tkeep
```

**NOTE**: A packet will always be a full buffer. All the data beats going to be
full beats (all the bytes of the bus are valid), except the last one. **axis_last**
and **axis_tkeep** will be used to indicate a partial last beat. This information
should be transferred from the source domain to the sink domain, so we can read
back the data from memory correctly.

#### FIFO source and destination interface to the storage unit

This is non-blocking (no back-pressure) interface for the storage unit,
having an address bus too, so an ad_mem module can be connected directly to controller IP.

```verilog
// This is a FIFO source interface - it's clocked on the source clock (s_axis_aclk)
// Reset signal
output                         fifo_src_resetn
// write enable signal
output                         fifo_src_wen
// address bus for internal memory
output   [SRC_ADDR_WIDTH-1:0]  fifo_src_waddr
// source data bus
output   [SRC_DATA_WIDTH-1:0]  fifo_src_wdata
// write last, indicates the last valid transfer
output                         fifo_src_wlast
```

```verilog
// This is a FIFO destination interface - it's clocked on the destination clock (m_axis_aclk)
// Reset signal
output                         fifo_dst_resetn
// read enable signal
output                         fifo_dst_ren
// indicate if the storage is ready to accept read requests
output                         fifo_dst_ready,
// address bus for internal memory
output   [DST_ADDR_WIDTH-1:0]  fifo_dst_raddr
// destination data bus
output   [DST_DATA_WIDTH-1:0]  fifo_dst_rdata
```

```verilog
// This is a Slave FIFO Read interface
// device digital interface clock, or core clock
input                     fifo_rd_clk
// enables the channel --  in our case this is redundant -- maybe we do neet to use it at all
input                     fifo_rd_enable
// validates the data on the bus, it's driven by the device indicates when the core latches the data
input                     fifo_rd_valid
// primary payload, its data width is equal with the channel's data width
output  [DATA_WIDTH-1:0]  fifo_rd_data
// indicates an underflow, the source (offload FIFO in this case) can not produce the data fast enough
output                    fifo_rd_unf
```

###  Initialization request interface

 - `TX_OR_RXN_PATH`의 경우 RLPM에서는 RX path로만 사용하는 것으로 한다.
 - `MEMORY_TYPE`의 경우 RLPM에서는 Embedded(FPGA BRAM)만 사용하는 것으로 한다. FIFO memory size는 현재 1Mbyte로 되어 있음
 - `RLPM_MODE`를 통하여 waveform mode과 calculation mode를 선택한다.
 - calculation mode시 전송되는 데이터는 각각 16bits이고, 전체 data size는 128bits, 그 종류는 아래와 같다.
 - {`GATE_COUNT`, `LM_COUNT`, `FMAX_LATENCY_NS`, `FMAX_VALUE_LSB`, `FWHM_COUNT`, `FWHM_DUTY`, `FWHM_2ND_NS`, `FWHM_1ST_NS`}
 - RLPM ADC 보드에는 2ch 입력이 있고, `RLPM_CH`를 통해서 Fmax, FWHM 계산을 위한 ADC channel을 선택한다. yu7777
 - !! todo :: 현재 waveform 모드 시, `RLPM_CH` 설정과 상관없이 2ch의 ADC data가 모두 전송되고 있음. 향후 선택된 채널만 전송할 수 있도록 한다.
 - `SYNC_CONFIG`를 통하여 레이져 펄스를 다양한 방법으로 저장/전송 할 수 있다.
 - `FWHM_COUNT` Fmax의 half 값을 가지는 ADC data의 갯 수. 정상적인 레이져 펄스는 항상 2개의 값을 가지고 있어야 함.
 - `FWHM_DUTY` 첫번째 Fmax half 값과 두번째 Fmax half 값의 시간차이. `FWHM_COUNT`가 2보다 크다면 `FWHM_DUTY`값이 정상이 아닐 수 있음

#### RLPM Config

`RLPM_MODE`를 통하여 크게 Waveform 과 Calculation mode로 나눌 수 있다. !! waveform data의 처음 32bits는 Gate count와 LM count 값이 채워진다.
 - '0' - Waveform mode : 레이져 펄스의 모든 데이터를 DMA를 통하여 CPU로 전송할 수 있다.
 - '1' - Calculation mode : 레이져 펄스에서 Fmax, FWHM의 계산 결과만 DMA를 통하여 전송한다.

`RLPM_CH` : '0' - ADC channel 0, '1' - ADC channel 1

#### Calculation Error 설정

 - `FMAX_VALUE_LSB_MIN`, `FMAX_VALUE_LSB_MAX` 를 통하여 `FMAX_VALUE_LSB`의 min/max를 설정
 - `FMAX_LATENCY_NS_MIN`, `FMAX_LATENCY_NS_MAX` 를 통하여 `FMAX_LATENCY_NS`의 min/max를 설정
 - `FWHM_DUTY_NS_MIN`, `FWHM_DUTY_NS_MAX` 를 통하여 `FWHM_DUTY`의 min/max를 설정

#### SYNC Configuration

레이져 펄스의 저장 및 DMA 전송은 아래와 같은 4가지 `SYNC_CONFIG`로 동작한다.

 - Sync None : 어떤 sync 동작 없이, 설정된 `WAVE_TRANSFER_LENGTH` 만큼 저장, 전송한다.
 - Sync LM : LM의 rising edge에 동기하여, `WAVE_TRANSFER_LENGTH` 만큼 레이져 펄스 저장, 전송한다.
 - Sync Calc : LM의 rising edge에 동기하여, `WAVE_TRANSFER_LENGTH` 만큼 레이져 펄스 저장하고, DMA로의 전송 여부는 Calculation error가 있을 경우에만 전송한다.
 - Sync SW : 쓸일이 있을지 모르겠지만, `SYNC_TRIGGER` 설정에 의하여, `WAVE_TRANSFER_LENGTH` 만큼 저장, 전송한다.

#### Transfer Length

FIFO로 저장할 Data size와 DMA로 전송할 Data size 를 64bytes 단위로 설정한다. 따라서 최소 단위는 64bytes 이다. ADC Data가 16bits 이므로 laser pulse 2ns 당 2bytes가 필요하다. (2ns 당 하나의 데이터를 모을수 있음)
 - Waveform mode에서는 `WAVE_TRANSFER_LENGTH` 저장/전송 한다. 약 2us laser pulse (2ns * 1024)를 저장/전송한다면, 1024개 * 2 adc data bytes * 2 adc channel = 4096bytes / 64bytes = 64가 된다.
 - Calculatoin mode에서는 `WAVE_TRANSFER_LENGTH` 저장할 laser pulse의 길이를 설정한다. `CALC_TRANSFER_LENGTH`는 얼마만큼의 계산결과를 모아서 전송할 것인지를 설정하면 된다. 예를 들어서, 256 laser pulse의 계산결과를 모아서 전송한다면, 256pulse * (128bits / 8bits) = 4096 bytes / 64bytes = 64가 된다. 만일 100khz의 laser pulse라면 10us(100khz) * 256pulse = 2.56ms 마다 DMA 인터럽트가 발생하도록 할 수 있다.

 - `WAVE_TRANSFER_LENGTH` : 내부 메모리로 저장할 source data size. LM edge부터 저장할 laser pulse의 길이를 설정, 최소 64bytes 단위, waveform mode에서는 `WAVE_TRANSFER_LENGTH` 사이즈 만큼 저장/전송 한다.
 - `CALC_TRANSFER_LENGTH` : DMA로 전송할 dstination data size. 최소 64bytes 단위, calculration mode에서는 `WAVE_TRANSFER_LENGTH`만큼 저장, `CALC_TRANSFER_LENGTH` 만큼 전송한다.
 
## Register Map

|  WORD  |  BYTE    |   BITS   |    NAME               | TYPE  | CLOCK DOMAIN |       DESCRIPTION       |
|-------:|:--------:|:--------:|:---------------------:|:-----:|:------------:|:-----------------------:|
| 0x0000 |  0x0000  |          | `VERSION`             |  RO   |  SYS         |  Version number         |
|        |          | [31:16]  | `MAJOR`               |       |              |                         |
|        |          | [15: 8]  | `MINOR`               |       |              |                         |
|        |          | [ 7: 0]  | `PATCH`               |       |              |                         |
| 0x0001 |  0x0004  |          | `PERIPHERAL_ID`       |  RO   |  SYS         |  Value of the IP configuration parameter |
| 0x0002 |  0x0008  |          | `SCRATCH`             |  RW   |  SYS         |  Scratch register !! 무엇에 사용되는 것인지 모르겠음 |
| 0x0003 |  0x000C  |          | `IDENTIFICATION`      |  RO   |  SYS         |  Peripheral identification. Default value: 0x524c504d - ('R','L','P','M') |
| 0x0004 |  0x0010  |          | `SYNTHESIS_CONFIG`    |  RO   |  SYS         |  Core configuration registers |
|        |          | [ 1: 1]  | `TX_OR_RXN_PATH`      |       |              |  RX Path => 0, TX => 1  |
|        |          | [ 0: 0]  | `MEMORY_TYPE`         |       |              |  The used storage type (embedded => 0 or external => 1) |
| 0x0005 |  0x0014  |          | `MEMORY_SIZE_LSB`     |  RO   |  SYS         |  32bits LSB of the memory size register |
| 0x0006 |  0x0018  |          | `MEMORY_SIZE_MSB`     |  RO   |  SYS         |  2bits MSB of the memory size register |
|        |          | [ 1: 0]  | `MEMORY_SIZE_MSB`     |       |              |  FIFO Memory szie       |
| 0x0007 |  0x001C  |          | `WAVE_TRANSFER_LENGTH` |  RW   |  SRC        |  Waveform load length |
| 0x0008 |  0x0020  |          | `CALC_TRANSFER_LENGTH` |  RW   |  DST        |  Calculatoin result Transfer length |
| 0x0009 |  0x0024  |          | `RLPM_CONFIG`         |  RW   |  SRC/DST     |  RLPM Configuration   |
|        |          | [ 1: 1]  | `RLPM_CH`             |       |              |  RLPM IP에 적용될 ADC channel number |
|        |          | [ 0: 0]  | `RLPM_MODE`           |       |              |  0 - Waveform mode, 1 - Calculation mode |
| 0x0020 |  0x0080  |          | `MEM_PHY_STATE`       |  RO   |  DDR         |  Status bits of the memory controller IP !! 나중에 삭제예정 |
|        |          | [ 0: 0]  | `CALIB_COMPLETE`      |       |              |  Indicates that the memory initialization and calibration have completed successfully |
| 0x0021 |  0x0084  |          | `RESETN_OFFLOAD`      |  RW   |  SRC/DST     |  Reset all the internal address registers and state machines |
|        |          | [ 0: 0]  | `RESETN`              |       |              |                         |
| 0x0022 |  0x0088  |          | `CONTROL`             |  RW   |  DST         |  A global control register !! 나중에 삭제예정 |
|        |          | [ 1: 1]  | `ONESHOT_EN`          |       |              |  By default the TX path runs on CYCLIC mode, set this bit to switch it to ONE-SHOT mode |
|        |          | [ 0: 0]  | `OFFLOAD_BYPASS`      |       |              |  Bypass the offload storage, the data path consist just of a CDC FIFO |
| 0x0040 |  0x0100  |          | `SYNC_TRIGGER`        |  RW1C |  SRC/DST     |  Synchronization by software |
|        |          | [ 0: 0]  | `SYNC_TRIGGER`        |       |              |  Sync Trigger           |
| 0x0041 |  0x0104  |          | `SYNC_CONFIG`         |  RW   |  SRC/DST     |  Synchronization setup  |
|        |          | [ 1: 0]  | `SYNC_CONFIG`         |       |              |  None - '0'; LM - '1'; Calculation - '2'; Software - '3' |
| 0x0080 |  0x0200  |          | `FSM_DBG`             |  RO   |              |  Debug register for the offload FSM |
|        |          | [ 5: 4]  | `FSM_STATE_READ`      |       |  SRC         |  The current state of the read-offload state machine |
|        |          | [ 1: 0]  | `FSM_STATE_WRITE`     |       |  DST         |  The current state of the write-offload state machine |
| 0x0081 |  0x0204  |          | `SAMPLE_COUNT_LSB`    |  RO   |  SRC         |  Stored sample count (32 LSB) |
| 0x0082 |  0x0208  |          | `SAMPLE_COUNT_MSB`    |  RO   |  SRC         |  Stored sample count (32 MSB) |
| 0x0090 |  0x0240  |          | `LM_COUNT`            |  RW   |  SRC         |  LM pulse 갯 수         |
| 0x0091 |  0x0244  |          | `GATE_COUNT`          |  RW   |  SRC         |  GATE pulse 갯 수       |
| 0x0092 |  0x0248  |          | `FMAX_VALUE_LSB`      |  RW   |  SRC         |  Fmax value in LSB      |
| 0x0093 |  0x024C  |          | `FMAX_LATENCY_NS`     |  RW   |  SRC         |  LM edge로 부터 Fmax latency time in nano second |
| 0x0094 |  0x0250  |          | `FWHM_COUNT`          |  RW   |  DST         |  Fmax value의 half 값을 지나는 data의 갯 수 |
| 0x0095 |  0x0254  |          | `FWHM_DUTY`           |  RW   |  DST         |  FWHM duty in nano second. `FWHM_1ST_NS` - `FWHM_2ND_NS` 차이 값 |
| 0x0096 |  0x0258  |          | `FWHM_2ND_NS`         |  RW   |  DST         |  두번째 Fmax half 값 위치 |
| 0x0097 |  0x025c  |          | `FWHM_1ST_NS`         |  RW   |  DST         |  첫번째 Fmax half 값 위치 |
| 0x00A0 |  0x0280  |          | `FMAX_VALUE_LSB_MIN`  |  RW   |  SRC         |  Fmax value의 minimum 설정 값  |
| 0x00A1 |  0x0284  |          | `FMAX_VALUE_LSB_MAX`  |  RW   |  SRC         |  Fmax value의 maximum 설정 값  |
| 0x00A2 |  0x0288  |          | `FMAX_LATENCY_NS_MIN` |  RW   |  SRC         |  Fmax latency의 minimum 설정 값  |
| 0x00A3 |  0x028C  |          | `FMAX_LATENCY_NS_MAX` |  RW   |  SRC         |  Fmax latency의 maximum 설정 값  |
| 0x00A4 |  0x0290  |          | `FWHM_DUTY_NS_MIN`    |  RW   |  DST         |  FWHM Duty의 minimum 설정 값  |
| 0x00A5 |  0x0294  |          | `FWHM_DUTY_NS_MAX`    |  RW   |  DST         |  FWHM Duty의 maximum 설정 값  |

