#include <iostream>
#include <math.h>
#include "instrument_samp.h"
#include "keyvalue.h"
#include "wavfile_mono.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

//param = cadena de parametros del instrument
InstrumentSamp::InstrumentSamp(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);

  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */
  // Requiere obtener un fichero wav con grabación de una nota entera
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


void InstrumentSamp::command(long cmd, long note, long vel) {
  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();
    // Recorremos la tabla muestra a muestra independientemente del pitch (percussión) (sampler)
    // Para instrumentos melódicos deberíamos mirar como hacer para cambiar el pitch de la ntoa según la que ya se tiene o tener más de una tabla
    this->phase = 0;
    this->step = 1;
  }
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    // Sin efecto, el final de la nota se produce cuando llegamos al final de la tabla
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    // Sin efecto, el final de la nota se produce cuando llegamos al final de la tabla
  }
}


const vector<float> & InstrumentSamp::synthesize() {
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