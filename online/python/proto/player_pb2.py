# Generated by the protocol buffer compiler.  DO NOT EDIT!

from google.protobuf import descriptor
from google.protobuf import message
from google.protobuf import reflection
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)


import data_pb2

DESCRIPTOR = descriptor.FileDescriptor(
  name='player.proto',
  package='onlineproto',
  serialized_pb='\n\x0cplayer.proto\x12\x0bonlineproto\x1a\ndata.proto\"\x19\n\x17\x63s_notify_player_update\"E\n\x17sc_notify_player_update\x12*\n\x06player\x18\x01 \x02(\x0b\x32\x1a.onlineproto.player_data_t')




_CS_NOTIFY_PLAYER_UPDATE = descriptor.Descriptor(
  name='cs_notify_player_update',
  full_name='onlineproto.cs_notify_player_update',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=41,
  serialized_end=66,
)


_SC_NOTIFY_PLAYER_UPDATE = descriptor.Descriptor(
  name='sc_notify_player_update',
  full_name='onlineproto.sc_notify_player_update',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    descriptor.FieldDescriptor(
      name='player', full_name='onlineproto.sc_notify_player_update.player', index=0,
      number=1, type=11, cpp_type=10, label=2,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=68,
  serialized_end=137,
)

_SC_NOTIFY_PLAYER_UPDATE.fields_by_name['player'].message_type = data_pb2._PLAYER_DATA_T
DESCRIPTOR.message_types_by_name['cs_notify_player_update'] = _CS_NOTIFY_PLAYER_UPDATE
DESCRIPTOR.message_types_by_name['sc_notify_player_update'] = _SC_NOTIFY_PLAYER_UPDATE

class cs_notify_player_update(message.Message):
  __metaclass__ = reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _CS_NOTIFY_PLAYER_UPDATE
  
  # @@protoc_insertion_point(class_scope:onlineproto.cs_notify_player_update)

class sc_notify_player_update(message.Message):
  __metaclass__ = reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _SC_NOTIFY_PLAYER_UPDATE
  
  # @@protoc_insertion_point(class_scope:onlineproto.sc_notify_player_update)

# @@protoc_insertion_point(module_scope)
