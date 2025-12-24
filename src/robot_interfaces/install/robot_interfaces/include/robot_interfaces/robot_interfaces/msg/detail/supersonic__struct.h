// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from robot_interfaces:msg/Supersonic.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_INTERFACES__MSG__DETAIL__SUPERSONIC__STRUCT_H_
#define ROBOT_INTERFACES__MSG__DETAIL__SUPERSONIC__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"

/// Struct defined in msg/Supersonic in the package robot_interfaces.
typedef struct robot_interfaces__msg__Supersonic
{
  std_msgs__msg__Header header;
  float distance_a;
  float distance_b;
  float distance_c;
  float distance_d;
  float distance_e;
  float distance_f;
  float distance_g;
  float distance_h;
} robot_interfaces__msg__Supersonic;

// Struct for a sequence of robot_interfaces__msg__Supersonic.
typedef struct robot_interfaces__msg__Supersonic__Sequence
{
  robot_interfaces__msg__Supersonic * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__msg__Supersonic__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROBOT_INTERFACES__MSG__DETAIL__SUPERSONIC__STRUCT_H_
