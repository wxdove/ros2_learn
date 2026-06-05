import launch
import launch_ros


def generate_launch_description():
    talker_param = launch_ros.actions.Node(
        executable='talker_param',
        package='cpp_topic',
        output='screen',
        parameters=[{
            'start_count': 100,
            'publish_interval_ms': 1000,
            'topic_name': '/my_topic',
        }],
    )

    listen_param=launch_ros.actions.Node(
        executable='listenr_param',
        package='cpp_topic',
        output='screen',
        parameters=[{
            "topic_name":"/my_topic"
        }]
    )

    return launch.LaunchDescription([talker_param,listen_param])
