AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} SRC)

ADD_LIBRARY(gui_static STATIC ${SRC})

TARGET_INCLUDE_DIRECTORIES(gui_static
   PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${PROJECT_SOURCE_DIR}/common
)

TARGET_LINK_LIBRARIES(gui_static
   PRIVATE
      Qt6::Core
      Qt6::Gui
      Qt6::Widgets
      common_static
)
