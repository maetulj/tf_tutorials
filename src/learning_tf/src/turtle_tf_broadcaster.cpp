#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <turtlesim/Pose.h>

std::string turtle_name;

void poseCallback(const turtlesim::PoseConstPtr& msg)
{
	// Create a broadcaster to send the transformations over the wire.
	static tf::TransformBroadcaster br; 

	// Create a Transform object and copy information from the 2D turtle pose into the 3D transform.
	tf::Transform transform;
	transform.setOrigin(tf::Vector3(msg->x, msg->y, 0.0));

	tf::Quaternion q;
	q.setRPY(0, 0, msg->theta);

	// Set the rotation.
	transform.setRotation(q);

	// Send the transform.
	// Include: transform, timestamp, parent frame of the link, name of the child frame of the link.
	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", turtle_name));
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "my_tf_broadcaster");

	if (argc != 2)
	{
		ROS_ERROR("Need turtle name as argument!");
		return -1;
	}

	turtle_name = argv[1];

	ros::NodeHandle node;

	ros::Subscriber sub = node.subscribe(turtle_name + "/pose", 10, &poseCallback);

	ros::spin();

	return 0;
}