#include <eigen3/Eigen/Dense>
using namespace Eigen;


 #include <chrono>
 #include <functional>
 #include <memory>
 #include <string>
 #include <math.h>

 #include <geometry_msgs/msg/pose_stamped.hpp>
 #include <geometry_msgs/msg/twist.hpp>
 #include "rclcpp/rclcpp.hpp"
 #include "std_msgs/msg/string.hpp"
 #include "tf2/LinearMath/Quaternion.h"
 #include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
 /*ajout du code de chat pour transformer en tf message*/
 #include "visualization_msgs/msg/marker.hpp"


 // Attention à bien inclure chaque type de message !
 using namespace std::chrono_literals;
 using namespace std::placeholders;
 /*
 * Classe qui hérite de l'objet rclcpp::Node.
 * Vous pouvez choisir de séparer votre classe dans un fichier .hpp et .cpp
 */
class VogueMerry : public rclcpp::Node {
    public:
        
        VogueMerry();

    private:
        void init_interfaces();
        void init_parameters();
        void integration_euler(float u1);
        
        void publish_pose(); // Publisher de la position du bateau
        void command_callback(const geometry_msgs::msg::Twist &msg); // Callback du publisher de la position

        rclcpp::TimerBase::SharedPtr timer_; // objet timer
        rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr pose_publisher_; // objet publisher position
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;

        rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr mesh_publisher_;
        void mesh_callback();

        double x0,y0,cap0;
        Matrix<double,3,1>dx_ = Matrix<double,3,1>::Zero();
        Matrix<double,3,1>x_;
        tf2::Quaternion bateau_orientation; 
        float dt = 0.1;
        double cap;
        std::chrono::milliseconds freq = 400ms;
        };



    