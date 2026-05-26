#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/msg/pose.hpp"
#include <cmath>
#include <iostream>

class Turtle_control: public rclcpp::Node
{
private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscriptor_;
    double target_x;
    double target_y;
    double p_=1.0;
    double max_speed_=3.0;

public:
    explicit Turtle_control(const std::string &node_name, double x,double y):Node(node_name){
        target_x=x;
        target_y=y;
        publisher_=this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10);
        subscriptor_=this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose",10,
            [this](const turtlesim::msg::Pose::SharedPtr pose){pose_receive(pose);});
    };

    void pose_receive(const turtlesim::msg::Pose::SharedPtr pose){
        auto msg=geometry_msgs::msg::Twist();
        auto current_x=pose->x;
        auto current_y=pose->y;
        RCLCPP_INFO(get_logger(),"当前x=%f,y=%f",current_x,current_y);
        auto distance=std::sqrt(
            (target_x-current_x)*(target_x-current_x)+
            (target_y-current_y)*(target_y-current_y)
        );
        auto angle=std::atan2(target_y-current_y,target_x-current_x)-pose->theta;
        if(distance>0.1){
            if(fabs(angle)>0.2){
                msg.angular.z=fabs(angle);
            }else{
                msg.linear.x=p_*distance;
            }
        };

        if(msg.linear.x>max_speed_){
            msg.linear.x=max_speed_;
        };
        publisher_->publish(msg);
    }

};

int main(int argc,char **argv){
    rclcpp::init(argc,argv);
    auto node=std::make_shared<Turtle_control>("turtle_control",3,3);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}