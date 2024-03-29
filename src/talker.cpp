/***************************************************************************
 MIT License

 Copyright © 2019 Raj Shinde
 
 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 Files (the “Software”), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge,
 publish, distribute, sublicense, and/or sell copies of the Software,
 and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 The above copyright notice and this permission notice shall be included 
 in all copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/

/**
 *  @copyright MIT License, © 2019 Raj Shinde
 *  @file    talker.cpp
 *  @author  Raj Shinde
 *  @date    11/10/2019
 *  @version 1.0
 *  @brief   ROS Publisher Node
 *  @section DESCRIPTION
 *  A Publisher node that publishes tf frame
 *  @mainpage This is beginners tutorial for creating a simple ROS package
 */

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <tf/transform_broadcaster.h>

#include <sstream>
#include <string>

#include "beginner_tutorials/serviceString.h"

// Creating a struct containing a string
struct X {
  std::string temp;
} t;


/**
 * @brief Function to provide service functionality
 * @param beginner_tutorials::serviceString::Request Request argument
 * @param beginner_tutorials::serviceString::Response Response argumnet
 * @return true
 */
bool string(beginner_tutorials::serviceString::Request &req,
            beginner_tutorials::serviceString::Response &res) {
  // Updating the string as per request
  res.sChanged = req.s;
  t.temp = res.sChanged;
  return true;
}

/**
 * @brief Main function for Publisher
 * @param argc no of argumnets 
 * @param argv char pointer consisting arguments 
 * @return 0
 */
int main(int argc, char **argv) {
  // The ros::init() function needs to see argc and argv so that it can perform
  // any ROS arguments and name remapping that were provided at the command
  // line. For programmatic remappings you can use a different version of
  // init() which takes remappings directly, but for most command-line programs,
  // passing argc and argv is the easiest way to do it.  The third argument to
  // init() is the name of the You must call one of the versions of ros::init()
  // before using any other node. part of the ROS system.
  int rate;
  rate = atoi(argv[1]);
  t.temp = "Default Message ";

  // Display ROS LOG messae if rate is 0
  if (rate < 1) {
    ROS_FATAL_STREAM("Rate cannot be 0 ");
    return 1;
  } else {
  ros::init(argc, argv, "talker");

  // NodeHandle is the main access point to communications with the ROS system.
  // The first NodeHandle constructed will fully initialize this node, and the
  // lastNodeHandle destructed will close down the node.
  ros::NodeHandle n;

  // Server object used to call the service function
  ros::ServiceServer server = n.advertiseService("service_string", string);

  // The advertise() function is how you tell ROS that you want to
  // publish on a given topic name. This invokes a call to the ROS
  // master node, which keeps a registry of who is publishing and who
  // is subscribing. After this advertise() call is made, the master
  // node will notify anyone who is trying to subscribe to this topic name,
  // and they will in turn negotiate a peer-to-peer connection with this
  // node.  advertise() returns a Publisher object which allows you to
  // publish messages on that topic through a call to publish().  Once
  // all copies of the returned Publisher object are destroyed, the topic
  // will be automatically unadvertised.
  // The second parameter to advertise() is the size of the message queue
  // used for publishing messages.  If messages are published more quickly
  // than we can send them, the number here specifies how many messages to
  // buffer up before throwing some away.
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
  ros::Rate loop_rate(rate);

  // Creating Objects
  tf::TransformBroadcaster br;
  tf::Transform transform;

  // A count of how many messages we have sent. This is used to create
  // a unique string for each message.
  int count = 0;
  while (ros::ok()) {
    // Display ROS LOG messages on rate status
    if (rate < 10) {
      ROS_WARN_STREAM("Rate is very low");
    } else {}
    ROS_INFO_STREAM("Loop Rate is "<< rate);
    // This is a message object. You stuff it with data, and then publish it.
    std_msgs::String msg;

    std::stringstream ss;
    ss << t.temp << count;

    // Display ROS LOG messages on string status
      if (t.temp.size() == 0) {
        ROS_ERROR_STREAM("String cannot be blank");
      } else {
        ROS_DEBUG_STREAM("New String is "<< t.temp);
      }
    msg.data = ss.str();
    ROS_INFO("%s", msg.data.c_str());

    // The publish() function is how you send messages. The parameter
    // is the message object. The type of this object must agree with the type
    // given as a template parameter to the advertise<>() call, as was done
    // in the constructor above.
    chatter_pub.publish(msg);

    // Set origin coordinates
    transform.setOrigin(tf::Vector3(10.0, 3.0, 0.0));

    // Creating a Quaternion Object
    tf::Quaternion q;

    // Set Roll, Pitch & Yaw
    q.setRPY(10, 20, 30);
    transform.setRotation(q);

    // Broadcast the transform
    br.sendTransform(tf::StampedTransform(transform,
                                          ros::Time::now(), "world", "talk"));
    ros::spinOnce();
    loop_rate.sleep();
    ++count;
  }
  return 0;
}
}
