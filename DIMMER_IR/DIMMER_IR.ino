
#include <TimerOne.h>              // Incluir Librería TimerOne        
#define DEBUG(a, b) for (int index = 0; index < b; index++) Serial.print(a[index]); Serial.println();


volatile int i=0;                  // Variable usada por el contador.
volatile boolean cruce_cero=0;     // variable que actúa como switch al detectar cruce por cero.
int Triac = 3;                     // Salida conectada al optoacoplador MOC 3021.
int dim = 100;                      // Controla la intensidad de iluminación, 0 = ON ; 83 = OFF                
int T_int = 100;                   // Tiempo en el cual se producen la interrupciones en us. 
String data = "";
int unidad, decena, centena, voltaje;
int voltajeRed = 12;
bool bandera = true;
char tension[3];

void setup() {   
  Serial.begin(9600);
  Serial.setTimeout(50);
  pinMode(Triac, OUTPUT);                               // Configurar como salida.
  attachInterrupt(0, deteccion_Cruce_cero, RISING);    // Realiza una interrupción al detectar el crice por cero en el pin 2
  Timer1.initialize(T_int);                            //Inicializa la librería con el tiempo deseado.
  Timer1.attachInterrupt(Dimer, T_int);                // En cada interrupción ejecuta el codigo Dimer. 
  Serial.println("1.ENCEDER");
  Serial.println("2.APAGAR");
  Serial.println("3.VOLTAJE+");
  Serial.println("4.LECTURA");
  Serial.println("5.TENSION");
}

void deteccion_Cruce_cero() 
{                                                      // Si existe un cruce por cero entonces la variable "cruce_cero" cambia a TRUE...
  cruce_cero = true;                                   //...reseteando el valor de "i", y colocando la salida conectada al Triac en estado...
  i=0;                                                 // ...bajo.
  digitalWrite(Triac, LOW);
}                                 

void Dimer() 
{                   
  if(cruce_cero == true) {                             // Al detectarse el cruce por cero inicia a correr un tiempo t = T_int*dim, en el cual...
    if(i>=dim) {                                       // ... el triac se dispara y empieza a conducir.
      digitalWrite(Triac, HIGH);                      
      i=0;                                              // Reseteo i.
      cruce_cero=false;                                   
    } 
    else {
      i++;                     
}}}                                      

  
void loop() { 
    
   if (Serial.available())
   {
      char data[10];
      size_t count = Serial.readBytesUntil('\n', data, 11);
      DEBUG(data, count)
      
        switch(data[0]){
          case 'E':
              dim = 0;
              Serial.println("Encendido");
              Serial.println(dim);
          break;
          case 'A':
              dim = 100;
              Serial.println("Apagado");
              Serial.println(dim);
          break;
          case 'V':
              centena = data[8] - 48; 
              decena = data[9] - 48; 
              unidad = data[10] - 48; 
              voltaje = centena * 100 + decena * 10 + unidad;
              dim = (voltaje*100)/voltajeRed;
              Serial.println("Voltaje");
              Serial.print(voltaje);
              Serial.print(dim);
          break;
          case 'L':
          break;
          case 'T':
            Serial.println("Setee el valor de tension en la red:");
            while(!Serial.available()){}
            size_t count2 = Serial.readBytesUntil('\n', tension, 3);
            DEBUG(tension, count2);
            bandera = false;
            bandera = true;
            centena = tension[0] - 48; 
            decena = tension[1] - 48; 
            unidad = tension[2] - 48; 
            voltajeRed = centena * 100 + decena * 10 + unidad;
            Serial.println("La Tension seteada es: ");
            Serial.print(voltajeRed);
          
          break;
        }      
   
   }
}
