#include <iostream>
#include <math.h>
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
  // Cambiar file_name a nombre del archivo con la tabla externa
  KeyValue kv(param);
  std::string file_name = "Ext_table.wav"; //Default value
  static string kv_null;
  if((file_name = kv("file")) == kv_null) {
    cerr << "Error: no se ha encontrado el campo con el fichero de la señal para un instrumento FicTabla" << endl;
    throw -1;
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
    if (phase > tbl.size()){
      x[i] = 0;
      adsr.end();
    }
    phase += step;
    while(phase >= tbl.size()-0.5){
      phase -= tbl.size();
    }
  }
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}