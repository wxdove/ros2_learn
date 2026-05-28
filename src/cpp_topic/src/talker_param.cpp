#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <chrono>

class TalkerParam : public rclcpp::Node {
private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    int count_;

public:
    TalkerParam(const std::string &node_name) : Node(node_name) {
        // ========== 区别1：declare_parameter 声明参数（名字 + 默认值）==========
        this->declare_parameter<int>("start_count", 0);
        this->declare_parameter<int>("publish_interval_ms", 1000);
        this->declare_parameter<std::string>("topic_name", "/my_topic");

        // ========== 区别2：get_parameter 读参数值（替代原来的硬编码）==========
        count_ = this->get_parameter("start_count").as_int();
        int interval = this->get_parameter("publish_interval_ms").as_int();
        std::string topic = this->get_parameter("topic_name").as_string();

        publisher_ = this->create_publisher<std_msgs::msg::String>(topic, 10);
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(interval),
            [this]() { my_callback(); });

        RCLCPP_INFO(get_logger(), "话题=%s, 间隔=%dms, 起始=%d", topic.c_str(), interval, count_);
    }

    void my_callback() {
        auto msg = std_msgs::msg::String();
        msg.data = "你好，我是第" + std::to_string(count_++) + "条消息";
        publisher_->publish(msg);
        RCLCPP_INFO(get_logger(), "发布%s", msg.data.c_str());
    }
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TalkerParam>("talker_param_node");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
