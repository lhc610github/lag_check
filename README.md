# Lag check package
## check msg type:
	nav_msgs::Odometry
	sensor_msgs::Imu
	sensor_msgs::Image

## usage:
	1. put .bag file into log folder
	2. modify "bag_name" param using bag filename (without .bag)
	3. roscore
	4. source YOUR_WS_NAME/devel/setup.bash && roslaunch lag_time_check lag_time_check_node.launch
	5. rosrun rqt_bag rqt_bag ${bag_name}_lag_check.bag
