//variaveis declaradas
int ronda;
int k;
String num;
int valor;
unsigned long tempo = millis();
unsigned long maxtime = 20000;
int target;
int start;
int botao;
int ler;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i<3; i++){       //configura os botões
    pinMode(2+i, INPUT_PULLUP);
  }
  for (int i = 0; i<4; i++){       //configura os LEDs
    pinMode(8+i, OUTPUT); 
  }
  randomSeed(analogRead(0));
  target = geravalores();
  start = geravalores();
  inicio();
}

int geranumero(){           //permite gerar números de 1 a 3
  int n = random(1, 4);
  return n;
}

int geravalores(){            //permite gerar numeros de 1 a 255
  int v = random(0, 256);    
  return v;
}

void apagaluzes(){              //função que desliga todos os LEDs
  for (int i = 0; i<4; i++){
    digitalWrite(8+i, 0);
  }
}
 
int operacoes(){      //função que define as operações possíveis
    k = geranumero();
    if (k==1)Serial.println("Operaçoes permitidas: AND-vermelho e OR-amarelo");    
    
    if (k==2)Serial.println("Operaçoes permitidas: XOR-branco e OR-amarelo");
    
    if (k==3)Serial.println("Operaçoes permitidas: AND-vermelho, XOR-branco e OR-amarelo"); 
  }

void inicio(){
  operacoes();
  Serial.print("Valor target: ");
  Serial.println(target, BIN);
  Serial.print("Valor base: ");
  Serial.println(start, BIN);
  Serial.println("Introduza um valor:");
}

void operacaoOR(){
  start = (start|valor);
  Serial.print("Valor obtido: ");
  Serial.println(start,BIN);
  if (start==target)vence();
  else {
    Serial.println("Introduza um valor:");
    botao = 0;
    ler = 1;
    delay(300);
  }
}

void operacaoAND(){
  start = start&valor;
  Serial.print("Valor obtido: ");
  Serial.println(start,BIN);
  if (start==target)vence();
  else {
    Serial.println("Introduza um valor:");
    botao = 0;
    ler = 1;
    delay(300);
  }
}

void operacaoXOR(){
  start = start^valor;
  Serial.print("Valor obtido: ");
  Serial.println(start,BIN);
  if (start==target)vence();
  else {
    Serial.println("Introduza um valor:");
    botao = 0;
    ler = 1;
    delay(300);
  }
}

void vence(){
  if(start==target){
    Serial.println("Boa! Acertaste");
    target = geravalores();
    start = geravalores();
    delay(1000);
    tempo=millis();
    inicio();
    ronda = 0;
    botao = 0;
    ler = 0;
    delay(100);
  }
}

void le_string(){
  if (Serial.available()>0){
    num= Serial.readStringUntil('\n');
    valor=num.toInt();
    Serial.print("Valor lido: ");
    Serial.println(valor,BIN);
    Serial.println("Prima um operador");
    ler = 0;
    botao = 1;
  }
}

void perde(){
  if (((millis()-tempo)==maxtime)&&(start!=target)){ //da restart ao jogo quando perde
    Serial.println("Falhaste! Tenta de novo");
    target = geravalores();
    start = geravalores();
    delay(1000);
    tempo=millis();
    inicio();
    ronda = 0;
    botao = 0;
    ler = 0;
  }
}
void tempoled(unsigned long maxtime){
  if ((millis()-tempo)>=(maxtime/4)) digitalWrite(8, 1);      
  if ((millis()-tempo)>=(maxtime/2)) digitalWrite(9, 1);    
  if ((millis()-tempo)>=(3*maxtime/4)) digitalWrite(10, 1);   
  if ((millis()-tempo)>=maxtime) digitalWrite(11, 1);
  perde();
}
  
void loop() {
  ronda = 1;
  botao = 0;
  ler = 1;
  apagaluzes();
    
    while(ronda == 1){      
    
    tempoled(maxtime);
    
    while(ler == 1){
      tempoled(maxtime);
      le_string();
    }
   
    while(botao == 1){
      tempoled(maxtime);
      bool AND = !digitalRead(4);
      bool XOR = !digitalRead(3);
      bool OR = !digitalRead(2);
      
      if (k==1){
        if (OR) operacaoOR();
  
        if (AND) operacaoAND(); 
      }
      
       if (k==2){
        if (OR) operacaoOR();
        
        if (XOR) operacaoXOR();
      }
       
       if(k==3){
        if (OR) operacaoOR();
        
        if (XOR) operacaoXOR();
        
        if (AND) operacaoAND();
      }
    }
  }
}
