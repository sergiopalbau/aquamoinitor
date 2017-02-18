
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library
uint8_t testChar[8] = {0x8,0xf4,0x8,0x43,0x4,0x4,0x43,0x0};
byte coco[8] = {
0b00000,
0b01110,
0b11111,
0b10101,
0b11111,
0b11111,
0b11111,
0b10101
};
byte primero[8] = {
    0b11111,
    0b11111,
    0b11001,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000
};

byte segundo[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B01111,
    B00111,
    B00011,
    B00011
};
byte tercero[8] = {
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11001,
    B11111,
    B11111
};

byte cuarto[8] = {
    B00011,
    B00011,
    B00111,
    B01111,
    B11111,
    B11111,
    B11111,
    B11111
};
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
byte unidad=0;  // 0=L ; 1=m3
byte caudal=0;  // 0= l/s;  ; 1= m3/h

//----------------------------------------------------------------------------------------------------
 void setup() { 

 // lcd.createChar (0, primero);
  lcd.createChar(0, testChar);
  lcd.createChar (1, segundo);
  lcd.createChar (2, tercero);
  lcd.createChar (3, cuarto);
  lcd.createChar (4, coco);
  byte bucle= true;
   pinMode (encoder0PinA,INPUT);
   pinMode (encoder0PinB,INPUT);
   pinMode (sw, INPUT);
   lcd.begin (16,2);  // inicializar lcd 
   lcd.setBacklight(LED_ON);
 
 pantallaInicio(); 
  do
  {
    
    bucle = ajuste();
  }while (!bucle);
 
 } 

//----------------------------------------------------------------------------------------------------
 void loop() { 
 lcd.setCursor (0,0);
 lcd.print ("presione para menu");
 
 
 lcd.setCursor (0,1);
 if (digitalRead (sw))
  lcd.print (" ON");  
 else
   lcd.print ("OFF");  

 lcd.setCursor (12,1);
 lcd.print (millis()/1000); 
 
 }// fin loop
 
//----------------------------------------------------------------------------------------------------
 void leeEncoder (byte min, byte max)
 {
  
     n = digitalRead(encoder0PinA);
     if ((encoder0PinALast == LOW) && (n == HIGH)) {
     if (digitalRead(encoder0PinB) == LOW) {
      
       encoder0Pos--;
       if ((encoder0Pos <min) || (encoder0Pos > max)) encoder0Pos = min;
     } else {
       encoder0Pos++;
       if (encoder0Pos >max) encoder0Pos = max;
     }
     //lcd.setCursor (0,0);
    // lcd.print (encoder0Pos);
    
   }
   encoder0PinALast = n;
  
 }

 //-------------------------------------------------------------------------------------------------
 boolean ajuste ()
 {
   n=0;                      //inicializamos el contador del encoder 
   byte n_max = 255;        // valor maximo para este caso
   boolean salida = true;   // variable control para salir de bucle 
   
   


   // ajuste peso de pulso
   encoder0Pos=0;
   lcd.setCursor (0,0);
   lcd.print ("Peso del pulso?");
   do 
   {
    
    leeEncoder(0,255);
    if (!digitalRead (sw))
      salida = false;
    lcd.setCursor (12,1);
    lcd.print (encoder0Pos);
   }while (salida);
   peso=encoder0Pos;
  
  do
  {
    delay(40);
  }while (digitalRead (sw));
  lcd.clear();
  lcd.print ("Peso: ");
  lcd.print (peso);
  delay(1000);
   
   // ajuste unidad de medida // 0=L ; 1=m3
   

  // ajuste salida caudal.
  encoder0Pos=0;
   lcd.clear();
   salida = true; 
   lcd.setCursor (0,0);
   lcd.print ("Unidad de medida?");
   do 
   {
    
    leeEncoder(0,1);
    if (!digitalRead (sw))
      salida = false;
    lcd.setCursor (5,1);
    if (encoder0Pos == 0)
          lcd.print ("Litros");
     if (encoder0Pos ==1)
          lcd.print ("  m3  ");
    }while (salida);
  unidad = encoder0Pos;
  
  do
  {
    delay(40);
  }while (digitalRead (sw));

  lcd.clear();
  lcd.print ("Unidad: ");
  if (unidad ==0)
    lcd.print ("Litros");
  if (unidad ==1 )
    lcd.print ("  m3  ");
     
  delay(1000);
  
  // ajuste caudal salida   // 0= l/s; 1= m3/h
  encoder0Pos=0;
   lcd.clear();
   salida = true; 
   lcd.setCursor (0,0);
   lcd.print ("Unidad de Caudal?");
   do 
   {
    
    leeEncoder(0,1);
    if (!digitalRead (sw))
      salida = false;
    lcd.setCursor (5,1);
    lcd.print (encoder0Pos);
    if (encoder0Pos == 0)
          lcd.print ("  L/s ");
     if (encoder0Pos ==1)
          lcd.print (" m3/h");
   }while (salida);     
 caudal = encoder0Pos;
  
  do
  {
    delay(40);
  }while (digitalRead (sw));

  lcd.clear();
  
  if (caudal ==0)
    lcd.print ("  L/s ");;
  if (caudal ==1 )
    lcd.print (" m3/h");
     
  delay(1000);

 // muestra las opciones
 lcd.clear();
 lcd.print ("P: ");
 lcd.print (peso);
 //lcd.print (" "); 
 if (encoder0Pos == 0)
        lcd.print ("L ");
 if (encoder0Pos ==1)
        lcd.print ("m3");
 //lcd.setCursor (0,1);
 lcd.print (" Q:");
  if (caudal ==0)
    lcd.print (" L/s");;
  if (caudal ==1 )
    lcd.print ("m3/h");
  lcd.setCursor (0,1);  
  lcd.print ("Correcto?");

  encoder0Pos=0;
  salida = true; 
   
  do 
   {
   
    leeEncoder(0,1);
    if (!digitalRead (sw))
      salida = false;
    lcd.setCursor (10,1);
    if (encoder0Pos == 0)
          lcd.print ("SI");
     if (encoder0Pos ==1)
          lcd.print ("NO");
    }while (salida);
  salida = encoder0Pos;
  
  do
  {
    delay(40);
  }while (digitalRead (sw));
  delay(500);
  if (salida==0)
     return true;
  else
     return false;
 }

//---------------------------------------------------------------------------------------
void pantallaInicio()
{
  lcd.clear();
  lcd.print ((char)(0));
  lcd.print ((char)(0));
  lcd.print ((char)(0));
  lcd.print ((char)(0));
  
  
  /*
  lcd.write (byte(1));
  lcd.print (" AQUONA ");
  lcd.setCursor (1,1);
  lcd.write (byte(2));
  lcd.write (byte(3));
  lcd.print (" San Andres. R");*/
  delay (10000);

  
}

