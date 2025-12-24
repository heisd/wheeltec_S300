// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from robot_interfaces:srv/SetRgb.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_INTERFACES__SRV__DETAIL__SET_RGB__STRUCT_H_
#define ROBOT_INTERFACES__SRV__DETAIL__SET_RGB__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/SetRgb in the package robot_interfaces.
typedef struct robot_interfaces__srv__SetRgb_Request
{
  bool en;
  uint8_t r;
  uint8_t g;
  uint8_t b;
} robot_interfaces__srv__SetRgb_Request;

// Struct for a sequence of robot_interfaces__srv__SetRgb_Request.
typedef struct robot_interfaces__srv__SetRgb_Request__Sequence
{
  robot_interfaces__srv__SetRgb_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__srv__SetRgb_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'res'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/SetRgb in the package robot_interfaces.
typedef struct robot_interfaces__srv__SetRgb_Response
{
  rosidl_runtime_c__String res;
} robot_interfaces__srv__SetRgb_Response;

// Struct for a sequence of robot_interfaces__srv__SetRgb_Response.
typedef struct robot_interfaces__srv__SetRgb_Response__Sequence
{
  robot_interfaces__srv__SetRgb_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__srv__SetRgb_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROBOT_INTERFACES__SRV__DETAIL__SET_RGB__STRUCT_H_
