/* ========================================
 * Fully working code: 
 * PWM      : 
 * Encoder  : 
 * ADC      :
 * USB      : port displays speed and position.
 * CMD: "PW xx"
 * Copyright Univ of Auckland, 2016
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Univ of Auckland.
 *
 * ========================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <project.h>
//* ========================================
#include "defines.h"
#include "vars.h"
//* ========================================
 void usbPutString(char *s);
 void usbPutChar(char c);
 void handle_usb();
 void turnRight();
 void turnLeft();
 void stop();
 void readADC();
 void readADC0();
 void readADC1();
 void readADC2();
 void readADC3();
 void readADC4();
 void readADC5();
 void readADC6();
 void receiveRX();
 void initRamp();
void techTest2RF();
void techTest2RF1();
void techTest2RF2();
void techTest2RF3();
void techTest2RF4();
void simpleCarControl();
void techTest2Sensor();

 int get_average_position();
 void stopAfterX(int quadAverage, int quadGoal);
 int getSpeed();
 void incrementQuadGoal();
 void setSpeed(int spd, int goalSpeed);

//* ========================================

 uint8 PWMPower = 158;

 uint8 turnConstant = 40;

 int speedCounter = 0;

 int prevQuadReading = 0;
 int spd = 0;


//------------------- INIT ---------------------------
 int goalSpeed = 15;

//RF--------------------
int RFStopValue = 648; //(1024/2.58)*distance in m, * by 1.02
int speedFlag = 0;

//Sensor----------------
//int distance = 1000; //change this constant, in mm
int goalQuad = 615; //(57*4)*(distance/194.68); //factor of 1.03 distance in mm when speed is 15, 1.06 when speed is 10

//------------------- INIT ---------------------------

//rf variables
 volatile uint8 rxdata = 0;
 volatile uint16 counter = 0;
 volatile uint8 datastring[99] = {};

int initcounter = 127;
int init = 1;

 char str[100];
 int asciiValue = 0;
 int packetCntr = 0;
 char locationArray[10] = {0};
 int locationArrayCntr = 0;
 int locationTemp = 0;
 int negate = 0;
 vtype1 robotLocation;
 int i;

int firstCounter = 0;
int firstx = 0;
int firsty = 0;
int firstAngle = 0;
int firstinit = 0;
int newx = 0;
int newy = 0;
int newAngle = 0;


 uint16 ADC_result0, ADC_result1, ADC_result2, ADC_result3, ADC_result4,
 ADC_result5, ADC_result6;
 uint16 result0, result1, result2, result3, result4, result5, result6;
 uint16 counter0, counter1, counter2, counter3, counter4, counter5, counter6 = 0;

 typedef enum {
    false, true}bool;
    bool sensorLogic0, sensorLogic1, sensorLogic2, sensorLogic3, sensorLogic4,
    sensorLogic5, sensorLogic6 = false;


    int main() {
// --------------------------------    
// ----- INITIALIZATIONS ----------

        CYGlobalIntEnable;
        
PWM_1_Start();
PWM_2_Start();
M1_EN_Write(1);
M2_EN_Write(1);
M1_D2_Write(1);
M2_D2_Write(1);

PWM_1_WriteCompare(PWMPower);
PWM_2_WriteCompare(PWMPower);


ADC_Start();
ADC_StartConvert();
isr_eoc_Start();

QuadDec_M1_Start();
QuadDec_M2_Start();
QuadDec_M1_SetCounter(0);
QuadDec_M2_SetCounter(0);

Timer_TS_Start();
isr_TS_Start();

// ------USB SETUP ----------------    
#ifdef USE_USB    
USBUART_Start(0,USBUART_5V_OPERATION);
#endif     
RF_BT_SELECT_Write(0);
isrRF_RX_Start();
UART_Start();

usbPutString (displaystring);

for (;;) {
    
    //techTest2Sensor();
    
    techTest2RF();
    
    
    


    
    
    
    

    /* Place your application code here. */
    handle_usb();
    if (flag_KB_string == 1) {
        usbPutString (line);
        flag_KB_string = 0;
    }

}
}

void techTest2Sensor(){
    stopAfterX(get_average_position(),goalQuad);
    setSpeed(getSpeed(),goalSpeed);
	readADC();
    simpleCarControl(); 
}
  


void techTest2RF(){
    
    setSpeed(getSpeed(), goalSpeed);
    receiveRX();
        
    if (firstinit == 1){
        
        
        if ((firstAngle >= 0) && (firstAngle < 900)){
            LED0_Write(1);
             LED1_Write(0);
             LED2_Write(0);
             LED3_Write(0);
            techTest2RF1();
        } else if ((firstAngle >= 900) && (firstAngle < 1800)){
            LED0_Write(0);
             LED1_Write(1);
             LED2_Write(0);
             LED3_Write(0);
             techTest2RF2();
        } else if ((firstAngle >= 1800) && (firstAngle < 2700)){
            LED0_Write(0);
             LED1_Write(0);
             LED2_Write(1);
             LED3_Write(0);
             techTest2RF3();
        } else if ((firstAngle >= 2700) && (firstAngle < 3600)){
            LED0_Write(0);
             LED1_Write(0);
             LED2_Write(0);
             LED3_Write(1);
             techTest2RF4();
        }
    }  
}
//void techTest2RF(){
//    
//        receiveRX();
//        
//        setSpeed(getSpeed(), goalSpeed);
//        
//        if (firstinit == 1){
//            newx = robotLocation.robot_xpos - firstx;
//            newy = robotLocation.robot_ypos - firsty;
//
//            if (newx > RFStopValue){
//                stop();
//                while(1);   
//            }
//            
//            //car gone right so curve left
//            if (newy > 2) {
//                if (((1800-robotLocation.robot_orientation)/10) < 10){
//                    PWM_1_WriteCompare(PWMPower+1+((1800-robotLocation.robot_orientation)/10));
//                    PWM_2_WriteCompare(PWMPower);
//                } else {
//                    PWM_1_WriteCompare(PWMPower+1+10);
//                    PWM_2_WriteCompare(PWMPower);
//                }
//            
//                while (newy < 0){
//                    receiveRX();
//                    newx = robotLocation.robot_xpos - firstx;
//                    newy = robotLocation.robot_ypos - firsty;
//                    
//                    if (newx > RFStopValue){
//                        stop();
//                        while(1);   
//                    }
//                }
//            //car gone left so curve right
//            } else if (newy < -2){
//                
//                if (((robotLocation.robot_orientation-1800)/10) < 10){
//                    PWM_1_WriteCompare(PWMPower);
//                    PWM_2_WriteCompare(PWMPower+1+((robotLocation.robot_orientation-1800)/10));
//                } else {
//                    PWM_1_WriteCompare(PWMPower);
//                    PWM_2_WriteCompare(PWMPower+1+10); 
//                }
//                while (newy > 0){
//                    receiveRX();
//                    newx = robotLocation.robot_xpos - firstx;
//                    newy = robotLocation.robot_ypos - firsty;
//                    
//                    if (newx > RFStopValue){
//                        stop();
//                        while(1);   
//                    }
//                }
//                        
//            } else {
//                PWM_1_WriteCompare(PWMPower);
//                PWM_2_WriteCompare(PWMPower);
//            }
//        } 
//}

void techTest2RF1(){
    
        //receiveRX();
        
        //setSpeed(getSpeed(), goalSpeed);
        
        if (firstinit == 1){
            newx = robotLocation.robot_xpos - firstx;
            newy = robotLocation.robot_ypos - firsty;

            if (newx > RFStopValue){
                stop();
                while(1);   
            }
            
            //car gone right so curve left
            if (newy > 2) {
                if (((1800-robotLocation.robot_orientation)/10) < 10){
                    PWM_1_WriteCompare(PWMPower+1+((1800-robotLocation.robot_orientation)/10));
                    PWM_2_WriteCompare(PWMPower);
                } else {
                    PWM_1_WriteCompare(PWMPower+1+10);
                    PWM_2_WriteCompare(PWMPower);
                }
            
                while (newy < 0){
                    receiveRX();
                    newx = robotLocation.robot_xpos - firstx;
                    newy = robotLocation.robot_ypos - firsty;
                    
                    if (newx > RFStopValue){
                        stop();
                        while(1);   
                    }
                }
            //car gone left so curve right
            } else if (newy < -2){
                
                if (((robotLocation.robot_orientation-1800)/10) < 10){
                    PWM_1_WriteCompare(PWMPower);
                    PWM_2_WriteCompare(PWMPower+1+((robotLocation.robot_orientation-1800)/10));
                } else {
                    PWM_1_WriteCompare(PWMPower);
                    PWM_2_WriteCompare(PWMPower+1+10); 
                }
                while (newy > 0){
                    receiveRX();
                    newx = robotLocation.robot_xpos - firstx;
                    newy = robotLocation.robot_ypos - firsty;
                    
                    if (newx > RFStopValue){
                        stop();
                        while(1);   
                    }
                }
                        
            } else {
                PWM_1_WriteCompare(PWMPower);
                PWM_2_WriteCompare(PWMPower);
            }
        } 
}

void techTest2RF2(){
    
        //receiveRX();
        
        //setSpeed(getSpeed(), goalSpeed);
        
        if (firstinit == 1){
            newx = firstx - robotLocation.robot_xpos;
            newy = firsty - robotLocation.robot_ypos;

            if (newy > RFStopValue){
                stop();
                while(1);   
            }
            
            //car gone right so curve left
            if (newx < -2) {
                if (((2700-robotLocation.robot_orientation)/10) < 10){
                    PWM_1_WriteCompare(PWMPower+1+((2700-robotLocation.robot_orientation)/10));
                    PWM_2_WriteCompare(PWMPower);
                } else {
                    PWM_1_WriteCompare(PWMPower+1+10);
                    PWM_2_WriteCompare(PWMPower);
                }
            
                while (newx > 0){
                    receiveRX();
                    newx = firstx - robotLocation.robot_xpos;
                    newy = firsty - robotLocation.robot_ypos;
                    
                    if (newy > RFStopValue){
                        stop();
                        while(1);   
                    }
                }
            //car gone left so curve right
            } else if (newx > 2){
                if (((robotLocation.robot_orientation-2700)/10) < 10){
                    PWM_1_WriteCompare(PWMPower);
                    PWM_2_WriteCompare(PWMPower+1+((robotLocation.robot_orientation-2700)/10));
                } else {
                    PWM_1_WriteCompare(PWMPower);
                    PWM_2_WriteCompare(PWMPower+1+10); 
                }
                while (newx < 0){
                    receiveRX();
                    newx = firstx - robotLocation.robot_xpos;
                    newy = firsty - robotLocation.robot_ypos;
                    
                    if (newy > RFStopValue){
                        stop();
                        while(1);   
                    }
                }
                        
            } else {
                PWM_1_WriteCompare(PWMPower);
                PWM_2_WriteCompare(PWMPower);
            }
        } 
}

void techTest2RF3(){
    
        //receiveRX();
        
        //setSpeed(getSpeed(), goalSpeed);
        
        if (firstinit == 1){
            newx = firstx - robotLocation.robot_xpos;
            newy = firsty - robotLocation.robot_ypos;

            newAngle = robotLocation.robot_orientation;
            
            if (newAngle < 1800){
                newAngle = newAngle + 1800;
            } else if (newAngle > 1800){
                newAngle = newAngle - 1800;
            }
                
            
            if (newx > RFStopValue){
                stop();
                while(1);   
            }
            
            //car gone right so curve left
            if (newy > 2) {
                if (((1800-newAngle)/10) < 10){
                    PWM_1_WriteCompare(PWMPower+1+((1800-newAngle)/10));
                    PWM_2_WriteCompare(PWMPower);
                } else {
                    PWM_1_WriteCompare(PWMPower+1+10);
                    PWM_2_WriteCompare(PWMPower);
                }
            
                while (newy < 0){
                    receiveRX();
                    newx = firstx - robotLocation.robot_xpos;
                    newy = firsty - robotLocation.robot_ypos;
                    
                    if (newx > RFStopValue){
                        stop();
                        while(1);   
                    }
                }
            //car gone left so curve right
            } else if (newy < -2){
                
                if (((newAngle-1800)/10) < 10){
                    PWM_1_WriteCompare(PWMPower);
                    PWM_2_WriteCompare(PWMPower+1+((newAngle-1800)/10));
                } else {
                    PWM_1_WriteCompare(PWMPower);
                    PWM_2_WriteCompare(PWMPower+1+10); 
                }
                while (newy > 0){
                    receiveRX();
                    newx = firstx - robotLocation.robot_xpos;
                    newy = firsty - robotLocation.robot_ypos;
                    
                    if (newx > RFStopValue){
                        stop();
                        while(1);   
                    }
                }
                        
            } else {
                PWM_1_WriteCompare(PWMPower);
                PWM_2_WriteCompare(PWMPower);
            }
        } 
}

void techTest2RF4(){
    
        //receiveRX();
        
        //setSpeed(getSpeed(), goalSpeed);
        
        if (firstinit == 1){
            newx = robotLocation.robot_xpos - firstx;
            newy = robotLocation.robot_ypos - firsty;

            if (newy > RFStopValue){
                stop();
                while(1);   
            }
            
            //car gone right so curve left
            if (newx < -2) {
                if (((900-robotLocation.robot_orientation)/10) < 10){
                    PWM_1_WriteCompare(PWMPower+1+((900-robotLocation.robot_orientation)/10));
                    PWM_2_WriteCompare(PWMPower);
                } else {
                    PWM_1_WriteCompare(PWMPower+1+10);
                    PWM_2_WriteCompare(PWMPower);
                }
            
                while (newx > 0){
                    receiveRX();
                    newx = robotLocation.robot_xpos - firstx;
                    newy = robotLocation.robot_ypos - firsty;
                    
                    if (newy > RFStopValue){
                        stop();
                        while(1);   
                    }
                }
            //car gone left so curve right
            } else if (newx > 2){
                if (((robotLocation.robot_orientation-900)/10) < 10){
                    PWM_1_WriteCompare(PWMPower);
                    PWM_2_WriteCompare(PWMPower+1+((robotLocation.robot_orientation-900)/10));
                } else {
                    PWM_1_WriteCompare(PWMPower);
                    PWM_2_WriteCompare(PWMPower+1+10); 
                }
                while (newx < 0){
                    receiveRX();
                    newx = robotLocation.robot_xpos - firstx;
                    newy = robotLocation.robot_ypos - firsty;
                    
                    if (newy > RFStopValue){
                        stop();
                        while(1);   
                    }
                }
                        
            } else {
                PWM_1_WriteCompare(PWMPower);
                PWM_2_WriteCompare(PWMPower);
            }
        } 
}

void initRamp() {
    //init process
    if (init == 1){
        stop();
        while(initcounter != PWMPower){
            PWM_1_WriteCompare(initcounter);
            PWM_2_WriteCompare(initcounter);
            CyDelay(100);
            initcounter++;
        }
        init = 0;
    }
} 

void simpleCarControl(){

	    //slight right
	if (sensorLogic4 == false && sensorLogic2 == false
			&& sensorLogic0 == false && sensorLogic1 == true && sensorLogic3 == true) {

		PWM_1_WriteCompare(PWMPower-10);
		PWM_2_WriteCompare(PWMPower);

        incrementQuadGoal(goalQuad);
        
		while (sensorLogic4 == false) {
			readADC4();
            stopAfterX(get_average_position(),goalQuad);
            readADC0();
            readADC2();
            if (sensorLogic0 == true || sensorLogic2 == true){
                break;
            }
		}

		//slight left
	} else if (sensorLogic3 == false && sensorLogic2 == false
			&& sensorLogic0 == false && sensorLogic1 == true && sensorLogic4 == true) {

		PWM_1_WriteCompare(PWMPower);
		PWM_2_WriteCompare(PWMPower-10);

        incrementQuadGoal(goalQuad);

		while (sensorLogic3 == false) {
			readADC3();
            stopAfterX(get_average_position(),goalQuad);
            readADC0();
            readADC2();
            if (sensorLogic0 == true || sensorLogic2 == true){
                break;
            }
		}
    //sharp right turn
	} else if (sensorLogic3 == false && sensorLogic4 == false
			&& sensorLogic5 == false && sensorLogic0 == true && sensorLogic1 == true) {

		//stop();
		turnRight();

		while (sensorLogic5 == false) {
			readADC5();
		}
    //sharp left turn
	} else if (sensorLogic3 == false && sensorLogic4 == false
			&& sensorLogic5 == false && sensorLogic2 == true && sensorLogic1 == true) {
                
		//stop();
		turnLeft();

		while (sensorLogic5 == false) {
			readADC5();
		}
	} else {
		PWM_1_WriteCompare(PWMPower);
		PWM_2_WriteCompare(PWMPower);
	}   
    
}

void readADC() {
///////////////////////////////////////////////////////////////////////////////////////////
//sensorLogicX true (1) = on the line
//sensorLogicX false (0) = NOT on the line

    if (dataready_flag == 1) {

    //---------ADC0----------------------
        result0 = ADC_CountsTo_mVolts(adcvalue[0]);

        if (result0 > 1200) {
            counter0 = 0;
            sensorLogic0 = false;
        } else {
            if (counter0 > 100) {
                sensorLogic0 = true;
            } else {
                counter0 = counter0 + 1;
            }
        }

        if (sensorLogic0 == false) {
            LED0_Write(0);
        } else if (sensorLogic0 == true) {
            LED0_Write(1);
        } else {
        }

    //---------ADC1----------------------
        result1 = ADC_CountsTo_mVolts(adcvalue[1]);

        if (result1 > 1200) {
            counter1 = 0;
            sensorLogic1 = false;
        } else {
            if (counter1 > 100) {
                sensorLogic1 = true;
            } else {
                counter1 = counter1 + 1;
            }
        }

        if (sensorLogic1 == false) {
            LED1_Write(0);
        } else if (sensorLogic1 == true) {
            LED1_Write(1);
        } else {
        }

    //---------ADC2----------------------
        result2 = ADC_CountsTo_mVolts(adcvalue[2]);

        if (result2 > 1200) {
            counter2 = 0;
            sensorLogic2 = false;
        } else {
            if (counter2 > 100) {
                sensorLogic2 = true;
            } else {
                counter2 = counter2 + 1;
            }
        }

        if (sensorLogic2 == false) {
            LED2_Write(0);
        } else if (sensorLogic2 == true) {
            LED2_Write(1);
        } else {
        }

    //---------ADC3----------------------
        result3 = ADC_CountsTo_mVolts(adcvalue[3]);

        if (result3 > 1200) {
            counter3 = 0;
            sensorLogic3 = false;
        } else {
            if (counter3 > 100) {
                sensorLogic3 = true;
            } else {
                counter3 = counter3 + 1;
            }
        }

        if (sensorLogic3 == false) {
            LED3_Write(0);
        } else if (sensorLogic3 == true) {
            LED3_Write(1);
        } else {
        }

    //---------ADC4----------------------
        result4 = ADC_CountsTo_mVolts(adcvalue[4]);

        if (result4 > 1200) {
            counter4 = 0;
            sensorLogic4 = false;
        } else {
            if (counter4 > 100) {
                sensorLogic4 = true;
            } else {
                counter4 = counter4 + 1;
            }
        }

        if (sensorLogic4 == false) {
            LED4_Write(0);
        } else if (sensorLogic4 == true) {
            LED4_Write(1);
        } else {
        }

    //---------ADC5----------------------
        result5 = ADC_CountsTo_mVolts(adcvalue[5]);

        if (result5 > 1200) {
            counter5 = 0;
            sensorLogic5 = false;
        } else {
            if (counter5 > 100) {
                sensorLogic5 = true;
            } else {
                counter5 = counter5 + 1;
            }
        }

        if (sensorLogic5 == false) {
            LED5_Write(0);
        } else if (sensorLogic5 == true) {
            LED5_Write(1);
        } else {
        }
    }
    dataready_flag = 0;
}

void readADC0() {
    if (dataready_flag == 1) {
        result0 = ADC_CountsTo_mVolts(adcvalue[0]);

        if (result0 > 1200) {
            counter0 = 0;
            sensorLogic0 = false;
        } else {
            if (counter0 > 100) {
                sensorLogic0 = true;
            } else {
                counter0 = counter0 + 1;
            }
        }

        dataready_flag = 0;
    }
}

void readADC1() {
    if (dataready_flag == 1) {
        result1 = ADC_CountsTo_mVolts(adcvalue[1]);

        if (result1 > 1200) {
            counter1 = 0;
            sensorLogic1 = false;
        } else {
            if (counter1 > 100) {
                sensorLogic1 = true;
            } else {
                counter1 = counter1 + 1;
            }
        }

        dataready_flag = 0;
    }
}

void readADC2() {
    if (dataready_flag == 1) {
        result2 = ADC_CountsTo_mVolts(adcvalue[2]);

        if (result2 > 1200) {
            counter2 = 0;
            sensorLogic2 = false;
        } else {
            if (counter2 > 100) {
                sensorLogic2 = true;
            } else {
                counter2 = counter2 + 1;
            }
        }

        dataready_flag = 0;
    }
}

void readADC3() {
    if (dataready_flag == 1) {
        result3 = ADC_CountsTo_mVolts(adcvalue[3]);

        if (result3 > 1200) {
            counter3 = 0;
            sensorLogic3 = false;
        } else {
            if (counter3 > 100) {
                sensorLogic3 = true;
            } else {
                counter3 = counter3 + 1;
            }
        }

        dataready_flag = 0;
    }
}

void readADC4() {
    if (dataready_flag == 1) {
        result4 = ADC_CountsTo_mVolts(adcvalue[4]);

        if (result4 > 1200) {
            counter4 = 0;
            sensorLogic4 = false;
        } else {
            if (counter4 > 100) {
                sensorLogic4 = true;
            } else {
                counter4 = counter4 + 1;
            }
        }

        dataready_flag = 0;
    }
}

void readADC5() {
    if (dataready_flag == 1) {
        result5 = ADC_CountsTo_mVolts(adcvalue[5]);

        if (result5 > 1200) {
            counter5 = 0;
            sensorLogic5 = false;
        } else {
            if (counter5 > 100) {
                sensorLogic5 = true;
            } else {
                counter5 = counter5 + 1;
            }
        }

        dataready_flag = 0;
    }
}

void readADC6() {
    if (dataready_flag == 1) {
        result6 = ADC_CountsTo_mVolts(adcvalue[6]);

        if (result6 > 1200) {
            counter6 = 0;
            sensorLogic6 = false;
        } else {
            if (counter6 > 100) {
                sensorLogic6 = true;
            } else {
                counter6 = counter6 + 1;
            }
        }

        dataready_flag = 0;
    }
}

void turnLeft() {
    PWM_1_WriteCompare(127 + turnConstant);
    PWM_2_WriteCompare(127 - turnConstant);
}

void turnRight() {
    PWM_1_WriteCompare(127 - turnConstant);
    PWM_2_WriteCompare(127 + turnConstant);
}

void stop() {
    PWM_1_WriteCompare(127);
    PWM_2_WriteCompare(127);
}

int get_average_position() {    
    int Quad1Reading = -QuadDec_M1_GetCounter();
    int Quad2Reading = -QuadDec_M2_GetCounter();
    int QuadAverage = (Quad1Reading + Quad2Reading)/2;
    
    //sprintf(str,"QUAD AVERAGE : %d\n",QuadAverage);
    //USBUART_PutString(str);
    
    return QuadAverage;
}

int getSpeed(){
   if (timerOverflow_flag == 1){
    
    int quadAverage = get_average_position();
        int quadDifference = quadAverage - prevQuadReading; // delta s
        int speed = (19.468*quadDifference*10)/(57*4); //measurement in cm/s for 100ms
        
        /*
        sprintf(str,"Speed : %d\n",speed);
        USBUART_PutString(str);
        */
        
        prevQuadReading = quadAverage;
        timerOverflow_flag = 0;
        
        speedFlag = 1;
        
        return speed;
    } 
    
    return 0;
}

void setSpeed(int spd, int goalSpeed){
    if (speedFlag == 1){
        if (spd < goalSpeed){
            PWMPower++;
            PWM_1_WriteCompare(PWMPower);
            PWM_2_WriteCompare(PWMPower);
        } else if (spd > goalSpeed){
            PWMPower--;
            PWM_1_WriteCompare(PWMPower);
            PWM_2_WriteCompare(PWMPower);
        }
        
        speedFlag = 0;
    }   
}

void incrementQuadGoal(int goalQuad){
    goalQuad = goalQuad + 2; 
}

void stopAfterX(int quadAverage, int goalQuad){
    if (quadAverage > goalQuad || quadAverage < -goalQuad){
        stop();
        while(1);
    } 
}

void receiveRX(){
    if (flag_rx == 1){
        asciiValue = UART_GetChar();
        USBUART_PutChar(asciiValue);

        if (asciiValue == '#'){
            packetCntr = 0;
        } else if (asciiValue == ','){

            switch (packetCntr) {
                case 0 :                    
                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.rssi = atoi(locationArray);
                if(negate == 1){
                    negate = 0;
                    robotLocation.rssi = -1 * robotLocation.rssi;
                }

                memset(locationArray, 0, 10);
                break;
                case 1 :
                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.index = atoi(locationArray);
                memset(locationArray, 0, 10);

                break;
                case 2 : 
                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.robot_xpos = atoi(locationArray);
                memset(locationArray, 0, 10);


                break;
                case 3 : 
                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.robot_ypos = atoi(locationArray);
                memset(locationArray, 0, 10);
                break;
                case 4 : 

                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.robot_orientation = atoi(locationArray);
                memset(locationArray, 0, 10);
                break;
                case 5 : 
                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.g0_xpos = atoi(locationArray);
                memset(locationArray, 0, 10);
                break;
                case 6 : 
                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.g0_ypos = atoi(locationArray);
                memset(locationArray, 0, 10);
                break;
                case 7 : 
                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.g0_speed = atoi(locationArray);
                memset(locationArray, 0, 10);
                break;
                case 8 : 
                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.g0_direction = atoi(locationArray);
                memset(locationArray, 0, 10);
                break;
                case 9 : 
                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.g1_xpos = atoi(locationArray);
                memset(locationArray, 0, 10);
                break;
                case 10 : 
                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.g1_ypos = atoi(locationArray);
                memset(locationArray, 0, 10);
                break;
                case 11 : 
                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.g1_speed = atoi(locationArray);
                memset(locationArray, 0, 10);
                break;
                case 12 : 
                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.g1_direction = atoi(locationArray);
                memset(locationArray, 0, 10);
                break;
                case 13 : 
                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.g2_xpos = atoi(locationArray);
                memset(locationArray, 0, 10);
                break;
                case 14 : 
                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.g2_ypos = atoi(locationArray);
                memset(locationArray, 0, 10);
                break;
                case 15 : 
                for(i = 10; i>locationArrayCntr-1; i--){
                    locationArray[i] = 0;
                }
                locationArrayCntr = 0;
                robotLocation.g2_speed = atoi(locationArray);
                memset(locationArray, 0, 10);
                break;

            }

            packetCntr = packetCntr + 1;


        } else {

            if (asciiValue == '[' || asciiValue == ']'){
                    //locationArrayCntr = 0;
            } else {
                if(asciiValue == 45){
                    negate = 1;
                }else if(asciiValue == 10){
                }
                else{

                    locationArray[locationArrayCntr] = asciiValue;
                    locationArrayCntr = locationArrayCntr + 1;
                }

            }
            if(packetCntr == 16){

                robotLocation.g2_direction = asciiValue-48;
                locationArrayCntr = 0;
                
                
                //////////////////////////////////////////////////////
                
                if (firstinit == 0){
                    if (firstCounter < 10){
                        firstx = firstx + robotLocation.robot_xpos;
                        firsty = firsty + robotLocation.robot_ypos;
                        
                        if ((robotLocation.robot_orientation - 1350)> 0){
                            firstAngle = firstAngle + (robotLocation.robot_orientation - 1350);    
                        } else if ((robotLocation.robot_orientation - 1350) <= 0){
                            firstAngle = firstAngle + (3600 + (robotLocation.robot_orientation - 1350));    
                        }
                        
                        stop();
                        
                        firstCounter++;
                    } else if (firstCounter == 10){
                        firstx = firstx / 10;
                        firsty = firsty / 10;
                        firstAngle = firstAngle / 10;
                        PWM_1_WriteCompare(PWMPower);
                        PWM_2_WriteCompare(PWMPower);
                        firstinit = 1;
                        
                        LED0_Write(1);
                    }
                }
                
                /////////////////////////////////////////////////////////
                
                /*
                sprintf(str,"[%d,%d,%d],[%d,%d,%d,%d],[%d,%d,%d,%d],[%d,%d,%d,%d]\n",robotLocation.robot_xpos,robotLocation.robot_ypos,robotLocation.robot_orientation,robotLocation.g0_xpos,robotLocation.g0_ypos,robotLocation.g0_speed,robotLocation.g0_direction,robotLocation.g1_xpos,robotLocation.g1_ypos,robotLocation.g1_speed,robotLocation.g1_direction,robotLocation.g2_xpos,robotLocation.g1_ypos,robotLocation.g2_speed,robotLocation.g2_direction);
                USBUART_PutString(str);
                */
                memset(locationArray, 0, 10);
                packetCntr++;
            }

        }
        UART_ReadControlRegister();   
        flag_rx = 0;
    }

}

//* ========================================
void usbPutString(char *s) {
// !! Assumes that *s is a string with allocated space >=64 chars     
//  Since USB implementation retricts data packets to 64 chars, this function truncates the
//  length to 62 char (63rd char is a '!')

#ifdef USE_USB     
    while (USBUART_CDCIsReady() == 0);
    s[63]='\0';
    s[62]='!';
    USBUART_PutData((uint8*)s,strlen(s));
#endif
}
//* ========================================
void usbPutChar(char c) {
#ifdef USE_USB     
    while (USBUART_CDCIsReady() == 0);
    USBUART_PutChar(c);
#endif    
}
//* ========================================
void handle_usb() {
// handles input at terminal, echos it back to the terminal
// turn echo OFF, key emulation: only CR
// entered string is made available in 'line' and 'flag_KB_string' is set

    static uint8 usbStarted = FALSE;
    static uint16 usbBufCount = 0;
    uint8 c;

    if (!usbStarted) {
        if (USBUART_GetConfiguration()) {
            USBUART_CDC_Init();
            usbStarted = TRUE;
        }
    } else {
        if (USBUART_DataIsReady() != 0) {
            c = USBUART_GetChar();

            if ((c == 13) || (c == 10)) {
//                if (usbBufCount > 0)
                {
                    entry[usbBufCount] = '\0';
                    strcpy(line, entry);
                    usbBufCount = 0;
                    flag_KB_string = 1;
                }
            } else {
                if (((c == CHAR_BACKSP) || (c == CHAR_DEL)) && (usbBufCount > 0))
                    usbBufCount--;
                else {
                if (usbBufCount > (BUF_SIZE - 2)) // one less else strtok triggers a crash
                {
                    USBUART_PutChar('!');
                } else
                entry[usbBufCount++] = c;
            }
        }
    }
}
}

/* [] END OF FILE */
