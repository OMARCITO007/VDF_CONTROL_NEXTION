#include <SoftwareSerial.h>
#include <Nextion.h>
#include <SPI.h>

#define SONIDOS_T 5 //PIN DE SALIDA DE SONIDO
#define SALIDA_PWM 6 //PIN DE SALIDA PWM 
#define ACT_VAR 4 //PIN DE SALIDA ON OFF VARIADOR DE FREC
#define ACT_LED 7 // activar led para ver si esta en funcioanmiento.

int HOLA = 0;     // VARIABLE VERDADERO FALSO PARA EL CONTROL DE  BUCLE
int PWM_S = 0;
int sensorPin = A0;    // select the input pin for the sensor voltage
int sensorValue = 0;  // variable to store the value coming from the sensor
int voltaje = 0;

SoftwareSerial nextion(2, 3);// Nextion TX to pin 2 and RX to pin 3 of Arduino
Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

void setup()
{
	Serial.begin(9600);
	myNextion.init();
	pinMode(SONIDOS_T, OUTPUT);
	pinMode(SALIDA_PWM, OUTPUT);
	pinMode(ACT_VAR, OUTPUT);
	pinMode(ACT_LED, OUTPUT);
	pinMode(sensorPin, INPUT);
}
void loop()
{
	
// programa 
  //Ave María - Los Chabelos https://www.youtube.com/watch?v=wjzjOj_IaCs 
   voltaje = map(analogRead(sensorPin), 0, 1024, 0, 920);
   sensorValue = map(voltaje, 0, 920, 0, 918);
   if(sensorValue == 5){ sensorValue = 0;}
   delay(500);
 
   int INICIO_APAGADO_MANUAL = myNextion.getComponentValue("MANUAL.Enc_apa"); // 12 OFF 11 ON
   int INICIO_APAGADO_AUTO = myNextion.getComponentValue("AUTO.GO"); //28 OFF 27 ON
   
   if(INICIO_APAGADO_MANUAL == 11){
   	PROCESO_MANUAL();
   }
   else if(INICIO_APAGADO_MANUAL == 12){ digitalWrite(ACT_VAR, LOW); digitalWrite(ACT_LED, LOW); digitalWrite(SALIDA_PWM, LOW); }


   if(INICIO_APAGADO_AUTO == 27){
   	PROCESO_AUTO();   
   }
   else if(INICIO_APAGADO_AUTO == 28){ digitalWrite(ACT_VAR, LOW); digitalWrite(ACT_LED, LOW); digitalWrite(SALIDA_PWM, LOW); }

}

void PROCESO_MANUAL(){
	int PWM_MANUAL = myNextion.getComponentValue("MANUAL.porcetaje"); // 0 - 900

    myNextion.setComponentValue("MANUAL.Vel_Manual", sensorValue);

    PWM_S = map(PWM_MANUAL,0,900,0,255);
	analogWrite(SALIDA_PWM, PWM_S);
	if(PWM_S > 0){digitalWrite(ACT_VAR, HIGH); digitalWrite(ACT_LED, HIGH);} else if(PWM_S == 0){digitalWrite(ACT_VAR, LOW); digitalWrite(ACT_LED, LOW);}

}

void PROCESO_AUTO(){
	
	int PWM_AUTO = myNextion.getComponentValue("AUTO.Velocidad_auto"); // 0 - 900
	int sonido_N = myNextion.getComponentValue("AUTO.Sonidos");// Datos On --->>5,6,7 off -->> 3
	//Velocidad_Var VARIABLE PARA ENVIAR DATOS DEL VARIADOR DE FRECUENCIA
    
    myNextion.setComponentValue("AUTO.Velocidad_Var", sensorValue);


	PWM_S = map(PWM_AUTO,0,900,0,255);
	analogWrite(SALIDA_PWM, PWM_S);
	if(PWM_S > 0){digitalWrite(ACT_VAR, HIGH); digitalWrite(ACT_LED, HIGH);} else if(PWM_S == 0){digitalWrite(ACT_VAR, LOW); digitalWrite(ACT_LED, LOW);}
	

	if(sonido_N == 5){ HOLA = 1; if(HOLA == 1){SONIDO_UNO();}}
	if(sonido_N == 6){ HOLA = 2; if(HOLA == 2){SONIDO_UNO();}}
	if(sonido_N == 7){ HOLA = 3; if(HOLA == 3){SONIDO_UNO();}}
	if(sonido_N == 3){ HOLA = 4; if(HOLA == 4){digitalWrite(SONIDOS_T, LOW);}}
	
}

void SONIDO_UNO(){
	digitalWrite(SONIDOS_T, HIGH);
	delay(200);
	digitalWrite(SONIDOS_T, LOW);
	delay(200);
	digitalWrite(SONIDOS_T, HIGH);
	delay(200);
	digitalWrite(SONIDOS_T, LOW);
	delay(200);
	myNextion.setComponentValue("AUTO.Sonidos", 3);
}
