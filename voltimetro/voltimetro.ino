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
bool modo_AC = false;

void setup() {
  // Se define la resolución de la pantalla
  lcd.begin(84, 48);

  // Se define el baudrate para el puerto serial USART
  Serial.begin(9600, SERIAL_8N1);

  // Se definen los pines medidores de voltaje como entrada
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

}

void refrescarPantalla(){
  // Mostrar el título
  lcd.setCursor(0, 0);
  lcd.print("  VOLTIMETRO");

  // Mostrar el voltaje 1
  lcd.setCursor(0, 2);
  lcd.print("V1: ");
  lcd.print(volt1);
  if (modo_AC) { lcd.print(" Vrms"); }
  else { lcd.print(" V"); }

  // Mostrar el voltaje 2
  lcd.setCursor(0, 3);
  lcd.print("V2: ");
  lcd.print(volt2);
  if (modo_AC) { lcd.print(" Vrms"); }
  else { lcd.print(" V"); }

  // Mostrar el voltaje 3
  lcd.setCursor(0, 4);
  lcd.print("V3: ");
  lcd.print(volt3);
  if (modo_AC) { lcd.print(" Vrms"); }
  else { lcd.print(" V"); }

  // Mostrar el voltaje 4
  lcd.setCursor(0, 5);
  lcd.print("V4: ");
  lcd.print(volt4);
  if (modo_AC) { lcd.print(" Vrms"); }
  else { lcd.print(" V"); }
}

void enviarDatos(){
  Serial.print(volt1);
  Serial.flush();
  Serial.print(volt2);
  Serial.flush();
  Serial.print(volt3);
  Serial.flush();
  Serial.print(volt4);
  Serial.flush();
}

void loop() {
  volt1 = -(analogRead(A0) - 512) * 48/1023; 
  volt2 = -(analogRead(A1) - 512) * 48/1023;
  volt3 = -(analogRead(A2) - 512) * 48/1023;
  volt4 = -(analogRead(A3) - 512) * 48/1023;
  refrescarPantalla();  
  enviarDatos();
}

/* EOF - voltimetro.ino */