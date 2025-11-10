# cmake files support debug production
include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(FR761_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group default-XC8
if(FR761_default_default_XC8_FILE_TYPE_assemble)
add_library(FR761_default_default_XC8_assemble OBJECT ${FR761_default_default_XC8_FILE_TYPE_assemble})
    FR761_default_default_XC8_assemble_rule(FR761_default_default_XC8_assemble)
    list(APPEND FR761_default_library_list "$<TARGET_OBJECTS:FR761_default_default_XC8_assemble>")
endif()

# Handle files with suffix S, for group default-XC8
if(FR761_default_default_XC8_FILE_TYPE_assemblePreprocess)
add_library(FR761_default_default_XC8_assemblePreprocess OBJECT ${FR761_default_default_XC8_FILE_TYPE_assemblePreprocess})
    FR761_default_default_XC8_assemblePreprocess_rule(FR761_default_default_XC8_assemblePreprocess)
    list(APPEND FR761_default_library_list "$<TARGET_OBJECTS:FR761_default_default_XC8_assemblePreprocess>")
endif()

# Handle files with suffix [cC], for group default-XC8
if(FR761_default_default_XC8_FILE_TYPE_compile)
add_library(FR761_default_default_XC8_compile OBJECT ${FR761_default_default_XC8_FILE_TYPE_compile})
    FR761_default_default_XC8_compile_rule(FR761_default_default_XC8_compile)
    list(APPEND FR761_default_library_list "$<TARGET_OBJECTS:FR761_default_default_XC8_compile>")
endif()

add_executable(FR761_default_image_P8Sr2SV8 ${FR761_default_library_list})

set_target_properties(FR761_default_image_P8Sr2SV8 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${FR761_default_output_dir})
set_target_properties(FR761_default_image_P8Sr2SV8 PROPERTIES OUTPUT_NAME "default")
set_target_properties(FR761_default_image_P8Sr2SV8 PROPERTIES SUFFIX ".elf")

target_link_libraries(FR761_default_image_P8Sr2SV8 PRIVATE ${FR761_default_default_XC8_FILE_TYPE_link})


# Add the link options from the rule file.
FR761_default_link_rule(FR761_default_image_P8Sr2SV8)



