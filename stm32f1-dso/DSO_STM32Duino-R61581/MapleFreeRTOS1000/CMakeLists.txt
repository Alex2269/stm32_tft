
SET(COMMON croutine.c  event_groups.c  list.c  queue.c  stream_buffer.c  tasks.c  timers.c)
SET(PORT   port.c)
#SET(MEM heap_1.c  heap_2.c  heap_3.c  heap_4.c  heap_5.c)
SET(MEM  heap_4.c )
FOREACH(in ${COMMON})
    LIST(APPEND COMMON_EX Source/${in})
ENDFOREACH(in ${COMMON})
FOREACH(in ${MEM})
    LIST(APPEND MEM_EX Source/portable/MemMang/${in})
ENDFOREACH(in ${MEM})
FOREACH(in ${PORT})
    LIST(APPEND PORT_EX Source/portable/Stm32duino/${in})
ENDFOREACH(in ${PORT})

generate_arduino_library(${libPrefix}FreeRTOS 
                            SRCS ${COMMON_EX} ${PORT_EX} ${MEM_EX} MapleFreeRTOS1000_pp.cpp
                            BOARD ${ARDUINO_DEFAULT_BOARD}
                        )

target_include_directories(${libPrefix}FreeRTOS  PRIVATE Source/portable/Stm32duino)                        