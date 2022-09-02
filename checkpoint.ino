#include <SPI.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
const int TAMANHO = 50;
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
  
char st[20];
 
void setup() 
{
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
}
 
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  
  String id = "";
  String est = "";
  
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  if (conteudo.substring(1) == "87 F1 4B 73") //UID 1 - Chaveiro
  {
    Serial.println("Ola belezura !");
    Serial.println();
    delay(3000);
    est = "Autorizado";
    id = "87 F1 4B 73";
  }
 
  if (conteudo.substring(1) == "2A 63 4C 73") //UID 2 - Cartao
  {
    Serial.println("Ola, Cartao diferente !");
    Serial.println();
    delay(3000);
    est = "Não autorizado";
    id = "2A 63 4C 73";
  }

  {
  StaticJsonDocument<TAMANHO> json; //Aloca buffer para objeto json

  json["cartao"] = id;
  json["estado"] = est;

  serializeJson(json, Serial);
  Serial.println();
  //delay
  delay(500);
}
} 
 
