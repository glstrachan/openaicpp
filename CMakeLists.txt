cmake_minimum_required(VERSION 3.10)
project(MyLibcurlProject)

set(CMAKE_CXX_STANDARD 17)

# Find the libcurl package
find_package(CURL REQUIRED)

# Fetch nlohmann/json
include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.10.5
)
FetchContent_MakeAvailable(json)

include_directories(${CURL_INCLUDE_DIR})
include_directories(include/)

# Add your source files
add_executable(MyLibcurlExecutable src/main.cpp)

# Link libcurl and nlohmann_json
target_link_libraries(MyLibcurlExecutable ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)
