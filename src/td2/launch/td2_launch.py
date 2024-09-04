from launch import LaunchDescription

from ament_index_python.packages import get_package_share_directory
from launch_ros.actions import Node
import os

def generate_launch_description():
    config = os.path.join(
        get_package_share_directory('td2'),
        'config',
        'td2.yaml'
    )

    node1 = Node(
        package = 'td2',
        namespace = '',
        executable = 'vogue',
        name = 'Zoboat',
        parameters = [config]
    )

    return LaunchDescription([node1])