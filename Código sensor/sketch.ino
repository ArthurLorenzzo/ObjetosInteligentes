#include <EspMQTTClient.h>

#define trigerPin 13
#define echoPin 12
#define ledPin 2
#define speakerPin 5
#define pitch 262

char SSIDName[] = "Wokwi-GUEST"; //nome da rede WiFi
char SSIDPass[] = ""; //senha da rede WiFI



char BrokerURL[] = "broker.hivemq.com"; //URL do broker MQTT
char BrokerUserName[] = ""; //nome do usuario para autenticar no broker MQTT
char BrokerPassword[] = ""; //senha para autenticar no broker MQTT
char MQTTClientName[] = "HiveMQ"; //nome do cliente MQTT
int BrokerPort = 1883; //porta do broker MQTT

String TopicoPrefixo = "Teste32153597"; //prefixo do topico
String Topico_01 = TopicoPrefixo+"/Alarme"; //nome do topico 01

EspMQTTClient clienteMQTT(SSIDName, SSIDPass, BrokerURL, BrokerUserName, BrokerPassword, MQTTClientName, BrokerPort); //inicializa o cliente MQTT

double duration, distance;

void onConnectionEstablished() {
}

void enviarDados() {
  
  clienteMQTT.publish(Topico_01, String(distance, 3) + "cm"); 
 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(ledPin, OUTPUT); 

  pinMode(speakerPin, OUTPUT);

  clienteMQTT.enableDebuggingMessages();

}

void loop() {
  digitalWrite(trigerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigerPin, LOW);
  delayMicroseconds(2);

  duration = pulseIn(echoPin, HIGH);

  distance = (duration/2) * 0.0343;
  
  if(distance<200){
    digitalWrite(ledPin, HIGH);
    tone(speakerPin, pitch);
    delay(300);

    digitalWrite(ledPin, LOW);
    noTone(speakerPin);
    delay(300);
  }
  else{
    digitalWrite(ledPin, LOW);
    noTone(speakerPin);
  }

  clienteMQTT.loop(); //funcao necessaria para manter a conexao com o broker MQTT ativa e coletar as mensagens recebidas
  enviarDados(); //funcao para publicar os dados no broker MQTT

  if (clienteMQTT.isWifiConnected() == 1) {
    Serial.println("Conectado ao WiFi!");
  } else {
    Serial.println("Nao conectado ao WiFi!");
  }

  if (clienteMQTT.isMqttConnected() == 1) {
    Serial.println("Conectado ao broker MQTT!");
  } else {
    Serial.println("Nao conectado ao broker MQTT!");
  }

  Serial.println("Nome do cliente: " + String(clienteMQTT.getMqttClientName())
    + " / Broker MQTT: " + String(clienteMQTT.getMqttServerIp())
    + " / Porta: " + String(clienteMQTT.getMqttServerPort())
  );

  delay(5000);

}

