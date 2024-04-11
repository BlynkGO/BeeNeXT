# CMakeLists for ESP-IDF

set(COMPONENT_ADD_INCLUDEDIRS
    src
    )
file(GLOB SRCS
    src/*.cpp
    src/lib/BeeNeXT_SoftTimer/*.cpp
    src/lib/BeeNeXT_SoftTimer/*.c
    src/lib/SoftwareSerial/*.cpp 
    )

set(COMPONENT_SRCS ${SRCS})

# set(COMPONENT_REQUIRES nvs_flash efuse)

if (IDF_VERSION_MAJOR GREATER_EQUAL 5)
    set(COMPONENT_REQUIRES nvs_flash efuse driver esp_timer)
elseif ((IDF_VERSION_MAJOR EQUAL 4) AND (IDF_VERSION_MINOR GREATER 3) OR IDF_VERSION_MAJOR GREATER 4)
    set(COMPONENT_REQUIRES nvs_flash efuse)
else()
    set(COMPONENT_REQUIRES nvs_flash efuse)
endif()


### If you use arduino-esp32 components, please activate next comment line.
list(APPEND COMPONENT_REQUIRES arduino-esp32)

# message(STATUS "BeeNeXT use components = ${COMPONENT_REQUIRES}")
register_component()
