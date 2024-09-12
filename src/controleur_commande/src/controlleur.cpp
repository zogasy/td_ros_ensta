#include "controleur_commande/controlleur.hpp"


BoatControlleur::BoatControlleur(): Node("ControleCommande"){

}
void BoatControlleur::calculPosition(){

}



int main(int argc , char * argv[]){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<BoatControlleur>());
    rclcpp::shutdown();
    return 0;
}