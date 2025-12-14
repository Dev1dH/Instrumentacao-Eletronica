/*  Projeto 7: Desenvolvimento de uma biblioteca para o Hx711
  Discente: Devid Henrique
  Disciplina: Instrumentação Eletrônica*/

//DOUT = informaçao
//PD_SCK = clock

//quando DOUT (HIGH) e PD_SCK(LOW) --> indica que output date não está pronto
//quando DOUT (LOW), PD_SCK(HIGH) --> indica que output data está pronto

 /*
    |-->Para 25 pulsos de clock: ganho = 128
    |-->Para 26 pulsos de clock: ganho = 32
    |-->Para 27 pulsos de clock: ganho = 64

    *Cada pulso de clock adicional desloca um bit, começando pelo bit menos
    significativo até que todos os 24 bit tenham sido deslocados   
 */

//pinagem de DOUT (envia informação do HX711 para o Arduíno)
 #define DOUT 4
 //pinagem de PD_SCK (envia o clock do Arduíno para o HX711)
 #define PD_SCK 5


void setup() {
  Serial.begin(9600);         //Taxa de comunicação no serial
  pinMode(PD_SCK, OUTPUT);
  pinMode(DOUT, INPUT);
}

void loop() {

unsigned long ReadCount(void){ 
  unsigned long Count; 
  unsigned char i; 

  digitalWrite(DOUT, HIGH);   //ADDO=1;            
  digialWrite(PD_SCK, LOW);   //ADSK=0; 
  Count=0; 
  
  while(digitalRead(DOUT)); 
    for(i=0;i<24;i++){ 
    digitalWrite(PD_SCK, HIGH);   //ADSK=1; 
    Count=Count<<1; 
    digitalWrite(PD_SCK, LOW);   //ADSK=0; 
    if(digitalRead(DOUT)) Count++; 
    } 
  digitalWrite(PD_SCK, HIGH);        //ADSK=1; 
  Count=Count^0x800000;  
  digitalWrite(PD_SCK, LOW);         //ADSK=0; 
  return(Count); 
} 



}
