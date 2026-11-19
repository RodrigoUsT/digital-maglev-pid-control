// Pines de conexión
const int PIN_SENSOR = A0;
const int PIN_ELECTROIMAN = 9;

// Configuración del PID
double kp = 40.0;
double ki = 0.6;
double kd = 1.5;

// Variables de control
int consigna = 38;
double errorActual = 0.0;
double errorPrevio = 0.0;
double integral = 0.0;
double derivada = 0.0;

// Temporización (T = 10 ms)
unsigned long tiempoPrevio = 0;
const unsigned long DT_MS = 10;

void setup() {
  pinMode(PIN_ELECTROIMAN, OUTPUT);
  Serial.begin(115200);
  tiempoPrevio = millis();
}

void loop() {
  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoPrevio >= DT_MS) {
    double dt = (tiempoActual - tiempoPrevio) / 1000.0;
    tiempoPrevio = tiempoActual;

    int posicionReal = analogRead(PIN_SENSOR);
    errorActual = posicionReal - consigna;

    integral += errorActual * dt;
    integral = constrain(integral, -100.0, 100.0);

    derivada = (errorActual - errorPrevio) / dt;

    double salidaPID = (kp * errorActual) + (ki * integral) + (kd * derivada);
    int pwmElectroiman = constrain((int)salidaPID, 0, 255);

    // Seguridad si el objeto sale del rango de lectura
    if (posicionReal > 600) {
      pwmElectroiman = 0;
      integral = 0.0;
    }

    analogWrite(PIN_ELECTROIMAN, pwmElectroiman);

    errorPrevio = errorActual;

    Serial.print(consigna);
    Serial.print(" ");
    Serial.print(posicionReal);
    Serial.print(" ");
    Serial.println(pwmElectroiman);
  }
}
