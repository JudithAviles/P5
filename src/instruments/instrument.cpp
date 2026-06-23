#include <iostream>
#include "instrument_dumb.h"
#include "instrument_clar.h"
#include "instrument_perc.h"
#include "instrument_plano.h"
#include "instrument_seno.h"
#include "instrument_ext.h"
#include "instrument_samp.h"
#include "instrument_fm.h"
#include "instrument_fm_N1N2.h"

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
    } else if (name == "InstrumentPerc") {
      pInst = (Instrument *) new InstrumentPerc(parameters);
    } else if (name == "InstrumentPlano") {
      pInst = (Instrument *) new InstrumentPlano(parameters);
    } else if (name == "InstrumentSeno") {
      pInst = (Instrument *) new InstrumentSeno(parameters);
    } else if (name == "InstrumentExt") {
      pInst = (Instrument *) new InstrumentExt(parameters);
    } else if (name == "InstrumentSamp") {
      pInst = (Instrument *) new InstrumentSamp(parameters);
    } else if (name == "InstrumentFM") {
      pInst = (Instrument *) new InstrumentFM(parameters);
    } else if (name == "InstrumentFMN1N2") {
      pInst = (Instrument *) new InstrumentFM_N1N2(parameters);
    }
    
    return pInst;
  }
}
