cmake_minimum_required(VERSION 3.10)
project(JobService VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(DOMAIN_SOURCES
    src/domain/Worker.cpp
    src/domain/Employer.cpp
    src/domain/JobPosting.cpp
    src/domain/Reply.cpp
)

set(REPOSITORY_SOURCES
    src/repository/WorkerRepository.cpp
    src/repository/EmployerRepository.cpp
    src/repository/JobPostingRepository.cpp
    src/repository/ReplyRepository.cpp
)

set(SERVICE_SOURCES
    src/service/WorkerService.cpp
    src/service/EmployerService.cpp
    src/service/JobPostingService.cpp
    src/service/ReplyService.cpp
    src/JobService.cpp
)

set(FILTER_SOURCES
    src/filter/JobFilter.cpp
)

set(UTILS_SOURCES
    src/utils/LockGuard.cpp
)

set(ALL_SOURCES
    ${DOMAIN_SOURCES}
    ${REPOSITORY_SOURCES}
    ${SERVICE_SOURCES}
    ${FILTER_SOURCES}
    ${UTILS_SOURCES}
    src/main.cpp
)

add_executable(job_service ${ALL_SOURCES})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(job_service PRIVATE -Wall -Wextra -pedantic)
endif()

set_target_properties(job_service PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
)