// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from robot_interfaces:srv/SetRgb.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_INTERFACES__SRV__DETAIL__SET_RGB__BUILDER_HPP_
#define ROBOT_INTERFACES__SRV__DETAIL__SET_RGB__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "robot_interfaces/srv/detail/set_rgb__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace robot_interfaces
{

namespace srv
{

namespace builder
{

class Init_SetRgb_Request_b
{
public:
  explicit Init_SetRgb_Request_b(::robot_interfaces::srv::SetRgb_Request & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::srv::SetRgb_Request b(::robot_interfaces::srv::SetRgb_Request::_b_type arg)
  {
    msg_.b = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::srv::SetRgb_Request msg_;
};

class Init_SetRgb_Request_g
{
public:
  explicit Init_SetRgb_Request_g(::robot_interfaces::srv::SetRgb_Request & msg)
  : msg_(msg)
  {}
  Init_SetRgb_Request_b g(::robot_interfaces::srv::SetRgb_Request::_g_type arg)
  {
    msg_.g = std::move(arg);
    return Init_SetRgb_Request_b(msg_);
  }

private:
  ::robot_interfaces::srv::SetRgb_Request msg_;
};

class Init_SetRgb_Request_r
{
public:
  explicit Init_SetRgb_Request_r(::robot_interfaces::srv::SetRgb_Request & msg)
  : msg_(msg)
  {}
  Init_SetRgb_Request_g r(::robot_interfaces::srv::SetRgb_Request::_r_type arg)
  {
    msg_.r = std::move(arg);
    return Init_SetRgb_Request_g(msg_);
  }

private:
  ::robot_interfaces::srv::SetRgb_Request msg_;
};

class Init_SetRgb_Request_en
{
public:
  Init_SetRgb_Request_en()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SetRgb_Request_r en(::robot_interfaces::srv::SetRgb_Request::_en_type arg)
  {
    msg_.en = std::move(arg);
    return Init_SetRgb_Request_r(msg_);
  }

private:
  ::robot_interfaces::srv::SetRgb_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::srv::SetRgb_Request>()
{
  return robot_interfaces::srv::builder::Init_SetRgb_Request_en();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace srv
{

namespace builder
{

class Init_SetRgb_Response_res
{
public:
  Init_SetRgb_Response_res()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::robot_interfaces::srv::SetRgb_Response res(::robot_interfaces::srv::SetRgb_Response::_res_type arg)
  {
    msg_.res = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::srv::SetRgb_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::srv::SetRgb_Response>()
{
  return robot_interfaces::srv::builder::Init_SetRgb_Response_res();
}

}  // namespace robot_interfaces

#endif  // ROBOT_INTERFACES__SRV__DETAIL__SET_RGB__BUILDER_HPP_
