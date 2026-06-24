#ifndef INSTRUMENT_SAMP
#define INSTRUMENT_SAMP

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentSamp: public upc::Instrument {
    EnvelopeADSR adsr;
    float phase, step;
	float A;
  int melodic;
    std::vector<float> tbl;
  public:
    InstrumentSamp(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif
