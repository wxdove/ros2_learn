#include "rclcpp/rclcpp.hpp"
#include "status_interfaces/msg/system_status.hpp"

#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/utsname.h>

class SystemStatusPublisher : public rclcpp::Node
{
private:
    rclcpp::Publisher<status_interfaces::msg::SystemStatus>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    // 用于计算 CPU 使用率
    long prev_idle_ = 0;
    long prev_total_ = 0;

    float get_cpu_percent()
    {
        std::ifstream file("/proc/stat");
        std::string line;
        std::getline(file, line);

        std::istringstream ss(line);
        std::string cpu;
        long user, nice, system, idle, iowait, irq, softirq, steal;
        ss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

        long idle_sum = idle + iowait;
        long total = user + nice + system + idle + iowait + irq + softirq + steal;

        long delta_idle = idle_sum - prev_idle_;
        long delta_total = total - prev_total_;

        prev_idle_ = idle_sum;
        prev_total_ = total;

        if (delta_total == 0) return 0.0f;
        return (1.0f - (float)delta_idle / delta_total) * 100.0f;
    }

    void get_memory(float &percent, float &total, float &available)
    {
        std::ifstream file("/proc/meminfo");
        std::string line;

        long mem_total = 0, mem_available = 0;

        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string key;
            long value;
            ss >> key >> value;

            if (key == "MemTotal:") mem_total = value;
            else if (key == "MemAvailable:") mem_available = value;

            if (mem_total && mem_available) break;
        }

        total = mem_total / 1024.0f;            // KB -> MB
        available = mem_available / 1024.0f;    // KB -> MB
        percent = mem_total > 0 ? (1.0f - (float)mem_available / mem_total) * 100.0f : 0.0f;
    }

    void get_network(double &sent, double &recv)
    {
        std::ifstream file("/proc/net/dev");
        std::string line;

        // 跳过前两行标题
        std::getline(file, line);
        std::getline(file, line);

        long total_rx = 0, total_tx = 0;

        while (std::getline(file, line)) {
            // 格式: interface: rx_bytes ... tx_bytes ...
            size_t colon = line.find(':');
            if (colon == std::string::npos) continue;

            std::string iface = line.substr(0, colon);
            // 跳过 lo 回环接口
            if (iface.find("lo") == 0) continue;

            std::istringstream ss(line.substr(colon + 1));
            long r_bytes, r_packets, r_errs, r_drop;
            ss >> r_bytes >> r_packets >> r_errs >> r_drop;

            // tx 列在 rx 之后，跳过 4 列进入 tx 区域
            long r_fifo, r_frame, r_compressed, r_multicast;
            ss >> r_fifo >> r_frame >> r_compressed >> r_multicast;

            long t_bytes;
            ss >> t_bytes;

            total_rx += r_bytes;
            total_tx += t_bytes;
        }

        sent = total_tx / (1024.0 * 1024.0);   // B -> MB
        recv = total_rx / (1024.0 * 1024.0);
    }

    void publish_status()
    {
        auto msg = status_interfaces::msg::SystemStatus();

        msg.stamp = this->now();

        char hostname[256];
        gethostname(hostname, sizeof(hostname));
        msg.host_name = hostname;

        msg.cpu_percent = get_cpu_percent();
        get_memory(msg.memory_percent, msg.memory_total, msg.memory_available);
        get_network(msg.net_sent, msg.net_recv);

        publisher_->publish(msg);
        RCLCPP_INFO(get_logger(),
            "CPU:%.1f%% MEM:%.1f%% (%.1f/%.1f MB) NET:↑%.2f ↓%.2f MB",
            msg.cpu_percent, msg.memory_percent,
            msg.memory_available, msg.memory_total,
            msg.net_sent, msg.net_recv);
    }

public:
    SystemStatusPublisher(const std::string &node_name) : Node(node_name)
    {
        publisher_ = this->create_publisher<status_interfaces::msg::SystemStatus>("system_status", 10);
        timer_ = this->create_wall_timer(std::chrono::seconds(2), [this]() { publish_status(); });
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SystemStatusPublisher>("system_status_publisher");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
