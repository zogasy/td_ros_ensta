#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"


class BoatControlleur : public rclcpp::Node{
    public:
        BoatControlleur();
    private:
        void calculPosition();
};