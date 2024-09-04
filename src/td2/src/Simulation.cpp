#include "Simulation.hpp"

VogueMerry::VogueMerry():  Node("Zoboat") {
            init_interfaces();
        } 

void VogueMerry::init_interfaces(){
    // Créer un publisher de type std_msgs/msg/String sur le topic "topic", avec
    // une liste d'attente de 10 messages maximum
    //publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    pose_publisher_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("position", 10);

    // Créer un timer qui appelle la fonction time_callback toutes les 500ms
    timer_ = this->create_wall_timer(freq, std::bind(&VogueMerry::publish_pose, this));

    //Abonnement au message Twist pour la commande
    subscription_ = this->create_subscription<geometry_msgs::msg::Twist>("angular_command", 10,
        std::bind(&VogueMerry::command_callback, this, _1));
}  

void VogueMerry::integration_euler(float u1){
    dx_(0,0) = cos(x_(2,0));
    dx_(1,0) = sin(x_(2,0));
    dx_(1,0) = u1;
    x_ = x_ + dx_*dt;
}
void VogueMerry::command_callback(const geometry_msgs::msg::Twist &msg){
    float cap = msg.angular.z;
    RCLCPP_INFO(this->get_logger(), "I heard: '%f'", cap);
}

 void VogueMerry::publish_pose() {
            VogueMerry::integration_euler(cap);
            tf2::Quaternion q;
            q.setRPY(0,0,x_(2,0));
            auto message = geometry_msgs::msg::PoseStamped();
            // Créer un object message de type String
            // Rempli le contenu du message
            message.header.set__stamp(this->get_clock()->now());
            message.header.set__frame_id("bato");
            message.pose.position.set__x(x_(0,0));
            message.pose.position.set__y(x_(1,0));
            message.pose.position.set__z(0.0);
            message.pose.orientation = tf2::toMsg(q);
            // Affiche un log dans la console (format de fprintf)
            // Il est également possible d'utiliser RCLCPP_WARN
            // En dehors d'un node, on peut utiliser rclcpp::get_logger("rclcpp")
            //RCLCPP_INFO(this->get_logger(), "J'envoie une position Pose Stamped ");
            
            //RCLCPP_INFO(this->get_logger(), "x : [%f] , y : [%f] " ,
            //message.pose.position.x , message.pose.position.y );

            // Publie le message en utilisation l'objet publisher
            pose_publisher_->publish(message);

        }



int main(int argc, char * argv[]) {
        // Initialise ROS 2 pour l'executable
        rclcpp::init(argc, argv);
        // Créer le node et se met en attente de messages ou d'évènements du timer
        // Attention, cette fonction est bloquante !
        rclcpp::spin(std::make_shared<VogueMerry>());
        // Coupe ROS 2 pour l'executable
        rclcpp::shutdown();
        return 0;
 }