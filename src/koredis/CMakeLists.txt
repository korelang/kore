set(DISASSEMBLER_NAME "koredis")

set(DISASSEMBLER_INCLUDE_DIRS
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/src/frontend"
    "${PROJECT_SOURCE_DIR}/src/backend"
)

set(DISASSEMBLER_SOURCES
    ${PROJECT_SOURCE_DIR}/src/frontend/location.cpp
    ${PROJECT_SOURCE_DIR}/src/logging/color_attributes.cpp
    ${PROJECT_SOURCE_DIR}/src/logging/colors.cpp
    ${PROJECT_SOURCE_DIR}/src/logging/logging.cpp
    ${PROJECT_SOURCE_DIR}/src/utf8/utf8.cpp

    # Utils
    ${PROJECT_SOURCE_DIR}/src/utils/endian.cpp

    # Files specific to koredis
    options.cpp
    main.cpp
)

add_executable(${DISASSEMBLER_NAME} ${DISASSEMBLER_SOURCES})

target_include_directories(${DISASSEMBLER_NAME} PRIVATE ${DISASSEMBLER_INCLUDE_DIRS})

target_compile_options(${DISASSEMBLER_NAME} PUBLIC ${SHARED_COMPILE_FLAGS})

set_target_properties(${DISASSEMBLER_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
