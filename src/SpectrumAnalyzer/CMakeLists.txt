add_library(SpectrumAnalyzer STATIC
    SpectrumAnalyzer.h
    SpectrumAnalyzer.cpp
)

target_link_libraries(SpectrumAnalyzer
    ${QT_CORE}
    ${QT_WIDGETS}
)










