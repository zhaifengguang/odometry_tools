#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>
#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_datatypes.h>
#include <tf/transform_broadcaster.h>


namespace odometry_utils {
    class odometry_to_tf : public nodelet::Nodelet {
        private:
            ros::Subscriber odom_sub;
            tf::TransformBroadcaster tf_br;
            
            void handle_odom(const nav_msgs::Odometry::ConstPtr& msg) {
                tf::Transform transform; poseMsgToTF(msg->pose.pose, transform);
                tf::StampedTransform stamped_transform(transform,
                    msg->header.stamp, msg->header.frame_id,
                    msg->child_frame_id);
                tf_br.sendTransform(stamped_transform);
            }
        
        public:
            odometry_to_tf() {}
            
            virtual void onInit() {
                odom_sub = getNodeHandle().subscribe<nav_msgs::Odometry>(
                    "odom", 10, boost::bind(&odometry_to_tf::handle_odom, this, _1));
            }

    };

    PLUGINLIB_DECLARE_CLASS(odometry_utils, odometry_to_tf, odometry_utils::odometry_to_tf, nodelet::Nodelet);
}
