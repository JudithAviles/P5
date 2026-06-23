#include <iostream>
#include <math.h>
<<<<<<< HEAD
=======
#include <algorithm>
#include <sndfile.h>
>>>>>>> 1592f1d31581fa3c8f2307bd1c6a4f46bbecc58c
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
  KeyValue kv(param);
  static string kv_null;
  std::string file_name = "piano-trident.wav"; //Default value
  std::string tmp = kv("File");
  if (tmp != kv_null) {
    file_name = tmp;
    file_name.erase(remove(file_name.begin(), file_name.end(), '"'), file_name.end());
  }

  SNDFILE *sndfile_in;
  SF_INFO sf_info;
  sndfile_in = sf_open(file_name.c_str(), SFM_READ, &sf_info);
  if (sndfile_in == 0) {
    cerr << "Error: no se puede leer el fichero " << file_name << endl;
    throw -1;
  }
  tbl.resize(sf_info.frames);
  if (sf_info.channels == 1) {
    sf_read_float(sndfile_in, tbl.data(), tbl.size());
  } else {
    vector<float> buf(sf_info.frames * sf_info.channels);
    sf_read_float(sndfile_in, buf.data(), buf.size());
    for (unsigned int i = 0; i < sf_info.frames; ++i) {
      float sum = 0;
      for (int c = 0; c < sf_info.channels; ++c)
        sum += buf[i * sf_info.channels + c];
      tbl[i] = sum / sf_info.channels;
    }
  }
  sf_close(sndfile_in);
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
    unsigned int idx = (unsigned int) phase;
    if (idx >= tbl.size()) {
      x[i] = 0;
      adsr.end();
      bActive = false;
      continue;
    }
    x[i] = A * tbl[idx];
    phase += step;
  }
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}