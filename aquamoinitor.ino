
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library
#include <EEPROM.h>


// posiciones de memoria eeprom
#define ee_peso 0
#define ee_unidad 1
#define ee_caudal 2

// declaraciones para el display i2c
#define I2C_ADDR    0x20  // Direccion I2C para PCF8574A que es el que lleva nuestra placa diseñada por MJKDZ
#define  LED_OFF  0
#define  LED_ON  1

//mjkdz i2c LCD board
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(I2C_ADDR, 4, 5, 6, 0, 1, 2, 3, 7, NEGATIVE);

// pines Encoder.
#define encoder0PinA  3
#define encoder0PinB  4
#define sw 5

// variables globales Encoder
int val;
byte encoder0Pos = 0;
byte encoder0PinALast = LOW;
byte n = LOW;

// Variables monitor
byte peso = 0;
byte unidad = 0; // 0=L ; 1=m3
byte caudal = 0; // 0= l/s;  ; 1= m3/h

//----------------------------------------------------------------------------------------------------
void setup() {


  byte bucle = true;

  // declaracion de los puertos
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  pinMode (sw, INPUT);

  // lectura de valores guardados en la ee_prom

  peso = EEPROM.read(ee_peso);
  unidad = EEPROM.read(ee_unidad);
  caudal = EEPROM.read(ee_caudal);

  // inicializar lcd
  lcd.begin (16, 2);
  lcd.setBacklight(LED_ON);

  pantallaInicio();
  do
  {

    bucle = muestraOpciones();
    if (bucle == false)
    {
      ajuste();
      EEPROM.write(ee_peso, peso);
      EEPROM.write(ee_unidad, unidad);
      EEPROM.write(ee_caudal, caudal);

    }
  } while (!bucle);
  lcd.clear();

}

//----------------------------------------------------------------------------------------------------
void loop() {
  lcd.setCursor (0, 0);
  lcd.print ("presione para menu");


  lcd.setCursor (0, 1);
  if (digitalRead (sw))
    lcd.print (" ON");
  else
    lcd.print ("OFF");

  lcd.setCursor (12, 1);
  lcd.print (millis() / 1000);

}// fin loop

//----------------------------------------------------------------------------------------------------
void leeEncoder (byte min, byte max)
{

  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {

      encoder0Pos--;
      if ((encoder0Pos < min) || (encoder0Pos > max)) encoder0Pos = min;
    } else {
      encoder0Pos++;
      if (encoder0Pos > max) encoder0Pos = max;
    }


  }
  encoder0PinALast = n;

}

//-------------------------------------------------------------------------------------------------
void ajuste ()
{
  n = 0;                    //inicializamos el contador del encoder
  byte n_max = 255;        // valor maximo para este caso
  boolean salida = true;   // variable control para salir de bucle


  lcd.clear();

  // ajuste peso de pulso
  encoder0Pos = 0;
  lcd.setCursor (0, 0);
  lcd.print ("Peso del pulso?");
  do
  {

    leeEncoder(0, 255);
    if (!digitalRead (sw))
      salida = false;
    lcd.setCursor (7, 1);
    lcd.print (encoder0Pos);
  } while (salida);
  peso = encoder0Pos;

  do
  {
    delay(40);
  } while (digitalRead (sw));
  lcd.clear();
  lcd.setCursor (4, 0);
  lcd.print ("Peso: ");
  lcd.print (peso);
  delay(1000);

  // ajuste unidad de medida // 0=L ; 1=m3

  encoder0Pos = 0;
  lcd.clear();
  salida = true;
  lcd.setCursor (0, 0);
  lcd.print ("Unidad de medida?");
  do
  {

    leeEncoder(0, 1);
    if (!digitalRead (sw))
      salida = false;
    lcd.setCursor (5, 1);
    if (encoder0Pos == 0)
      lcd.print ("Litros");
    if (encoder0Pos == 1)
      lcd.print ("  m3  ");
  } while (salida);
  unidad = encoder0Pos;

  do
  {
    delay(40);
  } while (digitalRead (sw));

  lcd.clear();
  lcd.setCursor (1, 0);
  lcd.print ("Unidad: ");
  if (unidad == 0)
    lcd.print ("Litros");
  if (unidad == 1 )
    lcd.print ("  m3  ");

  delay(1000);

  // ajuste caudal salida   // 0= l/s; 1= m3/h
  encoder0Pos = 0;
  lcd.clear();
  salida = true;
  lcd.setCursor (0, 0);
  lcd.print ("Unidad de Caudal?");
  do
  {

    leeEncoder(0, 1);
    if (!digitalRead (sw))
      salida = false;
    lcd.setCursor (5, 1);

    if (encoder0Pos == 0)
      lcd.print ("L / s ");
    if (encoder0Pos == 1)
      lcd.print (" m3/h ");
  } while (salida);
  caudal = encoder0Pos;

  do
  {
    delay(40);
  } while (digitalRead (sw));

  lcd.clear();
  lcd.setCursor (1, 0);
  lcd.print ("Caudal :");
  if (caudal == 0)
    lcd.print ("  L/s ");;
  if (caudal == 1 )
    lcd.print (" m3/h");

  delay(1000);
  return;

}

//---------------------------------------------------------------------------------------
void pantallaInicio()
{
  lcd.clear();
  lcd.setCursor (4, 0);
  lcd.print ("AQUONA");
  lcd.setCursor (3, 1);
  lcd.print ("S . A . R");
  delay (5000);
}


}

//-------------------------------------------------------------------------------------
boolean muestraOpciones ()
{
  boolean salida = true;   // variable control para salir de bucle
 
  lcd.clear();
  lcd.print ("P: ");
  lcd.print (peso);
  //lcd.print (" ");
  if (encoder0Pos == 0)
    lcd.print ("L ");
  if (encoder0Pos == 1)
    lcd.print ("m3");
  //lcd.setCursor (0,1);
  lcd.print (" Q:");
  if (caudal == 0)
    lcd.print (" L/s");;
  if (caudal == 1 )
    lcd.print ("m3/h");
  lcd.setCursor (0, 1);
  lcd.print ("Correcto?");

  encoder0Pos = 0;
  salida = true;

  do
  {

    leeEncoder(0, 1);
    if (!digitalRead (sw))
      salida = false;
    lcd.setCursor (10, 1);
    if (encoder0Pos == 0)
      lcd.print ("SI");
    if (encoder0Pos == 1)
      lcd.print ("NO");
  } while (salida);
  salida = encoder0Pos;

  do
  {
    delay(40);
  } while (digitalRead (sw));
  delay(500);
  if (salida == 0)
    return true;
  else
    return false;
}
