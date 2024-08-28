 #include <chrono>
 #include <functional>
 #include <memory>
 #include <string>
 #include <math.h>
 #include "rclcpp/rclcpp.hpp"
 #include "std_msgs/msg/string.hpp"
 #include "std_srvs/srv/trigger.hpp"
 #include "td1/msg/state_boat.hpp"


 // Attention à bien inclure chaque type de message !
 using namespace std::chrono_literals;
 using namespace std::placeholders;
 /*
 * Classe qui hérite de l'objet rclcpp::Node.
 * Vous pouvez choisir de séparer votre classe dans un fichier .hpp et .cpp
 */
class NodeA : public rclcpp::Node {
    public:
        /* Constructeur de votre node avec le nom du node "nodeA"
        */
        NodeA() : Node("nodeA") {
            // Créer un publisher de type std_msgs/msg/String sur le topic "topic", avec
            // une liste d'attente de 10 messages maximum
            //publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);

            custom_publisher_ = this->create_publisher<td1::msg::StateBoat>("topic_custom", 10);

            // Créer un timer qui appelle la fonction time_callback toutes les 500ms
            //timer_ = this->create_wall_timer(500ms, std::bind(&NodeA::timer_callback, this));
            timer_ = this->create_wall_timer(500ms, std::bind(&NodeA::Custom_callback, this));

            // À noter qu'il existe plusieurs base de temps possible
            service_ = this->create_service<std_srvs::srv::Trigger>("boat",bind(&NodeA::etat_bateau,this,_1,_2));
        }   
    private:
        /* Fonction de callback du timer
        */
        void timer_callback() {
            // Créer un object message de type String
            auto message = std_msgs::msg::String();
            double time = this->now().seconds();
            cap = sin(time);
            // Rempli le contenu du message
            message.data = "Le cap : " + std::to_string(cap);
            // Affiche un log dans la console (format de fprintf)
            // Il est également possible d'utiliser RCLCPP_WARN
            // En dehors d'un node, on peut utiliser rclcpp::get_logger("rclcpp")
            RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
            // Publie le message en utilisation l'objet publisher
            publisher_->publish(message);
        }
        void Custom_callback(){
            auto message = td1::msg::StateBoat();
            double time = this->now().seconds();
            cap = sin(time);
            elevation = cos(time);

            // Rempli le contenu du message
            message.cap = cap;
            message.elevation = elevation;
            // Affiche un log dans la console (format de fprintf)
            // Il est également possible d'utiliser RCLCPP_WARN
            // En dehors d'un node, on peut utiliser rclcpp::get_logger("rclcpp")
            RCLCPP_INFO(this->get_logger(), "Publishing: cap '%f',elevation '%f'", message.cap , message.elevation);
            // Publie le message en utilisation l'objet publisher
            custom_publisher_->publish(message);
        }



        void etat_bateau(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                        std::shared_ptr<std_srvs::srv::Trigger::Response> response){
            (void)request;
            response->success = true;
            response->message = "bateau_De_ZO";  
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "sending back response: state = [%d] name =[%s]", response->success , response->message.c_str());
        }

        rclcpp::TimerBase::SharedPtr timer_; // objet timer
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_; // objet publisher
        rclcpp::Publisher<td1::msg::StateBoat>::SharedPtr custom_publisher_; // objet publisher avec msg custom

        rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr service_;
        size_t count_ = 0; // un compteur
        _Float64 cap,elevation;
        };
    int main(int argc, char * argv[]) {
        // Initialise ROS 2 pour l'executable
        rclcpp::init(argc, argv);
        // Créer le node et se met en attente de messages ou d'évènements du timer
        // Attention, cette fonction est bloquante !
        rclcpp::spin(std::make_shared<NodeA>());
        // Coupe ROS 2 pour l'executable
        rclcpp::shutdown();
        return 0;
 }