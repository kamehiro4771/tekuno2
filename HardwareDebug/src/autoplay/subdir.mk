################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/autoplay/auto_play.c 

COMPILER_OBJS += \
src/autoplay/auto_play.obj 

C_DEPS += \
src/autoplay/auto_play.d 

# Each subdirectory must supply rules for building sources it contributes
src/autoplay/auto_play.obj: ../src/autoplay/auto_play.c src/autoplay/auto_play.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\autoplay\auto_playcDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\autoplay\auto_playcDepSubCommand.tmp"
	ccrx -subcommand="src\autoplay\auto_playcDepSubCommand.tmp" "$<"
	@echo src\autoplay\auto_playcSubCommand.tmp=
	@sed -e "s/^/ /" "src\autoplay\auto_playcSubCommand.tmp"
	ccrx -subcommand="src\autoplay\auto_playcSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo ''

