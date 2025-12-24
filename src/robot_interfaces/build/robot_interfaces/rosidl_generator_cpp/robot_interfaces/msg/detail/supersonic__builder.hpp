// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from robot_interfaces:msg/Supersonic.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_INTERFACES__MSG__DETAIL__SUPERSONIC__BUILDER_HPP_
#define ROBOT_INTERFACES__MSG__DETAIL__SUPERSONIC__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "robot_interfaces/msg/detail/supersonic__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace robot_interfaces
{

namespace msg
{

namespace builder
{

class Init_Supersonic_distance_h
{
public:
  explicit Init_Supersonic_distance_h(::robot_interfaces::msg::Supersonic & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::msg::Supersonic distance_h(::robot_interfaces::msg::Supersonic::_distance_h_type arg)
  {
    msg_.distance_h = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::msg::Supersonic msg_;
};

class Init_Supersonic_distance_g
{
public:
  explicit Init_Supersonic_distance_g(::robot_interfaces::msg::Supersonic & msg)
  : msg_(msg)
  {}
  Init_Supersonic_distance_h distance_g(::robot_interfaces::msg::Supersonic::_distance_g_type arg)
  {
    msg_.distance_g = std::move(arg);
    return Init_Supersonic_distance_h(msg_);
  }

private:
  ::robot_interfaces::msg::Supersonic msg_;
};

class Init_Supersonic_distance_f
{
public:
  explicit Init_Supersonic_distance_f(::robot_interfaces::msg::Supersonic & msg)
  : msg_(msg)
  {}
  Init_Supersonic_distance_g distance_f(::robot_interfaces::msg::Supersonic::_distance_f_type arg)
  {
    msg_.distance_f = std::move(arg);
    return Init_Supersonic_distance_g(msg_);
  }

private:
  ::robot_interfaces::msg::Supersonic msg_;
};

class Init_Supersonic_distance_e
{
public:
  explicit Init_Supersonic_distance_e(::robot_interfaces::msg::Supersonic & msg)
  : msg_(msg)
  {}
  Init_Supersonic_distance_f distance_e(::robot_interfaces::msg::Supersonic::_distance_e_type arg)
  {
    msg_.distance_e = std::move(arg);
    return Init_Supersonic_distance_f(msg_);
  }

private:
  ::robot_interfaces::msg::Supersonic msg_;
};

class Init_Supersonic_distance_d
{
public:
  explicit Init_Supersonic_distance_d(::robot_interfaces::msg::Supersonic & msg)
  : msg_(msg)
  {}
  Init_Supersonic_distance_e distance_d(::robot_interfaces::msg::Supersonic::_distance_d_type arg)
  {
    msg_.distance_d = std::move(arg);
    return Init_Supersonic_distance_e(msg_);
  }

private:
  ::robot_interfaces::msg::Supersonic msg_;
};

class Init_Supersonic_distance_c
{
public:
  explicit Init_Supersonic_distance_c(::robot_interfaces::msg::Supersonic & msg)
  : msg_(msg)
  {}
  Init_Supersonic_distance_d distance_c(::robot_interfaces::msg::Supersonic::_distance_c_type arg)
  {
    msg_.distance_c = std::move(arg);
    return Init_Supersonic_distance_d(msg_);
  }

private:
  ::robot_interfaces::msg::Supersonic msg_;
};

class Init_Supersonic_distance_b
{
public:
  explicit Init_Supersonic_distance_b(::robot_interfaces::msg::Supersonic & msg)
  : msg_(msg)
  {}
  Init_Supersonic_distance_c distance_b(::robot_interfaces::msg::Supersonic::_distance_b_type arg)
  {
    msg_.distance_b = std::move(arg);
    return Init_Supersonic_distance_c(msg_);
  }

private:
  ::robot_interfaces::msg::Supersonic msg_;
};

class Init_Supersonic_distance_a
{
public:
  explicit Init_Supersonic_distance_a(::robot_interfaces::msg::Supersonic & msg)
  : msg_(msg)
  {}
  Init_Supersonic_distance_b distance_a(::robot_interfaces::msg::Supersonic::_distance_a_type arg)
  {
    msg_.distance_a = std::move(arg);
    return Init_Supersonic_distance_b(msg_);
  }

private:
  ::robot_interfaces::msg::Supersonic msg_;
};

class Init_Supersonic_header
{
public:
  Init_Supersonic_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Supersonic_distance_a header(::robot_interfaces::msg::Supersonic::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Supersonic_distance_a(msg_);
  }

private:
  ::robot_interfaces::msg::Supersonic msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::msg::Supersonic>()
{
  return robot_interfaces::msg::builder::Init_Supersonic_header();
}

}  // namespace robot_interfaces

#endif  // ROBOT_INTERFACES__MSG__DETAIL__SUPERSONIC__BUILDER_HPP_
