// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MonitoringProtocol.proto

#include "MonitoringProtocol.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace Protocol {
PROTOBUF_CONSTEXPR M_REQ_SERVER_INFO::M_REQ_SERVER_INFO(
    ::_pbi::ConstantInitialized) {}
struct M_REQ_SERVER_INFODefaultTypeInternal {
  PROTOBUF_CONSTEXPR M_REQ_SERVER_INFODefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~M_REQ_SERVER_INFODefaultTypeInternal() {}
  union {
    M_REQ_SERVER_INFO _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 M_REQ_SERVER_INFODefaultTypeInternal _M_REQ_SERVER_INFO_default_instance_;
PROTOBUF_CONSTEXPR S_RES_SERVER_INFO::S_RES_SERVER_INFO(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.game_session_num_)*/uint64_t{0u}
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct S_RES_SERVER_INFODefaultTypeInternal {
  PROTOBUF_CONSTEXPR S_RES_SERVER_INFODefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~S_RES_SERVER_INFODefaultTypeInternal() {}
  union {
    S_RES_SERVER_INFO _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 S_RES_SERVER_INFODefaultTypeInternal _S_RES_SERVER_INFO_default_instance_;
}  // namespace Protocol
static ::_pb::Metadata file_level_metadata_MonitoringProtocol_2eproto[2];
static constexpr ::_pb::EnumDescriptor const** file_level_enum_descriptors_MonitoringProtocol_2eproto = nullptr;
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_MonitoringProtocol_2eproto = nullptr;

const uint32_t TableStruct_MonitoringProtocol_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Protocol::M_REQ_SERVER_INFO, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::Protocol::S_RES_SERVER_INFO, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::Protocol::S_RES_SERVER_INFO, _impl_.game_session_num_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::Protocol::M_REQ_SERVER_INFO)},
  { 6, -1, -1, sizeof(::Protocol::S_RES_SERVER_INFO)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::Protocol::_M_REQ_SERVER_INFO_default_instance_._instance,
  &::Protocol::_S_RES_SERVER_INFO_default_instance_._instance,
};

const char descriptor_table_protodef_MonitoringProtocol_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\030MonitoringProtocol.proto\022\010Protocol\"\023\n\021"
  "M_REQ_SERVER_INFO\"-\n\021S_RES_SERVER_INFO\022\030"
  "\n\020game_session_num\030\001 \001(\004b\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_MonitoringProtocol_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_MonitoringProtocol_2eproto = {
    false, false, 112, descriptor_table_protodef_MonitoringProtocol_2eproto,
    "MonitoringProtocol.proto",
    &descriptor_table_MonitoringProtocol_2eproto_once, nullptr, 0, 2,
    schemas, file_default_instances, TableStruct_MonitoringProtocol_2eproto::offsets,
    file_level_metadata_MonitoringProtocol_2eproto, file_level_enum_descriptors_MonitoringProtocol_2eproto,
    file_level_service_descriptors_MonitoringProtocol_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_MonitoringProtocol_2eproto_getter() {
  return &descriptor_table_MonitoringProtocol_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_MonitoringProtocol_2eproto(&descriptor_table_MonitoringProtocol_2eproto);
namespace Protocol {

// ===================================================================

class M_REQ_SERVER_INFO::_Internal {
 public:
};

M_REQ_SERVER_INFO::M_REQ_SERVER_INFO(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase(arena, is_message_owned) {
  // @@protoc_insertion_point(arena_constructor:Protocol.M_REQ_SERVER_INFO)
}
M_REQ_SERVER_INFO::M_REQ_SERVER_INFO(const M_REQ_SERVER_INFO& from)
  : ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase() {
  M_REQ_SERVER_INFO* const _this = this; (void)_this;
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:Protocol.M_REQ_SERVER_INFO)
}





const ::PROTOBUF_NAMESPACE_ID::Message::ClassData M_REQ_SERVER_INFO::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase::CopyImpl,
    ::PROTOBUF_NAMESPACE_ID::internal::ZeroFieldsBase::MergeImpl,
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*M_REQ_SERVER_INFO::GetClassData() const { return &_class_data_; }







::PROTOBUF_NAMESPACE_ID::Metadata M_REQ_SERVER_INFO::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_MonitoringProtocol_2eproto_getter, &descriptor_table_MonitoringProtocol_2eproto_once,
      file_level_metadata_MonitoringProtocol_2eproto[0]);
}

// ===================================================================

class S_RES_SERVER_INFO::_Internal {
 public:
};

S_RES_SERVER_INFO::S_RES_SERVER_INFO(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:Protocol.S_RES_SERVER_INFO)
}
S_RES_SERVER_INFO::S_RES_SERVER_INFO(const S_RES_SERVER_INFO& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  S_RES_SERVER_INFO* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.game_session_num_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _this->_impl_.game_session_num_ = from._impl_.game_session_num_;
  // @@protoc_insertion_point(copy_constructor:Protocol.S_RES_SERVER_INFO)
}

inline void S_RES_SERVER_INFO::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.game_session_num_){uint64_t{0u}}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

S_RES_SERVER_INFO::~S_RES_SERVER_INFO() {
  // @@protoc_insertion_point(destructor:Protocol.S_RES_SERVER_INFO)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void S_RES_SERVER_INFO::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void S_RES_SERVER_INFO::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void S_RES_SERVER_INFO::Clear() {
// @@protoc_insertion_point(message_clear_start:Protocol.S_RES_SERVER_INFO)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.game_session_num_ = uint64_t{0u};
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* S_RES_SERVER_INFO::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // uint64 game_session_num = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.game_session_num_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* S_RES_SERVER_INFO::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:Protocol.S_RES_SERVER_INFO)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // uint64 game_session_num = 1;
  if (this->_internal_game_session_num() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteUInt64ToArray(1, this->_internal_game_session_num(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Protocol.S_RES_SERVER_INFO)
  return target;
}

size_t S_RES_SERVER_INFO::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:Protocol.S_RES_SERVER_INFO)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // uint64 game_session_num = 1;
  if (this->_internal_game_session_num() != 0) {
    total_size += ::_pbi::WireFormatLite::UInt64SizePlusOne(this->_internal_game_session_num());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData S_RES_SERVER_INFO::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    S_RES_SERVER_INFO::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*S_RES_SERVER_INFO::GetClassData() const { return &_class_data_; }


void S_RES_SERVER_INFO::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<S_RES_SERVER_INFO*>(&to_msg);
  auto& from = static_cast<const S_RES_SERVER_INFO&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:Protocol.S_RES_SERVER_INFO)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_game_session_num() != 0) {
    _this->_internal_set_game_session_num(from._internal_game_session_num());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void S_RES_SERVER_INFO::CopyFrom(const S_RES_SERVER_INFO& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Protocol.S_RES_SERVER_INFO)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool S_RES_SERVER_INFO::IsInitialized() const {
  return true;
}

void S_RES_SERVER_INFO::InternalSwap(S_RES_SERVER_INFO* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_.game_session_num_, other->_impl_.game_session_num_);
}

::PROTOBUF_NAMESPACE_ID::Metadata S_RES_SERVER_INFO::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_MonitoringProtocol_2eproto_getter, &descriptor_table_MonitoringProtocol_2eproto_once,
      file_level_metadata_MonitoringProtocol_2eproto[1]);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace Protocol
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::Protocol::M_REQ_SERVER_INFO*
Arena::CreateMaybeMessage< ::Protocol::M_REQ_SERVER_INFO >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Protocol::M_REQ_SERVER_INFO >(arena);
}
template<> PROTOBUF_NOINLINE ::Protocol::S_RES_SERVER_INFO*
Arena::CreateMaybeMessage< ::Protocol::S_RES_SERVER_INFO >(Arena* arena) {
  return Arena::CreateMessageInternal< ::Protocol::S_RES_SERVER_INFO >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
