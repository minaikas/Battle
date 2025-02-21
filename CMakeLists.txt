cmake_minimum_required(VERSION 3.29 FATAL_ERROR)

set(PROJECT_NAME Battle)

project(${PROJECT_NAME})

# Настройка GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

# Добавляем GLFW как подкаталог (первая команда загружает cmake файл glfw)
add_subdirectory(external/glfw)
include_directories(external/glfw/include)

# Добавляем GLM
include_directories(external/glm)

# Добавляем исполняемый файл
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/Renderer/ShaderProgram.cpp
    src/Renderer/ShaderProgram.h
    src/Renderer/Texture2D.cpp
    src/Renderer/Texture2D.h
    src/Resources/ResourceManager.cpp
    src/Resources/ResourceManager.h
    src/Resources/stb_image.h
    src/Renderer/Sprite.cpp
    src/Renderer/Sprite.h
    )

# Устанавливаем стандарт C++17
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Линкуем GLFW
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

# Добавляем GLAD
add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

# Указываем путь для вывода
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

# Копирование ресурсов после сборки
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)