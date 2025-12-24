// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from robot_interfaces:srv/SetRgb.idl
// generated code does not contain a copyright notice

#ifndef ROBOT_INTERFACES__SRV__DETAIL__SET_RGB__STRUCT_HPP_
#define ROBOT_INTERFACES__SRV__DETAIL__SET_RGB__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__robot_interfaces__srv__SetRgb_Request __attribute__((deprecated))
#else
# define DEPRECATED__robot_interfaces__srv__SetRgb_Request __declspec(deprecated)
#endif

namespace robot_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SetRgb_Request_
{
  using Type = SetRgb_Request_<ContainerAllocator>;

  explicit SetRgb_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->en = false;
      this->r = 0;
      this->g = 0;
      this->b = 0;
    }
  }

  explicit SetRgb_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->en = false;
      this->r = 0;
      this->g = 0;
      this->b = 0;
    }
  }

  // field types and members
  using _en_type =
    bool;
  _en_type en;
  using _r_type =
    uint8_t;
  _r_type r;
  using _g_type =
    uint8_t;
  _g_type g;
  using _b_type =
    uint8_t;
  _b_type b;

  // setters for named parameter idiom
  Type & set__en(
    const bool & _arg)
  {
    this->en = _arg;
    return *this;
  }
  Type & set__r(
    const uint8_t & _arg)
  {
    this->r = _arg;
    return *this;
  }
  Type & set__g(
    const uint8_t & _arg)
  {
    this->g = _arg;
    return *this;
  }
  Type & set__b(
    const uint8_t & _arg)
  {
    this->b = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    robot_interfaces::srv::SetRgb_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const robot_interfaces::srv::SetRgb_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<robot_interfaces::srv::SetRgb_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<robot_interfaces::srv::SetRgb_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      robot_interfaces::srv::SetRgb_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<robot_interfaces::srv::SetRgb_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      robot_interfaces::srv::SetRgb_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<robot_interfaces::srv::SetRgb_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<robot_interfaces::srv::SetRgb_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<robot_interfaces::srv::SetRgb_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__robot_interfaces__srv__SetRgb_Request
    std::shared_ptr<robot_interfaces::srv::SetRgb_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__robot_interfaces__srv__SetRgb_Request
    std::shared_ptr<robot_interfaces::srv::SetRgb_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetRgb_Request_ & other) const
  {
    if (this->en != other.en) {
      return false;
    }
    if (this->r != other.r) {
      return false;
    }
    if (this->g != other.g) {
      return false;
    }
    if (this->b != other.b) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetRgb_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetRgb_Request_

// alias to use template instance with default allocator
using SetRgb_Request =
  robot_interfaces::srv::SetRgb_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace robot_interfaces


#ifndef _WIN32
# define DEPRECATED__robot_interfaces__srv__SetRgb_Response __attribute__((deprecated))
#else
# define DEPRECATED__robot_interfaces__srv__SetRgb_Response __declspec(deprecated)
#endif

namespace robot_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct SetRgb_Response_
{
  using Type = SetRgb_Response_<ContainerAllocator>;

  explicit SetRgb_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->res = "";
    }
  }

  explicit SetRgb_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : res(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->res = "";
    }
  }

  // field types and members
  using _res_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _res_type res;

  // setters for named parameter idiom
  Type & set__res(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->res = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    robot_interfaces::srv::SetRgb_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const robot_interfaces::srv::SetRgb_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<robot_interfaces::srv::SetRgb_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<robot_interfaces::srv::SetRgb_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      robot_interfaces::srv::SetRgb_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<robot_interfaces::srv::SetRgb_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      robot_interfaces::srv::SetRgb_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<robot_interfaces::srv::SetRgb_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<robot_interfaces::srv::SetRgb_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<robot_interfaces::srv::SetRgb_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__robot_interfaces__srv__SetRgb_Response
    std::shared_ptr<robot_interfaces::srv::SetRgb_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__robot_interfaces__srv__SetRgb_Response
    std::shared_ptr<robot_interfaces::srv::SetRgb_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const SetRgb_Response_ & other) const
  {
    if (this->res != other.res) {
      return false;
    }
    return true;
  }
  bool operator!=(const SetRgb_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct SetRgb_Response_

// alias to use template instance with default allocator
using SetRgb_Response =
  robot_interfaces::srv::SetRgb_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace robot_interfaces

namespace robot_interfaces
{

namespace srv
{

struct SetRgb
{
  using Request = robot_interfaces::srv::SetRgb_Request;
  using Response = robot_interfaces::srv::SetRgb_Response;
};

}  // namespace srv

}  // namespace robot_interfaces

#endif  // ROBOT_INTERFACES__SRV__DETAIL__SET_RGB__STRUCT_HPP_
