#include <iostream>
#include <math.h>
#include "instrument_fm_N1N2.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

InstrumentFM_N1N2::InstrumentFM_N1N2(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);
  KeyValue kv(param);

  if (!kv.to_float("I",I))
    I = 1; //default value
  if (!kv.to_float("N1",N1))
    N1 = 1; //default value
  if (!kv.to_float("N2",N2))
    N2 = 1; //default value
}


void InstrumentFM_N1N2::command(long cmd, long note, long vel) {
  if (cmd == 9) {
    bActive = true;
    adsr.start();
    A = vel / 127.;
    
    phase1 = 0;
    phase2 = 0;
    float f0 = 440*pow(2, (note-69)/12.)/SamplingRate;
    step1 = 2*M_PI*f0*N1; //carrier frequency step in rad/s
    step2 = 2*M_PI*f0*N2; //modulation frequency step in rad/s
  }
  else if (cmd == 8) {
    adsr.stop();
  }
  else if (cmd == 0) {
    adsr.end();
  }
}


const vector<float> & InstrumentFM_N1N2::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  for (unsigned int i=0; i<x.size(); ++i) {
    x[i] = A * sin(phase1 + I*sin(phase2));
    phase1 += step1;
    phase2 += step2;
    while(phase1 >= M_PI){
      phase1 -= 2*M_PI;
    }
    while(phase2 >= M_PI){
      phase2 -= 2*M_PI;
    }
  }
  adsr(x);

  return x;
}
