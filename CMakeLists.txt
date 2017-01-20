if (NOT TEMPILE_BUILD_MODE)
    set (TEMPILE_BUILD_MODE STATIC)
endif ()

include(pile_support)
pileInclude (TemPile)
tempileInit(${TEMPILE_BUILD_MODE})
