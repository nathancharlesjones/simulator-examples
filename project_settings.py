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
									["hardware/x86/x86_01_simple-printf.c"]

simple_printf_debug_name 		= simple_printf_name + "_debug"
simple_printf_debug_build_dir 	= simple_printf_build_dir + "/debug"

simple_printf_debug = target.executable(
	name 				= 	simple_printf_debug_name,
	build_dir 			= 	simple_printf_debug_build_dir,
	target 				= 	target_str.format(simple_printf_debug_name),
	c_compiler			= 	c_compiler,
	c_flags 			= 	common_flags + debug_flags,
	linker 				= 	linker,
	linker_flags 		=	linker_flags_common + \
							[map_file_str.format(simple_printf_debug_build_dir, simple_printf_debug_name)],
	source_files 		= 	simple_printf_source_files,
	include_dirs 		= 	include_dirs,
	libraries 			= 	libraries_common,
	debugger 			=	debugger,
	post_build_cmds 	= ["echo Finished building {0}".format(simple_printf_debug_name)]
)

targets[simple_printf_debug.name] = simple_printf_debug

#######################################
#                                     #
#        Printf Box Simulator         #
#                                     #
#######################################

printf_box_name				= "printf_box"
printf_box_build_dir			= "build/"+printf_box_name
printf_box_source_files		=  source_files_common + \
									["hardware/x86/x86_02_printf_box.c"]

printf_box_debug_name 		= printf_box_name + "_debug"
printf_box_debug_build_dir 	= printf_box_build_dir + "/debug"

printf_box_debug = target.executable(
	name 				= 	printf_box_debug_name,
	build_dir 			= 	printf_box_debug_build_dir,
	target 				= 	target_str.format(printf_box_debug_name),
	c_compiler			= 	c_compiler,
	c_flags 			= 	common_flags + debug_flags,
	linker 				= 	linker,
	linker_flags 		=	linker_flags_common + \
							[map_file_str.format(printf_box_debug_build_dir, printf_box_debug_name)],
	source_files 		= 	printf_box_source_files,
	include_dirs 		= 	include_dirs,
	libraries 			= 	libraries_common,
	debugger 			=	debugger,
	post_build_cmds 	= ["echo Finished building {0}".format(printf_box_debug_name)]
)

#targets[printf_box_debug.name] = printf_box_debug

#######################################
#                                     #
#         ncurses Simulator           #
#                                     #
#######################################

ncurses_name				= "ncurses"
ncurses_build_dir			= "build/"+ncurses_name
ncurses_source_files		=  source_files_common + \
									["hardware/x86/x86_03_ncurses.c"]

ncurses_debug_name 			= ncurses_name + "_debug"
ncurses_debug_build_dir 	= ncurses_build_dir + "/debug"

ncurses_debug = target.executable(
	name 				= 	ncurses_debug_name,
	build_dir 			= 	ncurses_debug_build_dir,
	target 				= 	target_str.format(ncurses_debug_name),
	c_compiler			= 	c_compiler,
	c_flags 			= 	common_flags + debug_flags,
	linker 				= 	linker,
	linker_flags 		=	linker_flags_common + \
							[map_file_str.format(ncurses_debug_build_dir, ncurses_debug_name)],
	source_files 		= 	ncurses_source_files,
	include_dirs 		= 	include_dirs,
	libraries 			= 	libraries_common + ["ncurses"],
	debugger 			=	debugger,
	post_build_cmds 	= ["echo Finished building {0}".format(ncurses_debug_name)]
)

#targets[ncurses_debug.name] = ncurses_debug