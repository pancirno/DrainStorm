cmake_minimum_required(VERSION 3.25)

project(DrainStorm LANGUAGES CXX)

include(CTest)

add_executable(DrainStorm WIN32
    src/application_state.cpp
    src/application_state.h
    src/bitmap_surface.cpp
    src/bitmap_surface.h
    src/game_data.cpp
    src/game_data.h
    src/high_scores.cpp
    src/high_scores.h
    src/main.cpp
)

target_compile_features(DrainStorm PRIVATE cxx_std_20)
target_compile_definitions(DrainStorm PRIVATE UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)
target_link_libraries(DrainStorm PRIVATE comdlg32 gdi32 user32 winmm)

if(MSVC)
    target_compile_options(DrainStorm PRIVATE /W4 /permissive- /EHsc /utf-8)
endif()

if(BUILD_TESTING)
    add_executable(DrainStormGameDataTests
        tests/game_data_tests.cpp
        src/application_state.cpp
        src/application_state.h
        src/bitmap_surface.cpp
        src/bitmap_surface.h
        src/game_data.cpp
        src/game_data.h
        src/high_scores.cpp
        src/high_scores.h
    )
    target_include_directories(DrainStormGameDataTests PRIVATE src)
    target_compile_features(DrainStormGameDataTests PRIVATE cxx_std_20)
    target_compile_definitions(DrainStormGameDataTests PRIVATE
        UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)
    target_link_libraries(DrainStormGameDataTests PRIVATE gdi32 user32 winmm)
    if(MSVC)
        target_compile_options(DrainStormGameDataTests PRIVATE /W4 /permissive- /EHsc /utf-8)
    endif()
    add_test(NAME GameDataRegression
        COMMAND DrainStormGameDataTests
            "${CMAKE_CURRENT_SOURCE_DIR}/assets/original/game-data")
    if(MSVC)
        add_executable(DrainStormAuditTests
            tests/audit_regression.cpp
            src/application_state.cpp src/bitmap_surface.cpp
            src/game_data.cpp src/high_scores.cpp src/main.cpp)
        target_include_directories(DrainStormAuditTests PRIVATE src)
        target_compile_features(DrainStormAuditTests PRIVATE cxx_std_20)
        target_compile_definitions(DrainStormAuditTests PRIVATE
            UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX)
        target_compile_options(DrainStormAuditTests PRIVATE /W4 /permissive- /EHsc /utf-8
            "/FI${CMAKE_CURRENT_SOURCE_DIR}/tests/audit_access.h")
        target_link_libraries(DrainStormAuditTests PRIVATE comdlg32 gdi32 user32 winmm)
        add_test(NAME AuditRegression COMMAND DrainStormAuditTests
            "${CMAKE_CURRENT_SOURCE_DIR}/assets/original/game-data")
    endif()
endif()
