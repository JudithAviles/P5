#include <iostream>
#include <math.h>
#include "instrument_seno.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

InstrumentSeno::InstrumentSeno(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */
  KeyValue kv(param);
  int N;

  // Si no encuentra el valor de N en sus valores registrados se pone directamente en el valor 40
  if (!kv.to_int("N",N))
    N = 40; //default value
  
  //Create a tbl with one period of a sinusoidal wave
  tbl.resize(N);
  phase = 0;
  float step = 2 * M_PI / (float) N;
  for (int i=0; i < N ; ++i) {
    tbl[i] = sin(phase);
    phase += step;
  }
  phase = 0;
}


void InstrumentSeno::command(long cmd, long note, long vel) {
  if (cmd == 9) {
    bActive = true;
    adsr.start();
    A = vel / 127.;
    phase = 0;
    this->step = 440*pow(2, (note-69)/12.)*tbl.size()/SamplingRate;
  }
  else if (cmd == 8) {
    adsr.stop();
  }
  else if (cmd == 0) {
    adsr.end();
  }
}


const vector<float> & InstrumentSeno::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  for (unsigned int i=0; i<x.size(); ++i) {
    unsigned int idx0 = (unsigned int) phase;
    unsigned int idx1 = idx0 + 1;
    if (idx1 >= tbl.size())
      idx1 = 0;
    float frac = phase - idx0;
    x[i] = A * (tbl[idx0] + frac * (tbl[idx1] - tbl[idx0]));
    phase += step;
    while (phase >= tbl.size())
      phase -= tbl.size();
  }
  adsr(x);

  return x;
}
