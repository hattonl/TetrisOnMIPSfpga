
#include "mfp_io.h"
#include <stdlib.h>

#define WEIGHT 10
#define HIGHT  20
#define BREAKPIONT(x) { MFP_SEGS = x; \
					while (!(MFP_BUTTONS & 0x01)); \
					while ((MFP_BUTTONS & 0x01));}
enum {X_X = 0, Y_Y, KIND, ROTA};

//X_X 表示所在的行
//Y_Y 表示所在的列

volatile unsigned int a[20][10] = {0};
volatile int diamonds[4] = {0};
volatile int score = 0;
void eliminate_diamonds();
void rotate_diamonds();
unsigned int is_illegal_merge(unsigned int merge);
void creat_diamonds(int val);
void sync_display();
void delay(unsigned int val);
void init_matrix();
unsigned int decimal_to_hex_for_display_on_seg(unsigned int dicimal);

//------------------
// main()
//------------------
int main() {
	volatile unsigned int val;
	volatile unsigned int key_press;
	volatile unsigned int old_rota;
	volatile unsigned int accelerate;
	volatile unsigned int illegal;
	volatile unsigned int conter;
	volatile unsigned int time_seg;
	//volatile unsigned int score;
	volatile int j;
	//MFP_BUTTONS
	goto GAME_OVER;
INIT:
	time_seg = MFP_TIME;
	srand(time_seg);
	accelerate = 0;
	illegal = 0;
	conter = 0;
	score = 0;
	MFP_SEGS = score;
	init_matrix();
	
CREAT:
	val = rand() % 7;
	creat_diamonds(val);   //创建一个方块
	if(is_illegal_merge(0))
		goto GAME_OVER;
	
	while (1) {
		accelerate = 0;
		key_press = MFP_BUTTONS;
		if(key_press&0x1E) { //有按键被按下
			if (key_press & 0x10) { //向左键被按下
				j = 500000;
				while ((MFP_BUTTONS & 0x10)&&(j>0)) j--;
				diamonds[Y_Y] --;
				if (is_illegal_merge(0)) {
					diamonds[Y_Y] ++;
				}
			} else if (key_press & 0x08) { //向右键被按下
				j = 500000;
				while ((MFP_BUTTONS & 0x08)&&(j>0)) j--;
				diamonds[Y_Y] ++;
				if (is_illegal_merge(0)) {
					diamonds[Y_Y] --;
				}
			} else if (key_press & 0x04) { //旋转
				while ((MFP_BUTTONS & 0x04));
				old_rota = diamonds[ROTA];
				rotate_diamonds();
				if (is_illegal_merge(0)) {
					diamonds[ROTA] = old_rota;
				}
				
			} else if (key_press & 0x02) { //加速
				accelerate = 1;
			}
		}
		
		//下降一格
		conter ++;
		if (conter >= 10) {
			//conter = 0;
			diamonds[X_X] ++ ;
			//ok = 0
			
		}
		
		illegal = is_illegal_merge(0); 
		if (conter >= 10) {
			conter = 0;
			if (illegal) 
				diamonds[X_X] -- ;
		}
		
		//合并至显示
		is_illegal_merge(1);
		
		if (illegal) {
			//判断并消除整行
			eliminate_diamonds();
		}
		
		sync_display();
		delay(accelerate);
		
		if (illegal == 0) {
			is_illegal_merge(2);
		}
				//消除显示
		else { //无法下降，则重新创建一个 {
			goto CREAT;
		}

	}
	
GAME_OVER:
	while(1) {
		
		j = 800000;
		while (((MFP_BUTTONS & 0x01) == 0) && j>0) 
			j--;
		if (j > 0) 
			goto INIT;
		MFP_VGA1 = 0x2AA556AA;
		MFP_VGA2 = 0x155AA955;
		MFP_VGA3 = 0x2AA556AA;
		MFP_VGA4 = 0x155AA955;
		MFP_VGA5 = 0x2AA556AA;
		MFP_VGA6 = 0x155AA955;
		MFP_VGA7 = 0x2AA556AA;
		
		j = 800000;
		while (((MFP_BUTTONS & 0x01) == 0) && j>0) 
			j--;
		if (j > 0) 
			goto INIT;
		//BREAKPIONT;
		MFP_VGA1 = 0x155AA955;
		MFP_VGA2 = 0x2AA556AA;
		MFP_VGA3 = 0x155AA955;
		MFP_VGA4 = 0x2AA556AA;
		MFP_VGA5 = 0x155AA955;
		MFP_VGA6 = 0x2AA556AA;
		MFP_VGA7 = 0x155AA955;
		/*for (conter = 0; conter <10; conter ++ )
			delay(0);*/

	}
  
  return 0;
}

void init_matrix()
{
	volatile unsigned i;
	volatile unsigned j;
	for (j=0; j<20; j++) {
		for (i=0; i<10; i++) {
			a[j][i] = 0;
		}
	}
}

void eliminate_diamonds()
{
	volatile  int i;
	volatile unsigned int j;
	volatile  int k;
	
	for (i = 19; i >= 0; i--) {
		for (j = 0; j < 10; j++) {
			if (a[i][j] != 1)
				break;
		}
		//BREAKPIONT(i);
		if (j >= 10) { //该合并了
			//BREAKPIONT(0xaaa);
			for (j = 0; j < 10; j++) {
				for (k = i-1; k>=0; k--) {
					a[k+1][j] = a[k][j];
				}
				a[0][j] = 0;
			}
			score += 10;
			MFP_SEGS = decimal_to_hex_for_display_on_seg(score);
			i++; //合并之后和i--相抵消
		}
	}
	
	
}

void rotate_diamonds()
{
	switch(diamonds[KIND]) {
		case 0:
			diamonds[ROTA]++;
			if (diamonds[ROTA]>=2)
				diamonds[ROTA] = 0;
			break;
			
		case 1:
			break;
			
		case 2:
			diamonds[ROTA]++;
			if (diamonds[ROTA]>=4)
				diamonds[ROTA] = 0;
			break;
			
		case 3:
			diamonds[ROTA]++;
			if (diamonds[ROTA]>=4)
				diamonds[ROTA] = 0;
			break;
			
		case 4:
			diamonds[ROTA]++;
			if (diamonds[ROTA]>=2)
				diamonds[ROTA] = 0;
			break;
			
		case 5:
			diamonds[ROTA]++;
			if (diamonds[ROTA]>=2)
				diamonds[ROTA] = 0;
			break;
			
		case 6:
			diamonds[ROTA]++;
			if (diamonds[ROTA]>=4)
				diamonds[ROTA] = 0;
			break;
			
		default:
			break;
	}
}

/*
reuturn
1 : 不合法
0 : 合法

merge = 1 为合并功能
merge = 0 为不增加合并功能
*/
unsigned int is_illegal_merge(unsigned int merge)
{
	volatile unsigned int y = diamonds[X_X];
	volatile unsigned int x = diamonds[Y_Y];
	volatile int temp[4][2] = {0}; // 0 0  //第一行 x 最左  第二行 x最右  第三行 y 最上  第四行 y 最下
	
	switch(diamonds[KIND]) {
		case 0:
			if (diamonds[ROTA] == 0) {
				temp[0][0] = x-1; temp[0][1] = y;
				temp[1][0] = x+2; temp[1][1] = y;
				temp[2][0] = x; temp[2][1] = y;
				temp[3][0] = x+1; temp[3][1] = y;
			}else if (diamonds[ROTA] == 1) {
				temp[0][0] = x; temp[0][1] = y+1;
				temp[1][0] = x; temp[1][1] = y;
				temp[2][0] = x; temp[2][1] = y-1;
				temp[3][0] = x; temp[3][1] = y+2;
			}
			break;
			
		case 1:
			if (diamonds[ROTA] == 0) {
				temp[0][0] = x; temp[0][1] = y;
				temp[1][0] = x+1; temp[1][1] = y+1;
				temp[2][0] = x+1; temp[2][1] = y;
				temp[3][0] = x; temp[3][1] = y+1;
			}
			break;
			
		case 2:
			if (diamonds[ROTA] == 0) {
				temp[0][0] = x-1; temp[0][1] = y;
				temp[1][0] = x+1; temp[1][1] = y;
				temp[2][0] = x; temp[2][1] = y;
				temp[3][0] = x-1; temp[3][1] = y+1;
			}else if (diamonds[ROTA] == 1) {
				temp[0][0] = x-1; temp[0][1] = y-1;
				temp[1][0] = x; temp[1][1] = y;
				temp[2][0] = x; temp[2][1] = y-1;
				temp[3][0] = x; temp[3][1] = y+1;
			}else if (diamonds[ROTA] == 2) {
				temp[0][0] = x-1; temp[0][1] = y;
				temp[1][0] = x+1; temp[1][1] = y;
				temp[2][0] = x+1; temp[2][1] = y-1;
				temp[3][0] = x; temp[3][1] = y;
			}else if (diamonds[ROTA] == 3) {
				temp[0][0] = x; temp[0][1] = y;
				temp[1][0] = x+1; temp[1][1] = y+1;
				temp[2][0] = x; temp[2][1] = y-1;
				temp[3][0] = x; temp[3][1] = y+1;
			}
			break;
			
		case 3:
			if (diamonds[ROTA] == 0) {
				temp[0][0] = x-1; temp[0][1] = y;
				temp[1][0] = x+1; temp[1][1] = y;
				temp[2][0] = x; temp[2][1] = y;
				temp[3][0] = x+1; temp[3][1] = y+1;
			}else if (diamonds[ROTA] == 1) {
				temp[0][0] = x; temp[0][1] = y;
				temp[1][0] = x+1; temp[1][1] = y-1;
				temp[2][0] = x; temp[2][1] = y-1;
				temp[3][0] = x; temp[3][1] = y+1;
			}else if (diamonds[ROTA] == 2) {
				temp[0][0] = x-1; temp[0][1] = y;
				temp[1][0] = x+1; temp[1][1] = y;
				temp[2][0] = x-1; temp[2][1] = y-1;
				temp[3][0] = x; temp[3][1] = y;
			}else if (diamonds[ROTA] == 3) {
				temp[0][0] = x-1; temp[0][1] = y+1;
				temp[1][0] = x; temp[1][1] = y;
				temp[2][0] = x; temp[2][1] = y-1;
				temp[3][0] = x; temp[3][1] = y+1;
			}
			break;
			
		case 4:
			if (diamonds[ROTA] == 0) {
				temp[0][0] = x-1; temp[0][1] = y+1;
				temp[1][0] = x+1; temp[1][1] = y;
				temp[2][0] = x; temp[2][1] = y;
				temp[3][0] = x; temp[3][1] = y+1;
			}else if (diamonds[ROTA] == 1) {
				temp[0][0] = x; temp[0][1] = y;
				temp[1][0] = x+1; temp[1][1] = y;
				temp[2][0] = x; temp[2][1] = y-1;
				temp[3][0] = x+1; temp[3][1] = y+1;
			}
			break;
			
		case 5:
			if (diamonds[ROTA] == 0) {
				temp[0][0] = x-1; temp[0][1] = y;
				temp[1][0] = x+1; temp[1][1] = y+1;
				temp[2][0] = x; temp[2][1] = y;
				temp[3][0] = x; temp[3][1] = y+1;
			}else if (diamonds[ROTA] == 1) {
				temp[0][0] = x-1; temp[0][1] = y;
				temp[1][0] = x; temp[1][1] = y;
				temp[2][0] = x; temp[2][1] = y-1;
				temp[3][0] = x-1; temp[3][1] = y+1;
			}
			break;
			
		case 6:
			if (diamonds[ROTA] == 0) {
				temp[0][0] = x-1; temp[0][1] = y;
				temp[1][0] = x+1; temp[1][1] = y;
				temp[2][0] = x; temp[2][1] = y;
				temp[3][0] = x; temp[3][1] = y+1;
			}else if (diamonds[ROTA] == 1) {
				temp[0][0] = x-1; temp[0][1] = y;
				temp[1][0] = x; temp[1][1] = y;
				temp[2][0] = x; temp[2][1] = y-1;
				temp[3][0] = x; temp[3][1] = y+1;
			}else if (diamonds[ROTA] == 2) {
				temp[0][0] = x-1; temp[0][1] = y;
				temp[1][0] = x+1; temp[1][1] = y;
				temp[2][0] = x; temp[2][1] = y-1;
				temp[3][0] = x; temp[3][1] = y;
			}else if (diamonds[ROTA] == 3) {
				temp[0][0] = x; temp[0][1] = y;
				temp[1][0] = x+1; temp[1][1] = y;
				temp[2][0] = x; temp[2][1] = y-1;
				temp[3][0] = x; temp[3][1] = y+1;
			}
			break;
			
		default:
			return 1;
	}
	
	if (merge == 0 ) {
		//判断是否在内部
		if (temp[0][0] < 0 || temp[1][0] > 9 || temp[2][1] < 0 || temp[3][1] > 19) {
			//BREAKPIONT(0x852);
			return 1;
		}
			
		if ( a[temp[0][1]][temp[0][0]] == 1 ||
			 a[temp[1][1]][temp[1][0]] == 1 ||
			 a[temp[2][1]][temp[2][0]] == 1 ||
			 a[temp[3][1]][temp[3][0]] == 1 )
			 {
				//BREAKPIONT(0x963)
				return 1;
			 }
	}
	
	if (merge == 1) {
		a[temp[0][1]][temp[0][0]] = 1;
		a[temp[1][1]][temp[1][0]] = 1;
		a[temp[2][1]][temp[2][0]] = 1;
		a[temp[3][1]][temp[3][0]] = 1;
		//BREAKPIONT(0x010101);
	}
	
	if (merge == 2) {
		a[temp[0][1]][temp[0][0]] = 0;
		a[temp[1][1]][temp[1][0]] = 0;
		a[temp[2][1]][temp[2][0]] = 0;
		a[temp[3][1]][temp[3][0]] = 0;
	}
	
	return 0;
}

void creat_diamonds(int val)
{
	diamonds[X_X] = 0;
	diamonds[Y_Y] = 5;
	diamonds[ROTA] = 0;
	diamonds[KIND] = val;
}

void sync_display()
{
	volatile unsigned int display [7] = {0};
	volatile unsigned int i, j;
	volatile unsigned int remainder = 0;
	
	for (i = 0; i < 20; i++) {
		remainder = i%3;
		for (j = 0; j < 10; j++) {
			if (a[i][j] == 1) {
				display[i/3] += (1 << (10*remainder + (9-j)) );
			}
		}
	}
	
	MFP_VGA1 = display[0];
 	MFP_VGA2 = display[1];
 	MFP_VGA3 = display[2];
 	MFP_VGA4 = display[3];
 	MFP_VGA5 = display[4];
 	MFP_VGA6 = display[5];
 	MFP_VGA7 = display[6];
 	
}

//缩小到原来的十分之一，每10次才才进行一下下降。
void delay(unsigned int val) {
  volatile unsigned int j;
  if (val == 1) {
	  for (j = 0; j < (25000); j++) ;	// delay 
  } else if (val == 0)
	for (j = 0; j < (150000); j++) ;	// delay 
}

/*
将十进制转化为能投在7段数码管显示相同数组的16进制数
*/
unsigned int decimal_to_hex_for_display_on_seg(unsigned int dicimal)
{
	volatile unsigned int ret, temp_val;
	volatile unsigned int i, j, remain;
	
	for (i = 0, ret = 0; dicimal != 0; i++) {
		temp_val = 1;
		remain = dicimal%10;
		if (remain != 0) {
			if (i == 0) 
				ret += remain;
			else 
			{
				for (j = 0; j < i; j++) {
					temp_val*=16;
				}
				ret += (remain*temp_val);
			}
		}
		dicimal /=10;
	}
	return ret;
}