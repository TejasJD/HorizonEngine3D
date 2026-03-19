set(HORIZON_ENGINE_SRC_FILES
	"src/App.cpp"
	"src/Window.cpp"
	"src/Buffer.cpp"
	"src/RendererAPI.cpp"
	"src/Shader.cpp"
	"src/Utility.cpp"
	"src/VertexArray.cpp"
	"src/Texture.cpp"
  "src/Input.cpp"
	"src/Platform/WindowsOS.cpp"
) 

set(HORIZON_ENGINE_HDR_FILES
	"include/HorizonEngine/App.h"
	"include/HorizonEngine/Buffer.h"
	"include/HorizonEngine/RendererAPI.h"
	"include/HorizonEngine/Utility.h"
	"include/HorizonEngine/VertexArray.h"
	"include/HorizonEngine/Window.h"
	"include/HorizonEngine/Shader.h"
	"include/HorizonEngine/Texture.h"
	"include/HorizonEngine/Input.h"
	"include/HorizonEngine/KeyCodes.h"
	"include/HorizonEngine/MouseCodes.h"
	"include/HorizonEngine/Camera.h"
	"include/HorizonEngine/CameraController.h"
	"include/HorizonEngine/Layer.h"
	"include/HorizonEngine/Platform.h"
	"src/Platform/WindowsOS.h"
)

add_library(HorizonEngine ${HORIZON_ENGINE_SRC_FILES} ${HORIZON_ENGINE_HDR_FILES})

set_property(TARGET HorizonEngine PROPERTY CXX_STANDARD 20)
target_include_directories(HorizonEngine
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

find_package(imgui CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED) 
find_package(Stb REQUIRED)

set(HORIZON_PUBLIC_EXT_DEPS
	glm::glm-header-only
	spdlog::spdlog_header_only
	imgui::imgui
)

set(HORIZON_PRIVATE_EXT_DEPS
	glfw
	glad::glad
)

target_link_libraries(HorizonEngine PUBLIC ${HORIZON_PUBLIC_EXT_DEPS})
target_link_libraries(HorizonEngine PRIVATE ${HORIZON_PRIVATE_EXT_DEPS})
target_include_directories(HorizonEngine PRIVATE ${Stb_INCLUDE_DIR})
