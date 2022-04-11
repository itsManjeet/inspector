cmake_minimum_required(VERSION 3.10)
project(inspector)

find_package(PkgConfig)
pkg_check_modules(LIBNOTIFY libnotify REQUIRED)

pkg_check_modules(APPIMAGE libappimage REQUIRED)

find_package(YAML-CPP REQUIRED)

file(GLOB_RECURSE Inspector_Src "src/*.cc" "src/*.hh")
add_executable(inspector ${Inspector_Src})

include_directories(${LIBNOTIFY_INCLUDE_DIRS} ${YAML_CPP_INCLUDE_DIRS} ${APPIMAGE_INCLUDE_DIRS})
target_link_libraries(inspector ${LIBNOTIFY_LIBRARIES} ${YAML_CPP_LIBRARIES} ${APPIMAGE_LIBRARIES})

install(TARGETS inspector 
    RUNTIME DESTINATION bin)

install(FILES inspector.service
    DESTINATION lib/systemd/user)