# cmake最低版本
cmake_minimum_required(VERSION 2.8)

# vitasdk工具链
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

## 项目参数
# 项目名称
project(Vita2d_Test)
# 项目cmake路径
include("${VITASDK}/share/vita.cmake" REQUIRED)

## 软件参数
# 软件名称
set(VITA_APP_NAME "Vita2d Test")
# 软件唯一ID，前四位配置为大写字母，后五位配置为数字
set(VITA_TITLEID  "VSDK00125")
# 软件版本
set(VITA_VERSION  "01.00")

# cmake的flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# 额外头文件
include_directories(
    src
)

# 额外库文件
link_directories(
)

## 构建与链接
# 源文件，所有需要编译的源文件都需要添加
add_executable(${PROJECT_NAME}
  src/main.cpp
  src/player.cpp
  src/block.cpp
)

# 库文件
target_link_libraries(${PROJECT_NAME}
    vita2d
    # 以下库文件为必须，勿删
    SceDisplay_stub
    SceCtrl_stub
    SceSysmodule_stub
    SceGxm_stub
    SceAppMgr_stub
    SceCommonDialog_stub
    # test
    freetype
    png
    z
    m
    pthread
)

## 构建PSVita可执行程序
vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
# 构建VPK安装程序，通过FILE方式可以向安装程序中添加额外文件
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  # LiveArea
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
  # 字体
  FILE assets/font.ttf assets/font.ttf
  # 地图
  FILE assets/map.csv assets/map.csv
  FILE assets/line.png assets/line.png
  FILE assets/delim.png assets/delim.png
)
