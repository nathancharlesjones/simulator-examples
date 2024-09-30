import target
import subprocess

default_path_to_docker_file = 'Cross-Platform-Build-System'
default_docker_name = 'devenv-simple-build-system'
default_debug_port_number = '5000'

targets = {}

#######################################
#                                     #
#       TinyGPS Library               #
#                                     #
#######################################



#######################################
#                                     #
#       Common to All                 #
#                                     #
#######################################

target_str 				= "{0}.elf"
common_flags 			= ["-Wall",
						   "-ffunction-sections",
						   "-fdata-sections"]
debug_flags 			= ["-g3",
						   "-O0"]
release_flags			= ["-O3"]
include_dirs 			= ["application", "hardware"]
source_files_common		= ["application/application.c"]
linker_flags_common		= ["-Wl,--gc-sections"]
libraries_common 		= ["m"]
map_file_str			= "-Wl,-Map,{0}/{1}.map,--cref"

#######################################
#                                     #
#      Simple Printf Simulator        #
#                                     #
#######################################

simple_printf_name				= "simple_printf"
simple_printf_build_dir			= "build/"+simple_printf_name
simple_printf_source_files		=  source_files_common + \
									["hardware/x86/x86_simple-printf.c"]
simple_printf_c_compiler		= "gcc"
simple_printf_linker			= "gcc"
simple_printf_debugger			= "gdb"

simple_printf_debug_name 		= simple_printf_name + "_debug"
simple_printf_debug_build_dir 	= simple_printf_build_dir + "/debug"

simple_printf_debug = target.executable(
	name 				= 	simple_printf_debug_name,
	build_dir 			= 	simple_printf_debug_build_dir,
	target 				= 	target_str.format(simple_printf_debug_name),
	c_compiler			= 	simple_printf_c_compiler,
	c_flags 			= 	common_flags + debug_flags,
	linker 				= 	simple_printf_linker,
	linker_flags 		=	linker_flags_common + \
							[map_file_str.format(simple_printf_debug_build_dir, simple_printf_debug_name)],
	source_files 		= 	simple_printf_source_files,
	include_dirs 		= 	include_dirs,
	libraries 			= 	libraries_common,
	debugger 			=	simple_printf_debugger,
	post_build_cmds 	= ["echo Finished building {0}".format(simple_printf_debug_name)]
)

targets[simple_printf_debug.name] = simple_printf_debug