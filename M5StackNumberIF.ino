// 数字入力IF
#include <M5Stack.h>

int number = 0; // 入力数字
int num = 0;    // 選択中の数字
int ok = 0;     // 数字入力確定、キャンセル
unsigned long displayTime = 0;  // 長押し判定用
int ll = 0;     // 数字入力状態 0=無選択、1=Bボタン押、2=長押状態、3=確定/キャンセル選択中


void setup() 
{
	// put your setup code here, to run once:
	M5.begin();
	M5.Lcd.setCursor(0, 0);
	M5.Lcd.fillScreen(BLACK);
	M5.Lcd.setTextFont(4);
	M5.Lcd.setTextColor(WHITE, BLACK);
	
	inputdisp(GREEN, 0, 0, " Number Input Demo ");
}


// 数字入力表示初期化
// (x,y)にstr文字を表示
void inputdisp(int col, int x, int y, char *str)
{
	int i;
	
	M5.Lcd.fillScreen(BLACK);
	M5.Lcd.setTextFont(4);
	M5.Lcd.setTextColor(col, BLACK);
	M5.Lcd.setCursor(x, y);
	M5.Lcd.print(str);
	
	// 文字入力エリア
	M5.Lcd.setTextFont(7);
	M5.Lcd.setTextColor(BLACK, col);
	numdisp(30, 40, number, 9, col, col);
	
	// 数字選択エリア
	for(i=0; i<10; i++){
		if(num == i){  // 選択中
			M5.Lcd.setTextColor(BLACK, WHITE);
		}
		else{
			M5.Lcd.setTextColor(DARKGREY, LIGHTGREY);
		}
		M5.Lcd.drawNumber(i, 10+30*i, 240-48, 7);
	}
}


// 数字選択描画
// on=選択数字、off=解除数字
void select(int off, int on)
{
	M5.Lcd.setTextColor(DARKGREY, LIGHTGREY);
	M5.Lcd.drawNumber(off, 10+30*off, 240-48, 7);
	M5.Lcd.setTextColor(BLACK, WHITE);
	M5.Lcd.drawNumber(on, 10+30*on, 240-48, 7);
}


// 数字入力エリア描画
// 右上指定座標(x, y)から9文字分(幅270、高48+2）のエリアに数字描画
// 数字は右詰め
void numdisp(int x, int y, int num, int keta, int col, int cur)
{
	// 下線
	if(cur != BLACK){
		M5.Lcd.fillRect(x, y+48, 270, 2, col);
	}
	// フォント7は、30×48画素
	int i;
	int j;
	int k = 0;
	int pos = 0;
	int div = pow(10, keta-1);
	for(i=0; i<keta; i++){
		j = (num / div) % 10;
		if(j != 0){   // はじめて1以上の数値が来た時から入力を認識
			k = 1;
		}
		if(j != 0 || k != 0){
			M5.Lcd.setTextColor(BLACK, col);
			M5.Lcd.drawNumber(j, x+30*i, y, 7);
		}
		if(k != 0){
			pos++;
		}
		div /= 10;
	}
}


// 数字入力確定、キャンセル選択表示
// 0=ok, 1=cancel
void okdisp(int ok)
{
	if(ok == 0){
		M5.Lcd.setTextColor(BLACK, WHITE);
	}
	else{
		M5.Lcd.setTextColor(DARKGREY, LIGHTGREY);
	}
	M5.Lcd.setCursor(30, 240-48-26);
	M5.Lcd.setTextFont(4);
	M5.Lcd.print(" [OK] ");
	
	if(ok != 0){
		M5.Lcd.setTextColor(BLACK, WHITE);
	}
	else{
		M5.Lcd.setTextColor(DARKGREY, LIGHTGREY);
	}
	M5.Lcd.setCursor(160, 240-48-26);
	M5.Lcd.setTextFont(4);
	M5.Lcd.print(" [CANCEL] ");
}



void loop() {
	// put your main code here, to run repeatedly:
	M5.update();
	if(M5.BtnA.wasPressed()){
		if(ll == 0){
			if(num > 0){
				select(num, num-1);
				num--;
			}
		}
		else if(ll == 3){
			ok = 0;
			okdisp(ok);
		}
	}
	if(M5.BtnC.wasPressed()){
		if(ll == 0){
			if(num < 9){
				select(num, num+1);
				num++;
			}
		}
		else if(ll == 3){
			ok = 1;
			okdisp(ok);
		}
	}
	
	if(M5.BtnB.wasPressed()){
		if(ll >= 2){
			
		}
		else{
			displayTime = millis();
			ll = 1;
		}
	}
	if(M5.BtnB.wasReleased()){
		if(ll == 1){
			if(number < 100000000){
				number = (number * 10) + num;
				numdisp(30, 40, number, 9, GREEN, GREEN);
			}
			ll = 0;
		}
		else if(ll == 2){
			ll++;
		}
		else if(ll == 3){
			number = 0;
			num = 0;
			inputdisp(GREEN, 0, 0, " Number Input Demo ");
			ll = 0;
		}
	}
	
	// 長押し判定
	if(ll == 1){
		if(millis() - displayTime >= 1000){
			okdisp(0);
			ll = 2;
		}
	}
}


