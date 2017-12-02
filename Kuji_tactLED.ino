const int LED_Number = 5;
int WaiteTime = 500;

//わかりやすくするため
const boolean B_HIGH = LOW;
const boolean B_LOW = HIGH;

int LEDpin[LED_Number] = {5, 3, 7, 9, 2};
const int SWpin = 8;
int mode = 0;
int hazureVal = 0; //外れたときのLEDの配列番号
int atari = 0;     //当たり＝1　はずれ＝0
int HL = HIGH;     //点滅用変数（初期値HIGH）

int sw_old = HIGH; //前のスイッチの値

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDpin, 1);
  pinMode(SWpin, INPUT_PULLUP);
  Serial.begin(9600);
  allLED_Control(LOW);
}

void loop() {
  LED_kuji();
}

void allLED_Control(boolean Val) {
  for (int i = 0; i < LED_Number; i++) {
    digitalWrite(LEDpin[i], Val);
  }
}

void LED_kuji() {
  switch (mode) {
    case 0: //まだ始まっていない
      if (P_edge() == true) { //ボタンを押すと始まる
        mode = 1;
      }
      break;
    case 1:// LEDが移動している
      for (int i = 0; i < LED_Number; i++) {
        LED_moving(i);
        for  (int j = 0; j < WaiteTime; j++) {
          if (P_edge() == true) {//立ち上がったら
            judgement(i);
            mode = 2;
            break;
          }
          delay(1);
        }
        if (mode == 2) break;
      }
      break;

    case 2:// 結果表示
      LED_result(); //結果表示
      for (int i = 0; i < WaiteTime; i++) {
        if (P_edge() == true) { //立ち上がったら
          mode = 0;
          HL = HIGH;
          allLED_Control(LOW);
          break;
        }
        delay(1);
      }
      break;
  }
}

void LED_moving(int i) { //LEDの移動　i = 今光っているLEDの配列番号
  if (i == 0) { //最初のLEDのとき
    digitalWrite(LEDpin[i], HIGH);
    digitalWrite(LEDpin[LED_Number - 1], LOW); //最後のLEDを消す
  }
  else {
    digitalWrite(LEDpin[i], HIGH);
    digitalWrite(LEDpin[i - 1], LOW); //前のを消す
  }
}

void judgement(int i) { //i = 今光っているLEDの配列番号
  if (i == 2) { //当たり番号
    atari = 1;
  }
  else {
    atari = 0;
    hazureVal = i;//はずれたときのLEDを格納しておく
  }
}

void LED_result() { //結果表示
  if (atari == 1) {
    allLED_Control(HL); //全て点滅(最初点灯)
    HL = !HL;
  }
  else {
    digitalWrite(LEDpin[hazureVal], HL); //1つ点滅(最初点灯)
    HL = !HL;
  }
}

boolean P_edge() { //ポジティブエッジでタクトスイッチの状態を判断
  boolean sw_now;
  boolean edge = false;
  sw_now = digitalRead(SWpin);
  if (sw_now == B_HIGH && sw_old == B_LOW) { //立ち上がり
    edge = true;
    delay(10);
  }
  else if (sw_now == B_LOW && sw_old == B_HIGH) { //立ち下がり
    delay(10);
  }
  sw_old = sw_now;
  return edge;
}






