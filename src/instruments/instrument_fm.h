#ifndef INSTRUMENT_FM
#define INSTRUMENT_FM

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentFM: public upc::Instrument {
    EnvelopeADSR adsr;
    float phase1, phase2, step1, step2, I, fm;
	float A;
    std::vector<float> tbl;
  public:
    InstrumentFM(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif
