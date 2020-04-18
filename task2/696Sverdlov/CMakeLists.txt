set(SRC_FILES
        KleinBottle.cpp
        common/Application.cpp
        common/DebugOutput.cpp
        common/Camera.cpp
        common/Mesh.cpp
        common/ShaderProgram.cpp
        common/Texture.cpp
        common/Framebuffer.cpp
)

set(HEADER_FILES
        common/Application.hpp
        common/DebugOutput.h
        common/Camera.hpp
        common/LightInfo.hpp
        common/Mesh.hpp
        common/ShaderProgram.hpp
        common/Texture.hpp
        common/Framebuffer.hpp
)

MAKE_OPENGL_TASK(696Sverdlov 2 "${SRC_FILES}")
target_include_directories(696Sverdlov2 PUBLIC common)

if (UNIX)
    target_link_libraries(696Sverdlov2)
endif()
