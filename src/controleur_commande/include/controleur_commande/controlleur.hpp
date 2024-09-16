#include <functional>
#include <chrono>
#include <memory>
#include <math.h>

#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include <eigen3/Eigen/Dense>
#include "tf2/utils.h"


using namespace Eigen;
using namespace std::placeholders;
using namespace std::chrono_literals;

class BoatControlleur : public rclcpp::Node{
    public:
        BoatControlleur();
    private:
        void calculPosition();
        void init_interfaces();
        void actual_pos_sub_callback(const geometry_msgs::msg::PoseStamped &msg);
        void objectif_sub_callback(const geometry_msgs::msg::PoseStamped &msg);

        rclcpp::TimerBase::SharedPtr timer_; // objet timer
        void control_loop();

        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr objectif_sub_;
        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr actual_pos_sub_;

        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cap_pub_;
        

        float theta,cap_bato,Dthetha;
        float yaw,pitch,roll;
        float const theta_max = 0.6;
        float e,u1;
        const float K = 1.0;
        Matrix<double,2,1>x_;
        Matrix<double,2,1>xhat_;



        
};