
# enable/disable cmake debug messages related to this pile
set (REGEXLITE_DEBUG_MSG ON)

# make sure support code is present; no harm
# in including it twice; the user, however, should have used
# pileInclude() from pile_support.cmake module.
include(pile_support)

# initialize this module
macro    (regexliteInit
          ref_cnt_use_mode)

    # default name
    if (NOT REGEXLITE_INIT_NAME)
        set(REGEXLITE_INIT_NAME "regexlite")
    endif ()

    # compose the list of headers and sources
    set(REGEXLITE_HEADERS
        "regexlite.h")
    set(REGEXLITE_SOURCES
        "regexlite.cc")

    pileSetSources(
        "${REGEXLITE_INIT_NAME}"
        "${REGEXLITE_HEADERS}"
        "${REGEXLITE_SOURCES}")

    pileSetCommon(
        "${REGEXLITE_INIT_NAME}"
        "0;0;1;d"
        "ON"
        "${ref_cnt_use_mode}"
        ""
        "category1"
        "tag1;tag2")

endmacro ()
