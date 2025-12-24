// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from robot_interfaces:srv/SetRgb.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_INTERFACES__SRV__DETAIL__SET_RGB__TRAITS_HPP_
#define ROBOT_INTERFACES__SRV__DETAIL__SET_RGB__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "robot_interfaces/srv/detail/set_rgb__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace robot_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetRgb_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: en
  {
    out << "en: ";
    rosidl_generator_traits::value_to_yaml(msg.en, out);
    out << ", ";
  }

  // member: r
  {
    out << "r: ";
    rosidl_generator_traits::value_to_yaml(msg.r, out);
    out << ", ";
  }

  // member: g
  {
    out << "g: ";
    rosidl_generator_traits::value_to_yaml(msg.g, out);
    out << ", ";
  }

  // member: b
  {
    out << "b: ";
    rosidl_generator_traits::value_to_yaml(msg.b, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetRgb_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: en
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "en: ";
    rosidl_generator_traits::value_to_yaml(msg.en, out);
    out << "\n";
  }

  // member: r
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "r: ";
    rosidl_generator_traits::value_to_yaml(msg.r, out);
    out << "\n";
  }

  // member: g
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "g: ";
    rosidl_generator_traits::value_to_yaml(msg.g, out);
    out << "\n";
  }

  // member: b
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "b: ";
    rosidl_generator_traits::value_to_yaml(msg.b, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetRgb_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace robot_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use robot_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const robot_interfaces::srv::SetRgb_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  robot_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use robot_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const robot_interfaces::srv::SetRgb_Request & msg)
{
  return robot_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<robot_interfaces::srv::SetRgb_Request>()
{
  return "robot_interfaces::srv::SetRgb_Request";
}

template<>
inline const char * name<robot_interfaces::srv::SetRgb_Request>()
{
  return "robot_interfaces/srv/SetRgb_Request";
}

template<>
struct has_fixed_size<robot_interfaces::srv::SetRgb_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<robot_interfaces::srv::SetRgb_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<robot_interfaces::srv::SetRgb_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace robot_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const SetRgb_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: res
  {
    out << "res: ";
    rosidl_generator_traits::value_to_yaml(msg.res, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const SetRgb_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: res
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "res: ";
    rosidl_generator_traits::value_to_yaml(msg.res, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const SetRgb_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace robot_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use robot_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const robot_interfaces::srv::SetRgb_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  robot_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use robot_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const robot_interfaces::srv::SetRgb_Response & msg)
{
  return robot_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<robot_interfaces::srv::SetRgb_Response>()
{
  return "robot_interfaces::srv::SetRgb_Response";
}

template<>
inline const char * name<robot_interfaces::srv::SetRgb_Response>()
{
  return "robot_interfaces/srv/SetRgb_Response";
}

template<>
struct has_fixed_size<robot_interfaces::srv::SetRgb_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<robot_interfaces::srv::SetRgb_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<robot_interfaces::srv::SetRgb_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<robot_interfaces::srv::SetRgb>()
{
  return "robot_interfaces::srv::SetRgb";
}

template<>
inline const char * name<robot_interfaces::srv::SetRgb>()
{
  return "robot_interfaces/srv/SetRgb";
}

template<>
struct has_fixed_size<robot_interfaces::srv::SetRgb>
  : std::integral_constant<
    bool,
    has_fixed_size<robot_interfaces::srv::SetRgb_Request>::value &&
    has_fixed_size<robot_interfaces::srv::SetRgb_Response>::value
  >
{
};

template<>
struct has_bounded_size<robot_interfaces::srv::SetRgb>
  : std::integral_constant<
    bool,
    has_bounded_size<robot_interfaces::srv::SetRgb_Request>::value &&
    has_bounded_size<robot_interfaces::srv::SetRgb_Response>::value
  >
{
};

template<>
struct is_service<robot_interfaces::srv::SetRgb>
  : std::true_type
{
};

template<>
struct is_service_request<robot_interfaces::srv::SetRgb_Request>
  : std::true_type
{
};

template<>
struct is_service_response<robot_interfaces::srv::SetRgb_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // ROBOT_INTERFACES__SRV__DETAIL__SET_RGB__TRAITS_HPP_
