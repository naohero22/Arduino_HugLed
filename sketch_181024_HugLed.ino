#include <Adafruit_NeoPixel.h>

double R1 = 10.0;
const int PIN_PRESSURE = 1;
const int PIN_LED = 7;
const int ledCount = 60;
const int delayTime = 10;
int r = 255;
int g = 30;
int b = 0;
double pl = 0; //Pressure→LED 最小0 最大1
double plTime = 0; //光っている時間

Adafruit_NeoPixel strip = Adafruit_NeoPixel(ledCount, PIN_LED, NEO_GRB + NEO_KHZ800);

void setup(){
  Serial.begin(9600);  // シリアル通信速度
  strip.begin();
  strip.show();
}

void loop(){
  checkPressure();
  if (plTime < 1000){
    flat(strip.Color(r*pl, g*pl, b*pl), delayTime);
  }else if(plTime >= 1000){
    //flat(strip.Color(255, 0, 255), 50);
    //rainbow(10);
    rainbowCycle(1);
    delay(1000);
    plTime = 0;
  }
}

//圧力センサのチェック
void checkPressure(){
  // 変数の宣言
  double Vo, Rf, fg;
  int ain = analogRead(PIN_PRESSURE);
  // アナログ入力値から出力電圧を計算
  Vo = ain * 5.0 / 1024;
  // 出力電圧からFRSの抵抗値を計算
  Rf = R1*Vo / (5.0 - Vo);
  // FRSの抵抗値から圧力センサの荷重を計算
  fg = 880.79/Rf + 47.96;
  // 荷重データをLEDの光の強さのパラメータに計算。最小fg=50のとき0、最大fg200のとき1.0
  pl = fg*1.0/(100-50)-1.0*50/(100-50);
  if (pl >= 1.0){ 
    pl = 1.0;
    plTime = plTime + delayTime;
  } else if (pl < 1.0 && pl > 0){
    plTime = 0;
  } else if (pl <= 0){
    pl = 0.0;
    plTime = 0;
  }
  // 荷重データをシリアル通信で送る
  Serial.println(fg);
  Serial.println(pl);
  Serial.println(plTime);
  delay(delayTime);
}


//全てのLEDが指定した色になる。
void flat(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;
  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


//参考
//http://denshi.blog.jp/arduino/fsr
//http://tsuttsu305.hatenablog.jp/entry/2016/09/27/CH34x_USB-UART_macOS_10.12_Sierra%E5%AF%BE%E5%BF%9C%E3%83%89%E3%83%A9%E3%82%A4%E3%83%90

