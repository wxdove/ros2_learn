#include "rclcpp/rclcpp.hpp"
#include "status_interfaces/msg/cus_tom.hpp"
#include <chrono>
#include <string>

class Custom_Msg : public rclcpp::Node
{
    private:
        rclcpp::Publisher<status_interfaces::msg::CusTom>::SharedPtr publisher_;
        rclcpp::TimerBase ::SharedPtr timer_;
        int count_=0;
    public:
        Custom_Msg(const std::string &node_name):Node(node_name){
            publisher_=this->create_publisher<status_interfaces::msg::CusTom>("self_node",10);
            timer_=this->create_wall_timer(std::chrono::milliseconds(1000),[this](){call_back();});
        }

        void call_back(){
            auto msg=status_interfaces::msg::CusTom();
            msg.age=count_;
            count_++;
            publisher_->publish(msg);
            RCLCPP_INFO(get_logger(),"今年%d岁",msg.age);
        }
};

int main(int argc,char **argv){
    rclcpp::init(argc,argv);
    auto node=std::make_shared<Custom_Msg>("Custom_Msg_node");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}