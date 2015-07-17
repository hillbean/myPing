################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cksum.c \
../src/hostConvIP.c \
../src/icmpackage.c \
../src/main.c \
../src/main_loop.c \
../src/proc_v4.c \
../src/send.c 

OBJS += \
./src/cksum.o \
./src/hostConvIP.o \
./src/icmpackage.o \
./src/main.o \
./src/main_loop.o \
./src/proc_v4.o \
./src/send.o 

C_DEPS += \
./src/cksum.d \
./src/hostConvIP.d \
./src/icmpackage.d \
./src/main.d \
./src/main_loop.d \
./src/proc_v4.d \
./src/send.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


