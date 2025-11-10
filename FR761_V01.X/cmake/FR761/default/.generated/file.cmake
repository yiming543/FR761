# The following variables contains the files used by the different stages of the build process.
set(FR761_default_default_XC8_FILE_TYPE_assemble)
set_source_files_properties(${FR761_default_default_XC8_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${FR761_default_default_XC8_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(FR761_default_default_XC8_FILE_TYPE_assemblePreprocess)
set_source_files_properties(${FR761_default_default_XC8_FILE_TYPE_assemblePreprocess} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${FR761_default_default_XC8_FILE_TYPE_assemblePreprocess})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(FR761_default_default_XC8_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../PWM.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/LINDrivers/lin_app.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/LINDrivers/lin_slave.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/device_config.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/eusart.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/interrupt_manager.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/mcc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/pin_manager.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/pwm5.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/tmr0.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/tmr2.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../mcc_generated_files/tmr6.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../timer.c")
set_source_files_properties(${FR761_default_default_XC8_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(FR761_default_default_XC8_FILE_TYPE_link)
set(FR761_default_image_name "default.elf")
set(FR761_default_image_base_name "default")

# The output directory of the final image.
set(FR761_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/FR761")

# The full path to the final image.
set(FR761_default_full_path_to_image ${FR761_default_output_dir}/${FR761_default_image_name})
