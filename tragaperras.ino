int laser = 0;    //Pin detector moneda
int LED = 1;      //Pin indicador LED
int boton = 2;    //Pin boton comienzo
int motores[] = {3,4,5};  //Pin motores
int sensor[] = {6,7,8}; //Pin sensores
int motorP = 9;   //Pin motor premio
int sensorP = 10; //Pin sensor premio
int divi = 3;     //Divisiones por octavo
int pos[] = {1,1,1}; //Posicion inicial
int premios[] = {1,2,3,4};  //Valor de los premios
int monedas = 0;  //Premio monetario

void setup() {
  pinMode(laser,INPUT);
  pinMode(LED,OUTPUT);
  pinMode(boton,INPUT);
  pinMode(motores[1],OUTPUT);
  pinMode(motores[2],OUTPUT);
  pinMode(motores[3],OUTPUT);
  pinMode(sensor[1],INPUT);
  pinMode(sensor[2],INPUT);
  pinMode(sensor[3],INPUT);
}

void loop() {
  // Detectar moneda
  int i=0;
  while( digitalRead(laser)){ //laser==0 -> moneda  
    i++;
    if (i==255){
      i=0;
    }
  }
  randomSeed(i);
  // LED + boton
  digitalWrite(LED,1);
  while (digitalRead(boton)){ //boton==0 -> epezar
  }
  digitalWrite(LED,0);
  // Girar motores
  for(i=1;i<=3;i++){
    digitalWrite(motores[i],1);
  }
    //Crear valores de parada aleatorios
  int paradas[3];
  int octavos[] = {0,0,0};  //Octavos de vuelta
  for (i=1;i<=3;i++){
    paradas[i] = random(16,50);
  }
  // Parada
  int pulsos[] = {0,0,0};
  while(paradas[1] || paradas[2] || paradas[3]){
    for (i=1;i<=3;i++){ //Cuenta de vueltas
      if((sensor[i] && !pulsos[i]%2)||(!sensor[i] && pulsos[i]%2)){
        pulsos[i]++;
        if(pulsos[i]==6){
          octavos[i]++;
          pulsos[i] = 0;
        }
      }
      if(paradas[i]<=octavos[i]){ //Condicion de parada
        digitalWrite(motores[i],0); //Parar motor
        paradas[i] = 0;
      }
    }
  }
  //Resultado
    //Calcular posiciones finales
  for(i=1;i<=3;i++){
    pos[i] = pos[i] + octavos[i]%8;
    if(pos[i]>8){
      pos[i] = pos[i] - 8;
    }
  }
    //Determinar caras
  int caras[3];
  for(i=1;i<=3;i++){
    if(pos[i]>4){
      caras[i] = pos[i] - 4;
    } else{
      caras[i] = pos[i];
    }
  }
    //Repartir premios
  bool a = caras[1] == caras[2];
  bool b = caras[2] == caras[3];
  bool c = caras[1] == caras[3];
  if(a && b && c){
    //Premio x1
    monedas = premios[caras];
  }else if(a || b || c){
    //Premio x0.5
    if(a || b){
      monedas = floor(premios[caras[2]]/2);
    } else{
      monedas = floor(premios[caras[3]]/2);
    }
  }
    //Arrancar motor
    
}
