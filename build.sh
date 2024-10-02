#!/bin/sh

mkdir -p build_shell/application/basic
mkdir -p build_shell/application/advanced
mkdir -p build_shell/hardware/x86

#######################################
#                                     #
#             Compiling               #
#                                     #
#######################################

###################################################################################################################################################################################################
# CC   |    C Flags                                        |     Include dirs                  |      Source file                                  |      Output file
###################################################################################################################################################################################################

# Compile basic/application.o
gcc 	-Wall -ffunction-sections -fdata-sections -g3 -O0	-I application/basic -I hardware	-c application/basic/application.c 					-o build_shell/application/basic/application.o

# Compile advanced/application.o
gcc 	-Wall -ffunction-sections -fdata-sections -g3 -O0 	-I application/advanced -I hardware -c application/advanced/application.c 				-o build_shell/application/advanced/application.o

# Compile hardware/x86/x86_00_simple-printf_basic.o
gcc 	-Wall -ffunction-sections -fdata-sections -g3 -O0 	-I application/basic -I hardware 	-c hardware/x86/x86_00_simple-printf_basic.c 		-o build_shell/hardware/x86/x86_00_simple-printf_basic.o

# Compile hardware/x86/x86_00_simple-printf_advanced.o
gcc 	-Wall -ffunction-sections -fdata-sections -g3 -O0 	-I application/advanced -I hardware -c hardware/x86/x86_00_simple-printf_advanced.c 	-o build_shell/hardware/x86/x86_00_simple-printf_advanced.o

# Compile hardware/x86/x86_01_pretty-printf_basic.o
gcc 	-Wall -ffunction-sections -fdata-sections -g3 -O0 	-I application/basic -I hardware 	-c hardware/x86/x86_01_pretty-printf_basic.c 		-o build_shell/hardware/x86/x86_01_pretty-printf_basic.o

# Compile hardware/x86/x86_01_pretty-printf_advanced_v1.o
gcc 	-Wall -ffunction-sections -fdata-sections -g3 -O0 	-I application/advanced -I hardware -c hardware/x86/x86_01_pretty-printf_advanced_v1.c 	-o build_shell/hardware/x86/x86_01_pretty-printf_advanced_v1.o

# Compile hardware/x86/x86_01_pretty-printf_advanced_v2.o
gcc 	-Wall -ffunction-sections -fdata-sections -g3 -O0 	-I application/advanced -I hardware -c hardware/x86/x86_01_pretty-printf_advanced_v2.c 	-o build_shell/hardware/x86/x86_01_pretty-printf_advanced_v2.o

#######################################
#                                     #
#              Linking                #
#                                     #
#######################################

########################################################################################################################################################################################################################################################################################################################
# Linker   |    Linker Flags                                                                   |     Include dirs                     |      Object files                                                                                          | Libs  |     Output file
########################################################################################################################################################################################################################################################################################################################

# Link x86_00_simple-printf_basic.out
gcc 		-Wl,--gc-sections -Wl,-Map,build_shell/x86_00_simple-printf_basic.map,--cref 		-I application/basic -I hardware 		build_shell/application/basic/application.o build_shell/hardware/x86/x86_00_simple-printf_basic.o 			-lm 	-o build_shell/x86_00_simple-printf_basic.out

# Link x86_00_simple-printf_advanced.out
gcc 		-Wl,--gc-sections -Wl,-Map,build_shell/x86_00_simple-printf_advanced.map,--cref 	-I application/advanced -I hardware 	build_shell/application/advanced/application.o build_shell/hardware/x86/x86_00_simple-printf_advanced.o 	-lm 	-o build_shell/x86_00_simple-printf_advanced.out

# Link x86_01_pretty-printf_basic.out
gcc 		-Wl,--gc-sections -Wl,-Map,build_shell/x86_01_pretty-printf_basic.map,--cref 		-I application/basic -I hardware 		build_shell/application/basic/application.o build_shell/hardware/x86/x86_01_pretty-printf_basic.o 			-lm 	-o build_shell/x86_01_pretty-printf_basic.out

# Link x86_01_pretty-printf_advanced_v1.out
gcc 		-Wl,--gc-sections -Wl,-Map,build_shell/x86_01_pretty-printf_advanced_v1.map,--cref 	-I application/advanced -I hardware 	build_shell/application/advanced/application.o build_shell/hardware/x86/x86_01_pretty-printf_advanced_v1.o 	-lm 	-o build_shell/x86_01_pretty-printf_advanced_v1.out

# Link x86_01_pretty-printf_advanced_v2.out
gcc 		-Wl,--gc-sections -Wl,-Map,build_shell/x86_01_pretty-printf_advanced_v2.map,--cref 	-I application/advanced -I hardware 	build_shell/application/advanced/application.o build_shell/hardware/x86/x86_01_pretty-printf_advanced_v2.o 	-lm 	-o build_shell/x86_01_pretty-printf_advanced_v2.out