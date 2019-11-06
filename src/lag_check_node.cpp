#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/Image.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float32.h>
#include <ros/ros.h>


int main(int argc, char** argv) {
    ros::init(argc, argv, "lag_check_node");
    ros::NodeHandle node("~");
    std::string bag_name;
    node.param<std::string>("bag_name", bag_name, "test");
    std::string bag_path_name = bag_name + ".bag";
    std::string bag_lag_path_name = bag_name + "_lag_check.bag";
    /* read */
    rosbag::Bag bag;
    bag.open(bag_path_name);
    std::vector<std::string> topics;
    topics.push_back(std::string("/imu/imu"));
    topics.push_back(std::string("/right/image_raw"));
    topics.push_back(std::string("/vins_estimator/odometry"));

    /* write */
    rosbag::Bag bag2;
    bag2.open(bag_lag_path_name, rosbag::bagmode::Write);
    std::vector<std::string> lag_topics;
    lag_topics.resize(0);
    for (int i = 0; i < topics.size(); i++) {
        lag_topics.push_back(topics[i] + "_lag");
    }

//    rosbag::View view(bag, rosbag::TopicQuery(topics));
    for (rosbag::MessageInstance const m: rosbag::View(bag)) {
        for(int i = 0; i < topics.size(); i++) {
            if (m.getTopic() == topics[i]) {
                ros::Time pub_time = m.getTime();
                ros::Time msg_time;// = m.header.stamp;
                switch (i) {
                    case 0: {
                        sensor_msgs::Imu::ConstPtr s = m.instantiate<sensor_msgs::Imu>();
                        msg_time = s->header.stamp;
                    }
                        break;
                    case 1: {
                        sensor_msgs::Image::ConstPtr s = m.instantiate<sensor_msgs::Image>();
                        msg_time = s->header.stamp;
                    }
                        break;
                    case 2: {
                        nav_msgs::Odometry::ConstPtr s = m.instantiate<nav_msgs::Odometry>();
                        msg_time = s->header.stamp;
                    }
                        break;
                    default:
                        break;
                }
                float lag_time = (pub_time-msg_time).toSec();
                std_msgs::Float32 lag_msg;
                lag_msg.data = lag_time;
                bag2.write(lag_topics[i], pub_time, lag_msg);
            }
        }
    }
    bag.close();
    bag2.close();
}
