################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/utils/containers/u_dual_list.c \
../User/utils/containers/u_fifo.c \
../User/utils/containers/u_list.c \
../User/utils/containers/u_queue.c \
../User/utils/containers/u_ring.c \
../User/utils/containers/u_stack.c 

OBJS += \
./User/utils/containers/u_dual_list.o \
./User/utils/containers/u_fifo.o \
./User/utils/containers/u_list.o \
./User/utils/containers/u_queue.o \
./User/utils/containers/u_ring.o \
./User/utils/containers/u_stack.o 

C_DEPS += \
./User/utils/containers/u_dual_list.d \
./User/utils/containers/u_fifo.d \
./User/utils/containers/u_list.d \
./User/utils/containers/u_queue.d \
./User/utils/containers/u_ring.d \
./User/utils/containers/u_stack.d 


# Each subdirectory must supply rules for building sources it contributes
User/utils/containers/%.o User/utils/containers/%.su User/utils/containers/%.cyclo: ../User/utils/containers/%.c User/utils/containers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/utils" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/utils/containers" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/utils/logger" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/utils/string" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/utils/timing" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/hals/include" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/drivers/lte" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/common/at_modem" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/common/mqtt" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/bsp" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/apps" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-utils-2f-containers

clean-User-2f-utils-2f-containers:
	-$(RM) ./User/utils/containers/u_dual_list.cyclo ./User/utils/containers/u_dual_list.d ./User/utils/containers/u_dual_list.o ./User/utils/containers/u_dual_list.su ./User/utils/containers/u_fifo.cyclo ./User/utils/containers/u_fifo.d ./User/utils/containers/u_fifo.o ./User/utils/containers/u_fifo.su ./User/utils/containers/u_list.cyclo ./User/utils/containers/u_list.d ./User/utils/containers/u_list.o ./User/utils/containers/u_list.su ./User/utils/containers/u_queue.cyclo ./User/utils/containers/u_queue.d ./User/utils/containers/u_queue.o ./User/utils/containers/u_queue.su ./User/utils/containers/u_ring.cyclo ./User/utils/containers/u_ring.d ./User/utils/containers/u_ring.o ./User/utils/containers/u_ring.su ./User/utils/containers/u_stack.cyclo ./User/utils/containers/u_stack.d ./User/utils/containers/u_stack.o ./User/utils/containers/u_stack.su

.PHONY: clean-User-2f-utils-2f-containers

