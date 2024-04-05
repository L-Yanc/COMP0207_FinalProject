#include "mbed.h"
#include <UCL_Encoder.h>

PwmOut motor1_PWM(D5);
PwmOut motor2_PWM(D4); 

DigitalOut motor1_dir(D6);
DigitalOut motor2_dir(D7);

DigitalOut Enable_Motor_Shield(A0);

float motor1_voltage = 0.0;
float motor2_voltage = 0.0;

DigitalIn up_button(D1); 
DigitalIn down_button(D0);
DigitalIn reverse_button(D3);

PwmOut led_up(PE_12);
PwmOut led_down(PE_14);

int main() {

    Enable_Motor_Shield.write(1);

    // Initializing motor variables
    motor1_PWM.period_us(50);
    motor2_PWM.period_us(50);

    led_up.period_us(50);
    led_down.period_us(50);

    motor1_dir = 1;
    motor2_dir = 1;

    motor1_voltage = 0.5;
    motor2_voltage = 0.5;

    // Instantiating state variables for buttons 
    int up_button_state;
    int down_button_state;
    int reverse_button_state;

    // Instantiating brightness (percent) variables for LEDs
    float led_up_bright = 0.0;
    float led_down_bright = 0.0;

    // Change variable for LEDs
    float brightness_change = 0.0;
    
    while(true) {

        up_button_state = up_button.read();
        down_button_state = down_button.read();
        reverse_button_state = reverse_button.read();

        if(reverse_button_state == 1) {

            motor1_dir = 0;
            motor2_dir = 0;

            brightness_change = -0.01;
            printf("Button reversed \n");

        } else {

            motor1_dir = 1;
            motor2_dir = 1;

            brightness_change = 0.01;
        }

        if (up_button_state == 1 && down_button_state == 0) {

            motor1_PWM.write(motor1_voltage);
            motor2_PWM.write(0.0);
            if(led_up_bright <= 1 && led_up_bright >= 0) {
                led_up_bright = led_up_bright + brightness_change;
            }
            printf("     Button pressed 1\n");

        } else if (down_button_state == 1 && up_button_state == 0) {

            motor2_PWM.write(motor2_voltage);
            motor1_PWM.write(0.0);
            if(led_down_bright <= 1 && led_down_bright >= 0) {
                led_down_bright = led_down_bright + brightness_change;
            }
            printf("        Button pressed 2\n");

        } else if (down_button_state == 1 && up_button_state == 1) {

            motor1_PWM.write(motor1_voltage);
            motor2_PWM.write(motor2_voltage);

            if(led_up_bright <= 1 && led_up_bright >= 0) {
                led_up_bright = led_up_bright + brightness_change;
            }
            if(led_down_bright <= 1 && led_down_bright >= 0) {
                led_down_bright = led_down_bright + brightness_change;
            }

            printf("Both buttons pressed\n");

        } else {

            motor1_PWM.write(0.0);
            motor2_PWM.write(0.0);

        }

        led_up.write(led_up_bright);
        led_down.write(led_down_bright);

        thread_sleep_for(100);

    }

    motor1_PWM.write(0.0);
    motor2_PWM.write(0.0);

    thread_sleep_for(200);

}