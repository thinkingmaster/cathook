target_sources(cathook PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/BaseMenuObject.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/Box.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/CenteredString.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/Checkbox.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/Container.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/GUI.h"
        "${CMAKE_CURRENT_LIST_DIR}/Input.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/Label.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/List.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/menu.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/TabButton.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/TabContainer.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/TabSelection.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/Variable.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/VariableInput.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/VariableSwitch.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/Window.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/ZeroKernelWindow.hpp"
        )

target_include_directories(cathook PRIVATE "${CMAKE_CURRENT_LIST_DIR}")