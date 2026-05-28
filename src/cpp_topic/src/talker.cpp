#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <chrono>

class Talker:public rclcpp::Node{
    private:
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
        int count_=0;
    public:
    Talker(const std::string &node_name):Node(node_name){
        publisher_=this->create_publisher<std_msgs::msg::String>("/my_topic",10);
        timer_=this->create_wall_timer(std::chrono::milliseconds(1000),[this](){my_callback();});
    }

    void my_callback(){//因为发布消息是主动的，所以一般不需要消息，他一直发
        auto msg=std_msgs::msg::String();//创建新的对象msg
        msg.data="你好，我是第"+std::to_string(count_++)+"条消息";//往空对象里面写东西
        publisher_->publish(msg);//发布消息
        RCLCPP_INFO(get_logger(),"发布%s",msg.data.c_str());//打印日志
    }
};
int main(int argc,char **argv){
    rclcpp::init(argc,argv);
    auto node=std::make_shared<Talker>("talker_node");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}