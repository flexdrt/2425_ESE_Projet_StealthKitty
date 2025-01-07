################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/mylibs.c/drv_mot.c 

OBJS += \
./Core/Src/mylibs.c/drv_mot.o 

C_DEPS += \
./Core/Src/mylibs.c/drv_mot.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/mylibs.c/%.o Core/Src/mylibs.c/%.su Core/Src/mylibs.c/%.cyclo: ../Core/Src/mylibs.c/%.c Core/Src/mylibs.c/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I"/home/vincent/Documents/ese_3a/projet/StealthKitty/Motorisation/cube_rep_pwm/Core/Inc/mylibs.h" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-mylibs-2e-c

clean-Core-2f-Src-2f-mylibs-2e-c:
	-$(RM) ./Core/Src/mylibs.c/drv_mot.cyclo ./Core/Src/mylibs.c/drv_mot.d ./Core/Src/mylibs.c/drv_mot.o ./Core/Src/mylibs.c/drv_mot.su

.PHONY: clean-Core-2f-Src-2f-mylibs-2e-c

