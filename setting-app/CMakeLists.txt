SET(SETTING_SRCS
    src/ttssmt_setting_main.c
)

INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs_setting REQUIRED 
	appcore-efl elementary ecore-imf eina ecore ecore-input evas edje glib-2.0
	capi-base-common aul capi-appfw-application efl-extension vconf vconf-internal-keys capi-web-url-download libxml-2.0
)

FOREACH(flag ${pkgs_setting_CFLAGS})
    SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
    SET(EXTRA_CXXFLAGS "${EXTRA_CXXFLAGS} ${flag}")
ENDFOREACH(flag)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -Wall -fPIE")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXXFLAGS} -Wall -fPIE")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g -Wall -fPIE")
SET(CMAKE_C_FLAGS_RELEASE "-O2 -Wall -fPIE")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -fPIE")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -g -fPIE")
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed -pie")

SET(BINDIR "${APP_INSTALL_PREFIX}/bin")

ADD_EXECUTABLE("${PROJECT_NAME}-setting" ${SETTING_SRCS})
TARGET_LINK_LIBRARIES("${PROJECT_NAME}-setting" ${pkgs_setting_LDFLAGS})
INSTALL(TARGETS "${PROJECT_NAME}-setting" DESTINATION ${BINDIR})

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/org.tizen.ttssmt-setting.xml DESTINATION ${APP_MANIFESTDIR})
