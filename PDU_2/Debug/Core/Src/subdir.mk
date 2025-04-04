################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/adc_handler.c \
../Core/Src/bsp.c \
../Core/Src/led_handler.c \
../Core/Src/main.c \
../Core/Src/pdu.c \
../Core/Src/retarget.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c \
../Core/Src/tasks.c \
../Core/Src/uart_handler.c 

OBJS += \
./Core/Src/adc_handler.o \
./Core/Src/bsp.o \
./Core/Src/led_handler.o \
./Core/Src/main.o \
./Core/Src/pdu.o \
./Core/Src/retarget.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o \
./Core/Src/tasks.o \
./Core/Src/uart_handler.o 

C_DEPS += \
./Core/Src/adc_handler.d \
./Core/Src/bsp.d \
./Core/Src/led_handler.d \
./Core/Src/main.d \
./Core/Src/pdu.d \
./Core/Src/retarget.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d \
./Core/Src/tasks.d \
./Core/Src/uart_handler.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU -I../Middlewares/uCOSII/Cfg/Template -I../Middlewares/uCOSII/Source -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/adc_handler.cyclo ./Core/Src/adc_handler.d ./Core/Src/adc_handler.o ./Core/Src/adc_handler.su ./Core/Src/bsp.cyclo ./Core/Src/bsp.d ./Core/Src/bsp.o ./Core/Src/bsp.su ./Core/Src/led_handler.cyclo ./Core/Src/led_handler.d ./Core/Src/led_handler.o ./Core/Src/led_handler.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/pdu.cyclo ./Core/Src/pdu.d ./Core/Src/pdu.o ./Core/Src/pdu.su ./Core/Src/retarget.cyclo ./Core/Src/retarget.d ./Core/Src/retarget.o ./Core/Src/retarget.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su ./Core/Src/tasks.cyclo ./Core/Src/tasks.d ./Core/Src/tasks.o ./Core/Src/tasks.su ./Core/Src/uart_handler.cyclo ./Core/Src/uart_handler.d ./Core/Src/uart_handler.o ./Core/Src/uart_handler.su

.PHONY: clean-Core-2f-Src

