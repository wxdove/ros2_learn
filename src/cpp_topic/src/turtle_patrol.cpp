#include <iostream>
#include <vector>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/msg/pose.hpp"

class Turtle_Patrol : public rclcpp::Node
{
private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr Publisher_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr Subscriptor_;
    std::vector<std::vector<double>> waypoints = {
        // 左耳
        {3.0, 8.0},  // 左耳尖
        {4.0, 7.0},  // 左耳根右
        // 头顶
        {5.5, 7.5},  // 头顶中间
        // 右耳
        {7.0, 7.0},  // 右耳根左
        {8.0, 8.0},  // 右耳尖
        {7.5, 6.5},  // 右耳根右
        // 右脸
        {8.5, 5.0},  // 右脸
        {7.0, 3.5},  // 右下巴
        // 下巴
        {5.5, 3.0},  // 下巴中间
        // 左脸
        {4.0, 3.5},  // 左下巴
        {3.0, 5.0},  // 左脸
        {3.0, 8.0},  // 回到左耳尖
    };
    int P_=1;
    int max_speed_=3;
    int current_index_=0;

public:
    explicit Turtle_Patrol(const std::string &node_name):Node(node_name)
    {
        Publisher_=this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10);
        Subscriptor_=this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose",10,[this](turtlesim::msg::Pose::SharedPtr pose){Patrol_Callback(pose);});
    };

    void Patrol_Callback(turtlesim::msg::Pose::SharedPtr pose){
        auto msg=geometry_msgs::msg::Twist();
        auto current_x=pose->x;
        auto current_y=pose->y;

        auto point=waypoints[current_index_];
        auto distance=std::sqrt(
                (current_x-point[0])*(current_x-point[0])+
                (current_y-point[1])*(current_y-point[1])
            );
            auto angle=std::atan2(point[1]-current_y,point[0]-current_x)-pose->theta;
            angle = std::atan2(std::sin(angle), std::cos(angle));
            if(distance>0.1){
                if(fabs(angle)>0.2){
                    msg.angular.z=angle;
                }else{
                    msg.linear.x=P_*distance;
                }
            }else{
                current_index_=(current_index_+1)%waypoints.size();
            }
            if(msg.linear.x>max_speed_){
                msg.linear.x=max_speed_;
            };
        Publisher_->publish(msg);
    }
};

int main(int argc, char **argv){
    rclcpp::init(argc,argv);
    auto node=std::make_shared<Turtle_Patrol>("turtle_patrol");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
