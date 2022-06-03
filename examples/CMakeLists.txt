
add_executable(polygon polygon.c)
target_link_libraries(polygon rdrawing raylib opengl32 winmm)

add_executable(testrdraw testrdraw.c)
target_link_libraries(testrdraw rdrawing raylib opengl32 winmm)

add_executable(testrturtle testrturtle.c)
target_link_libraries(testrturtle rturtle rdrawing raylib opengl32 winmm)


