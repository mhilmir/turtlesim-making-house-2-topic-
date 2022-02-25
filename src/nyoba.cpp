#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"

// global object declaration
ros::Publisher trans_pub;
ros::Subscriber pose_sub;
turtlesim::Pose current_pose;

// declaration of functions
void poseCallback(const turtlesim::Pose::ConstPtr& pose_msg);
void toDest(double x, double y);
void makingStandardHouse();
void making3DHouse();
void addGridtoRoof();

int main(int argc, char **argv){
    // node inisialization
	ros::init(argc, argv, "nyoba");

    // for covering all communication function in ROS
	ros::NodeHandle nh;

    // pub and sub assignment
	trans_pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);
    pose_sub = nh.subscribe("/turtle1/pose", 1000, poseCallback);

    // grab the pose of the turtle in the first place
    int i = 0;
    ros::Rate r(10);
    while(i < 5){
        i++;
        ROS_INFO("halo kura-kura bakal bikin rumah horee");
        ros::spinOnce();
        r.sleep();
    }

    // making the house
    makingStandardHouse();
    making3DHouse();
    addGridtoRoof();

	ros::spin();
	return 0;
}

void toDest(double x, double y){
    // object declaration
    geometry_msgs::Twist msg;

    // adjust target coordinate
    x += 5.544444561004639, y += 5.544444561004639;

    // assign the turtle track
    msg.linear.x = x-current_pose.x;
    msg.linear.y = y-current_pose.y;
    msg.linear.z = 0;
    msg.angular.x = 0;
    msg.angular.y = 0;
    msg.angular.z = 0;

    // measuring distance of turtle will take
    double distance = sqrt(pow(msg.linear.x, 2) + pow(msg.linear.y, 2));

    double current_distance = 0.0;

    // i make it 1000 Hz so the track will be made more accurate
    ros::Rate loop_rate(1000);
    double t0 = ros::Time::now().toSec();
    while(current_distance < distance){  // turtle will keep moving until the distance exceeded
        double t1 = ros::Time::now().toSec();
        current_distance = distance * (t1-t0);
        trans_pub.publish(msg);  // make the turtle move
        ros::spinOnce();
        loop_rate.sleep();
    }
    // reset the value so the turtle will no longer move
    msg.linear.x = 0;
    msg.linear.y = 0;
    trans_pub.publish(msg);
}

void poseCallback(const turtlesim::Pose::ConstPtr& pose_msg){
    // store the turtle position to current_pose
    current_pose.x = pose_msg->x;
    current_pose.y = pose_msg->y;
}

void makingStandardHouse(){
    // making triangle
    toDest(-3, 0); toDest(-1.5, 2); toDest(0, 0); toDest(-3, 0); 
    // making rectangle (and the door)
    toDest(-3, -3); toDest(-1, -3); toDest(-1, -1.7); toDest(-2, -1.7); toDest(-2, -3); toDest(0, -3); toDest(0, 0);
}
void making3DHouse(){
    // making the right side of house
    toDest(2.5, 2.5); toDest(2.5, 0.5); toDest(0, -3);
    // making the roof
    toDest(0, 0); toDest(2.5, 2.5); toDest(1.15, 3.85); toDest(-1.5, 2);

}
void addGridtoRoof(){
    // making roof grid
    toDest(-1.2, 1.6); toDest(1.42, 3.58); toDest(1.69, 3.31); toDest(-0.9, 1.2);
    toDest(-0.6, 0.8); toDest(1.96, 3.04); toDest(2.23, 2.77); toDest(-0.3, 0.4);
}
