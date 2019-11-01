//UNIVERSIDAD MARIANO GALVEZ DE GUATEMALA
//PROYECTO FINAL
//ARQUITECTURA DE COMPUTADORAS 2

//VARIABLES PARA PINES
#define pin_in4 4
#define pin_in3 5
#define pin_in2 7
#define pin_in1 8
#define pin_ena 9
#define pin_enb 6

//PINES PARA INTERRUPCIONES
#define pin_interrupcion 2
#define pin_pivote 10

//VARIABLES LOCALES
char str_senal;
int velocidad_base;
bool sn_encendido;
int contador_bitacora;
volatile int conteo = 0;
volatile int estado_pin = HIGH;

//METODO PARA QUE AVANCE HACIA ADELANTE (MOTOR)
void voidAdelante(int pin_x, int pin_y, int potencia, int pin_velocidad){
  digitalWrite(pin_x, HIGH);
  digitalWrite(pin_y, LOW);
  analogWrite(pin_velocidad, potencia);
}

//METODO PARA QUE AVANCE HACIA ATRAS (MOTOR)
void voidAtras(int pin_x, int pin_y, int potencia, int pin_velocidad){
  digitalWrite(pin_x, LOW);
  digitalWrite(pin_y, HIGH);
  analogWrite(pin_velocidad, potencia);
}

//METODO PARA QUE GIRE HACIA LA DERECHA (MOTOR)
void voidDerecha(int potencia){
  voidAdelante(pin_in4, pin_in3, potencia, pin_ena);
  voidAtras(pin_in2, pin_in1, potencia, pin_enb);
}

//METODO PARA QUE GIRE HACIA LA IZQUIERDA (MOTOR)
void voidIzquierda(int potencia){
  voidAdelante(pin_in2, pin_in1, potencia, pin_enb);
  voidAtras(pin_in4, pin_in3, potencia, pin_ena);
}

//METODO PARA APAGAR DISPOSITIVO
void voidApagar(){
  digitalWrite(pin_in1, LOW);
  digitalWrite(pin_in2, LOW);
  digitalWrite(pin_in3, LOW);
  digitalWrite(pin_in4, LOW);
}

//CONFIGURACIÓN
void setup() {
  //INICIALIZA VARIABLES
  velocidad_base = 200;
  str_senal = ' ';
  conteo = 0;
  estado_pin = HIGH;
  sn_encendido = false;
  contador_bitacora = 0;
  
  //ASIGNA PINES DE SALIDA - LOS QUE UTILIZARAN LOS MOTORES
  pinMode(pin_in4, OUTPUT);
  pinMode(pin_in3, OUTPUT);
  pinMode(pin_in2, OUTPUT);
  pinMode(pin_in1, OUTPUT);
  pinMode(pin_ena, OUTPUT);
  pinMode(pin_enb, OUTPUT);
  pinMode(pin_pivote, OUTPUT);

  //ASIGNA PINES PARA INTERRUPCIONES
  pinMode(pin_interrupcion, INPUT_PULLUP);
      
  //VELOCIDAD DE COMUNICACIÓN
  Serial.begin(9600);

  //SE DEFINEN INTERRUPCIONES
  attachInterrupt(digitalPinToInterrupt(pin_interrupcion), void_conteo, CHANGE);
 
  void_escribirLog("TORITO ENCENDIDO.");
}

//EJECUCIÓN 
void loop() {
  //SE MANTIENE ESCUCHANDO LA SEÑAL 
  if(Serial.available() > 0){
    str_senal = Serial.read();
    void_escribirLog("NUEVA SEÑAL DETECTADA.");
  }
    
  //VALIDA SEÑAL
  if(str_senal == 'E'){
    //SEÑAL PARA ENCENDER
    sn_encendido = true;
    voidAdelante(pin_in1, pin_in2, velocidad_base, pin_ena);
    voidAdelante(pin_in3, pin_in4, velocidad_base, pin_enb);
    void_escribirLog("EN MOVIMIENTO CONTINUO.");
     
  }else if(str_senal == 'P'){
    //SEÑAL PARA PARAR 
    sn_encendido = false;  
    voidApagar();
    void_escribirLog("DETENIDO."); 
    str_senal = ' ';
    
  }else if(str_senal == 'I'){       
    //VALIDA QUE ESTE ENCENDIDO PARA APLICAR REALIZAR GIRO
    if (sn_encendido == true){
      //SEÑAL PARA LA IZQUIERDA
      voidIzquierda(velocidad_base);
      void_escribirLog("GIRO HACIA LA IZQUIERDA."); 
      //SEÑAL PARA INTERRUPCION
      digitalWrite(pin_pivote, estado_pin);
      //VALIDA SI PASO EL TIEMPO ESTABLECIDO PARA GIRAR Y MODIFICA PARA ENVIAR DE NUEVO HACIA ADELANTE
      if (conteo >= 2){
        str_senal = 'E';
        conteo = 0;      
      }                       
    }else{
      void_escribirLog("NO SE GENERARA EL GIRO PORQUE NO ESTA EN MOVIMIENTO.");
      str_senal = ' ';
    }
    
  }else if(str_senal == 'D'){  
    //VALIDA QUE ESTE ENCENDIDO PARA APLICAR REALIZAR GIRO
    if (sn_encendido == true){
      //SEÑAL PARA LA DERECHA
      voidDerecha(velocidad_base);
      void_escribirLog("GIRO HACIA LA DERECHA."); 
      //SEÑAL PARA INTERRUPCION
      digitalWrite(pin_pivote, estado_pin);
      //VALIDA SI PASO EL TIEMPO ESTABLECIDO PARA GIRAR Y MODIFICA PARA ENVIAR DE NUEVO HACIA ADELANTE
      if (conteo >= 2){
        str_senal = 'E';
        conteo = 0;      
      }                    
    }else{
      void_escribirLog("NO SE GENERARA EL GIRO PORQUE NO ESTA EN MOVIMIENTO.");
      str_senal = ' ';
    }          
  }

}

//RUTINA PARA BITACORA
void void_escribirLog(String mensaje){
  contador_bitacora++;
  Serial.print(contador_bitacora);
  Serial.println(":   " + mensaje);
}

//RUTINAS DE INTERRUPCION
void void_conteo(){
 conteo++; 
 estado_pin = !estado_pin;
}
