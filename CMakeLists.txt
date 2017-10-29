cmake_minimum_required(VERSION 2.8.3)
project(joybot)

find_package(catkin REQUIRED COMPONENTS
  roscpp geometry_msgs
)

set (wiringPi_include "/usr/local/include")

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES joybot
  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  ${wiringPi_include}
)

LINK_DIRECTORIES ("/usr/local/lib")

add_executable(${PROJECT_NAME}_node src/joybot_node.cpp)
target_link_libraries(${PROJECT_NAME}_node
  ${catkin_LIBRARIES} wiringPi pthread
)
