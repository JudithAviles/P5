#include <iostream>
#include <math.h>
#include "instrument_perc.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

InstrumentPerc::InstrumentPerc(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  KeyValue kv(param);
  int N;

  if (!kv.to_int("N",N))
    N = 40;
  
  tbl.resize(N);
  float phase = 0, step = 2 * M_PI /(float) N;
  for (int i=0; i < N ; ++i) {
    tbl[i] = sin(phase);
    phase += step;
  }
}


void InstrumentPerc::command(long cmd, long note, long vel) {
  if (cmd == 9) {
    bActive = true;
    adsr.start();
	  A = vel / 127.;
    this->step = 440*pow(2, (note-69)/12.)*tbl.size()/SamplingRate;
  }
  else if (cmd == 8) {
    adsr.stop();
  }
  else if (cmd == 0) {
    adsr.end();
  }
}


const vector<float> & InstrumentPerc::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  for (unsigned int i=0; i<x.size(); ++i) {
    x[i] = A * tbl[(int) phase+0.5];
    phase += step;
    while(phase >= tbl.size()-0.5){
      phase -= tbl.size();
    }
  }
  adsr(x);

  return x;
}
