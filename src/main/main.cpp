#include <Servo.h>

static const int NECK_LEFT_PIN = 25;
static const int NECK_RIGHT_PIN = 26;
static const int NECK_PAN_PIN = 14;
static const int MOUTH_PIN = 27;

static const int LEFT_EYE = 32;
static const int RIGHT_EYE = 33;





#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <micro_ros_utilities/type_utilities.h>
#include <micro_ros_utilities/string_utilities.h>

#include <rmw_microros/rmw_microros.h>
#include <std_msgs/msg/int32.h>

#define LED_PIN 2 

Servo neck_left;
Servo neck_right;
Servo neck_pan;
Servo mouth;

rclc_executor_t executor;
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;  

rcl_subscription_t neck_pos_left_topic;
std_msgs__msg__Int32 neck_pos_left_message;

rcl_subscription_t neck_pos_right_topic;
std_msgs__msg__Int32 neck_pos_right_message;

rcl_subscription_t neck_pos_pan_topic;
std_msgs__msg__Int32 neck_pos_pan_message;

rcl_subscription_t mouth_topic;
std_msgs__msg__Int32 mouth_message;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}

void error_loop(){
  for (int i = 0; i < 10; i++)
  {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
  ESP.restart();
}

void neck_left_callback(const void *msgin)
{
  const std_msgs__msg__Int32 *msg = (const std_msgs__msg__Int32 *)msgin;
  neck_left.write(msg->data);
}

void neck_right_callback(const void *msgin)
{
  const std_msgs__msg__Int32 *msg = (const std_msgs__msg__Int32 *)msgin;
  neck_right.write(msg->data);
}

void neck_pan_callback(const void *msgin)
{
  const std_msgs__msg__Int32 *msg = (const std_msgs__msg__Int32 *)msgin;
  neck_pan.write(msg->data);
}

void mouth_callback(const void *msgin)
{
  const std_msgs__msg__Int32 *msg = (const std_msgs__msg__Int32 *)msgin;
  mouth.write(msg->data);
}

void setup() {
  Serial.begin(115200);

  pinMode(LEFT_EYE,OUTPUT);
  pinMode(RIGHT_EYE,OUTPUT);

  digitalWrite(LEFT_EYE,HIGH);
  digitalWrite(RIGHT_EYE,HIGH);
  
  neck_left.attach(NECK_LEFT_PIN);
  neck_right.attach(NECK_RIGHT_PIN);
  neck_pan.attach(NECK_PAN_PIN);
  mouth.attach(MOUTH_PIN);

  neck_left.write(90);
  neck_right.write(90);
  neck_pan.write(90);
  mouth.write(90);

  set_microros_transports();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  
  
  delay(2000);

  allocator = rcl_get_default_allocator();

  // create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_power_node", "", &support));

  // create subscribers
  RCCHECK(rclc_subscription_init_default(
    &neck_pos_left_topic,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "ozzy/servo/neck/left"));

  RCCHECK(rclc_subscription_init_default(
    &neck_pos_right_topic,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "ozzy/servo/neck/right"));

  RCCHECK(rclc_subscription_init_default(
    &neck_pos_pan_topic,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "ozzy/servo/neck/pan"));

  RCCHECK(rclc_subscription_init_default(
    &mouth_topic,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "ozzy/servo/mouth"));

  // initialize executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 4, &allocator));
  
  RCCHECK(rclc_executor_add_subscription(&executor, &neck_pos_left_topic, &neck_pos_left_message, &neck_left_callback, ON_NEW_DATA));
  RCCHECK(rclc_executor_add_subscription(&executor, &neck_pos_right_topic, &neck_pos_right_message, &neck_right_callback, ON_NEW_DATA));
  RCCHECK(rclc_executor_add_subscription(&executor, &neck_pos_pan_topic, &neck_pos_pan_message, &neck_pan_callback, ON_NEW_DATA));
  RCCHECK(rclc_executor_add_subscription(&executor, &mouth_topic, &mouth_message, &mouth_callback, ON_NEW_DATA));
}

void loop()
{
  RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
  delay(100);
}
