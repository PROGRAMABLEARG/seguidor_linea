#include <MeMCore.h>

MeLineFollower sensorLinea(PORT_1);  // Seguidor de línea en el puerto 1
MeDCMotor motorIzq(M1);              // Motor izquierdo
MeDCMotor motorDer(M2);              // Motor derecho

const int VEL_RAPIDA = 80;  // Rueda externa en el giro
const int VEL_LENTA  = 0;   // Rueda interna en el giro

/* El motor M1 está montado al revés, por eso va en negativo para avanzar
void mover(int velIzq, int velDer) {
  motorIzq.run(-velIzq);
  motorDer.run(velDer);
}
*/
// Velocidad actual real de cada motor (persiste entre llamadas)
int velActualIzq = 0;
int velActualDer = 0;

// Máximo cambio de velocidad por ciclo. Menor = más suave, más lento en responder.
const int PASO_RAMPA = 8;

int suavizar(int actual, int objetivo) {
  if (objetivo > actual) {
    actual += PASO_RAMPA;
    if (actual > objetivo) actual = objetivo;
  } else if (objetivo < actual) {
    actual -= PASO_RAMPA;
    if (actual < objetivo) actual = objetivo;
  }
  return actual;
}

void mover(int velIzq, int velDer) {
  velActualIzq = suavizar(velActualIzq, velIzq);
  velActualDer = suavizar(velActualDer, velDer);

  motorIzq.run(-velActualIzq);
  motorDer.run(velActualDer);
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  int valor = sensorLinea.readSensors();

  // Solo usamos el sensor 1 (izquierdo):
  // valores 0 y 1 = sensor 1 sobre negro; 2 y 3 = sensor 1 sobre blanco
  bool sobreNegro = (valor == S1_IN_S2_IN || valor == S1_IN_S2_OUT);

  if (sobreNegro) {
    mover(VEL_LENTA, VEL_RAPIDA);   // Curva a la izquierda
    Serial.println("NEGRO -> izquierda");
  } else {
    mover(VEL_RAPIDA, VEL_LENTA);   // Curva a la derecha
    Serial.println("BLANCO -> derecha");
  }
}
