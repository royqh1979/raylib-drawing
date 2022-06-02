project(raylib-drawing2d )
set(PROJECT_VERSION 1.0.0)
set(API_VERSION 100)

find_package(raylib QUIET)

include(GNUInstallDirs)

set(rturtle_public_headers
    rturtle.h
    )

set(rturtle_sources
    rturtle.cpp
    )

set(rdrawing_public_headers
    rdrawing.h
    )

set(rdrawing_sources
    rdrawing.cpp
    )

add_library(rturtle ${rturtle_sources} ${rturtle_public_headers})
add_library(rdrawing ${rdrawing_sources} ${rdrawing_public_headers})

set_target_properties(rturtle PROPERTIES
                      PUBLIC_HEADER "${rturtle_public_headers}"
                      VERSION ${PROJECT_VERSION}
                      SOVERSION ${API_VERSION}
                      )

set_target_properties(rdrawing PROPERTIES
                      PUBLIC_HEADER "${rdrawing_public_headers}"
                      VERSION ${PROJECT_VERSION}
                      SOVERSION ${API_VERSION}
                      )
