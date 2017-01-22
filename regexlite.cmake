
# enable/disable cmake debug messages related to this pile
set (REGEXLITE_DEBUG_MSG OFF)

# make sure support code is present; no harm
# in including it twice; the user, however, should have used
# pileInclude() from pile_support.cmake module.
include(pile_support)

# initialize this module
macro    (regexliteInit
          regexlite_use_mode)

    # default name
    if (NOT REGEXLITE_INIT_NAME)
        set(REGEXLITE_INIT_NAME "regexlite")
    endif ()

    find_package(ICU 57 REQUIRED
        COMPONENTS uc i18n)

    set(REGEXLITE_DEPENDENCIES
        ${ICU_UC_LIBRARIES}
        ${ICU_I18N_LIBRARIES})

    set(REGEXLITE_INCLUDES
        ${ICU_INCLUDE_DIRS})
    message(STATUS "ICU_ROOT = ${ICU_ROOT}")
    message(STATUS "ICU_ROOT = ${ICU_UC_LIBRARIES}")
    message(STATUS "ICU_INCLUDE_DIRS = ${ICU_INCLUDE_DIRS}")

    # compose the list of headers and sources
    set(REGEXLITE_HEADERS
        "regexlite.h")
    set(REGEXLITE_SOURCES
        "regexlite.cc"
        "drvsqlite.cc"
        "qsqlcachedresult.cpp"
        "sqlite/sqlite3.c")

    set(REGEXLITE_QT_MODS
        sql)

    pileSetSources(
        "${REGEXLITE_INIT_NAME}"
        "${REGEXLITE_HEADERS}"
        "${REGEXLITE_SOURCES}")

    pileSetCommon(
        "${REGEXLITE_INIT_NAME}"
        "0;0;1;d"
        "ON"
        "${regexlite_use_mode}"
        ""
        "database"
        "qt;GUI;sqlite")

endmacro ()
