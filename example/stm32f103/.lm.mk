#****************************************************************
#* lite-manager                                                 *
#* NOTE: do not edit this file as it is automatically generated *
#****************************************************************


# Variables provided for Makefile
C_SOURCE := stm32f10x_it.c main.c hal/STM32F10x_StdPeriph_Driver/system_stm32f10x.c hal/STM32F10x_StdPeriph_Driver/src/misc.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_adc.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_bkp.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_can.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_cec.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_crc.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_dac.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_dbgmcu.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_dma.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_fsmc.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_i2c.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_iwdg.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_pwr.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_rtc.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_sdio.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c hal/STM32F10x_StdPeriph_Driver/src/stm32f10x_wwdg.c
C_PATH := -Ilib/cmsis -I. -Ihal/STM32F10x_StdPeriph_Driver -Ihal/STM32F10x_StdPeriph_Driver/inc
C_DEFINE := -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER
ASM_SOURCE := startup_stm32f10x_md.s
LDS_SOURCE := stm32f10x_64KB_flash.ld
AS_FLAG := -Wl, -Bstatic
C_FLAG := -O2 -mthumb -mcpu=cortex-m3 -march=armv7-m -Wl,-Bstatic -ffunction-sections -fdata-sections -nostdlib -ffreestanding
LD_FLAG := -lnosys -Wl,--cref -Wl,--no-relax -Wl,--gc-sections -ffreestanding -Wl,-Bstatic -Wl,--no-warn-rwx-segments