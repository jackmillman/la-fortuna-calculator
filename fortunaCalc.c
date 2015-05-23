#define _GNU_SOURCE
#include <string.h>
#include "os.h"
#include "audio.h"


int blink(int);
int update_dial(int);
int collect_delta(int);
int check_switches(int);

int position = 0;
int count = 0;
char options[] = "0123456789+-*/";
char string[10];

int current = 0;
int disx = 0;
int disy = 0;
 
void main(void) {
    os_init();

    os_add_task( collect_delta,   500, 1);
    os_add_task( check_switches,  100, 1);
     
    sei();		
	display.x = disx;
	display.y = disy;
	display_char(options[current]);
    for(;;){} 
}

int collect_delta(int state) {
	position += os_enc_delta();
	return state;
}

int check_switches(int state) {
	
	if (get_switch_press(_BV(SWN))) {
		int size = sizeof(options)/sizeof(options[0]);
		display.x = disx;
		display.y = disy;
		
		if(current<size-2){
			current = current + 1;
			display_char(options[current]);
		}
	}
		
	if (get_switch_press(_BV(SWE))) {
		string[count] = options[current];
		count = count + 1;
		disx = disx + 7;
		display.x = disx;
		display.y = disy;
		current = 0;
		display_char(options[current]);	
	}
		
	if (get_switch_press(_BV(SWS))) {
		display.x = disx;
		display.y = disy;

		if(current>0){
			current = current - 1;
			display_char(options[current]);
		}	
    }
		
	if (get_switch_press(_BV(SWW))) {
	}
		
	if (get_switch_press(_BV(SWC))) {
		string[count] = options[current];
		count = count + 1;
		string[count] = '\0';
		int res = 0;
		int d;
		
		for(d=0;d<count;d++) {
			if(string[d] == '+'){
				char *word = strtok(string, "+");
				char *second = strtok(NULL, "0");
				int num1 = atoi(word);
				int num2 = atoi(second);
				res = num1 + num2;
			}
			if(string[d] == '-'){
				char *word = strtok(string, "-");
				char *second = strtok(NULL, "0");
				int num1 = atoi(word);
				int num2 = atoi(second);
				res = num1 - num2;
			}	
			if(string[d] == '*'){
				char *word = strtok(string, "*");
				char *second = strtok(NULL, "0");
				int num1 = atoi(word);
				int num2 = atoi(second);
				res = num1 * num2;
			}
			if(string[d] == '/'){
				char *word = strtok(string, "/");
				char *second = strtok(NULL, "0");
				int num1 = atoi(word);
				int num2 = atoi(second);
				res = num1 / num2;
			}			
		}

		char c[10] = {0};
		itoa(res, c, 10);
		disx = 0;
		disy = disy + 10;
		display.x = disx;
		display.y = disy;
		display_string(c);
		
		count = 0;
		current = 0;
		string[0] = '\0';
		
		if(display.height-30<=disy){
			clear_screen();
			disx = 0;
			disy = 0;
		}else{
			disy = disy + 20;
		}
		
		display.x = disx;
		display.y = disy;
		display_char(options[current]);
	}
	
	return state;	
}