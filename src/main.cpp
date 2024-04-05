#include "mbed.h"
#include <UCL_Encoder.h>

// Motor and Shield Port initializations
PwmOut motor1_PWM(D5);
PwmOut motor2_PWM(D4); 

DigitalOut motor1_dir(D6);
DigitalOut motor2_dir(D7);

DigitalOut Enable_Motor_Shield(A0);

float motor1_voltage = 0.0;
float motor2_voltage = 0.0;

// Button initializations
DigitalIn up_button(D0); 
DigitalIn down_button(D1);
DigitalIn reverse_button(D3);

// LED initializations
PwmOut led_up(PE_12);
PwmOut led_down(PE_14);

int main() {

    Enable_Motor_Shield.write(1);

    // Initializing motor variables
    motor1_PWM.period_us(50);
    motor2_PWM.period_us(50);

    motor1_dir = 1;
    motor2_dir = 1;

    motor1_voltage = 0.25;
    motor2_voltage = 0.25;

    // Instantiating state variables for buttons 
    int up_button_state;
    int down_button_state;
    int reverse_button_state;

    // Instantiating brightness (percent) variables for LEDs
    float led_up_bright = 0.0;
    float led_down_bright = 0.0;

    // Change variable for LEDs
    float brightness_change = 0.0;

    // Setting LED periods
    led_up.period_us(50);
    led_down.period_us(50);
    
    // Continuous Loop
    while(true) {

        // Collecting input from the buttons
        up_button_state = up_button.read();
        down_button_state = down_button.read();
        reverse_button_state = reverse_button.read();

        // Setting motor directions with respect to the input from the reverse button
        // Also changing the brightness increment respectively
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
  
        // Activating motor 1 if the up button is pressed
        if (up_button_state == 1 && down_button_state == 0) {

            // Setting both motors
            motor1_PWM.write(motor1_voltage);
            motor2_PWM.write(0.0);

            // Changing LED brightness if within range
            if(led_up_bright <= 1 && led_up_bright >= 0) {
                led_up_bright = led_up_bright + brightness_change;
            }

            printf("Up Button pressed\n");

        // Activating motor 2 if the down button is pressed
        } else if (up_button_state == 0 && down_button_state == 1) {

            // Setting both motors
            motor2_PWM.write(motor2_voltage);
            motor1_PWM.write(0.0);

            // Changing LED brightness if within range
            if(led_down_bright <= 1 && led_down_bright >= 0) {
                led_down_bright = led_down_bright + brightness_change;
            }

            printf("Down Button pressed\n");

        // Activating both motors when both are pressed for compression
        } else if (up_button_state == 1 && down_button_state == 1) {

            // Setting both motors
            motor1_PWM.write(motor1_voltage);
            motor2_PWM.write(motor2_voltage);

            // Changing LED brightnesses if within range
            if(led_up_bright <= 1 && led_up_bright >= 0) {
                led_up_bright = led_up_bright + brightness_change;
            }
            if(led_down_bright <= 1 && led_down_bright >= 0) {
                led_down_bright = led_down_bright + brightness_change;
            }

            printf("Both buttons pressed\n");

        // Setting both motors to zero when nothing is pressed
        } else {
            motor1_PWM.write(0.0);
            motor2_PWM.write(0.0);
        }

        // Outputting the allocated brightnesses for the LEDs
        led_up.write(led_up_bright);
        led_down.write(led_down_bright);

        thread_sleep_for(100);

    }

    // Turning off the motos and LEDs
    motor1_PWM.write(0.0);
    motor2_PWM.write(0.0);

    led_up.write(0.0);
    led_down.write(0.0);

    thread_sleep_for(200);

}