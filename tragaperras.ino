//Proyecto tragaperras

//Pines
int LED = 3;					  				//Pin indicador LED
int boton = 5;					  			//Pin boton comienzo
int botones[] = {18,19,20};		  //Pin botones de parada
int motores[] = {36,34,32,30,47,45,43,41,52,50,48,46};  //Pin motores 1,2,3
int encoders[] = {22,23,24};	  //Pin encoders motores

//Variables
bool gira[] = {0,0,0};	    	//Control de encendido de motores
int premios[] = {2,6,8};   	 	//Valor de los premios

int monedas = 0;  	  				//Premio monetario
unsigned int vel = 9200;			//Velocidad de giro de los motores (us/tic)
int a = 7;		  							//Secciones por tambor
int b[] = {0,0,0};	  				//tic/rev por tambor
int offset = 0;								//Ofset para ajustar la pos final
int bmax = 0;	            		//Tics medios por rev
int TDE[] = {0,0,0};					//Tics Desde Encoder
int nTDE[] = {0,0,0};	    		//Nuevos TDEs para el ajuste fino
unsigned long t[] = {0,0,0};	//Tiempo desde ultimo paso
int paso[] = {0,0,0};					//Numero del paso que lleva el motor
bool reseteo = 1;							//Control de reset de ciertas variables
int creditos = 4;							//Numero de creditos para realizar operaciones
bool ajuste = 0;							//Control del ajuste fino


//Funcion interrupcion
void interrupcion(){
  if(!ajuste){	//Parada motores
		if(bmax && (bmax!=1)){
			for(int i=0;i++;i<=3){
				if(gira[i] && !digitalRead(botones[i])){	//¿Motor gira?
					gira[i] = 0;														//Parar motor
					nTDE[i] = TDE[i] * a/bmax + 1;					//Calcular cara a parar
					if(nTDE[i] == 8){nTDE[i] = 1;}
					nTDE[0] = nTDE[0] * bmax/a + offset;
				}
			}
		}
	}else{	//Ajuste fino
		if(creditos > 0){
			creditos--;
			for(i=0;i++;i<=3){
				if(!digitalRead(botones[i])){
					nTDE[i] += bmax/a;
				}
			}
		}
  }
}


void setup() {
  //Configurar pines
  pinMode(LED,OUTPUT);
  pinMode(boton,INPUT_PULLUP);
  pinMode(botones[0],INPUT_PULLUP);
  pinMode(botones[1],INPUT_PULLUP);
  pinMode(botones[2],INPUT_PULLUP);
  pinMode(encoders[0],INPUT);
  pinMode(encoders[1],INPUT);
  pinMode(encoders[2],INPUT);
  //Inicializar motores (configura pines)
	for(int i=0;i<12;i++){
		pinMode(motores[i],OUTPUT);
	}
  //Habilitar interrupciones
  interrupts();
  detachInterrupt(digitalPinToInterrupt(boton));
}

void reset(){
  b[0] = 0;      	//Tics por rev
  b[1] = 0;
  b[2] = 0;
  gira[0] = 1;    //Control encendido motores
  gira[1] = 1;
  gira[2] = 1;
	ajuste = 0;			//Ajuste fino
  for(int i=0;i<=11;i++){
    digitalWrite(motores[i],0);
  }
	creditos = 4;
}


void tick(int n){	//Paso del motor
	if(micros()-t[n] > vel){
		t[n] = micros();
		paso[n]++;
		TDE[n]++;
		if(paso[n] > 3){paso[n]=0;}
		int pin1 = motores[n*4];
		int pin2 = motores[n*4+1];
		int pin3 = motores[n*4+2];
		int pin4 = motores[n*4+3];
		switch (paso[n]) {
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
		if(creditos){digitalWrite(LED,1);}
  }
  digitalWrite(LED,0);
  creditos--;
  
  //Declarar interrupciones
  attachInterrupt(digitalPinToInterrupt(botones[0]),interrupcion,FALLING);
  attachInterrupt(digitalPinToInterrupt(botones[1]),interrupcion,FALLING);
  attachInterrupt(digitalPinToInterrupt(botones[2]),interrupcion,FALLING);
  
  // Girar motores
  while(gira[0] || gira[1] || gira[2] || TDE[0]!=nTDE[0] || TDE[1]!=nTDE[1] || TDE[2]!=nTDE[2]){
    //Girar motores
		for(int i=0;i<3;i++){
			if(gira[i] || TDE[i]!=nTDE[i]){tick(i);}
		}
    delayMicroseconds(80);
		
    //Registrar encoders
		for(int i=0;i<3;i++){
			if(!digitalRead(encoders[i]) && TDE[i] > 5){
				if(b[i] == 0){b[i]=1;}
				else if(b[i]){b[i] = TDE[i];}
				TDE[i] = 0;
			}
		}
		if(b[0] b[1] && b[2] && !bmax){
		  if(b[0]!=1 && b[1]!=1 && b[2]!=1){
				bmax = (b[0]+b[1]+b[2])/3;
			}
		}
  } //Fin giro motores
	
	ajuste = 1;
  
  //Ajuste fino
  while(!digitalRead(boton) && creditos > 0){
		if(TDE[0] != nTDE[0]){tick(0);}
		if(TDE[1] != nTDE[1]){tick(1);}
		if(TDE[2] != nTDE[2]){tick(2);}
		delayMicroseconds(80);
  }
	
	ajuste = 0;
  
  //Desactivar interrupciones
  detachInterrupt(digitalPinToInterrupt(botones[0]));
  detachInterrupt(digitalPinToInterrupt(botones[1]));
  detachInterrupt(digitalPinToInterrupt(botones[2]));
  
  //Obtener posicion
  int pos[3];		//Cara final

  pos[0] = (TDE[0]) * a/bmax + 1;	//Posicion = tics * (caras/rev)/(tic/rev) + 1
  pos[1] = (TDE[1]) * a/bmax + 1;
  pos[2] = (TDE[2]) * a/bmax + 1;
	for(int i=0;i<=2;i++){if(pos[i]==8){pos[i]=1;}}	//Correccion

  //Repartir premios
  int tablaP[] = {0,0,0,1,0,2,0};	//Fruta = 0, campana = 1, BAR = 2
  bool x = tablaP[pos[0]] == tablaP[pos[1]];
  bool y = tablaP[pos[1]] == tablaP[pos[2]];
  bool z = tablaP[pos[2]] == tablaP[pos[0]];
  int pasta = 0;
  if(x && y){
    //Premio x1 (tres iguales)
    pasta = premios[tablaP[pos[0]]];
  }else if(x){
    //Premio x0.5 (dos iguales)
    pasta = premios[tablaP[pos[0]]]/2;
  }else if(y){
    pasta = premios[tablaP[pos[1]]]/2;
  }else if(z){
    pasta = premios[tablaP[pos[2]]]/2;
  }
	
  //Arrancar motor
  while(pasta){
		digitalWrite(LED,1);
		delay(500);
		digitalWrite(LED,0);
		delay(500);
		pasta--;
  }
  reseteo = 1;	//Habilita reset
}

