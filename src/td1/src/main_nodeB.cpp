 #include <memory>
 #include "rclcpp/rclcpp.hpp"
 #include "std_msgs/msg/string.hpp"
 #include "std_srvs/srv/trigger.hpp"


 // Namespace utiles pour les bindings (_1, _2, etc.)
using namespace std::placeholders;
using namespace std::chrono_literals;

class NodeB : public rclcpp::Node {
public:
    NodeB() : Node("minimal_subscriber"){
        // Créer un subcripter sur le topic "topic" de type String qui appèlera la fonction
        // topic_callback pour chaque nouveau message
        subscription_ = this->create_subscription<std_msgs::msg::String>("topic", 10,
        std::bind(&NodeB::topic_callback, this, _1));

        client_ = this->create_client<std_srvs::srv::Trigger>("boat");
        auto request = std::make_shared<std_srvs::srv::Trigger::Request>();
        auto respond = client_->async_send_request(request);
            // Wait for the result.
        if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), respond) ==
        rclcpp::FutureReturnCode::SUCCESS)
        {
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Name = %s", respond.get()->message.c_str());
        } 
        else {
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service boat");
        }

        }
private:


    // Fonction de callback
    void topic_callback(const std_msgs::msg::String &msg) const {
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg.data.c_str());
    }
    // Objet de subscription, membre de la classe
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    
    rclcpp::Client<std_srvs::srv::Trigger>::SharedPtr client_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<NodeB>());
    rclcpp::shutdown();
    return 0;
 }