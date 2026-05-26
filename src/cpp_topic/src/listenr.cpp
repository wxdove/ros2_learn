#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Listenr:public rclcpp::Node{
    private:
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriptor_;

    public:
        Listenr(const std::string &node_name):Node(node_name){
            subscriptor_=this->create_subscription<std_msgs::msg::String>("/my_topic",10,[this](std_msgs::msg::String::SharedPtr st){my_callback(st);});
        }

        void my_callback(std_msgs::msg::String::SharedPtr st){
            RCLCPP_INFO(get_logger(),"收到：%s",st->data.c_str());
        }
};
int main(int argc,char **argv){
    rclcpp::init(argc,argv);
    auto node=std::make_shared<Listenr>("listenr_node");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}