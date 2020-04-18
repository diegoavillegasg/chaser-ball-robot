#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"

// ROS::Publisher motor commands;
ros::Publisher motor_cmd_publisher;


// TODO: Create a handle_drive_request callback function that executes whenever a drive_bot service is requested
// This function should publish the requested linear x and angular velocities to the robot wheel joints
// After publishing the requested velocities, a message feedback should be returned with the requested wheel velocities
bool handle_drive_request (ball_chaser::DriveToTarget::Request& request, ball_chaser::DriveToTarget::Response& response)
{
	ROS_INFO("DriveToTargetRequest received - linear_x:%1.2f, angular_z:%1.2f", (float)request.linear_x, (float)request.angular_z);

	geometry_msgs::Twist motor_cmd;

	motor_cmd.linear.x = request.linear_x;
	motor_cmd.angular.z = request.angular_z;

	motor_cmd_publisher.publish(motor_cmd);

	// return a response message
	response.msg_feedback = "Motor command set - linear_x: " + std::to_string(request.linear_x) +
										" , angular_z: " + std::to_string(request.angular_z);
	ROS_INFO_STREAM(response.msg_feedback);
	return true;
}

int main(int argc, char** argv)
{
	// Initialize a ROS node
	ros::init(argc, argv, "drive_bot");
	ros::NodeHandle n_;

	//Topic you want to publish
	motor_cmd_publisher = n_.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

	// ROS::ServiceServer for drive robot;
	ros::ServiceServer drive_robot_service = n_.advertiseService("/ball_chaser/command_robot", handle_drive_request);

	ROS_INFO("Ready to send drive commands");

	ros::spin();

	return 0;
}
