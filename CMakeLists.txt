cmake_minimum_required(VERSION 3.2)

project(cmake VERSION 0.0.1$ENV{TRAVIS_BUILD_NUMBER})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan%22")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                TLS_VERIFY ON)
endif()



include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_CURRENT_SOURCE_DIR}
                    INSTALL_FOLDER ${CMAKE_BINARY_DIR}
                    BUILD missing
                    SETTINGS ${settings})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)



find_package(Threads REQUIRED)


#find_package(spdlog REQUIRED)

# set(Boost_USE_DEBUG_LIBS         oFF) # ignore debug libs and 
# set(Boost_USE_RELEASE_LIBS       ON)
# set(Boost_USE_STATIC_LIBS ON)
# find_package(Boost REQUIRED COMPONENTS unit_test_framework)
# include_directories(${Boost_INCLUDE_DIR})



configure_file(version.h.in ${PROJECT_SOURCE_DIR}/version.h)

add_executable(helloworld_cli main.cpp)

add_library(helloworld lib.cpp)

add_executable(test_version test_version.cpp)

set_target_properties(helloworld_cli helloworld test_version PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    
)

#target_link_libraries(${PROJECT_NAME} helloworld CONAN_PKG::spdlog Threads::Threads )

#CONAN_PKG::boost
# set_target_properties(test_version PROPERTIES
# COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
# INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
# )

 target_link_libraries(helloworld_cli
    CONAN_PKG::spdlog
    helloworld
    Threads::Threads
 )

 target_link_libraries(test_version
    CONAN_PKG::boost
    helloworld
)

install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")


set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

enable_testing()

add_test(helloworld_tests test_version)
