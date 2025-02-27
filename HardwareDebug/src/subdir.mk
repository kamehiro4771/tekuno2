################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/ad.c \
../src/auto_play.c \
../src/battle.c \
../src/clock.c \
../src/damage.c \
../src/flash.c \
../src/game.c \
../src/initialize.c \
../src/lcd.c \
../src/main.c \
../src/output_led.c \
../src/output_speaker.c \
../src/port.c \
../src/puzzle.c \
../src/sci.c \
../src/score.c \
../src/sw.c \
../src/timer.c 

COMPILER_OBJS += \
src/ad.obj \
src/auto_play.obj \
src/battle.obj \
src/clock.obj \
src/damage.obj \
src/flash.obj \
src/game.obj \
src/initialize.obj \
src/lcd.obj \
src/main.obj \
src/output_led.obj \
src/output_speaker.obj \
src/port.obj \
src/puzzle.obj \
src/sci.obj \
src/score.obj \
src/sw.obj \
src/timer.obj 

C_DEPS += \
src/ad.d \
src/auto_play.d \
src/battle.d \
src/clock.d \
src/damage.d \
src/flash.d \
src/game.d \
src/initialize.d \
src/lcd.d \
src/main.d \
src/output_led.d \
src/output_speaker.d \
src/port.d \
src/puzzle.d \
src/sci.d \
src/score.d \
src/sw.d \
src/timer.d 

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\cDepSubCommand.tmp"
	ccrx -subcommand="src\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" "$<"
	@echo src\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\cSubCommand.tmp"
	ccrx -subcommand="src\cSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo ''

src/main.obj: ../src/main.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\maincDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\maincDepSubCommand.tmp"
	ccrx -subcommand="src\maincDepSubCommand.tmp" "$<"
	@echo src\maincSubCommand.tmp=
	@sed -e "s/^/ /" "src\maincSubCommand.tmp"
	ccrx -subcommand="src\maincSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo ''

