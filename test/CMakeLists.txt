set(GTEST_VERSION "1.14.0")

SET(GTest_DIR ${PROJECT_SOURCE_DIR}/lib/googletest-${GTEST_VERSION}/lib/cmake/GTest)
FIND_PACKAGE(GTest CONFIG REQUIRED)

INCLUDE_DIRECTORIES(
   ${PROJECT_SOURCE_DIR}/lib/googletest-${GTEST_VERSION}/include
   ${PROJECT_SOURCE_DIR}/src/common
   ${PROJECT_SOURCE_DIR}/src/gui
)

AUX_SOURCE_DIRECTORY(. SRCS)
QT_ADD_EXECUTABLE(${PROJECT_NAME}_ut
   MANUAL_FINALIZATION
      ${SRCS}
      ${QT_RESOURCES_FILE}
)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}_ut
   PRIVATE
      ${PROJECT_SOURCE_DIR}/src/common
      ${PROJECT_SOURCE_DIR}/src/gui
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}_ut
   PRIVATE
      pthread
      GTest::gtest
      GTest::gmock
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

QT_FINALIZE_EXECUTABLE(${PROJECT_NAME}_ut)
