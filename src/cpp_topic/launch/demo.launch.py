import launch
import launch_ros

def generate_launch_description():
    """产生launch描述"""
    #生成两个动作
    talker=launch_ros.actions.Node(
        executable='listenr',
        package='cpp_topic',
        output='screen'
    )
    listener=launch_ros.actions.Node(
        executable='talker',
        package='cpp_topic',
        output='screen'
    )


    return launch.LaunchDescription([
        #actions的动作，是个数组
        talker,
        listener
    ])