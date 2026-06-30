cmake_minimum_required(VERSION 3.15)
project(AiSqlGenerator LANGUAGES CXX)

# Use modern C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate our packages inside vcpkg
find_package(cpr REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)

# Define our application executable name
add_executable(AiSqlGenerator main.cpp)

# Link the libraries securely using the correct modern vcpkg targets
target_link_libraries(AiSqlGenerator PRIVATE 
    cpr::cpr 
    nlohmann_json::nlohmann_json 
    unofficial::mysql-connector-cpp::connector
)