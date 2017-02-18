
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library


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
byte tiempo=0;  // 0= s; 1= m ; 2= m3

//----------------------------------------------------------------------------------------------------
 void setup() { 
   pinMode (encoder0PinA,INPUT);
   pinMode (encoder0PinB,INPUT);
   pinMode (sw, INPUT);
   lcd.begin (16,2);  // inicializar lcd 
   lcd.setBacklight(LED_ON);
  ajuste();
  
 
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
       if ((encoder0Pos <min) || (encoder0Pos > max)) encoder0Pos = max;
     } else {
       encoder0Pos++;
       if (encoder0Pos >max) encoder0Pos = min;
     }
     //lcd.setCursor (0,0);
    // lcd.print (encoder0Pos);
    
   }
   encoder0PinALast = n;
  
 }

 //-------------------------------------------------------------------------------------------------
 void ajuste ()
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

  
 }


