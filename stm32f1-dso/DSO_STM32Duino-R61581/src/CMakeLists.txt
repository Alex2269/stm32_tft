
SET(SRCS 
    dso_adc_gain.cpp
    dso_autoSetup.cpp
    dso_calibrate.cpp
    dso_control.cpp
    dso_display.cpp
    dso_eeprom.cpp
    dso_frequency.cpp
    dso_logger.cpp
    dso_gfx.cpp
    dso_mainUI.cpp
    ui/dso_menuButton.cpp
    ui/dso_menu.cpp
    ui/dso_menuEngine.cpp
    dso_test_signal.cpp
    ) 
generate_arduino_library(${libPrefix}src 
                            SRCS ${SRCS}
                            BOARD ${ARDUINO_DEFAULT_BOARD}
                        )
target_include_directories(${libPrefix}src PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

