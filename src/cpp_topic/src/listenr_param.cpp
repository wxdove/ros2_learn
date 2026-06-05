#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Litenr_param:public rclcpp::Node{
    private:
        rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriptor_;
    public:
        Litenr_param(const std::string &node_name):Node(node_name){
            this->declare_parameter<std::string>("topic_name","/my_topic");
            std::string topic=this->get_parameter("topic_name").as_string();

            subscriptor_=this->create_subscription<std_msgs::msg::String>(topic,10,[this](std_msgs::msg::String::SharedPtr st){my_cb(st);});


        };
        void my_cb(std_msgs::msg::String::SharedPtr st){
            RCLCPP_INFO(get_logger(),"收到：%s",st->data.c_str());
        };
};

int main(int argc,char **argv){
    rclcpp::init(argc,argv);
    auto node=std::make_shared<Litenr_param>("listenr_node");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}