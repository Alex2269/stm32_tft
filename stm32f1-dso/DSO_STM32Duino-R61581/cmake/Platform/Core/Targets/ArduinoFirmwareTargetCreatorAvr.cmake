#=============================================================================#
# create_arduino_target
# [PRIVATE/INTERNAL]
#
# create_arduino_target(TARGET_NAME ALL_SRCS ALL_LIBS COMPILE_FLAGS LINK_FLAGS MANUAL)
#
#        TARGET_NAME - Target name
#        BOARD_ID    - Arduino board ID
#        ALL_SRCS    - All sources
#        ALL_LIBS    - All libraries
#        COMPILE_FLAGS - Compile flags
#        LINK_FLAGS    - Linker flags
#        MANUAL - (Advanced) Only use AVR Libc/Includes
#
# Creates an Arduino firmware target.
#
#=============================================================================#
function(create_arduino_firmware_target TARGET_NAME BOARD_ID ALL_SRCS ALL_LIBS
        COMPILE_FLAGS LINK_FLAGS MANUAL)

    string(STRIP "${ALL_SRCS}" ALL_SRCS)
    if(ARDUINO_CMAKE_GENERATE_SHARED_LIBRARIES)
        add_library(${TARGET_NAME} SHARED "${ALL_SRCS}")
    else()
        add_executable(${TARGET_NAME} "${ALL_SRCS}")
    endif()
    set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".elf")

    set_board_flags(ARDUINO_COMPILE_FLAGS ARDUINO_LINK_FLAGS ${BOARD_ID} ${MANUAL})

    set_target_properties(${TARGET_NAME} PROPERTIES
            COMPILE_FLAGS "${ARDUINO_COMPILE_FLAGS} ${COMPILE_FLAGS}"
            LINK_FLAGS "${ARDUINO_LINK_FLAGS} ${LINK_FLAGS}")
            
    list(REMOVE_DUPLICATES ALL_LIBS)

    if(ARDUINO_CMAKE_GENERATE_SHARED_LIBRARIES)
      # When building a shared library we must make sure that
      # all symbols from the intermediate static libraries end up in the
      # static library
      #
      if(CMAKE_HOST_APPLE)
         foreach(lib ${ALL_LIBS})
            if(NOT "${linker_flags}" STREQUAL "")
               set(linker_flags "${linker_flags} ")
            endif()
            set(linker_flags "${linker_flags}-Wl,-force_load,lib${lib}.a")
         endforeach()
#          message("linker_flags: ${linker_flags}")
         target_link_libraries(${TARGET_NAME} PUBLIC ${linker_flags})
      else()
         target_link_libraries(${TARGET_NAME} PUBLIC "-Wl,--whole-archive" ${ALL_LIBS} "-Wl,--no-whole-archive")
      endif()
      
      add_dependencies(${TARGET_NAME} ${ALL_LIBS})
    else()
      target_link_libraries(${TARGET_NAME} ${ALL_LIBS} "-lc -lm")
    endif()
    
    if (NOT EXECUTABLE_OUTPUT_PATH)
        set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
    endif ()
    
    if(ARDUINO_CMAKE_ONLY_ELF)
       return()
    endif()
    
    set(TARGET_PATH ${EXECUTABLE_OUTPUT_PATH}/${TARGET_NAME})
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_OBJCOPY}
            ARGS ${ARDUINO_OBJCOPY_EEP_FLAGS}
            ${TARGET_PATH}.elf
            ${TARGET_PATH}.eep
            COMMENT "Generating EEP image"
            VERBATIM)

    # Convert firmware image to ASCII HEX format
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_OBJCOPY}
            ARGS ${ARDUINO_OBJCOPY_HEX_FLAGS}
            ${TARGET_PATH}.elf
            ${TARGET_PATH}.hex
            COMMENT "Generating HEX image"
            VERBATIM)
    _get_board_property(${BOARD_ID} build.mcu MCU)
    # Display target size
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            ARGS -DFIRMWARE_IMAGE=${TARGET_PATH}.elf
            -DMCU=${MCU}
            -DEEPROM_IMAGE=${TARGET_PATH}.eep
            -P ${ARDUINO_SIZE_SCRIPT}
            COMMENT "Calculating image size"
            VERBATIM)

    # Create ${TARGET_NAME}-size target
    add_custom_target(${TARGET_NAME}-size
            COMMAND ${CMAKE_COMMAND}
            -DFIRMWARE_IMAGE=${TARGET_PATH}.elf
            -DMCU=${MCU}
            -DEEPROM_IMAGE=${TARGET_PATH}.eep
            -P ${ARDUINO_SIZE_SCRIPT}
            DEPENDS ${TARGET_NAME}
            COMMENT "Calculating ${TARGET_NAME} image size")

endfunction()
