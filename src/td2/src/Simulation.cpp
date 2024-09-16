#include "Simulation.hpp"

VogueMerry::VogueMerry():  Node("Zoboat") {
            init_interfaces();

            x_(0) = x0;
            x_(1) = y0;
            x_(2) = cap0;

        } 

void VogueMerry::init_interfaces(){
    // Créer un publisher de type std_msgs/msg/String sur le topic "topic", avec
    // une liste d'attente de 10 messages maximum
    //publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    pose_publisher_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("position_bato", 10);
    mesh_publisher_ = this->create_publisher<visualization_msgs::msg::Marker>("bateau3D",10);

    // Créer un timer qui appelle la fonction time_callback toutes les 500ms
    timer_ = this->create_wall_timer(freq, std::bind(&VogueMerry::publish_pose, this));
    //Abonnement au message Twist pour la commande
    subscription_ = this->create_subscription<geometry_msgs::msg::Twist>("angular_command", 10,
        std::bind(&VogueMerry::command_callback, this, _1));
    VogueMerry::init_parameters();
}  

void VogueMerry::init_parameters(){
    float val_default = 0.0;
    this->declare_parameter<double>("abscisse_initial",val_default);
    this->declare_parameter<double>("ordonne_initial",val_default);
    this->declare_parameter<double>("cap_initial",val_default);

    cap0 = this->get_parameter("cap_initial").as_double();
    x0 = this->get_parameter("abscisse_initial").as_double();
    y0 = this->get_parameter("ordonne_initial").as_double();
}

void VogueMerry::integration_euler(float u1){
    dx_(0,0) = cos(x_(2,0));
    dx_(1,0) = sin(x_(2,0));
    dx_(2,0) = u1;
    x_ = x_ + dx_*dt;
}

void VogueMerry::command_callback(const geometry_msgs::msg::Twist &msg){
    cap = msg.angular.z;
    RCLCPP_INFO(this->get_logger(), "I heard: '%f'", cap);
}

 void VogueMerry::publish_pose() {
            VogueMerry::integration_euler(cap);
            //tf2::Quaternion q;
            //q.setRPY(0,0,x_(2,0));
            bateau_orientation.setRPY(0,0,x_(2));
            auto message = geometry_msgs::msg::PoseStamped();
            // Créer un object message de type String
            // Rempli le contenu du message
            message.header.stamp = this->now();
            message.header.frame_id = "map";
            message.pose.position.set__x(x_(0,0));
            message.pose.position.set__y(x_(1,0));
            message.pose.position.set__z(0.0);
            message.pose.orientation = tf2::toMsg(bateau_orientation);

//Marker pour visualiser le meshfile sur rviz
            visualization_msgs::msg::Marker marker;
            marker.header.frame_id = "map";
            marker.header.stamp = this->now();
            marker.ns = "boat";

            marker.id = 0;
            marker.type = visualization_msgs::msg::Marker::MESH_RESOURCE;
            marker.action = visualization_msgs::msg::Marker::ADD;

            marker.pose.position.set__x(x_(0,0));
            marker.pose.position.set__y(x_(1,0));
            marker.pose.position.set__z(0.0);
            marker.pose.orientation = tf2::toMsg(bateau_orientation);
            

            marker.scale.x = 1;
            marker.scale.y = 1;
            marker.scale.z = 1;
            marker.color.a = 1.0; // alpha = transparence
            marker.color.r = 1.0;
            marker.color.g = 1.0;
            marker.color.b = 1.0;
            marker.mesh_resource = "package://td2/meshes/boat.dae";
            //marker.mesh_resource = "package://td2/meshes/coque_solide.dae";

            // Affiche un log dans la console (format de fprintf)
            // Il est également possible d'utiliser RCLCPP_WARN
            // En dehors d'un node, on peut utiliser rclcpp::get_logger("rclcpp")
            pose_publisher_->publish(std::move(message));
            mesh_publisher_->publish(marker);
            RCLCPP_INFO(this->get_logger(), "x : [%f] , y : [%f] " ,
            message.pose.position.x , message.pose.position.y );

            RCLCPP_INFO(this->get_logger(),"cap  : [%f]" , x_(2,0));
            // Publie le message en utilisation l'objet publisher
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