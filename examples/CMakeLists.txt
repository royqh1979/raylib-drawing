
add_executable(polygon polygon.c)
add_executable(testrturtle testrturtle.c)
target_link_libraries(polygon rdrawing raylib opengl32 winmm)
target_link_libraries(testrturtle rturtle rdrawing raylib opengl32 winmm)


