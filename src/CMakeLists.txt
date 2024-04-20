AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} SRCS)

ADD_SUBDIRECTORY(./common)
ADD_SUBDIRECTORY(./gui)

SET(QT_RESOURCES_FILE ${PROJECT_SOURCE_DIR}/resource/resource.qrc)

# QT根据.ts文件发布为.qm文件，保存在QM_FILES变量中
QT6_CREATE_TRANSLATION(QM_FILES ${PROJECT_SOURCE_DIR} ${TS_FILES} OPTIONS -source-language zh_CN -noobsolete)

QT_ADD_EXECUTABLE(${PROJECT_NAME}
   MANUAL_FINALIZATION
      ${SRCS}
      ${QT_RESOURCES_FILE}
      ${QM_FILES}
)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}
   PRIVATE
      ${PROJECT_SOURCE_DIR}/src/common
      ${PROJECT_SOURCE_DIR}/src/gui
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
   PRIVATE
      Qt6::Core
      Qt6::Gui
      Qt6::Widgets
      common_static
      gui_static
)

INSTALL(TARGETS ${PROJECT_NAME}
   RUNTIME DESTINATION bin                      # 可执行文件安装路径
   LIBRARY DESTINATION lib                      # 动态库安装路径
   ARCHIVE DESTINATION lib                      # 静态库安装路径
   PUBLIC_HEADER DESTINATION include            # 头文件安装路径
)

QT_FINALIZE_EXECUTABLE(${PROJECT_NAME})
