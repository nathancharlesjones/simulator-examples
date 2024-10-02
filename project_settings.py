import target
import subprocess

default_path_to_docker_file = 'Cross-Platform-Build-System'
default_docker_name = 'devenv-simple-build-system'
default_debug_port_number = '5000'

targets = {}

#######################################
#                                     #
#       Common to All                 #
#                                     #
#######################################

target_str 				= "{0}.elf"
c_compiler				= "gcc"
cpp_compiler			= "gcc"
linker					= "gcc"
debugger				= "gdb"
common_flags 			= ["-Wall",
						   "-ffunction-sections",
						   "-fdata-sections"]
debug_flags 			= ["-g3",
						   "-O0"]
release_flags			= ["-O3"]
include_dirs 			= ["hardware"]
linker_flags_common		= ["-Wl,--gc-sections"]
libraries_common 		= ["m"]
map_file_str			= "-Wl,-Map,{0}/{1}.map,--cref"

source_files_common = []

#######################################
#                                     #
#       Basic application             #
#                                     #
#######################################

include_dirs_basic		= include_dirs + ["application/basic"]
source_files_basic		= ["application/basic/application.c"]

#######################################
#                                     #
#       Advanced application          #
#                                     #
#######################################

include_dirs_advanced		= include_dirs + ["application/advanced"]
source_files_advanced		= ["application/advanced/application.c"]

#######################################
#                                     #
#   Simple Printf Simulator (Basic)   #
#                                     #
#######################################

simple_printf_basic_name				= "simple_printf_basic"
simple_printf_basic_build_dir			= "build/"+simple_printf_basic_name
simple_printf_basic_source_files		=  source_files_basic + \
											["hardware/x86/x86_00_simple-printf_basic.c"]

simple_printf_basic_debug_name 			= simple_printf_basic_name + "_debug"
simple_printf_basic_debug_build_dir 	= simple_printf_basic_build_dir + "/debug"

simple_printf_basic_debug = target.executable(
	name 				= 	simple_printf_basic_debug_name,
	build_dir 			= 	simple_printf_basic_debug_build_dir,
	target 				= 	target_str.format(simple_printf_basic_debug_name),
	c_compiler			= 	c_compiler,
	c_flags 			= 	common_flags + debug_flags,
	linker 				= 	linker,
	linker_flags 		=	linker_flags_common + \
							[map_file_str.format(simple_printf_basic_debug_build_dir, simple_printf_basic_debug_name)],
	source_files 		= 	simple_printf_basic_source_files,
	include_dirs 		= 	include_dirs_basic,
	libraries 			= 	libraries_common,
	debugger 			=	debugger,
	post_build_cmds 	= ["echo Finished building {0}".format(simple_printf_basic_debug_name)]
)

targets[simple_printf_basic_debug.name] = simple_printf_basic_debug

#######################################
#                                     #
# Simple Printf Simulator (Advanced)  #
#                                     #
#######################################

simple_printf_advanced_name				= "simple_printf_advanced"
simple_printf_advanced_build_dir			= "build/"+simple_printf_advanced_name
simple_printf_advanced_source_files		=  source_files_advanced + \
											["hardware/x86/x86_00_simple-printf_advanced.c"]

simple_printf_advanced_debug_name 			= simple_printf_advanced_name + "_debug"
simple_printf_advanced_debug_build_dir 	= simple_printf_advanced_build_dir + "/debug"

simple_printf_advanced_debug = target.executable(
	name 				= 	simple_printf_advanced_debug_name,
	build_dir 			= 	simple_printf_advanced_debug_build_dir,
	target 				= 	target_str.format(simple_printf_advanced_debug_name),
	c_compiler			= 	c_compiler,
	c_flags 			= 	common_flags + debug_flags,
	linker 				= 	linker,
	linker_flags 		=	linker_flags_common + \
							[map_file_str.format(simple_printf_advanced_debug_build_dir, simple_printf_advanced_debug_name)],
	source_files 		= 	simple_printf_advanced_source_files,
	include_dirs 		= 	include_dirs_advanced,
	libraries 			= 	libraries_common,
	debugger 			=	debugger,
	post_build_cmds 	= ["echo Finished building {0}".format(simple_printf_advanced_debug_name)]
)

targets[simple_printf_advanced_debug.name] = simple_printf_advanced_debug

#######################################
#                                     #
#     Printf Box Simulator (Basic)    #
#                                     #
#######################################

printf_box_basic_name				= "printf-box_basic"
printf_box_basic_build_dir			= "build/"+printf_box_basic_name
printf_box_basic_source_files		=  source_files_basic + \
									["hardware/x86/x86_01_pretty-printf_basic.c"]

printf_box_basic_debug_name 		= printf_box_basic_name + "_debug"
printf_box_basic_debug_build_dir 	= printf_box_basic_build_dir + "/debug"

printf_box_basic_debug = target.executable(
	name 				= 	printf_box_basic_debug_name,
	build_dir 			= 	printf_box_basic_debug_build_dir,
	target 				= 	target_str.format(printf_box_basic_debug_name),
	c_compiler			= 	c_compiler,
	c_flags 			= 	common_flags + debug_flags,
	linker 				= 	linker,
	linker_flags 		=	linker_flags_common + \
							[map_file_str.format(printf_box_basic_debug_build_dir, printf_box_basic_debug_name)],
	source_files 		= 	printf_box_basic_source_files,
	include_dirs 		= 	include_dirs_basic,
	libraries 			= 	libraries_common,
	debugger 			=	debugger,
	post_build_cmds 	= ["echo Finished building {0}".format(printf_box_basic_debug_name)]
)

targets[printf_box_basic_debug.name] = printf_box_basic_debug

#######################################
#                                     #
#   Printf Box Simulator (Advanced)   #
#                                     #
#######################################

printf_box_advanced_name				= "printf-box_advanced"
printf_box_advanced_build_dir			= "build/"+printf_box_advanced_name
printf_box_advanced_source_files		=  source_files_advanced + \
									["hardware/x86/x86_01_pretty-printf_advanced_v1.c"]

printf_box_advanced_debug_name 		= printf_box_advanced_name + "_debug"
printf_box_advanced_debug_build_dir 	= printf_box_advanced_build_dir + "/debug"

printf_box_advanced_debug = target.executable(
	name 				= 	printf_box_advanced_debug_name,
	build_dir 			= 	printf_box_advanced_debug_build_dir,
	target 				= 	target_str.format(printf_box_advanced_debug_name),
	c_compiler			= 	c_compiler,
	c_flags 			= 	common_flags + debug_flags,
	linker 				= 	linker,
	linker_flags 		=	linker_flags_common + \
							[map_file_str.format(printf_box_advanced_debug_build_dir, printf_box_advanced_debug_name)],
	source_files 		= 	printf_box_advanced_source_files,
	include_dirs 		= 	include_dirs_advanced,
	libraries 			= 	libraries_common,
	debugger 			=	debugger,
	post_build_cmds 	= ["echo Finished building {0}".format(printf_box_advanced_debug_name)]
)

targets[printf_box_advanced_debug.name] = printf_box_advanced_debug

#######################################
#                                     #
# Printf Box Simulator (Advanced, v2) #
#                                     #
#######################################

printf_box_advanced_v2_name				= "printf-box_advanced_v2"
printf_box_advanced_v2_build_dir			= "build/"+printf_box_advanced_v2_name
printf_box_advanced_v2_source_files		=  source_files_advanced + \
									["hardware/x86/x86_01_pretty-printf_advanced_v2.c"]

printf_box_advanced_v2_debug_name 		= printf_box_advanced_v2_name + "_debug"
printf_box_advanced_v2_debug_build_dir 	= printf_box_advanced_v2_build_dir + "/debug"

printf_box_advanced_v2_debug = target.executable(
	name 				= 	printf_box_advanced_v2_debug_name,
	build_dir 			= 	printf_box_advanced_v2_debug_build_dir,
	target 				= 	target_str.format(printf_box_advanced_v2_debug_name),
	c_compiler			= 	c_compiler,
	c_flags 			= 	common_flags + debug_flags,
	linker 				= 	linker,
	linker_flags 		=	linker_flags_common + \
							[map_file_str.format(printf_box_advanced_v2_debug_build_dir, printf_box_advanced_v2_debug_name)],
	source_files 		= 	printf_box_advanced_v2_source_files,
	include_dirs 		= 	include_dirs_advanced,
	libraries 			= 	libraries_common,
	debugger 			=	debugger,
	post_build_cmds 	= ["echo Finished building {0}".format(printf_box_advanced_v2_debug_name)]
)

targets[printf_box_advanced_v2_debug.name] = printf_box_advanced_v2_debug