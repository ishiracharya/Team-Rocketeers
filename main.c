#include <kipr/wombat.h>

int LEFT_MOTOR_PORT = 0;
int RIGHT_MOTOR_PORT = 1;

int LEFT_TOPHAT_PORT = 1;
int RIGHT_TOPHAT_PORT = 0;
int BACK_TOPHAT_PORT = 2;

int BLACK_VALUE = 3800;
int WHITE_VALUE = 300;
int SMALL_BLACK_VALUE = 4000;
int SMALL_WHITE_VALUE = 3200;

int CLAW_PORT = 2;
int CLAW_OPEN = 0;
int CLAW_ATTACK = 12529;
int CLAW_CLOSED = 16000;

float LINE_FOLLOW_SENSITIVITY_LOW = 0.2;
float LINE_FOLLOW_SENSITIVITY_HIGH = 0.5;

float TICKS_PER_DEGREE = 13.1;
float TICKS_PER_INCH = 222.2;

void drive_forward(float distance, int speed){
    cmpc(LEFT_MOTOR_PORT);
    cmpc(RIGHT_MOTOR_PORT);
    while (gmpc(LEFT_MOTOR_PORT) < distance*TICKS_PER_INCH){
        mav(LEFT_MOTOR_PORT, speed);
        mav(RIGHT_MOTOR_PORT, speed);
    }
    mav(RIGHT_MOTOR_PORT, 0);
    mav(LEFT_MOTOR_PORT, 0);
}

void turn_right(int degrees, int speed){
	cmpc(LEFT_MOTOR_PORT);
    cmpc(RIGHT_MOTOR_PORT);
    while (gmpc(LEFT_MOTOR_PORT) < degrees*TICKS_PER_DEGREE){
    	mav(LEFT_MOTOR_PORT, speed);
        mav(RIGHT_MOTOR_PORT, -speed);
    }
    mav(RIGHT_MOTOR_PORT, 0);
    mav(LEFT_MOTOR_PORT, 0);
}    
void turn_left(int degrees, int speed){
	cmpc(LEFT_MOTOR_PORT);
    cmpc(RIGHT_MOTOR_PORT);
    while (gmpc(RIGHT_MOTOR_PORT) < degrees*TICKS_PER_DEGREE){
    	mav(RIGHT_MOTOR_PORT, speed);
        mav(LEFT_MOTOR_PORT, -speed);
    }
    mav(RIGHT_MOTOR_PORT, 0);
    mav(LEFT_MOTOR_PORT, 0);
}

void line_follow(float distance, int speed, int is_left, int port, float sensitivity){
    float percentage;
    int mid_color = (BLACK_VALUE + WHITE_VALUE)/2;

    cmpc(LEFT_MOTOR_PORT);
    cmpc(RIGHT_MOTOR_PORT);
    
    while (gmpc(LEFT_MOTOR_PORT) < distance*TICKS_PER_INCH){
        float current_color = analog(port);
        if (is_left){
            percentage = (current_color-mid_color)/(mid_color-WHITE_VALUE);
    	}
    	else {
            percentage = -(current_color-mid_color)/(mid_color-WHITE_VALUE);
    	}

        mav(LEFT_MOTOR_PORT, (1 - percentage*sensitivity)*speed);
        mav(RIGHT_MOTOR_PORT, (1 + percentage*sensitivity)*speed);
        printf("right %f",(1 + percentage*sensitivity)*speed);
        printf("left %f",(1 - percentage*sensitivity)*speed);
        msleep(100);
    }
    mav(RIGHT_MOTOR_PORT, 0);
    mav(LEFT_MOTOR_PORT, 0);
    
}
void line_follow_backwards(float distance, int speed, int is_left, int port, float sensitivity){
    float percentage;
    int mid_color = (SMALL_BLACK_VALUE + SMALL_WHITE_VALUE)/2;

    cmpc(LEFT_MOTOR_PORT);
    cmpc(RIGHT_MOTOR_PORT);
    
    while (gmpc(LEFT_MOTOR_PORT) > -distance*TICKS_PER_INCH){
        float current_color = analog(port);
        if (is_left){
            percentage = -(current_color-mid_color)/(mid_color-SMALL_WHITE_VALUE);
    	}
    	else {
            percentage = (current_color-mid_color)/(mid_color-SMALL_WHITE_VALUE);
    	}

        mav(LEFT_MOTOR_PORT, -(1 - percentage*sensitivity)*speed);
        mav(RIGHT_MOTOR_PORT, -(1 + percentage*sensitivity)*speed);
        printf("right %f",(1 + percentage*sensitivity)*speed);
        printf("left %f",(1 - percentage*sensitivity)*speed);
        msleep(100);
    }
    mav(RIGHT_MOTOR_PORT, 0);
    mav(LEFT_MOTOR_PORT, 0);
}


void move_servo(int port, int position, int time) {
}

void move_claw(int target, int speed) {
    while (gmpc(CLAW_PORT) < target){
        mav(CLAW_PORT, speed);
        msleep(100);
    }
    mav(CLAW_PORT, 0);
}

int main()
{
    /*cmpc(CLAW_PORT);
    drive_forward(11, 1000);
    turn_left(85, 1000);
    move_claw(CLAW_ATTACK, 1000);
    line_follow_backwards(29.0, 1000, 1, BACK_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    move_claw(CLAW_CLOSED, 1000);
    line_follow(58, 1000, 0, RIGHT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    turn_left(180, 1000);
    line_follow_backwards(15.0, 400, 1, BACK_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    drive_forward(1, 400);
    msleep(10000);
    line_follow(7, 1000, 1, LEFT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    turn_left(90, 1000);
*/
    drive_forward(15,1000);
    return 0;
}
