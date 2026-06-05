import launch
import launch_ros
from launch.substitutions import LaunchConfiguration
from launch.actions import DeclareLaunchArgument

def generate_launch_description():
    # 声明可从命令行传入的参数
    start_count_arg = DeclareLaunchArgument(
        'start_count', default_value='0',
        description='talker 起始计数值')
    publish_interval_arg = DeclareLaunchArgument(
        'publish_interval_ms', default_value='1000',
        description='发布间隔 (ms)')
    topic_name_arg = DeclareLaunchArgument(
        'topic_name', default_value='/my_topic',
        description='话题名称')

    #和不从命令行获取参数的区别，参数直接写死和使用LaunchConfiguration读取参数
    talker_param = launch_ros.actions.Node(
        executable='talker_param',
        package='cpp_topic',
        output='screen',
        parameters=[{
            'start_count': LaunchConfiguration('start_count'),
            'publish_interval_ms': LaunchConfiguration('publish_interval_ms'),
            'topic_name': LaunchConfiguration('topic_name'),
        }],
    )

    listenr_param = launch_ros.actions.Node(
        executable='listenr_param',
        package='cpp_topic',
        output='screen',
        parameters=[{
            'topic_name': LaunchConfiguration('topic_name'),
        }],
    )

    return launch.LaunchDescription([
        start_count_arg,
        publish_interval_arg,
        topic_name_arg,
        talker_param,
        listenr_param,
    ])
