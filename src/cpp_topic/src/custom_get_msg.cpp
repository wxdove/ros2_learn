#include "rclcpp/rclcpp.hpp"
#include "status_interfaces/msg/cus_tom.hpp"

class Custom_get: public rclcpp::Node
{
    private:
        rclcpp::Subscription<status_interfaces::msg::CusTom> ::SharedPtr subscriptor_;
    public:
        Custom_get(const std::string &node_name):Node(node_name){
            subscriptor_=this->create_subscription<status_interfaces::msg::CusTom>("self_node",10,[this](status_interfaces::msg::CusTom::SharedPtr Age){cb(Age);});
        }

        void cb(status_interfaces::msg::CusTom::SharedPtr Age){
            RCLCPP_INFO(get_logger(),"收到：%d岁",Age->age);
        }
};
int main(int argc,char **argv){
    rclcpp::init(argc,argv);
    auto node=std::make_shared<Custom_get>("Custom_get_node");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}