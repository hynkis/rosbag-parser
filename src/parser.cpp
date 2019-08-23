#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <iostream>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>


int main(int argc, char **argv)
{
    // Set bag file to read
    rosbag::Bag bag;
    bag.open("/home/user/catkin_ws/src/rosbag_parser/test.bag", rosbag::bagmode::Read);
    int count_img = 0;
    int count_steer = 0;
    int count_save = 0;

    bool img_done = false;
    bool steer_done = false;

    // Set topics to read
    std::vector<std::string> topics;
    topics.push_back(std::string("/camera/image_raw"));
    topics.push_back(std::string("/steering_angle"));

    rosbag::View view(bag, rosbag::TopicQuery(topics));

    // Read data of the topics
    BOOST_FOREACH(rosbag::MessageInstance const m, view)
    {
        // 1st topic : /camera/image_raw
        sensor_msgs::ImageConstPtr msg_img = m.instantiate<sensor_msgs::Image>();
        if (msg_img != NULL)
        {
            count_img++;
            std::cout << "count of /camera/image_raw : " << count_img << std::endl;
            // std::cout << "header image : " << msg_img->header.stamp << std::endl;
            cv::Mat image_raw = cv_bridge::toCvCopy(msg_img, "bgr8")->image;
            img_done = true;
        }

        // 2nd topic : /steering_angle
        std_msgs::Int32::ConstPtr msg_steer = m.instantiate<std_msgs::Int32>();
        if (msg_steer != NULL)
        {
            count_steer++;
            std::cout << "count of /steering_angle : " << count_steer << " steering_angle : " << msg_steer->data << std::endl;
            steer_done = true;
        }

        // Precessing and Saving only when both of image and steer given.
        if (img_done == true && steer_done == true)
        {
            count_save++;
            std::cout << "count of save : " << count_save << " | save image and steer : " << count_img << " " << count_steer << std::endl;
            // Processing block start

            // Processing block end
            img_done = false;
            steer_done = false;
        }

    }
    bag.close();

    return 0;
}