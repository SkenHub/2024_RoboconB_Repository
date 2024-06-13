import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32MultiArray
import pyrealsense2 as RS

class RSNode(Node):
    def __init__(self):
        super().__init__("RS_node")

        self.pipe = RS.pipeline()
        self.cfg = RS.config()
        self.cfg.enable_stream(RS.stream.pose)
        self.publish_data = []*3

        self.publisher = self.create_publisher(Float32MultiArray,"RS_topic",10)
        self.timer = self.create_timer(0.001,self.interrupt)
    
    def interrupt(self):
        self.frames = self.pipe.wait_for_frames()
        self.pose = self.frames.get_pose_frame()
        if self.pose:
            self.data = self.pose.get_pose_data()
            self.publish_data[0] = -1.0 * self.data.translation.x
            self.publish_data[1] = self.data.translation.y
            self.publish_data[2] = 0
            self.publisher.publish(self.publish_data)

def main(args = None):
    rclpy.init(args=args)

    RS_node = RSNode()

    rclpy.spin(RS_node)

    RS_node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
