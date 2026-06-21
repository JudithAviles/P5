#include <iostream>
#include "instrument_dumb.h"
#include "instrument_clar.h"
#include "instrument_seno.h"
#include "instrument_ext.h"
#include "instrument_samp.h"
#include "instrument_fm.h"

/*
  For each new instrument:
  - Add the header in this file
  - Add the call to the constructor in get_instrument() (also in this file)
  - Add the source file to src/meson.build
*/

using namespace std;

namespace upc {
  Instrument * get_instrument(const string &name,
			      const string &parameters) {
    Instrument * pInst = 0;
    //    cout << name << ": " << parameters << endl;
    if (name == "InstrumentDumb") {
      pInst = (Instrument *) new InstrumentDumb(parameters);
    } else if (name == "InstrumentClar") {
      pInst = (Instrument *) new InstrumentClar(parameters);
    } else if (name == "InstrumentSeno") {
      pInst = (Instrument *) new InstrumentSeno(parameters);
    } else if (name == "InstrumentExt") {
      pInst = (Instrument *) new InstrumentExt(parameters);
    } else if (name == "InstrumentSamp") {
      pInst = (Instrument *) new InstrumentSamp(parameters);
    } else if (name == "InstrumentFM") {
      pInst = (Instrument *) new InstrumentFM(parameters);
    }
    
    return pInst;
  }
}
