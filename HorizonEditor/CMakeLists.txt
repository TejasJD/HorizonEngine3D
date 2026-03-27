set(HORIZON_EDITOR_SRC_FILES
	"AssetUITool.cpp"
	"EditorCameraTool.cpp"
	"EditorMenu.cpp"
	"EditorUI.cpp"
	"Entrypoint.cpp"
	"GameLayer.cpp"
	"HorizonEditor.cpp"
)

set(HORIZON_EDITOR_HDR_FILES
	"AssetUITool.h"
	"EditorCameraTool.h"
	"EditorUI.h"
	"EditorUITool.h"
	"GameLayer.h"
	"HorizonEditor.h"
	"ImGuiDemoWindowTool.h"
)

add_executable(HorizonEditor ${HORIZON_EDITOR_SRC_FILES} ${HORIZON_EDITOR_HDR_FILES}) 

set_property(TARGET HorizonEditor PROPERTY CXX_STANDARD 20)

set(HORIZON_EDITOR_PRIVATE_DEPENDENCIES
	HorizonEngine
)

target_link_libraries(HorizonEditor PRIVATE ${HORIZON_EDITOR_PRIVATE_DEPENDENCIES})

set(HORIZON_RESOURCE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(HORIZON_RESOURCE_BIN_DIR "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_target(CopyResources ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
	"${HORIZON_RESOURCE_SRC_DIR}" "${HORIZON_RESOURCE_BIN_DIR}"
	DEPENDS ${HORIZON_RESOURCE_SRC_DIR}
	COMMENT "Copying updated resources...")

add_dependencies(HorizonEngine CopyResources)
