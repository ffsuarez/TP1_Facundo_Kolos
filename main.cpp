//===[Libraries]===
#include "mbed.h"
#include "arm_book_lib.h"
//===[Definitions]===
#define KEYPAD_NUMBER_OF_ROWS 4
#define KEYPAD_NUMBER_OF_COLS 4
#define DEBOUNCE_BUTTON_TIME_MS 40
#define TIME_INCREMENT_MS 10
//===[Declaration of public data types]===
typedef enum{
MATRIX_KEYPAD_SCANNING,
MATRIX_KEYPAD_DEBOUNCE,
MATRIX_KEYPAD_KEY_HOLD_PRESSED
} matrixKeypadState_t;


//===[Declaration and initialization of public global objects]===
AnalogIn sensorLDR(PA_0);
DigitalIn pushButton(D2);
DigitalOut focoDesk(D3);
DigitalOut lockSignal(D4,ON);

class Lockhandle {
    private:
        bool time1;
        bool time2;
    public:
        void update(){
            time1 = pushButton.read();
            wait_ms(100);//ms
            time2 = pushButton.read();
            if(time1 == time2){
                if(time1==OFF){
                    lockSignal = ON;
                }
                else{
                    lockSignal = OFF;
                }
            }
            else{
                lockSignal = OFF;
            }            
        }
};

Serial uartUsb(USBTX,USBRX,115200);

DigitalOut keypadRowPins[KEYPAD_NUMBER_OF_ROWS] =
{PB_7, PA_15, PA_14, PA_13};
DigitalIn keypadColPins[KEYPAD_NUMBER_OF_COLS] =
{PF_7, PF_6, PC_12, PC_10};

//===[Declaration and Initialization of public global variables]===
float umbral= 0.5;
float lecturaLDR;
char keyReleased;

int accumulatedDebounceMatrixKeypadTime = 0;
char matrixKeypadLastKeyPressed = '\0';
char matrixKeypadIndexToCharArray[] = {
'1', '2', '3', 'A',
'4', '5', '6', 'B',
'7', '8', '9', 'C',
'*', '0', '#', 'D',
};
matrixKeypadState_t matrixKeypadState;
Lockhandle cerradura;
//===[Declaration and Initialization of public functions]===
void inputsInit();
void outputsInit();
void sensorLEDUpdate();
void uartTask();
void availableCommands();

void matrixKeypadInit();
char matrixKeypadScan();
char matrixKeypadUpdate();
//==[Main function]===
int main(){
    
    inputsInit();
    outputsInit();
    matrixKeypadInit();
    while(true){
        sensorLEDUpdate();
        cerradura.update();
        matrixKeypadScan();
        uartTask();
    }
    return(0); 
}
//===[Implementation of public functions]===
void inputsInit(){
    pushButton.mode(PullUp);
}

void outputsInit(){
    focoDesk = OFF;
    lockSignal = OFF;
}

void sensorLEDUpdate(){
    lecturaLDR = sensorLDR.read();
    if(lecturaLDR < umbral){
        focoDesk = OFF;
    }
    else{
        focoDesk = ON;
        //uartUsb.printf("Sensor value: %f\r\n", lecturaLDR);

    }
}

void uartTask() {
    char receivedUartChar = '\0';
    char str[100];
    if (uartUsb.readable()) {
        receivedUartChar = uartUsb.getc();

        switch (receivedUartChar) {
            case '1':
                uartUsb.printf("focoDesk ON\r\n");
                focoDesk = ON;
                wait_ms(200);
                break;
            case 's':
            case 'S':
                struct tm rtcTime;
                int strIndex;
                uartUsb.printf("Configuracion RTC(Insertar Año[YYYY])\r\n");
                for( strIndex=0; strIndex<3; strIndex++ ) {
                    str[strIndex]=uartUsb.getc();
                }
                str[4] = '\0';
                rtcTime.tm_year = atoi(str) - 1900;                 
                uartUsb.printf("Configuracion RTC(Insertar Mes[MM])\r\n");
                for( strIndex=0; strIndex<2; strIndex++ ) {
                    str[strIndex]=uartUsb.getc();
                }
                str[2] = '\0';
                rtcTime.tm_mon = atoi(str) - 1; 
                uartUsb.printf("Configuracion RTC(Insertar Dia[DD])\r\n");
                for( strIndex=0; strIndex<2; strIndex++ ) {
                    str[strIndex]=uartUsb.getc();
                }
                str[2] = '\0';
                rtcTime.tm_mday = atoi(str);
                uartUsb.printf("Configuracion RTC(Insertar Hora[hh])\r\n");
                for( strIndex=0; strIndex<2; strIndex++ ) {
                    str[strIndex]=uartUsb.getc();
                }
                str[2] = '\0';
                rtcTime.tm_hour = atoi(str);
                uartUsb.printf("Configuracion RTC(Insertar Mins[mm])\r\n");
                for( strIndex=0; strIndex<2; strIndex++ ) {
                    str[strIndex]=uartUsb.getc();
                }
                str[2] = '\0';
                rtcTime.tm_min = atoi(str);
                uartUsb.printf("Configuracion RTC(Insertar Segs[ss])\r\n");
                for( strIndex=0; strIndex<2; strIndex++ ) {
                    str[strIndex]=uartUsb.getc();
                }
                str[2] = '\0';
                rtcTime.tm_sec = atoi(str);
                rtcTime.tm_isdst = -1;
                set_time( mktime( &rtcTime ) );                                            
            default:
                availableCommands();
                break;
        }
    }
}

void availableCommands(){
    uartUsb.printf("Available commands1:\r\n");
    uartUsb.printf("Press '1' to turn ON the light\r\n");
}

void matrixKeypadInit(){
	matrixKeypadState = MATRIX_KEYPAD_SCANNING;
	int pinIndex = 0;
		for( pinIndex=0; pinIndex<KEYPAD_NUMBER_OF_COLS; pinIndex++ ) {
			(keypadColPins[pinIndex]).mode(PullUp);
		}
        printf("Inicializada matriz");
}

char matrixKeypadScan(){
	int row = 0;
	int col = 0;
	int i = 0;
	for( row=0; row<KEYPAD_NUMBER_OF_ROWS; row++ ) {
		for( i=0; i<KEYPAD_NUMBER_OF_ROWS; i++ ) {
			keypadRowPins[i] = ON;
		}
		keypadRowPins[row] = OFF;
		for( col=0; col<KEYPAD_NUMBER_OF_COLS; col++ ) {
			if( keypadColPins[col] == OFF ) {
                uartUsb.printf("%c",matrixKeypadIndexToCharArray[row*KEYPAD_NUMBER_OF_ROWS + col]);
				return matrixKeypadIndexToCharArray[row*KEYPAD_NUMBER_OF_ROWS + col];
			}
		}
	}
	return '\0';
}

char matrixKeypadUpdate(){
	char keyDetected = '\0';
	char keyReleased = '\0';
	switch( matrixKeypadState ) {
		case MATRIX_KEYPAD_SCANNING:
			keyDetected = matrixKeypadScan();
			if( keyDetected != '\0' ) {
				matrixKeypadLastKeyPressed = keyDetected;
				accumulatedDebounceMatrixKeypadTime = 0;
				matrixKeypadState = MATRIX_KEYPAD_DEBOUNCE;
			}
			break;
		case MATRIX_KEYPAD_DEBOUNCE:
			if( accumulatedDebounceMatrixKeypadTime >=
			DEBOUNCE_BUTTON_TIME_MS ) {
				keyDetected = matrixKeypadScan();
				if( keyDetected == matrixKeypadLastKeyPressed ) {
				matrixKeypadState = MATRIX_KEYPAD_KEY_HOLD_PRESSED;
				} else {
				matrixKeypadState = MATRIX_KEYPAD_SCANNING;
				}
			}
			accumulatedDebounceMatrixKeypadTime =
			accumulatedDebounceMatrixKeypadTime + TIME_INCREMENT_MS;
			break;
		case MATRIX_KEYPAD_KEY_HOLD_PRESSED:
			keyDetected = matrixKeypadScan();
			if( keyDetected != matrixKeypadLastKeyPressed ) {
				if( keyDetected == '\0' ) {
					keyReleased = matrixKeypadLastKeyPressed;
				}
				matrixKeypadState = MATRIX_KEYPAD_SCANNING;
			}
			break;
		default:
			matrixKeypadInit();
			break;
	}
	return keyReleased;
}