CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

SET (CMAKE_CXX_STANDARD 11)
SET (CMAKE_CXX_STANDARD_REQUIRED ON)
SET (CMAKE_CXX_EXTENSIONS OFF)

IF (MSVC)
    SET (CMAKE_CXX_FLAGS "/Wall")
ELSE (MSVC)
    SET (CMAKE_CXX_FLAGS "-Wall -Wextra")
ENDIF (MSVC)

SET (SOURCES
  "dsp/polynomials.cc"
  "Amp.cc"
  "AutoFilter.cc"
  "CabIII.cc"
  "CabIV.cc"
  "Chorus.cc"
  "Click.cc"
  "Compress.cc"
  "Eq.cc"
  "Fractals.cc"
  "interface.cc"
  "Noisegate.cc"
  "Pan.cc"
  "Phaser.cc"
  "Reverb.cc"
  "Saturate.cc"
  "Scape.cc"
  "Sin.cc"
  "ToneStack.cc"
  "White.cc")

SET (HEADERS
  "dsp/Butterworth.h"
  "dsp/ChebyshevPoly.h"
  "dsp/complex.h"
  "dsp/Compress.h"
  "dsp/Delay.h"
  "dsp/Eq.h"
  "dsp/FIR.h"
  "dsp/FPTruncateMode.h"
  "dsp/IIR1.h"
  "dsp/IIR2.h"
  "dsp/Lorenz.h"
  "dsp/Oversampler.h"
  "dsp/polynomials.h"
  "dsp/RBJ.h"
  "dsp/RMS.h"
  "dsp/Roessler.h"
  "dsp/sinc.h"
  "dsp/Sine.h"
  "dsp/SVF.h"
  "dsp/TDFII.h"
  "dsp/ToneStack.h"
  "dsp/util.h"
  "dsp/v4f_FIR.h"
  "dsp/v4f.h"
  "dsp/v4f_IIR2.h"
  "dsp/White.h"
  "dsp/windows.h"
  "Amp.h"
  "AutoFilter.h"
  "CabIIIModels.h"
  "CabIV_64_128.h"
  "Chorus.h"
  "Click.h"
  "Compress.h"
  "Eq.h"
  "Fractals.h"
  "Noisegate.h"
  "Pan.h"
  "Phaser.h"
  "Reverb.h"
  "Saturate.h"
  "Scape.h"
  "Sin.h"
  "ToneStack.h"
  "White.h")

ADD_LIBRARY ("caps" SHARED ${SOURCES} ${HEADERS})

