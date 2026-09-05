cmake_minimum_required(VERSION 3.22)

project(MyDAW VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Pull in JUCE via FetchContent so this stays CLI/Cursor-friendly -------
# Pin to a specific tag rather than a floating branch so builds are
# reproducible across machines and CI.
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 7.0.12
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

# --- Phase 1 target: a GUI app with a working audio device + test tone ----
juce_add_gui_app(MyDAW
    PRODUCT_NAME "MyDAW"
    COMPANY_NAME  "YourCompany"
)

target_sources(MyDAW PRIVATE
    src/app/Main.cpp
    src/app/MainComponent.cpp
    src/audio/AudioEngine.cpp
)

target_include_directories(MyDAW PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(MyDAW PRIVATE
    # No browser/networking needed inside the app itself; keeps the binary
    # smaller and avoids pulling in curl on platforms that lack it.
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:MyDAW,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:MyDAW,JUCE_VERSION>"
)

target_link_libraries(MyDAW PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
