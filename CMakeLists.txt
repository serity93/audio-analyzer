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

set(AUDIO_ANALYZER_SOURCES
    Source/Main.cpp
    Source/Audio/AudioStream.cpp
    Source/Audio/AudioStream.h
    Source/Analysis/LevelAnalyzer.cpp
    Source/Analysis/LevelAnalyzer.h
    Source/UI/LevelMeter.cpp
    Source/UI/LevelMeter.h
    Source/UI/MainComponent.cpp
    Source/UI/MainComponent.h)

target_sources(AudioAnalyzer PRIVATE ${AUDIO_ANALYZER_SOURCES})

# Includes are written relative to Source/ (e.g. "Audio/AudioStream.h").
target_include_directories(AudioAnalyzer PRIVATE Source)

# Mirror the Source/ folder structure in IDE project trees (e.g. Visual Studio filters).
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX Source FILES ${AUDIO_ANALYZER_SOURCES})

# JUCE compiles its modules as part of this target; group them instead of leaving them in the
# IDE's default "Source Files" filter.
source_group("JUCE Modules" REGULAR_EXPRESSION "/modules/juce_[a-z0-9_]+/[^/]+$")

target_compile_definitions(AudioAnalyzer
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:AudioAnalyzer,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:AudioAnalyzer,JUCE_VERSION>")

target_link_libraries(AudioAnalyzer
    PRIVATE
        juce::juce_audio_utils
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
