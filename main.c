#include <kipr/wombat.h>

//constants
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

int LIFT_UP = 324;
int LIFT_DOWN = 2047;
int LIFT_PORT = 0;

int SORTER_LEFT = 0;
int SORTER_RIGHT = 1458;
int SORTER_MIDDLE = 694;
int SORTER_PORT = 1;

void drive(float distance, int speed){
    cmpc(LEFT_MOTOR_PORT);
    cmpc(RIGHT_MOTOR_PORT);
    if(distance > 0){
        while (gmpc(LEFT_MOTOR_PORT) < distance*TICKS_PER_INCH){
            mav(LEFT_MOTOR_PORT, speed);
            mav(RIGHT_MOTOR_PORT, speed);
        }
    }
    else {
        while (gmpc(LEFT_MOTOR_PORT) > distance*TICKS_PER_INCH){
            mav(LEFT_MOTOR_PORT, -speed);
            mav(RIGHT_MOTOR_PORT, -speed);
        }
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

void move_servo(int port, int target_position, int target_time) {
    int interval = 10;
    int current_position = get_servo_position(port);
    int current_time = 0;
    while (current_time<target_time) {
       	set_servo_position(port, current_position + (current_time*(target_position-current_position)/target_time));
       	msleep(interval);
       	current_time = current_time + interval;
    }
    set_servo_position(port, target_position);
}

void close_claw(int target, int speed) {
    while (gmpc(CLAW_PORT) < target){
        mav(CLAW_PORT, speed);
        msleep(100);
    }
    mav(CLAW_PORT, 0);
}

void open_claw(int target, int speed) {
    while (gmpc(CLAW_PORT) > target){
        mav(CLAW_PORT, -speed);
        msleep(100);
    }
    mav(CLAW_PORT, 0);
}

void calibrate(){
    move_servo(LIFT_PORT, LIFT_UP, 1000);
    move_servo(SORTER_PORT, SORTER_MIDDLE, 500);
    open_claw(0, 1000);
}

int main()
{
    enable_servos();
    calibrate();
    
    // scoring hella points
    drive(-2,1000);
    turn_left(3,400);
    move_servo(LIFT_PORT, LIFT_DOWN, 1000);
    drive(11, 1000);
    move_servo(SORTER_PORT, SORTER_RIGHT, 500);
    turn_right(85, 1000);
    move_servo(SORTER_PORT, SORTER_LEFT, 500);
    line_follow(6, 1000, 1, LEFT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    move_servo(SORTER_PORT, SORTER_RIGHT, 500);
    line_follow(8, 1000, 1, LEFT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    move_servo(SORTER_PORT, SORTER_LEFT, 500);    
    close_claw(CLAW_ATTACK, 1000);
    turn_left(180, 1000);
    line_follow_backwards(10, 1000, 0, BACK_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    close_claw(CLAW_CLOSED, 1000);
    
    
   
    return 0;
}
