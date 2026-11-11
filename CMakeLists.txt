cmake_minimum_required(VERSION 3.22)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13" CACHE STRING "")
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "")

project(L_R VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(JUCE
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 8.0.4
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(L_R
  COMPANY_NAME "LR"
  PLUGIN_MANUFACTURER_CODE LrCo
  PLUGIN_CODE LrPk
  FORMATS VST3
  PRODUCT_NAME "L_R"
  IS_SYNTH FALSE
  NEEDS_MIDI_INPUT FALSE
  NEEDS_MIDI_OUTPUT FALSE
  COPY_PLUGIN_AFTER_BUILD FALSE)

target_sources(L_R PRIVATE Plugin.cpp)

target_compile_definitions(L_R PUBLIC
  JUCE_WEB_BROWSER=0
  JUCE_USE_CURL=0)

target_link_libraries(L_R
  PRIVATE juce::juce_audio_utils
  PUBLIC juce::juce_recommended_config_flags)
