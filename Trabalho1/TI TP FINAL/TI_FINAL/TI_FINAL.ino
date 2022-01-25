int n;
bool truereading=false;
bool lastbts = false;
unsigned long debounceDelay = 50;
unsigned long bounce;
int count = 0;
int interval = 15000;
int a = 1;

void setup(){
  Serial.begin(9600);
  Serial.println("Tem 15 segundos para acertar o numero!");
  pinMode(5, INPUT_PULLUP);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  randomSeed(analogRead(0));
  n = random(1, 32);
}

void loop(){
    
  bool bts=!digitalRead(5);    //lê o estado esperado do pino 5


  for (int bit = 0; bit<=4; bit++){    //ascende cada led conforme o numero que lhe corresponde (0/1)
    digitalWrite(6+bit, (n>>bit)&1);
  }

  if (bts!=lastbts){    //Sempre que o botão mudar de estado guardamos o valor do millis()(instante atual)
    bounce = millis();
  }
  if (((millis()-bounce)>1000)&&(bts==true)){    //reinicia se o botão for primido por mais de 1 segundo
    loop();
  }

  if ((millis()-bounce)>debounceDelay){    //Se passaram 50ms desde a ultima variação do estado do botão
    if (bts!=truereading){    //Se a leitura atual for diferente da última leitura
      truereading = bts;    //Estado verdadeiro = estado atual

      if (truereading==true){ // Quando o estado verdadeiro for verdadeiro, então o botão foi primido
        count++;    //Quando o estado do botão mudar de low para high, o contador aumenta +1
        Serial.println(count);
      }
   
      if (count >= 31){   //Se o limite máximo for atingido o contador retorna a 0
        count = 0;
      }
    }
  }
  if ((count == n)&&(millis()>=(interval*a))){   //Se passarem 15segundos e o numero do contador correspoder ao numero certo então completou o jogo
        Serial.println("Acertou!!!");
        count = 0;
        for (int bit=0; bit<=4; bit++) {   //apaga todos os leds
          digitalWrite(6+bit, LOW);
        }
        delay(1000);
        
        for (int bit=0; bit<=4; bit++) {   //ascende os leds com o efeito de varrimento
          digitalWrite(6+bit, HIGH);
          delay(100);
        }
        for (int bit=0; bit<=4; bit++){   //desliga os leds com o efeito de varrimento
          digitalWrite(6+bit,LOW);
          delay(100);
        }
        n = random(1, 32);
        delay(500);
        a++;
        loop();
  }
    
  if ((count != n)&& (millis()>=(interval*a))){   //Se passarem 15segundos e o numero do contador não correspoder ao numero certo então o jogo recomeça
     Serial.print("Falhou, a resposta correta era  ");
     Serial.print(n);
     Serial.println(".");
     count = 0;
     n = random(1, 32);
     a++;
     loop(); 
  }
 lastbts = bts;    //atualizamos o estado anterior com o ultimo estado do botão neste loop
}                              





 
