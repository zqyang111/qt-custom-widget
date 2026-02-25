add_library(DigitalWaveform STATIC
    DigitalWaveform.h
    DigitalWaveform.cpp
)

target_link_libraries(DigitalWaveform
    ${QT_CORE}
    ${QT_WIDGETS}
)










