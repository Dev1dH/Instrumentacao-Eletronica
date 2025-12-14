/*
  PROJETO FINAl de INSTRUMENTAÇÃO ELETRÔNICA
  Discente: Devid Henrique
  Docente: Wilton Lacerda
  Projeto: Multímetro digital
*/

/*DECLARAÇÃO DAS BIBLIOTECAS*/
#include <Wire.h>               // biblioteca para comunicação i2c
#include <Adafruit_SSD1306.h>   // biblioteca para o display OLED

Adafruit_SSD1306 OLED; // meu objeto do display OLED

#define nf 12   // pino da saída do NE555
#define R_4 11  // pino para o resistor de 220 kohm
#define R_3 10  // pino para o resistor de 22 kohm
#define R_2 9   // pino para o resistor de 2.2 kohm
#define R_1 8   // pino para o resistor de 220 ohm
#define dt 7    //pino da data (informação)
#define clk 6   //pino do clock do encoder
#define Ra 10000.0  // valor do resistor Ra do capacímetro
#define Rb 100000.0 // valor do resistor Rb do capacímetro
#define R1 1000000.0 // valor do resistor R1 do voltímetro
#define R3 330000.0 // valor do resistor R3 do voltímetro

int th_us = 0;                 //
int tl_us = 0;                 // 
volatile int contador_dt = 0;  // contador inicia zerado
int currentStateCLK;           // valor atual do encoder
int lastStateCLK;              // último valor do encoeder
float T_us = 0.0;              //
float freq = 0.0;              //
float cap_valor = 0.0;         //
float BitsArduino = 1023.0;    //

// função para medir resistência
float medir_resistencia(int analogico, int Rref){
  // Relação de Van/V1
  float VanV1 = BitsArduino/analogico;
  // Calcula a resistência desconhecida
  float R2 = Rref/(VanV1-1);            
  return R2;
}

// função para configurar os pinos
void PinosMode(int modo8, int modo9, int modo10, int modo11){    
  pinMode(8, modo8);
  pinMode(9, modo9);
  pinMode(10, modo10);
  pinMode(11, modo11);             
}

// função para habilitar os pinos cmo high ou low
void PinosDigitais(bool pino8, bool pino9, bool pino10, bool pino11){
  digitalWrite(8, pino8);
  digitalWrite(9, pino9);
  digitalWrite(10, pino10);
  digitalWrite(11, pino11);
}

// função para ler o valor das resistências
void ler_resistencia(){
  
  // verifica o valor das saídas analógicas para cada porta digital
  // habilita apenas a porta 8 como saída
  PinosMode(OUTPUT, INPUT, INPUT, INPUT);
  // habitita apenas o pino para resistor de 220 R
  PinosDigitais(1,0,0,0);
  int Digital_8 = analogRead(A1);
  delay(10);
  
  // habilita apenas a porta 9 como saída
  PinosMode(INPUT, OUTPUT, INPUT, INPUT);
  // habitita apenas o pino para resistor de 22 kR
  PinosDigitais(0,1,0,0);
  int Digital_9 = analogRead(A1);
  delay(10);
  
  // habilita apenas a porta 10 como saída
  PinosMode(INPUT, INPUT, OUTPUT,INPUT);
  // habitita apenas o pino para resistor de 22 kR
  PinosDigitais(0,0,1,0);
  int Digital_10 = analogRead(A1);
  delay(10);

   // habilita apenas a porta 11 como saída
  PinosMode(INPUT, INPUT, INPUT, OUTPUT);
  // habitita apenas o pino para resistor de 220 kR
  PinosDigitais(0,0,0,1);
  int Digital_11 = analogRead(A1);
  delay(10);
  
  if(Digital_11 < 800){
    if(Digital_8 <= 520){
        float resistor_220 = medir_resistencia(Digital_8, 220.0);
        display_mensagem(resistor_220);
        OLED.print(" R");
        OLED.display();
    
        Serial.print("Escala: 220 Ω | ");
        Serial.print("Resistência: ");
        Serial.print(resistor_220);
        Serial.println(" Ω");}
    else if(Digital_9 >= 512 && Digital_9 <= 930){
      float resistor_2k2 = medir_resistencia(Digital_9, 2200.0);
    
      display_mensagem(resistor_2k2/1000.0);
      OLED.print(" KR");          
      OLED.display();
    
      Serial.print("Escala: 2.2 kΩ | ");
  
      Serial.print("Resistência: ");
      Serial.print(resistor_2k2/1000);
      Serial.println(" kΩ"); } 

    else if(Digital_10 >= 512 && Digital_10 <=930){
      float resistor_22k = medir_resistencia(Digital_10, 22000.0)/1000.0; 
    
      display_mensagem(resistor_22k);
      OLED.print(" KR");          
      OLED.display();

      Serial.print("Escala: 22 kΩ | ");
      Serial.print("Resistência: ");
      Serial.print(resistor_22k);
      Serial.println(" kΩ");} }
  else{
    display_mensagem(0.00);
    OLED.print(" R");
    OLED.display();

    Serial.print("Resistência: ");
    Serial.print("0.00");
    Serial.println(" R");}
}

// função para ler o valor da tensão
void ler_tensao(){
  
  float volts = (6.85*(analogRead(A0)/1024.0))*((R1+R3)/R3);
    
    if(analogRead(A0) < 30){
     display_mensagem(0.00);
     OLED.print(" V");          
     OLED.display();

     Serial.print("Tensão: ");
     Serial.print("0.00");
     Serial.print(" V\n");
    }else{
    display_mensagem(volts);
    OLED.print(" V");          
    OLED.display();

    Serial.print("Tensão: ");
    Serial.print(volts);
    Serial.print(" V\n");
    }
  
}

// função para ler a capacitância desconhecida
void ler_capacitancia_nf(){
    // variável que guarda o valor de subida na saída do NE555
    th_us = pulseIn(nf,HIGH);
    // variável que guarda o valor de descida na saída do NE555
    tl_us = pulseIn(nf,LOW);
    // variável que calcula o período da oscilação do NE555
    T_us  = th_us + tl_us;
    // variável que calcula a frequência da oscilação do NE555
    freq  = 1.06E6/T_us;
    // variável que calcula o valor da capacitância desconhecida
    cap_valor = 1.0E9*(1.443/((Ra+(2*Rb))*freq));
    // exibe no serial monitor o valor da capacitância em nF
    
    // Exibe no display
    display_mensagem(cap_valor);
    OLED.print(" nF");          
    OLED.display();

    // Exibir a capacitância medida no monitor serial
    Serial.print("Capacitância: ");
    Serial.print(cap_valor, 2); // Exibir dois dígitos decimais
    Serial.print(" nF\n");
}

// função para ler o valor da corrente
void ler_corrente(){
  
  float tensaoSensor = (5.0*analogRead(A2))/1023;
  // sensibilidade de 100 mV/A
  float corrente = ((tensaoSensor - 2.5)/(0.100)); 
  
  if(analogRead(A2) < 515){
    display_mensagem(0.00);
    OLED.print(" A");          
    OLED.display();

    Serial.print("Corrente: ");
    Serial.print("0.00"); // Exibir dois dígitos decimais
    Serial.print(" A\n");
  }else{
  display_mensagem(corrente);
  OLED.print(" A");          
  OLED.display();

  // Exibir a corrente medida no monitor serial
  Serial.print("Corrente: ");
  Serial.print(corrente, 2); // Exibir dois dígitos decimais
  Serial.print(" A\n");
  }
}

void display_mensagem(float valor){
  OLED.clearDisplay();                      
  OLED.setTextColor(WHITE);                 
  OLED.setTextSize(2);                      
  OLED.setCursor(20, 12); 
  OLED.print(valor);     
}

void tela_inicial(){

  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);     // inicializa o display
  OLED.clearDisplay();                         
  OLED.setTextSize(1);                         
  OLED.setTextColor(WHITE);                    
  OLED.setCursor(20,10);
  OLED.print("GIRE O");
  OLED.setCursor(20,20);
  OLED.print("ENCODER");
  OLED.display();
}

void setup() {
  
  Serial.begin(9600);          // inicializa a comunicação serial
  Wire.begin();   
    
  pinMode(R_1, INPUT);         // pino digital como saída para o resistor 220 Ω
  pinMode(R_2, INPUT);         // pino digital como saída para o resistor 2.2 kΩ
  pinMode(R_3, INPUT);         // pino digital como saída para o resistor 22 kΩ
  pinMode(R_4, INPUT);         // pino digital como saída para o resistor 220 kΩ
  pinMode(clk, INPUT_PULLUP);  // define o pino do CLK do encoder como input pullup
  pinMode(dt, INPUT_PULLUP);   // define o pino do DT do encoder como input pullup
  pinMode(nf, INPUT);          //o pino da infomração é definido como uma entrada do NE555

  // Lê a valor inicial do clk  
  lastStateCLK = digitalRead(clk); // inicializa o estado anterior
}

void loop() {

  currentStateCLK = digitalRead(clk);

  if (currentStateCLK != lastStateCLK && currentStateCLK == HIGH) {

      if (digitalRead(dt) != currentStateCLK) {
        contador_dt--;
      } else {
        contador_dt++;
      }
        if (contador_dt > 4){ contador_dt = 0; }
        if (contador_dt < 0){contador_dt = 4;} 
      }

      lastStateCLK = currentStateCLK;

          if(contador_dt == 0){tela_inicial(); }
          else if(contador_dt == 1){ler_corrente();}
          else if(contador_dt == 2){ler_capacitancia_nf();}
          else if(contador_dt == 3){ler_tensao();}
          else if(contador_dt == 4){ler_resistencia();}
}
