set(SOURCES
    main.cpp
    display.cpp
    game.cpp
    texture.cpp
    quad.cpp
    shaderloading.cpp
    quadshader.cpp
    raytracer.cpp
    camera.cpp
    scene.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} m ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw)

install(TARGETS ${PROJECT_NAME} DESTINATION ${TRAYRACER_INSTALL_BIN_DIR})
