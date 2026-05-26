import rclpy
from rclpy import Node

class NovelPublish(Node):
    def __init__(self,node_name):
        super().__init__(node_name=node_name)
        self.get_logger().info(f'{node_name}启动')

def main():
    rclpy.init()
    node=NovelPublish('novel_pub')
    rclpy.spin(node)
    rclpy.shutdown()