#include <iostream>
#include <math.h>
#include "instrument_plano.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

InstrumentPlano::InstrumentPlano(const std::string &param) 
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
  // Si en vez de utilizar un sinusoide utilizamos una función de sierra o otras conseguimos diferentes efectos
  tbl.resize(N);
  float phase = 0, step = 2 * M_PI /(float) N;
  for (int i=0; i < N ; ++i) {
    tbl[i] = sin(phase);
    phase += step;
  }
}


void InstrumentPlano::command(long cmd, long note, long vel) {
  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();
    // Indicamos como queremos que recorra la tabla --> Que nota debe tocar
    // Por ahora, para frecuencias múltiples decimales (no coincide exactamente con las muestras que tenemos en la tabla)
    // Utilizaremos la muestra más cercana a la que relamente queremos
    // Para mejorar el código podríamos interpolar para obtener una mejor muestra
	  A = vel / 127.;
    //this->phase = 0;
    this->step = 440*pow(2, (note-69)/12.)*tbl.size()/SamplingRate;
  }
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    adsr.end();
  }
}


const vector<float> & InstrumentPlano::synthesize() {
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
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}
