//Proyecto tragaperras

//Pines
int laser = 2;									//Pin detector moneda
int LED = 3;										//Pin indicador LED
int boton = 4;									//Pin boton comienzo
int botones[] = {18,19,20};			//Pin botones de parada
int motores[] = {52,50,48,46,47,45,43,41,36,34,32,30};  //Pin motores 1,2,3
int motorP[] = {9,10,11,12};		//Pin motor premio
int encoders[] = {22,23,24};		//Pin encoders motores
int laserP = 13;								//Pin sensor premio

//Variables
bool gira[] = {0,0,0};    //Variable giro motor
int premios[] = {2,6,8};  //Valor de los premios
int monedas = 0;  	  		//Premio monetario
int vel = 2300;		  			//Velocidad de giro de los motores (us/tic)
int a = 7;		  					//Secciones por tambor
int b[] = {0,0,0};	  		//us/rev por tambor
int offset = 0;						//Ofset para ajustar la pos final

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

//Funcion interrupcion
void interrupcion(){
  for(int i=0;i<=3;i++){
    if(botones[i]){gira[i] = 0;}
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
  m1->setCycleDuration(vel);
  m2 = new Motor(motores[4],motores[5],motores[6],motores[7]);
  m2->setCycleDuration(vel);
  m3 = new Motor(motores[8],motores[9],motores[10],motores[11]);
  m3->setCycleDuration(vel);
  mP = new Motor(motorP[0],motorP[1],motorP[2],motorP[3]);
  mP->setCicleDuration(vel/2);
  //Declarar interrupciones
  attachInterrupt(botones[0],interrupcion,RISING);
  attachInterrupt(botones[1],interrupcion,RISING);
  attachInterrupt(botones[2],interrupcion,RISING);
}

void loop() {
  // Detectar moneda
  while(digitalRead(laser)){ //laser==0 -> moneda  
  }

  // LED + boton
  digitalWrite(LED,1);
  while (digitalRead(!boton)){ //boton==1 -> empezar
  }
  digitalWrite(LED,0);
  
  // Girar motores
  gira = {1,1,1};
  while(gira[0] || gira[1] || gira[2]){
    //Girar motores
    if(gira[0]){m1->tick();}
    if(gira[1]){m2->tick();}
    if(gira[2]){m3->tick();}
    delayMicroseconds(80);
    //Registrar encoders
    if(!digitalRead(encoders[0] && m1->TDE < 10000){
      if(b[0] == 0){b[0]=1;}
      else if(b[0] == 1){b[0] = m1->TDE;}
      m1->TDE = 0;
    }
    if(!digitalRead(encoders[1] && m2->TDE < 10000){
      if(b[1] == 0){b[1]=1;}
      else if(b[1] == 1){b[1] = m2->TDE;}
      m2->TDE = 0;
    }
    if(!digitalRead(encoders[2] && m3->TDE < 10000){
      if(b[2] == 0){b[2]=1;}
      else if(b[2] == 1){b[2] = m3->TDE;}
      m3->TDE = 0;
    }
  }
  //Ajustar caras
       
  //Resultado
    //Calcular posiciones finales
  int pos[3];		//Cara final
  int bmax = (b[0]+b[1]+b[2])/3;
  pos[0] = (m1->TDE) * vel * a/bmax + offset;
  pos[1] = (m2->TDE) * vel * a/bmax + offset;
  pos[2] = (m3->TDE) * vel * a/bmax + offset;
    //Repartir premios
  int tablaP = [0,0,0,1,0,2,0];	//Fruta = 0, campana = 1, BAR = 2
  bool x = tablaP[pos[0]] == tablaP[pos[1]];
  bool y = tablaP[pos[1]] == tablaP[pos[2]];
  bool z = tablaP[pos[2]] == tablaP[pos[0]];
    //Premio x1
  int pasta = 0;
  if(x && y){
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
    if(!laserP){	//!Detectamos moneda
      pasta --;
    }
    mP->tick();
    delayMicroseconds(100)
  }
}

