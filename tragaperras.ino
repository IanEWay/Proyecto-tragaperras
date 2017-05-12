//Proyecto tragaperras

//Pines
int laser = 2;					  //Pin detector moneda
int LED = 3;					  //Pin indicador LED
int boton = 4;					  //Pin boton comienzo
int botones[] = {18,19,20};		  //Pin botones de parada
int motores[] = {52,50,48,46,47,45,43,41,36,34,32,30};  //Pin motores 1,2,3
int motorP[] = {9,10,11,12};	  //Pin motor premio
int encoders[] = {22,23,24};	  //Pin encoders motores
int laserP = 5;					  //Pin sensor premio

//Variables
bool gira[] = {0,0,0};	    //Control de encendido de motores
int premios[] = {2,6,8};    //Valor de los premios

int monedas = 0;  	  		//Premio monetario
unsigned long vel = 9200;	//Velocidad de giro de los motores (us/tic)
int a = 7;		  			//Secciones por tambor
int b[] = {0,0,0};	  		//tic/rev por tambor
int offset = 0;				//Ofset para ajustar la pos final
int bmax = 0;	            //Tics medios por rev
int nTDE[] = {0,0,0};	    //Nuevos TDEs para el ajuste fino
int truco = 0;
bool reseteo = 1;			//Control de reset de ciertas variables
int creditos = 5;			//Numero de creditos para realizar operaciones
bool ajuste = 0;			//Control del ajuste fino


// StepperMotor class is used to asynchronously
// drive a stepper motor.  The motor is driven using
// full-stepping where 2 coils are active during any
// one cycle.  
class Motor {
private:
	// the pin values represent which GPIO pin on
	// the microcontroller output the motor is attached to
	int pin1;
	int pin2;
	int pin3;
	int pin4;
	
	// the direction the motor is driving
//	MotorDirection dir;

	// how long to wait in microseconds between state changes
	// on the output pins
	unsigned long cycleDuration;

	// which step the motor is currently on
	int step;

	// the last time the motor was stepped
	unsigned long lastStepTime;

public:
	Motor(int p1, int p2, int p3, int p4) :
		pin1(p1), pin2(p2), pin3(p3), pin4(p4),
		dir(1),
		cycleDuration(2000),
		step(0),
		lastStepTime(0)
	{
		// set all the pins on the motor to OUTPUT mode
		pinMode(pin1, OUTPUT);
		pinMode(pin2, OUTPUT);
		pinMode(pin3, OUTPUT);
		pinMode(pin4, OUTPUT);
	}

  //Direccion de giro
	bool dir = 1;
  
  //Tics desde encoder
  unsigned int TDE = 0;

	void setCycleDuration(unsigned long duration) {
		cycleDuration = duration;
	}

	// call tick frequently to update the motor state
	void tick();

private:

	// update the current step and set the pin voltages
	void doStep();

	// increment (or decrement) the step depending on the
	// direction and set the new step modulus 4
	void advanceStep();
};

void Motor::tick() {
	unsigned long now = micros();
	
	if (now - lastStepTime > cycleDuration) {
		doStep();
		lastStepTime = now;
	}
}

void Motor::advanceStep() {
	step = step + dir;

	if (step == 4)  step = 0;
	if (step == -1) step = 3;
}

void Motor::doStep() {
	advanceStep();
  TDE ++;

	switch (step) {
	case 0:  // 1010
		digitalWrite(pin1, HIGH);
		digitalWrite(pin2, LOW);
		digitalWrite(pin3, HIGH);
		digitalWrite(pin4, LOW);
		break;
	case 1:    // 0110
		digitalWrite(pin1, LOW);
		digitalWrite(pin2, HIGH);
		digitalWrite(pin3, HIGH);
		digitalWrite(pin4, LOW);
		break;
	case 2:    //0101
		digitalWrite(pin1, LOW);
		digitalWrite(pin2, HIGH);
		digitalWrite(pin3, LOW);
		digitalWrite(pin4, HIGH);
		break;
	case 3:    //1001
		digitalWrite(pin1, HIGH);
		digitalWrite(pin2, LOW);
		digitalWrite(pin3, LOW);
		digitalWrite(pin4, HIGH);
		break;
	} 
}

//Declarar motores
Motor *m1 = NULL, *m2 = NULL, *m3 = NULL;
Motor *mP = NULL;

//Funcion interrupcion-----------------------------------------------------------------
void interrupcion(){
  if(!ajuste){	//Parada motores
		if(bmax && (bmax!=1)){
			if(gira[0] && digitalRead(botones[0])){
				gira[0] = 0;
				if(!truco){
					nTDE[0] = (m1->TDE) * a/bmax + 1;
					if(nTDE[0] == 8){nTDE[0] = 1;}
					nTDE[0] = nTDE[0] * bmax/a + offset;
				}else{nTDE[0] = bmax/a * truco + offset;}
				m1->setCycleDuration(vel*2);
			}else if(gira[1] && digitalRead(botones[1])){
				gira[1] = 0;
				if(!truco){
					nTDE[1] = (m2->TDE) * a/bmax + 1;
					if(nTDE[1] == 8){nTDE[1] = 1;}
					nTDE[1] = nTDE[1] * bmax/a + offset;
				}else{nTDE[1] = bmax/a * truco + offset;}
				m2->setCycleDuration(vel*2);
			}else if(gira[2] && digitalRead(botones[2])){
				gira[2] = 0;
				if(!truco){
					nTDE[2] = (m3->TDE) * a/bmax + 1;
				if(nTDE[2] == 8){nTDE[2] = 1;}
				nTDE[2] = nTDE[2] * bmax/a + offset;
				}else{nTDE[2] = bmax/a * truco + offset;}
				m3->setCycleDuration(vel*2);
			}
		}
	}else{	//Ajuste fino
		if(creditos > 0){
			creditos--;
			for(i=0;i++;i<=3){
				if(digitalRead(botones[i])){
					nTDE[i] += bmax/a;
				}
			}
		}
  }
}

void setup() {
  //Configurar pines
  pinMode(laser,INPUT);
  pinMode(LED,OUTPUT);
  pinMode(boton,INPUT);
  pinMode(botones[0],INPUT);
  pinMode(botones[1],INPUT);
  pinMode(botones[2],INPUT);
  pinMode(laserP,INPUT);
  //Inicializar motores (configura pines)
  m1 = new Motor(motores[0],motores[1],motores[2],motores[3]);
  m2 = new Motor(motores[4],motores[5],motores[6],motores[7]);
  m3 = new Motor(motores[8],motores[9],motores[10],motores[11]);
  mP = new Motor(motorP[0],motorP[1],motorP[2],motorP[3]);
  mP->setCycleDuration(vel/2);
  //Habilitar interrupciones
  interrupts();
  detachInterrupt(digitalPinToInterrupt(laser));
  detachInterrupt(digitalPinToInterrupt(boton));
}

void reset(){
  b[0] = 0;      //Tics por rev
  b[1] = 0;
  b[2] = 0;
  gira[0] = 1;    //Control encendido motores
  gira[1] = 1;
  gira[2] = 1;
  m1->setCycleDuration(vel);  //Velocidades motor
  m2->setCycleDuration(vel);
  m3->setCycleDuration(vel);
}

void loop() {
  //Reset de variables
  if(reseteo){
    reseteo = 0;
    reset();
  }
delay(1000);  //Retardo de testeo
  // LED + boton
  while (!digitalRead(boton) && creditos > 0){ //boton==1 -> empezar
	//Detectar moneda
	while(digitalRead(laser)){ //laser==0 -> moneda
		creditos++;
	}
	if(creditos){digitalWrite(LED,1);}
  }
  digitalWrite(LED,0);
  creditos--;
  
  if(digitalRead(botones[0])){truco = 1;}
  else if(digitalRead(botones[1])){truco = 2;}
  else if(digitalRead(botones[2])){truco = 3;}
  
  //Declarar interrupciones
  attachInterrupt(digitalPinToInterrupt(botones[0]),interrupcion,RISING);
  attachInterrupt(digitalPinToInterrupt(botones[1]),interrupcion,RISING);
  attachInterrupt(digitalPinToInterrupt(botones[2]),interrupcion,RISING);
  
  // Girar motores
  while(gira[0] || gira[1] || gira[2] || m1->TDE!=nTDE[0] || m2->TDE!=nTDE[1] || m3->TDE!=nTDE[2]){
    //Girar motores
    if(gira[0]){m1->tick();}
    else if(m1->TDE != nTDE[0]){  //---------------------------------------
      m1->tick();
    }
    if(gira[1]){m2->tick();}
    else if(m2->TDE != nTDE[1]){
      m2->tick();
    }
    if(gira[2]){m3->tick();}
    else if(m3->TDE != nTDE[2]){
      m3->tick();
    }
    delayMicroseconds(80);
    //Registrar encoders
    if(!digitalRead(encoders[0]) && m1->TDE > 5){
      if(b[0] == 0){b[0]=1;}
      else if(b[0]){b[0] = m1->TDE;}
      m1->TDE = 0;
    }
    if(!digitalRead(encoders[1]) && m2->TDE > 5){
      if(b[1] == 0){b[1]=1;}
      else if(b[1]){b[1] = m2->TDE;}
      m2->TDE = 0;
    }
    if(!digitalRead(encoders[2]) && m3->TDE > 5){
      if(b[2] == 0){b[2]=1;}
      else if(b[2]){b[2] = m3->TDE;}
      m3->TDE = 0;
    }
		if(b[0] b[1] && b[2] && !bmax){
		  if(b[0]!=1 && b[1]!=1 && b[2]!=1){
				bmax = (b[0]+b[1]+b[2])/3;
			}
		}
  } //Fin giro motores
  
  //Ajuste fino
  while(!digitalRead(boton) && creditos > 0){
		if(m1->TDE != nTDE[0]){m1->tick();}
		if(m2->TDE != nTDE[1]){m2->tick();}
		if(m3->TDE != nTDE[2]){m3->tick();}
  }
  
  //Desactivar interrupciones
  detachInterrupt(digitalPinToInterrupt(botones[0]));
  detachInterrupt(digitalPinToInterrupt(botones[1]));
  detachInterrupt(digitalPinToInterrupt(botones[2]));
  
  //Obtener posicion
  int pos[3];		//Cara final

  pos[0] = (m1->TDE) * a/bmax + 1;	//Posicion = tics * (caras/rev)/(tic/rev) + 1
  pos[1] = (m2->TDE) * a/bmax + 1;
  pos[2] = (m3->TDE) * a/bmax + 1;
	for(int i=0;i<=2;i++){if(pos[i]==8){pos[i]=1;}}	//Correccion

    //Repartir premios
  int tablaP[] = {0,0,0,1,0,2,0};	//Fruta = 0, campana = 1, BAR = 2
  bool x = tablaP[pos[0]] == tablaP[pos[1]];
  bool y = tablaP[pos[1]] == tablaP[pos[2]];
  bool z = tablaP[pos[2]] == tablaP[pos[0]];
  int pasta = 0;
  if(x && y){
    //Premio x1
    pasta = premios[tablaP[pos[0]]];
  }else if(x){
    //Premio x0.5
    pasta = premios[tablaP[pos[0]]]/2;
  }else if(y){
    pasta = premios[tablaP[pos[1]]]/2;
  }else if(z){
    pasta = premios[tablaP[pos[2]]]/2;
  }
    //Arrancar motor
  while(pasta){
/*    if(!laserP){	//!Detectamos moneda
      pasta --;
    }
    mP->tick();
    delayMicroseconds(100);*/
digitalWrite(LED,1);
delay(500);
digitalWrite(LED,0);
delay(500);
  }
  reseteo = 1;
}

