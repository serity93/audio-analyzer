cmake_minimum_required(VERSION 3.22)

# JUCE needs both the C and C++ compilers enabled.
project(AudioAnalyzer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download JUCE at configure time (into build/_deps) instead of committing it to the repo.
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        9.0.4
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE)
FetchContent_MakeAvailable(JUCE)

juce_add_gui_app(AudioAnalyzer
    PRODUCT_NAME "Audio Analyzer"
    COMPANY_NAME "Audio Analyzer"
    NEEDS_WEB_BROWSER FALSE
    NEEDS_CURL FALSE)

target_sources(AudioAnalyzer
    PRIVATE
        Source/Main.cpp
        Source/MainComponent.cpp)

target_compile_definitions(AudioAnalyzer
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:AudioAnalyzer,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:AudioAnalyzer,JUCE_VERSION>")

target_link_libraries(AudioAnalyzer
    PRIVATE
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
