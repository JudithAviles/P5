#ifndef INSTRUMENT_PLANO
#define INSTRUMENT_PLANO

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentPlano: public upc::Instrument {
    EnvelopeADSR adsr;
    float phase, step;
	float A;
    std::vector<float> tbl;
  public:
    InstrumentPlano(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif
