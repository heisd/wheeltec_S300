# generated from rosidl_generator_py/resource/_idl.py.em
# with input from robot_interfaces:srv/SetRgb.idl
# generated code does not contain a copyright notice


# Import statements for member types

import builtins  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_SetRgb_Request(type):
    """Metaclass of message 'SetRgb_Request'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('robot_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'robot_interfaces.srv.SetRgb_Request')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__set_rgb__request
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__set_rgb__request
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__set_rgb__request
            cls._TYPE_SUPPORT = module.type_support_msg__srv__set_rgb__request
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__set_rgb__request

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class SetRgb_Request(metaclass=Metaclass_SetRgb_Request):
    """Message class 'SetRgb_Request'."""

    __slots__ = [
        '_en',
        '_r',
        '_g',
        '_b',
    ]

    _fields_and_field_types = {
        'en': 'boolean',
        'r': 'uint8',
        'g': 'uint8',
        'b': 'uint8',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.en = kwargs.get('en', bool())
        self.r = kwargs.get('r', int())
        self.g = kwargs.get('g', int())
        self.b = kwargs.get('b', int())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.en != other.en:
            return False
        if self.r != other.r:
            return False
        if self.g != other.g:
            return False
        if self.b != other.b:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def en(self):
        """Message field 'en'."""
        return self._en

    @en.setter
    def en(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'en' field must be of type 'bool'"
        self._en = value

    @builtins.property
    def r(self):
        """Message field 'r'."""
        return self._r

    @r.setter
    def r(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'r' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'r' field must be an unsigned integer in [0, 255]"
        self._r = value

    @builtins.property
    def g(self):
        """Message field 'g'."""
        return self._g

    @g.setter
    def g(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'g' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'g' field must be an unsigned integer in [0, 255]"
        self._g = value

    @builtins.property
    def b(self):
        """Message field 'b'."""
        return self._b

    @b.setter
    def b(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'b' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'b' field must be an unsigned integer in [0, 255]"
        self._b = value


# Import statements for member types

# already imported above
# import builtins

# already imported above
# import rosidl_parser.definition


class Metaclass_SetRgb_Response(type):
    """Metaclass of message 'SetRgb_Response'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('robot_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'robot_interfaces.srv.SetRgb_Response')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__set_rgb__response
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__set_rgb__response
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__set_rgb__response
            cls._TYPE_SUPPORT = module.type_support_msg__srv__set_rgb__response
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__set_rgb__response

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class SetRgb_Response(metaclass=Metaclass_SetRgb_Response):
    """Message class 'SetRgb_Response'."""

    __slots__ = [
        '_res',
    ]

    _fields_and_field_types = {
        'res': 'string',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.UnboundedString(),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.res = kwargs.get('res', str())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.res != other.res:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def res(self):
        """Message field 'res'."""
        return self._res

    @res.setter
    def res(self, value):
        if __debug__:
            assert \
                isinstance(value, str), \
                "The 'res' field must be of type 'str'"
        self._res = value


class Metaclass_SetRgb(type):
    """Metaclass of service 'SetRgb'."""

    _TYPE_SUPPORT = None

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('robot_interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'robot_interfaces.srv.SetRgb')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._TYPE_SUPPORT = module.type_support_srv__srv__set_rgb

            from robot_interfaces.srv import _set_rgb
            if _set_rgb.Metaclass_SetRgb_Request._TYPE_SUPPORT is None:
                _set_rgb.Metaclass_SetRgb_Request.__import_type_support__()
            if _set_rgb.Metaclass_SetRgb_Response._TYPE_SUPPORT is None:
                _set_rgb.Metaclass_SetRgb_Response.__import_type_support__()


class SetRgb(metaclass=Metaclass_SetRgb):
    from robot_interfaces.srv._set_rgb import SetRgb_Request as Request
    from robot_interfaces.srv._set_rgb import SetRgb_Response as Response

    def __init__(self):
        raise NotImplementedError('Service classes can not be instantiated')
