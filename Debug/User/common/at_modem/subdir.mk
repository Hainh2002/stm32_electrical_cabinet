################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/common/at_modem/sm_modem.c 

OBJS += \
./User/common/at_modem/sm_modem.o 

C_DEPS += \
./User/common/at_modem/sm_modem.d 


# Each subdirectory must supply rules for building sources it contributes
User/common/at_modem/%.o User/common/at_modem/%.su User/common/at_modem/%.cyclo: ../User/common/at_modem/%.c User/common/at_modem/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/utils" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/utils/containers" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/utils/logger" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/utils/string" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/utils/timing" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/hals/include" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/drivers/lte" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/common/at_modem" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/common/mqtt" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/bsp" -I"C:/Users/Admin/STM32CubeIDE/workspace_1.16.1/stm32_electrical_cabinet/User/apps" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-common-2f-at_modem

clean-User-2f-common-2f-at_modem:
	-$(RM) ./User/common/at_modem/sm_modem.cyclo ./User/common/at_modem/sm_modem.d ./User/common/at_modem/sm_modem.o ./User/common/at_modem/sm_modem.su

.PHONY: clean-User-2f-common-2f-at_modem

