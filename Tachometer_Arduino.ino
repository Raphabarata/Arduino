/*   Codigo de leitor de rotaçao
 *    Desenvolvido no Laboratório de Distribuição de Energia Elétrica
 *    O sensor tem como objetivo captar a rotação por minuto do eixo de um motor 
 *    gravando seus dados e calculando sua média.
 *    31/01/2018
*/

#include <SD.h>
#include<SPI.h>
#include <LiquidCrystal.h>



int CS_pin = 4;
int pow_pin = 8;
int buttonPin = 3;
int buttonPin2 = 1;

LiquidCrystal lcd(10, 9, 8, 7, 5, 6); //Define os pinos que serão utilizados para ligação ao display

int pino = 2;
int pulsos;
int button1 = 1;
int button2 =1;
unsigned long r;
unsigned long  tempo1;
unsigned long  tempo2;
float rpm;
float cont;
float soma;
float rpm_medio;

// Conta o número de pulsos durante a rotaçao do motor
void contagem () {
  
  pulsos++;  
  
  }
  
//  Deleta os arquivos existentes no cartao SD
void deletarfiles(){

  button1 = 1;
  button2 = 1;
  lcd.clear(); //Limpa a tela
  lcd.setCursor(0, 0);
  lcd.print("Delete all files? ");
  lcd.setCursor(0, 1);
  lcd.print("1-YES   2-NO?");
  delay(1000);
  
  while (button1 == 1 && button2 == 1){
      
    button1 = digitalRead(buttonPin);
    button2 = digitalRead(buttonPin2);
  
      if(button1 == 0){
        
      SD.remove("RPM.txt");
      SD.remove("MEDIA.txt");
      
      }

      if(button2 == 0){

      button2 = 0;
      }
 
    }
  button2 = 1;
  button1 = 0;
  
}

// Quando pressionado o botao 1, cria os arquivos e começa a copiar os dados
void entrarbotao1(){

File dataRpm = SD.open("RPM.txt", FILE_WRITE); // Cria o arquivo RPM.txt


 if(dataRpm){         // If necessario para caso ja exista o arquivo 
   
   if ( pulsos == 2) {
    tempo1 = micros();
    delay(200);
  }
  
  if ( pulsos == 17) {
    tempo2 = micros();
    detachInterrupt(0);       // Para a funçao attachInterrupt e começa a calcular
    rpm = (60000000.0000 /(tempo2 - tempo1))*15.00;  

  lcd.clear(); //Limpa a tela
  lcd.setCursor(0, 0);  // Coloca o cursor no espaço especificado
  lcd.print("RPMi = ");
  lcd.print(rpm);
  lcd.setCursor(0, 1);
  lcd.print("RPMag = ");
  lcd.print(rpm_medio);
        
  dataRpm.println(rpm);

    cont++;
    pulsos = 0;
    tempo1 = 0;
    tempo2 = 0;
    
    attachInterrupt(0, contagem, RISING); // Inicia a funçao para captar os pulsos
  
      if (cont < 5){
       soma = soma + rpm;
      }

      if (cont == 5){
       File dataMedia = SD.open("MEDIA.txt", FILE_WRITE);
       rpm_medio = (soma + rpm)/cont;
       dataMedia.println(rpm_medio);
       dataMedia.close();
     
       soma = 0;
       cont = 0;
      }
} 
dataRpm.close();
} 
}

// Quando pressionado o botao 2, apenas começa a ler os dados
void entrarbotao2(){

 if ( pulsos == 2) {
    tempo1 = micros();
    delay(200);
  }
  if ( pulsos == 17) {
    tempo2 = micros();
    detachInterrupt(0);
    rpm = (60000000.0000 /(tempo2 - tempo1))*15.00;

    lcd.clear(); //Limpa a tela
    lcd.setCursor(0, 0);
    lcd.print("RPMi = ");
    lcd.print(rpm);
    lcd.setCursor(0, 1);
    lcd.print("RPMag = ");
    lcd.print(rpm_medio);
      
    cont++;
    pulsos = 0;
    tempo1 = 0;
    tempo2 = 0;
    delay(1000);
    attachInterrupt(0, contagem, RISING);
  
     if (cont < 5){
     
      soma = soma + rpm;
      }

      if (cont == 5){
       rpm_medio = (soma + rpm)/cont;
     
       soma = 0;
       cont = 0;
       }
} 
  
}



void setup() {
    
  Serial.end(); // Necessario para uso das portas digitais 0 e 1
  lcd.begin(16, 2);  //Define o número de colunas e linhas do LCD
  
  pinMode(CS_pin,OUTPUT); // Ativa o pino 4 necessario para o uso do SD shield
  digitalWrite(CS_pin,HIGH);
  pinMode(pow_pin,OUTPUT);  // Ativa o pino 8 necessario para o uso do SD shield 
  digitalWrite(pow_pin,HIGH);  
  pinMode(10,OUTPUT);
  pinMode(buttonPin,INPUT_PULLUP); // Funcao pullup utiliza o resistor interno do arduino
  pinMode(buttonPin2,INPUT_PULLUP);
  pinMode(pino, INPUT);

  lcd.clear(); //Limpa a tela
  lcd.setCursor(2, 0);
  lcd.print("Developed by");
  lcd.setCursor(5, 1);
  lcd.print("LADEE");
  delay(3000); // posso colocar delay aqui? sim

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1- Log Recording");
  lcd.setCursor(0, 1);
  lcd.print("2- Display"); 

 while (button1 == 1 && button2 == 1){

  button1 = digitalRead(buttonPin);
  button2 = digitalRead(buttonPin2);

  if (button1 == 0){
    
    button1 = 0;  
       
    while(!SD.begin(CS_pin))
     {
      lcd.clear(); //Limpa a tela
      lcd.setCursor(0, 0);
      lcd.print("Card Failed");
     
      //Serial.println("Card Failed");
      digitalWrite(6,HIGH);
      delay(2000);
  
      lcd.clear(); //Limpa a tela
      lcd.setCursor(5, 0);
      lcd.print("Error");
      lcd.setCursor(1, 1);
      lcd.print("Reinsert card.");    
     }  
       
      lcd.clear(); //Limpa a tela
      lcd.setCursor(0, 0);
      lcd.print("Card Ready");
      deletarfiles();
      }

     if( button2 == 0) {
       
       button2 = 0;
         
       }


 }

  lcd.clear(); //Limpa a tela
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  
  pulsos = 0;
  tempo1 = 0;
  tempo2 = 0;
  cont = 0;
  soma = 0;
  rpm_medio = 0;
  
 attachInterrupt(0, contagem, RISING); // Funcao que capta a subida de um pulso
}

// É importante saber que o button1 é normalmente aberto, enquanto que o button2 é normalmente fechado. Portanto, sem pressionar, button1 = 1 e button2 = 0
void loop() {

if(button2 == 0){   
  entrarbotao2();

}
 
if(button1 == 0){

  entrarbotao1();
}


  }
