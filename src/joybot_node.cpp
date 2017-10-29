#include <ros/ros.h>
#include "wiringPi.h"
#include <geometry_msgs/Twist.h>
#include <stdlib.h>
#include <cmath>

#define RE 23
#define LE 1
#define RB 2
#define RF 3
#define LB 4
#define LF 5

double plin = 0;
double pang = 0;

double lspeed = 0;
double rspeed = 0;

double getPWM(double speed) {
	return 70*speed + 190;
}

using namespace std;
void callback(const geometry_msgs::Twist &msg) {
	plin = msg.linear.x;
	pang = msg.angular.z;
	ROS_INFO("P: %lf %lf", plin, pang);
	lspeed = (2.0*plin - pang) / 2.0;
	rspeed = (2.0*plin + pang) / 2.0;

	//Set the wheel directions
	if (rspeed > 0) {
		digitalWrite(RF,HIGH);
		digitalWrite(RB,LOW);
	}
	else {
		digitalWrite(RB,HIGH);
		digitalWrite(RF,LOW);
	}
	
	if (lspeed > 0) {
		digitalWrite(LF,HIGH);
		digitalWrite(LB,LOW);
	}
	else {
		digitalWrite(LB,HIGH);
		digitalWrite(LF,LOW);
	}
	
	// Now set the PWM values
	pwmWrite(RE, getPWM(abs(rspeed)));
	pwmWrite(LE, getPWM(abs(lspeed)));
}

int main(int argc, char **argv) {
	
	wiringPiSetup();
	
	for (int i = 2; i < 6; i++){
		pinMode(i, OUTPUT);
	}

	pinMode(RE,PWM_OUTPUT);
	pinMode(LE,PWM_OUTPUT);

	pwmSetMode(PWM_MODE_MS);
	pwmSetClock(192);
	pwmSetRange(400);


	double plin;
	double pang;

	ros::init(argc, argv, "JoyBot");
	ros::NodeHandle nh;
	ros::Subscriber sub = nh.subscribe("/turtle1/cmd_vel",10, callback);

	while(ros::ok()) {
		ROS_INFO("Speed: %lf %lf", lspeed, rspeed);
		ros::spinOnce();
	}
	return 0;
}
