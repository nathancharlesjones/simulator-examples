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

# Compile advanced/application_MainMajor.o
gcc 	-Wall -ffunction-sections -fdata-sections -g3 -O0 	-I application/advanced -I hardware -c application/advanced/application_MainMajor.c		-o build_shell/application/advanced/application_MainMajor.o

# Compile hardware/x86/simple-printf_basic.o
gcc 	-Wall -ffunction-sections -fdata-sections -g3 -O0 	-I application/basic -I hardware 	-c hardware/x86/printf_basic.c 						-o build_shell/hardware/x86/simple-printf_basic.o

# Compile hardware/x86/simple-printf_advanced.o
gcc 	-Wall -ffunction-sections -fdata-sections -g3 -O0 	-I application/advanced -I hardware -c hardware/x86/printf_advanced.c 					-o build_shell/hardware/x86/simple-printf_advanced.o

# Compile hardware/x86/pretty-printf_basic.o
gcc 	-Wall -ffunction-sections -fdata-sections -g3 -O0 	-I application/basic -I hardware 	-c hardware/x86/printf_basic.c 						-o build_shell/hardware/x86/pretty-printf_basic.o

# Compile hardware/x86/pretty-printf_advanced_v1.o
gcc 	-Wall -ffunction-sections -fdata-sections -g3 -O0 	-I application/advanced -I hardware -c hardware/x86/printf_advanced_v1.c 				-o build_shell/hardware/x86/pretty-printf_advanced_v1.o

# Compile hardware/x86/pretty-printf_advanced_v2.o
gcc 	-Wall -ffunction-sections -fdata-sections -g3 -O0 	-I application/advanced -I hardware -c hardware/x86/printf_advanced_v2.c 				-o build_shell/hardware/x86/pretty-printf_advanced_v2.o

# Compile hardware/x86/serial_advanced.o
gcc 	-Wall -ffunction-sections -fdata-sections -g3 -O0 	-I application/advanced -I hardware -c hardware/x86/serial_advanced.c 					-o build_shell/hardware/x86/serial_advanced.o

#######################################
#                                     #
#              Linking                #
#                                     #
#######################################

########################################################################################################################################################################################################################################################################################################################
# Linker   |    Linker Flags                                                                   |     Include dirs                     |      Object files                                                                                          | Libs  |     Output file
########################################################################################################################################################################################################################################################################################################################

# Link simple-printf_basic.out
gcc 		-Wl,--gc-sections -Wl,-Map,build_shell/simple-printf_basic.map,--cref 				-I application/basic -I hardware 		build_shell/application/basic/application.o build_shell/hardware/x86/simple-printf_basic.o 					-lm 	-o build_shell/simple-printf_basic.elf

# Link simple-printf_advanced.out
gcc 		-Wl,--gc-sections -Wl,-Map,build_shell/simple-printf_advanced.map,--cref 			-I application/advanced -I hardware 	build_shell/application/advanced/application.o build_shell/hardware/x86/simple-printf_advanced.o 			-lm 	-o build_shell/simple-printf_advanced.elf

# Link pretty-printf_basic.out
gcc 		-Wl,--gc-sections -Wl,-Map,build_shell/pretty-printf_basic.map,--cref 				-I application/basic -I hardware 		build_shell/application/basic/application.o build_shell/hardware/x86/pretty-printf_basic.o 					-lm 	-o build_shell/pretty-printf_basic.elf

# Link pretty-printf_advanced_v1.out
gcc 		-Wl,--gc-sections -Wl,-Map,build_shell/pretty-printf_advanced_v1.map,--cref 		-I application/advanced -I hardware 	build_shell/application/advanced/application.o build_shell/hardware/x86/pretty-printf_advanced_v1.o 		-lm 	-o build_shell/pretty-printf_advanced_v1.elf

# Link pretty-printf_advanced_v2.out
gcc 		-Wl,--gc-sections -Wl,-Map,build_shell/pretty-printf_advanced_v2.map,--cref 		-I application/advanced -I hardware 	build_shell/application/advanced/application.o build_shell/hardware/x86/pretty-printf_advanced_v2.o 		-lm 	-o build_shell/pretty-printf_advanced_v2.elf

# Link serial_advanced.out
gcc 		-Wl,--gc-sections -Wl,-Map,build_shell/serial_advanced.map,--cref 					-I application/advanced -I hardware 	build_shell/application/advanced/application.o build_shell/hardware/x86/serial_advanced.o 					-lm 	-o build_shell/serial_advanced.elf