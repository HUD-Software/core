cmake_minimum_required(VERSION 3.18)

# !add_hudengine_cxx_linker_static_options : Add HUD Engine compiler options for static target (.lib)
# 
# \arg:target The target
#
function(add_hudengine_cxx_linker_static_options target)
	# set_target_properties(${target} PROPERTIES STATIC_LIBRARY_OPTIONS /NOLOGO /WX /ERRORREPORT:PROMPT)
endfunction()


# !add_hudengine_cxx_linker_shared_options : Add HUD Engine compiler options for shared target (.dll)
# 
# \arg:target The target
#
function(add_hudengine_cxx_linker_shared_options target)
    # target_link_options(${target} PRIVATE )
endfunction()


# !add_hudengine_cxx_linker_exectuable_options : Add HUD Engine compiler options for shared target (.elf)
# 
# \arg:target The target
#elf
function(add_hudengine_cxx_linker_executable_options target)
	add_hudengine_cxx_linker_shared_options(${target})
endfunction()