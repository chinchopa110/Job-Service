cmake_minimum_required(VERSION 3.16)
project(JobServiceClient VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt auto-tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Network Test QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 5.15 COMPONENTS Core Gui Widgets Network Test REQUIRED)
    set(QT_VERSION_MAJOR 5)
else()
    set(QT_VERSION_MAJOR 6)
endif()

set(QT_LIBS
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
)

# ── Include paths ─────────────────────────────────────────────────────────────
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# ── Client library (shared between app and tests) ────────────────────────────
set(CLIENT_LIB_SOURCES
    src/ApiClient.cpp
    src/MainWindow.cpp
    src/WorkerWindow.cpp
    src/EmployerWindow.cpp
    # Headers listed explicitly so AUTOMOC generates moc_*.cpp for this target
    include/ApiClient.hpp
    include/MainWindow.hpp
    include/WorkerWindow.hpp
    include/EmployerWindow.hpp
)

add_library(job_client_lib STATIC ${CLIENT_LIB_SOURCES})
target_link_libraries(job_client_lib PUBLIC ${QT_LIBS})
target_include_directories(job_client_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ── Main application ──────────────────────────────────────────────────────────
add_executable(job_client src/main.cpp)
target_link_libraries(job_client PRIVATE job_client_lib ${QT_LIBS})

set_target_properties(job_client PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(job_client PRIVATE -Wall -Wextra -pedantic)
endif()

# ── Tests ─────────────────────────────────────────────────────────────────────
enable_testing()

add_executable(test_client
    tests/test_client.cpp
)

target_link_libraries(test_client PRIVATE
    job_client_lib
    ${QT_LIBS}
    Qt${QT_VERSION_MAJOR}::Test
)

target_include_directories(test_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(test_client PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(test_client PRIVATE -Wall -Wextra)
endif()

add_test(NAME ClientTests COMMAND test_client)
