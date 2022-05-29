#define KEY1 0 //diziden gelecek değişkenler için tanımladık (bu örnekte 2 veri alacağız)
#define KEY2 0


/*-----( kütüphaneleri yükleyelim )-----*/ 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//RF modülün pinleri
#define CE_PIN   8
#define CSN_PIN 9
 #define IRQ_PIN 10

const uint64_t pipe = 0xE8E8F0F0E1LL; // adres
 
 
/*-----( radio nesnesi yaratılıyor)-----*/
RF24 radio(CE_PIN, CSN_PIN); // 
/*-----( değişkenler tanımlanıyor )-----*/
 
// radiodan gelen 5 adet veriyi almak için bir dizi oluşturuyoruz
int verilerim[] = {KEY1, KEY2}; // keylerimiz
int yon_degeri,motor_degeri;
const int inA=6;
const int inB=7;
 
void setup()   
{
  Serial.begin(57600);
  delay(1000);
  pinMode(inA,OUTPUT);
  pinMode(inB,OUTPUT);
  //////////////////////////////////////////////////////////
  
 //radio ile iletişim kuruyoruz
  Serial.println("Nrf24L01 Alıcı Başlatılıyor");
  radio.begin();
  radio.openReadingPipe(1, pipe);
  radio.startListening();
}//--(setup bitti )---
 
 
void loop()
{
  
 
  if ( radio.available() ) //eğer radyo ile temas kurabilmişse
  {
    
    // done değişkeni false yapıyoruz ve while içinde sonsuz döngü oluşturarak verileri toplayacağız
    bool done = false;
    while (!done) 
    {
      // veriyi alıyoruz.
      done = radio.read( verilerim, sizeof(verilerim)); 
      yon_degeri=map(verilerim[0],0,1023,0,255);
      motor_degeri=map(verilerim[1],0,1023,0,255);
      //serial monitörden verileri görmek için;
      Serial.print("yön= ");
      Serial.println(yon_degeri);
      Serial.print("motor = ");
      Serial.println(motor_degeri);
     //motor set değerinden yukarıda veya aşağıda ise bu işlemler gerçekleşecek
   if( motor_degeri >=0 && motor_degeri <= 5)
     {
      digitalWrite(inB,HIGH);
      digitalWrite(inA,HIGH);
     }
      if( motor_degeri >=10 && motor_degeri <= 255)
     {
      digitalWrite(inB,LOW);
      digitalWrite(inA,LOW);
     }
   

    if( yon_degeri >=170 && yon_degeri <= 255)
     {
      digitalWrite(inB,LOW);
      digitalWrite(inA,HIGH);
     }
   
   if(yon_degeri >= 0 &&yon_degeri <=140)
     {
     digitalWrite(inB,HIGH);
     digitalWrite(inA,LOW);
     }

    }//while
  }//ilk if  
 
  
  else//eğer radyo bulunamazsa
  {
    Serial.println("Verici Bulunamadı");
    verilerim[0] = 0;
    verilerim[1] = 0;

 
  }
     
 
}//--(main loop sonu )---
