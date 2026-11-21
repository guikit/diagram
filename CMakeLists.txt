cmake_minimum_required(VERSION 3.21)
project(guikit_diagram VERSION 1.0.0 LANGUAGES CXX)

if (PROJECT_IS_TOP_LEVEL)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

option(GUIKIT_DIAGRAM_BUILD_TESTS "Build GuiKit Diagram unit tests" ${PROJECT_IS_TOP_LEVEL})
option(GUIKIT_DIAGRAM_INSTALL "Enable CMake install targets for guikit_diagram" ${PROJECT_IS_TOP_LEVEL})

if (GUIKIT_DIAGRAM_BUILD_TESTS)
    enable_testing()
endif()

if (NOT TARGET guikit::core)
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/guikit/CMakeLists.txt")
        add_subdirectory(third_party/guikit EXCLUDE_FROM_ALL)
    else()
        find_package(guikit REQUIRED)
    endif()
endif()

add_library(guikit_diagram STATIC
    src/guikit/diagram/bpmn_editor.cpp
)
add_library(guikit::diagram ALIAS guikit_diagram)
set_target_properties(guikit_diagram PROPERTIES EXPORT_NAME diagram)

target_compile_features(guikit_diagram PUBLIC cxx_std_20)
target_include_directories(guikit_diagram PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(guikit_diagram PUBLIC guikit::core)

if (COMMAND guikit_enforce_guidelines)
    guikit_enforce_guidelines(guikit_diagram
        HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include"
        STANDALONE_HEADERS ${GUIKIT_DIAGRAM_BUILD_TESTS}
        TEST_NAME guikit_diagram_header_purity_scan
    )
endif()

if (GUIKIT_DIAGRAM_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(TARGETS guikit_diagram
        EXPORT guikitDiagramTargets
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    )
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/guikit"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )
    install(EXPORT guikitDiagramTargets
        FILE guikitDiagramTargets.cmake
        NAMESPACE guikit::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/guikit_diagram"
    )
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/guikit_diagramConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/guikit_diagramConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/guikit_diagram"
    )
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/guikit_diagramConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/guikit_diagramConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/guikit_diagramConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/guikit_diagram"
    )
endif()

if (GUIKIT_DIAGRAM_BUILD_TESTS AND TARGET guikit::guikit)
    add_executable(test_diagram
        tests/test_diagram.cpp
    )
    target_link_libraries(test_diagram PRIVATE guikit::diagram guikit::guikit)
    if (COMMAND guikit_enforce_guidelines)
        guikit_enforce_guidelines(test_diagram)
    endif()
    add_test(NAME diagram COMMAND test_diagram)
endif()
