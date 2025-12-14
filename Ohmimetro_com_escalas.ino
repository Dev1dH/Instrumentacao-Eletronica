/*
  Projeto 2: Ohmímetro automático 
  Discente: Devid Henrique
  Disciplina: Instrumentação Eletrônica
*/
float BitsArduino = 1023.0;

// função para medir resistência (recebe o valor analógico e o resistor de referência como parâmetros)
float MedirResistencia(int analogico, int Rref){
  float VanV1 = BitsArduino/analogico;  // Relação de Van/V1
  float R2 = Rref/(VanV1-1);            // Calcula a resistência desconhecida
  return R2;
}

// função para configurar os pinos
void PinosMode(int modo5, int modo6, int modo7){    
  pinMode(5, modo5);    
  pinMode(6, modo6);
  pinMode(7, modo7);        
}

// função para habilitar os pinos cmo high ou low
void PinosDigitais(bool pino5, bool pino6, bool pino7){
  digitalWrite(5, pino5);
  digitalWrite(6, pino6);
  digitalWrite(7, pino7);
}

void setup() {
  Serial.begin(9600);   //Taxa de comunicação no serial
  pinMode(7, INPUT);    // pino digital como saída para o resistor 220 Ω
  pinMode(6, INPUT);    // pino digital como saída para o resistor 2.2 kΩ
  pinMode(5, INPUT);    // pino digital como saída para o resistor 22 kΩ 
  
}

void loop() {

// verifica o valor das saídas analógicas para cada porta digital
// habilita apenas a porta 5 como saída
PinosMode(OUTPUT, INPUT, INPUT);
PinosDigitais(1,0,0);
delay(100);
int MedidaAnalog5 = analogRead(A0);

// habilita apenas a porta 6 como saída
PinosMode(INPUT, OUTPUT, INPUT);
PinosDigitais(0,1,0);
delay(100);
int MedidaAnalog6 = analogRead(A0);

// habilita apenas a porta 7 como saída
PinosMode(INPUT, INPUT, OUTPUT);
PinosDigitais(0,0,1);
delay(100);
int MedidaAnalog7 = analogRead(A0);

if(MedidaAnalog5 >= 500){
  float resistor5 = MedirResistencia(MedidaAnalog5, 22000);
  Serial.print("Escala: 22 kΩ | ");
  
  if(resistor5 >= 1000 && resistor5 <1000000){
  Serial.print("Resistência: ");
  Serial.print(resistor5/1000);
  Serial.println(" kΩ");
  }
  else if(resistor5 >= 1000000){
  Serial.print("Resistência: ");
  Serial.print(resistor5/1000000);
  Serial.println(" MΩ");
  }
  else{
  Serial.print("Resistência: ");
  Serial.print(resistor5);
  Serial.println(" Ω");     
 }
}
 
else if(MedidaAnalog6 <= 930 && MedidaAnalog6 > 95){
  float resistor6 = MedirResistencia(MedidaAnalog6, 2200);
  Serial.print("Escala: 2.2 kΩ | ");
  
  if(resistor6 >=1000){
  Serial.print("Resistência: ");
  Serial.print(resistor6/1000);
  Serial.println(" kΩ");
  }else{
  Serial.print("Resistência: ");
  Serial.print(resistor6);
  Serial.println(" Ω");
 }
} 

else if(MedidaAnalog7 <= 512){
  float resistor7 = MedirResistencia(MedidaAnalog7, 237); 
  Serial.print("Escala: 220 Ω | ");
  Serial.print("Resistência: ");
  Serial.print(resistor7);
  Serial.println(" Ω");
 }

 delay(1000);
}
