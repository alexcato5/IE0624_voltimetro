#include <PCD8544.h>

/*
 * To use this sketch, connect the eight pins from your LCD like thus:
 *
 * Pin 1 -> +3.3V (rightmost, when facing the display head-on)
 * Pin 2 -> Arduino digital pin 3
 * Pin 3 -> Arduino digital pin 4
 * Pin 4 -> Arduino digital pin 5
 * Pin 5 -> Arduino digital pin 7
 * Pin 6 -> Ground
 * Pin 7 -> 10uF capacitor -> Ground
 * Pin 8 -> Arduino digital pin 6
 *
 * Since these LCDs are +3.3V devices, you have to add extra components to
 * connect it to the digital pins of the Arduino (not necessary if you are
 * using a 3.3V variant of the Arduino, such as Sparkfun's Arduino Pro).
 */

static PCD8544 lcd;

// Definición de los cuatro voltajes por medir
static float volt1 = 0.0;
static float volt2 = 0.0;
static float volt3 = 0.0;
static float volt4 = 0.0;

// Definición de los voltajes pico para el modo AC
float vp1 = 0.0;
float vp2 = 0.0;
float vp3 = 0.0;
float vp4 = 0.0;

// Definición de variable para cambio de modo DC/AC
bool modo_AC = false;

// Definición de variable para activar o desactivar la transmisión USART
bool transmitir = false;

// Definición de variable para refrescamiento de pantalla al cambiar de modo
bool refrescar = false;


// Función de configuración
void setup() {
  // Se define la resolución de la pantalla
  lcd.begin(84, 48);

  // Se define el baudrate para el puerto serial USART
  Serial.begin(9600, SERIAL_8N1);

  // Se definen los pines medidores de voltaje como entradas
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  // Se definen los pines encargados de los leds de seguridad como salidas
  pinMode(8, OUTPUT); 
  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT); 
  pinMode(11, OUTPUT);
}


// Función que actualiza los valores en pantalla
void refrescarPantalla(){
  // Mostrar el título
  lcd.setCursor(0, 0);
  lcd.print("  VOLTIMETRO");

  // Mostrar el voltaje 1
  lcd.setCursor(0, 2);
  lcd.print("V1 ");
  lcd.print(volt1);
  if (modo_AC) { lcd.print(" Vrms"); }
  else { lcd.print(" V"); }

  // Mostrar el voltaje 2
  lcd.setCursor(0, 3);
  lcd.print("V2 ");
  lcd.print(volt2);
  if (modo_AC) { lcd.print(" Vrms"); }
  else { lcd.print(" V"); }

  // Mostrar el voltaje 3
  lcd.setCursor(0, 4);
  lcd.print("V3 ");
  lcd.print(volt3);
  if (modo_AC) { lcd.print(" Vrms"); }
  else { lcd.print(" V"); }

  // Mostrar el voltaje 4
  lcd.setCursor(0, 5);
  lcd.print("V4 ");
  lcd.print(volt4);
  if (modo_AC) { lcd.print(" Vrms"); }
  else { lcd.print(" V"); }
}


// Función que envía los datos actuales mediante USART
void enviarDatos(){
  Serial.print(volt1);
  Serial.flush();
  Serial.print(volt2);
  Serial.flush();
  Serial.print(volt3);
  Serial.flush();
  Serial.print(volt4);
  Serial.flush();
  Serial.print(modo_AC);
  Serial.flush();
}


// Función para medir el valor Vp en AC
void medirAC(){
  vp1 = 0.0;
  vp2 = 0.0;
  vp3 = 0.0;
  vp4 = 0.0;

  for(int i = 0; i < 100; i++){ // Se realizan 100 mediciones
    // Primero se leen los pines
    int v1_tmp = analogRead(A0);    
    int v2_tmp = analogRead(A1);
    int v3_tmp = analogRead(A2);
    int v4_tmp = analogRead(A3);

    // Se comparan sus valores con los anteriores
    if(v1_tmp > vp1) { vp1 = v1_tmp; }
    if(v2_tmp > vp2) { vp2 = v2_tmp; }
    if(v3_tmp > vp3) { vp3 = v3_tmp; }
    if(v4_tmp > vp4) { vp4 = v4_tmp; }

    delay(0.2); // 200 us de separación entre mediciones
  }
}


// Función para el control de los LEDs de alarma
void leds(){
  // Voltaje 1
  if(volt1 >= 20 || volt1 <= -20){
    digitalWrite(11, HIGH);
  } else {
    digitalWrite(11, LOW);
  }

  // Voltaje 2
  if(volt2 >= 20 || volt2 <= -20){
    digitalWrite(10, HIGH);
  } else {
    digitalWrite(10, LOW);
  }

  // Voltaje 3
  if(volt3 >= 20 || volt3 <= -20){
    digitalWrite(9, HIGH);
  } else {
    digitalWrite(9, LOW);
  }

  // Voltaje 4
  if(volt4 >= 20 || volt4 <= -20){
    digitalWrite(8, HIGH);
  } else {
    digitalWrite(8, LOW);
  }
  
}//fin void leds


void ledsAC(){
  // Voltaje 1
  if(volt1 >= 13.5){
    digitalWrite(11, HIGH);
  } else {
    digitalWrite(11, LOW);
  }

  // Voltaje 2
  if(volt2 >= 13.5){
    digitalWrite(10, HIGH);
  } else {
    digitalWrite(10, LOW);
  }

  // Voltaje 3
  if(volt3 >= 13.5){
    digitalWrite(9, HIGH);
  } else {
    digitalWrite(9, LOW);
  }

  // Voltaje 4
  if(volt4 >= 13.5){
    digitalWrite(8, HIGH);
  } else {
    digitalWrite(8, LOW);
  }
  
}//fin void ledsAC


// Ciclo central de operación
void loop() {
  // Se lee el pin 13 para verificar si se está operando en modo AC o DC
  
  if(digitalRead(13)){ 
    modo_AC = true;
    refrescar = true; 
  } else {
    modo_AC = false;
    if(refrescar){ lcd.clear(); }
    refrescar = false;
  }
  
  if (modo_AC){
    medirAC();
    volt1 = ( (vp1 - 511.0) * 48.0/1023.0) / sqrt(2); 
    volt2 = ( (vp2 - 511.0) * 48.0/1023.0) / sqrt(2);
    volt3 = ( (vp3 - 511.0) * 48.0/1023.0) / sqrt(2);
    volt4 = ( (vp4 - 511.0) * 48.0/1023.0) / sqrt(2);
    ledsAC();
    } else {
    volt1 = (511.0 - analogRead(A0)) * 48.0/1023.0; 
    volt2 = (511.0 - analogRead(A1)) * 48.0/1023.0;
    volt3 = (511.0 - analogRead(A2)) * 48.0/1023.0;
    volt4 = (511.0 - analogRead(A3)) * 48.0/1023.0;
    leds();
  }
  
  

  refrescarPantalla(); 

  transmitir = digitalRead(12);
  if (transmitir){ enviarDatos(); }
}

/* EOF - voltimetro.ino */