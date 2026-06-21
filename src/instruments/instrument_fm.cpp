#include <iostream>
#include <math.h>
#include "instrument_fm.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

InstrumentFM::InstrumentFM(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */
  KeyValue kv(param);

  if (!kv.to_float("I",I))
    I = 1; //default value
  if (!kv.to_float("fm",fm))
    fm = 100; //default value

  // modulating freq fm
  // d deviation, d = N1*fm = (N2-N1)*fm
  
  //Create a tbl with one period of a sinusoidal wave
  /*tbl.resize(N);
  phase = 0;
  float step = 2 * M_PI / (float) N;
  for (int i=0; i < N ; ++i) {
    tbl[i] = sin(phase);
    phase += step;
  }
  phase = 0;*/
}


void InstrumentFM::command(long cmd, long note, long vel) {
  if (cmd == 9) {
    bActive = true;
    adsr.start();
    A = vel / 127.;

    phase1 = 0;
    phase2 = 0;
    float f0 = 440*pow(2, (note-69)/12.)/SamplingRate;
    step1 = 2*M_PI*f0; //carrier frequency step in rad/s
    step2 = 2*M_PI*fm/SamplingRate; //modulation frequency step in rad/s
  }
  else if (cmd == 8) {
    adsr.stop();
  }
  else if (cmd == 0) {
    adsr.end();
  }
}


const vector<float> & InstrumentFM::synthesize() {
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
