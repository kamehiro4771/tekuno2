################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/common/clock.c \
../src/common/initialcheck.c \
../src/common/initialize.c \
../src/common/lcd.c \
../src/common/output_led.c \
../src/common/output_speaker.c \
../src/common/sci.c \
../src/common/sw.c \
../src/common/timer.c 

COMPILER_OBJS += \
src/common/clock.obj \
src/common/initialcheck.obj \
src/common/initialize.obj \
src/common/lcd.obj \
src/common/output_led.obj \
src/common/output_speaker.obj \
src/common/sci.obj \
src/common/sw.obj \
src/common/timer.obj 

C_DEPS += \
src/common/clock.d \
src/common/initialcheck.d \
src/common/initialize.d \
src/common/lcd.d \
src/common/output_led.d \
src/common/output_speaker.d \
src/common/sci.d \
src/common/sw.d \
src/common/timer.d 

# Each subdirectory must supply rules for building sources it contributes
src/common/clock.obj: ../src/common/clock.c src/common/clock.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\common\clockcDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\clockcDepSubCommand.tmp"
	ccrx -subcommand="src\common\clockcDepSubCommand.tmp" "$<"
	@echo src\common\clockcSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\clockcSubCommand.tmp"
	ccrx -subcommand="src\common\clockcSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo ''

src/common/initialcheck.obj: ../src/common/initialcheck.c src/common/initialcheck.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\common\initialcheckcDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\initialcheckcDepSubCommand.tmp"
	ccrx -subcommand="src\common\initialcheckcDepSubCommand.tmp" "$<"
	@echo src\common\initialcheckcSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\initialcheckcSubCommand.tmp"
	ccrx -subcommand="src\common\initialcheckcSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo ''

src/common/initialize.obj: ../src/common/initialize.c src/common/initialize.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\common\initializecDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\initializecDepSubCommand.tmp"
	ccrx -subcommand="src\common\initializecDepSubCommand.tmp" "$<"
	@echo src\common\initializecSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\initializecSubCommand.tmp"
	ccrx -subcommand="src\common\initializecSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo ''

src/common/lcd.obj: ../src/common/lcd.c src/common/lcd.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\common\lcdcDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\lcdcDepSubCommand.tmp"
	ccrx -subcommand="src\common\lcdcDepSubCommand.tmp" "$<"
	@echo src\common\lcdcSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\lcdcSubCommand.tmp"
	ccrx -subcommand="src\common\lcdcSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo ''

src/common/output_led.obj: ../src/common/output_led.c src/common/output_led.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\common\output_ledcDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\output_ledcDepSubCommand.tmp"
	ccrx -subcommand="src\common\output_ledcDepSubCommand.tmp" "$<"
	@echo src\common\output_ledcSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\output_ledcSubCommand.tmp"
	ccrx -subcommand="src\common\output_ledcSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo ''

src/common/output_speaker.obj: ../src/common/output_speaker.c src/common/output_speaker.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\common\output_speakercDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\output_speakercDepSubCommand.tmp"
	ccrx -subcommand="src\common\output_speakercDepSubCommand.tmp" "$<"
	@echo src\common\output_speakercSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\output_speakercSubCommand.tmp"
	ccrx -subcommand="src\common\output_speakercSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo ''

src/common/sci.obj: ../src/common/sci.c src/common/sci.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\common\scicDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\scicDepSubCommand.tmp"
	ccrx -subcommand="src\common\scicDepSubCommand.tmp" "$<"
	@echo src\common\scicSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\scicSubCommand.tmp"
	ccrx -subcommand="src\common\scicSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo ''

src/common/sw.obj: ../src/common/sw.c src/common/sw.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\common\swcDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\swcDepSubCommand.tmp"
	ccrx -subcommand="src\common\swcDepSubCommand.tmp" "$<"
	@echo src\common\swcSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\swcSubCommand.tmp"
	ccrx -subcommand="src\common\swcSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo ''

src/common/timer.obj: ../src/common/timer.c src/common/timer.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@echo src\common\timercDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\timercDepSubCommand.tmp"
	ccrx -subcommand="src\common\timercDepSubCommand.tmp" "$<"
	@echo src\common\timercSubCommand.tmp=
	@sed -e "s/^/ /" "src\common\timercSubCommand.tmp"
	ccrx -subcommand="src\common\timercSubCommand.tmp" "$<"
	@echo 'Finished Scanning and building: $<'
	@echo ''

