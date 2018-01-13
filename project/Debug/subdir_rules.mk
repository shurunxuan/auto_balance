################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/shuru/workspace_v7/auto_balance" --include_path="C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" --include_path="C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/include" --advice:power=none --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

main_prog.obj: ../main_prog.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/shuru/workspace_v7/auto_balance" --include_path="C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" --include_path="C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/include" --advice:power=none --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="main_prog.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

motor.obj: ../motor.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/shuru/workspace_v7/auto_balance" --include_path="C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" --include_path="C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/include" --advice:power=none --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="motor.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

mpu_6050.obj: ../mpu_6050.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/shuru/workspace_v7/auto_balance" --include_path="C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" --include_path="C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/include" --advice:power=none --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="mpu_6050.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

system_msp432p401r.obj: ../system_msp432p401r.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/shuru/workspace_v7/auto_balance" --include_path="C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" --include_path="C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/include" --advice:power=none --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="system_msp432p401r.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

uart.obj: ../uart.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/shuru/workspace_v7/auto_balance" --include_path="C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" --include_path="C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_17.9.0.STS/include" --advice:power=none --define=__MSP432P401R__ --define=DeviceFamily_MSP432P401x -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="uart.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


