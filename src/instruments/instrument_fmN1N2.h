#ifndef INSTRUMENT_FM_N1N2
#define INSTRUMENT_FM_N1N2

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentFMN1N2: public upc::Instrument {
    EnvelopeADSR adsr;
    float phase1, phase2, step1, step2, I, N1, N2;
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
