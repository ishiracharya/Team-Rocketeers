#include <kipr/wombat.h>

/*
+--------------------------------------+    +--\  /--\  --+--  +--\   /\   |     |       +--\  /--\  /---  |   /  +---  --+--  +---  +---  +--\  +---  |
|   Rocketeers - 2023 Botball #0328    |    |__/  |  |    |    |__/  /__\  |     |       |__/  |  |  |     |__/   |__     |    |__   |__   |__/  |     |
|          Competition Code            |    |  \  |  |    |    |  \  |  |  |     |       |  \  |  |  |     |  \   |       |    |     |     |  \  +--+  |
+--------------------------------------+    |__/  \__/    |    |__/  |  |  |___  |___    |  |  \__/  \___  |   \  +---    |    +---  +---  |  |  ___|  o
*/

// constants
int LEFT_MOTOR_PORT = 0;
int RIGHT_MOTOR_PORT = 1;

int LEFT_TOPHAT_PORT = 1;
int RIGHT_TOPHAT_PORT = 0;
//int BACK_RIGHT_TOPHAT_PORT = 2;
int BACK_LEFT_TOPHAT_PORT = 3;

int LIGHT_SENSOR_PORT = 4;

int BLACK_VALUE = 3200;
int WHITE_VALUE = 200;
int SMALL_BLACK_VALUE = 3800;
int SMALL_WHITE_VALUE = 2600;

float LINE_FOLLOW_SENSITIVITY_LOW = 0.2;
float LINE_FOLLOW_SENSITIVITY_HIGH = 0.5;

float TICKS_PER_DEGREE = 12.3; //13.1;
float TICKS_PER_INCH = 222.2;

int CLAW_PORT = 2;
int CLAW_OUT = 369;
int CLAW_OPEN = 1400;
int CLAW_CLOSED = 1910;

int LIFT_UP = 1660;
int LIFT_DOWN = 490;
int LIFT_PORT = 0;

int SORTER_LEFT = 510;
int SORTER_RIGHT = 1719;
int SORTER_MIDDLE = 1024;
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


void calibrate(){ // pronounced "CAH-lee-bra-TAY"
    move_servo(LIFT_PORT, LIFT_UP, 1000);
    move_servo(SORTER_PORT, SORTER_MIDDLE, 500);
    move_servo(CLAW_PORT, CLAW_OPEN, 500);
}

int main()
{
    // get ready for scoring hella points
    enable_servos();
    //calibrate(); return 0;

    //wait_for_light(LIGHT_SENSOR_PORT);

    // scoring hella points, act 1: "the claw close"
    /*
    move_servo(CLAW_PORT, CLAW_OUT, 500);
    turn_left(2, 300);
    move_servo(LIFT_PORT, LIFT_DOWN, 1000);
    drive(6.5, 1000);
    move_servo(SORTER_PORT, SORTER_RIGHT, 500);
    turn_right(90, 500);
    move_servo(SORTER_PORT, SORTER_LEFT, 500);
    line_follow(5, 1000, 1, LEFT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    move_servo(SORTER_PORT, SORTER_RIGHT, 500);
    line_follow(8, 1000, 1, LEFT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    move_servo(SORTER_PORT, SORTER_LEFT, 500);  
    line_follow(4, 1000, 1, LEFT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    turn_left(180, 500);
    line_follow_backwards(10.5, 1000, 0, BACK_LEFT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    move_servo(CLAW_PORT, CLAW_CLOSED, 500);

    // scoring hella points, act 2: "the drive back"
    line_follow(28, 1000, 0, RIGHT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    move_servo(SORTER_PORT, SORTER_RIGHT, 500);
    line_follow(8, 1000, 0, RIGHT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    move_servo(SORTER_PORT, SORTER_LEFT, 500);
    line_follow(8, 1000, 0, RIGHT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    move_servo(SORTER_PORT, SORTER_RIGHT, 500);
    line_follow(8, 1000, 0, RIGHT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    move_servo(SORTER_PORT, SORTER_LEFT, 500);  
    line_follow(8, 1000, 0, RIGHT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    move_servo(SORTER_PORT, SORTER_RIGHT, 500);
    turn_right(180, 500);
    move_servo(SORTER_PORT, SORTER_LEFT, 500);
    line_follow_backwards(8, 1000, 0, BACK_LEFT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    drive(2, 1000);

    // intermission: please return to your seats in 10 seconds
    msleep(10000);
	*/
    // scoring hella points, act 3: "the finale"v2
    drive(6, 1000);
    turn_left(100,500);
    move_servo(SORTER_PORT, SORTER_RIGHT, 500);
    move_servo(SORTER_PORT, SORTER_LEFT, 500);
    drive(-3, 1000);
    move_servo(CLAW_PORT, CLAW_OUT, 500);
    drive(3,1000);
    turn_left(125,500);
    move_servo(SORTER_PORT, SORTER_RIGHT, 500);
    drive(5,1000);
    drive(-10,1000);
    turn_left(45,500);
    turn_left(5,500);
    turn_right(5,500);
    turn_left(5,500);
    turn_right(5,500);
    
    
    
    // scoring hella points, act 3: "the finale"v1
    /*
    line_follow(8, 1000, 0, LEFT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    move_servo(SORTER_PORT, SORTER_RIGHT, 500);
    line_follow(8, 1000, 0, LEFT_TOPHAT_PORT, LINE_FOLLOW_SENSITIVITY_LOW);
    move_servo(SORTER_PORT, SORTER_LEFT, 500);
    drive(-16,1000);
    turn_left(45,500);
    drive(5,1000);
    turn_left(65,500);
    move_servo(CLAW_PORT, CLAW_OUT, 500);
    turn_left(25,500);
    drive(-3,1000);
    turn_left(65,500);
    move_servo(SORTER_PORT, SORTER_RIGHT, 500);
    turn_left(5,500);
    turn_right(5,500);
    turn_left(5,500);
    turn_right(5,500);
    */

    return 0;
}

/*
   __
  |  |
  |  o
  | /|\
  | / 
__|__     ROC_ETEERS
A N L M U

*/
