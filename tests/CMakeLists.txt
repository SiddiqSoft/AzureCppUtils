if(${${PROJECT_NAME}_BUILD_TESTS})
    set(TESTPROJ ${PROJECT_NAME}_tests)

    add_executable(${TESTPROJ})

    target_compile_features(${TESTPROJ} PRIVATE cxx_std_20)
    target_compile_options( ${TESTPROJ}
                            PRIVATE
                            $<$<CXX_COMPILER_ID:MSVC>:/std:c++20> )
    target_compile_options( ${TESTPROJ}
                            PRIVATE
                            $<$<CXX_COMPILER_ID:Clang>:-fexperimental-library> )

    # Temporary whilst development of the feature/unix-support branch
    if((CMAKE_CXX_COMPILER_ID MATCHES [Cc][Ll][Aa][Nn][Gg])
        OR (CMAKE_CXX_COMPILER_ID MATCHES [Gg][Nn][Uu]))
    # We add one at a time as we complete the re-implementation
    target_sources( ${TESTPROJ}
                    PRIVATE
                    ${PROJECT_SOURCE_DIR}/tests/date-utils-tests.cpp)
    else()
    # Windows should have all tests!
    target_sources( ${TESTPROJ}
                    PRIVATE
                    ${PROJECT_SOURCE_DIR}/tests/base64-utils-tests.cpp
                    ${PROJECT_SOURCE_DIR}/tests/encryption-utils-tests.cpp
                    ${PROJECT_SOURCE_DIR}/tests/date-utils-tests.cpp)
    endif()

    # ASAN and Coverage only for Debug builds on Linux
    if(((CMAKE_CXX_COMPILER_ID MATCHES [Cc][Ll][Aa][Nn][Gg]) 
        OR (CMAKE_CXX_COMPILER_ID MATCHES [Gg][Nn][Uu]))
       AND (CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg]))
        message(STATUS "Using sanitizers for leak and address on ${CMAKE_CXX_COMPILER_ID}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,leak")
        target_link_libraries(${TESTPROJ} PRIVATE -fsanitize=address,leak)
        message(STATUS "Enable instrumentation for code coverage")
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
        #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-instr-generate")
        #target_compile_options(${TESTPROJ} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        #target_link_options(${TESTPROJ} PRIVATE -fprofile-instr-generate) 
        target_compile_options(${TESTPROJ} PRIVATE -coverage)
        target_link_options(${TESTPROJ} PRIVATE -coverage)
    endif()

    if( CMAKE_BUILD_TYPE MATCHES [Dd][Ee][Bb][Uu][Gg])
        add_compile_definitions(${TESTPROJ} PRIVATE DEBUG=1 _DEBUG=1)
    endif()

    # Dependencies
    find_package(OpenSSL 3.0 REQUIRED)
    target_include_directories(${TESTPROJ} PRIVATE ${OPENSSL_INCLUDE_DIRS})
    target_link_libraries(${TESTPROJ} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

    cpmaddpackage("gh:google/googletest#v1.15.2")
    target_link_libraries(${TESTPROJ} PRIVATE GTest::gtest_main RunOnEnd::RunOnEnd StringHelpers::StringHelpers)


    include(GoogleTest)
    gtest_discover_tests(${TESTPROJ} XML_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/tests/results")

    include(CTest)
    message(STATUS "  Finished configuring for ${PROJECT_NAME} -- ${PROJECT_NAME}_BUILD_TESTS = ${${PROJECT_NAME}_BUILD_TESTS}")
endif()
