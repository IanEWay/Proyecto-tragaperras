//Proyecto tragaperras

//Pines
int laser = 2;    //Pin detector moneda
int LED = 3;      //Pin indicador LED
int boton = 4;    //Pin boton comienzo
int botones[] = {18,19,20};  //Pin botones de parada
int motores[] = {52,50,48,46,47,45,43,41,36,34,32,30};  //Pin motores
int motorP[] = {9,10,11,12};    //Pin motor premio
int encoders[] = {22,23,24};    //Pin encoders motores
int sensorP = 13; //Pin sensor premio

//Variables
int divi = 3;     //Divisiones por octavo
bool gira[] = {0,0,0};  //Variable giro motor
int premios[] = {1,2,3};  //Valor de los premios
int monedas = 0;  //Premio monetario

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

//Funcion interrupcion
void interrupcion(){
  if(digitalRead(botones[0]){
    motor1.steps(0);
    gira[0] = 0;
  }else if(digitalRead(botones[1]){
    motor2.steps(0);
    gira[1] = 0;
  }else if(digitalRead(botones[2]){
    motor3.steps(0);
    gira[2] = 0;
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
  for(int i=0;i<=12;i++){
    pinMode(motores[i],OUTPUT);
  }
  pinMode(motorP[0],OUTPUT);
  pinMode(motorP[1],OUTPUT);
  pinMode(motorP[2],OUTPUT);
  pinMode(sensorP,INPUT);
  //Inicializar motores
  m1 = new Motor(motores[0],motores[1],motores[2],motores[3]);
	m1->setCycleDuration(4600); //Velocidad del motor (us)
	m2 = new Motor(motores[4],motores[5],motores[6],motores[7]);
	m2->setCycleDuration(4600);
  m3 = new Motor(motores[8],motores[9],motores[10],motores[11]);
  m3->setCycleDuration(4600);
  //Declarar interrupciones
  attachInterrupt(botones[0],interrupcion,RISING);
  attachInterrupt(botones[1],interrupcion,RISING);
  attachInterrupt(botones[2],interrupcion,RISING);
}

void loop() {
  // Detectar moneda
  int i=0;
  while(digitalRead(laser)){ //laser==0 -> moneda  
    i++;
    if (i==255){
      i=0;
    }
  }
  randomSeed(i);
  
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
    //Registrar encoders
    if(!digitalRead(encoders[0] && m1->TDE < 10000){m1->TDE = 0;}
    if(!digitalRead(encoders[1] && m2->TDE < 10000){m2->TDE = 0;}
    if(!digitalRead(encoders[2] && m3->TDE < 10000){m3->TDE = 0;}
  }
  //Resultado
    //Calcular posiciones finales

    //Determinar caras

    //Repartir premios

    //Premio x1

    //Premio x0.5

    //Arrancar motor
    
}

