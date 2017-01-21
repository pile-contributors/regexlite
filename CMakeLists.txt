if (NOT REGEXLITE_BUILD_MODE)
    set (REGEXLITE_BUILD_MODE STATIC)
endif ()

include(pile_support)
pileInclude (regexlite)
regexliteInit(${REGEXLITE_BUILD_MODE})
