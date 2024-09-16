#include "controleur_commande/controlleur.hpp"


BoatControlleur::BoatControlleur(): Node("ControleCommande"){
    BoatControlleur::init_interfaces();
}

void BoatControlleur::calculPosition(){
    
}

void BoatControlleur::init_interfaces(){
    objectif_sub_ = this->create_subscription<geometry_msgs::msg::PoseStamped>("desired_point", 10,
        std::bind(&BoatControlleur::objectif_sub_callback, this, _1));

    actual_pos_sub_ = this->create_subscription<geometry_msgs::msg::PoseStamped>("position_bato", 10,
        std::bind(&BoatControlleur::actual_pos_sub_callback, this, _1));

    cap_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("angular_command", 10);

    timer_ = this->create_wall_timer(400ms, std::bind(&BoatControlleur::control_loop, this));

}

void BoatControlleur::objectif_sub_callback(const geometry_msgs::msg::PoseStamped &msg){
    xhat_(0) = msg.pose.position.x;
    xhat_(1) = msg.pose.position.y;
    RCLCPP_INFO(this->get_logger(),"I heard \nx :'%f'  \n y: '%f'" ,msg.pose.position.x,msg.pose.position.y);
}

void BoatControlleur::actual_pos_sub_callback(const geometry_msgs::msg::PoseStamped &msg){
    //RCLCPP_INFO(this->get_logger(),"I heard \nx :'%f'  \n y: '%f'" ,msg.pose.position.x,msg.pose.position.y);
    x_(0) = msg.pose.position.x;
    x_(1) = msg.pose.position.y;
    tf2::Quaternion q(
        msg.pose.orientation.x,
        msg.pose.orientation.y,
        msg.pose.orientation.z,
        msg.pose.orientation.w);
    tf2::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);
    cap_bato = yaw;
}

void BoatControlleur::control_loop(){ 
    //Twist a calculer et a envoyer 
    float theta = atan2(xhat_(1) - x_(1),xhat_(0) - x_(0));
    Dthetha = theta - cap_bato;
    e = 2* atan(tan(Dthetha/2));

    if(abs(e) > M_PI/4){
        u1 = -theta_max;
    }
    else{
        u1 = K*e;
    }
    geometry_msgs::msg::Twist msg;
    msg.angular.z = u1;
    cap_pub_->publish(msg);
}



int main(int argc , char * argv[]){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<BoatControlleur>());
    rclcpp::shutdown();
    return 0;
}