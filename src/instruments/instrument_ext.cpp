#include <iostream>
#include <math.h>
#include <algorithm>
#include "instrument_ext.h"
#include "keyvalue.h"
#include "wavfile_mono.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

//param = cadena de parametros del instrument
InstrumentExt::InstrumentExt(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */
  KeyValue kv(param);
  static string kv_null;
  std::string file_name = "Ext_table.wav"; //Default value
  std::string tmp = kv("File");
  if (tmp != kv_null) {
    file_name = tmp;
    file_name.erase(remove(file_name.begin(), file_name.end(), '"'), file_name.end());
  }

  unsigned int fm;
  if (readwav_mono(file_name,fm,tbl) < 0) {
    cerr << "Error: no se puede leer el fichero " << file_name << " para un instrumento FicTabla" << endl;
    throw -1;
  }
}


void InstrumentExt::command(long cmd, long note, long vel) {
  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();
    A = vel / 127.;
    phase = 0;
    this->step = 440*pow(2, (note-69)/12.)*tbl.size()/SamplingRate;
  }
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    adsr.end();
  }
}


const vector<float> & InstrumentExt::synthesize() {
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
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}