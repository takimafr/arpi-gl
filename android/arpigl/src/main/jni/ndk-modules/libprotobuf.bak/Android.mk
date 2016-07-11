# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#

LOCAL_PATH := $(call my-dir)

IGNORED_WARNINGS := -Wno-sign-compare -Wno-unused-parameter -Wno-sign-promo -Wno-error=return-type

CC_LITE_SRC_FILES := \
    src/google/protobuf/stubs/atomicops_internals_x86_gcc.cc         \
    src/google/protobuf/stubs/atomicops_internals_x86_msvc.cc        \
    src/google/protobuf/stubs/common.cc                              \
    src/google/protobuf/stubs/once.cc                                \
    src/google/protobuf/stubs/stringprintf.cc                        \
    src/google/protobuf/extension_set.cc                             \
    src/google/protobuf/generated_message_util.cc                    \
    src/google/protobuf/message_lite.cc                              \
    src/google/protobuf/repeated_field.cc                            \
    src/google/protobuf/wire_format_lite.cc                          \
    src/google/protobuf/io/coded_stream.cc                           \
    src/google/protobuf/io/zero_copy_stream.cc                       \
    src/google/protobuf/io/zero_copy_stream_impl_lite.cc

# JAVA_LITE_SRC_FILES := \
#     java/src/main/java/com/google/protobuf/UninitializedMessageException.java \
#     java/src/main/java/com/google/protobuf/MessageLite.java \
#     java/src/main/java/com/google/protobuf/InvalidProtocolBufferException.java \
#     java/src/main/java/com/google/protobuf/CodedOutputStream.java \
#     java/src/main/java/com/google/protobuf/ByteString.java \
#     java/src/main/java/com/google/protobuf/CodedInputStream.java \
#     java/src/main/java/com/google/protobuf/ExtensionRegistryLite.java \
#     java/src/main/java/com/google/protobuf/AbstractMessageLite.java \
#     java/src/main/java/com/google/protobuf/AbstractParser.java \
#     java/src/main/java/com/google/protobuf/FieldSet.java \
#     java/src/main/java/com/google/protobuf/Internal.java \
#     java/src/main/java/com/google/protobuf/WireFormat.java \
#     java/src/main/java/com/google/protobuf/GeneratedMessageLite.java \
#     java/src/main/java/com/google/protobuf/BoundedByteString.java \
#     java/src/main/java/com/google/protobuf/LazyField.java \
#     java/src/main/java/com/google/protobuf/LazyFieldLite.java \
#     java/src/main/java/com/google/protobuf/LazyStringList.java \
#     java/src/main/java/com/google/protobuf/LazyStringArrayList.java \
#     java/src/main/java/com/google/protobuf/UnmodifiableLazyStringList.java \
#     java/src/main/java/com/google/protobuf/LiteralByteString.java \
#     java/src/main/java/com/google/protobuf/MessageLiteOrBuilder.java \
#     java/src/main/java/com/google/protobuf/Parser.java \
#     java/src/main/java/com/google/protobuf/ProtocolStringList.java \
#     java/src/main/java/com/google/protobuf/RopeByteString.java \
#     java/src/main/java/com/google/protobuf/SmallSortedMap.java \
#     java/src/main/java/com/google/protobuf/Utf8.java
#
# # This contains more source files than needed for the full version, but the
# # additional files should not create any conflict.
# JAVA_FULL_SRC_FILES := \
#     $(call all-java-files-under, java/src/main/java) \
#     src/google/protobuf/descriptor.proto

COMPILER_SRC_FILES :=  \
    src/google/protobuf/descriptor.cc \
    src/google/protobuf/descriptor.pb.cc \
    src/google/protobuf/descriptor_database.cc \
    src/google/protobuf/dynamic_message.cc \
    src/google/protobuf/extension_set.cc \
    src/google/protobuf/extension_set_heavy.cc \
    src/google/protobuf/generated_message_reflection.cc \
    src/google/protobuf/generated_message_util.cc \
    src/google/protobuf/message.cc \
    src/google/protobuf/message_lite.cc \
    src/google/protobuf/reflection_ops.cc \
    src/google/protobuf/repeated_field.cc \
    src/google/protobuf/service.cc \
    src/google/protobuf/text_format.cc \
    src/google/protobuf/unknown_field_set.cc \
    src/google/protobuf/wire_format.cc \
    src/google/protobuf/wire_format_lite.cc \
    src/google/protobuf/compiler/code_generator.cc \
    src/google/protobuf/compiler/command_line_interface.cc \
    src/google/protobuf/compiler/importer.cc \
    src/google/protobuf/compiler/main.cc \
    src/google/protobuf/compiler/parser.cc \
    src/google/protobuf/compiler/plugin.cc \
    src/google/protobuf/compiler/plugin.pb.cc \
    src/google/protobuf/compiler/subprocess.cc \
    src/google/protobuf/compiler/zip_writer.cc \
    src/google/protobuf/compiler/cpp/cpp_enum.cc \
    src/google/protobuf/compiler/cpp/cpp_enum_field.cc \
    src/google/protobuf/compiler/cpp/cpp_extension.cc \
    src/google/protobuf/compiler/cpp/cpp_field.cc \
    src/google/protobuf/compiler/cpp/cpp_file.cc \
    src/google/protobuf/compiler/cpp/cpp_generator.cc \
    src/google/protobuf/compiler/cpp/cpp_helpers.cc \
    src/google/protobuf/compiler/cpp/cpp_message.cc \
    src/google/protobuf/compiler/cpp/cpp_message_field.cc \
    src/google/protobuf/compiler/cpp/cpp_primitive_field.cc \
    src/google/protobuf/compiler/cpp/cpp_service.cc \
    src/google/protobuf/compiler/cpp/cpp_string_field.cc \
    src/google/protobuf/compiler/java/java_context.cc \
    src/google/protobuf/compiler/java/java_enum.cc \
    src/google/protobuf/compiler/java/java_enum_field.cc \
    src/google/protobuf/compiler/java/java_extension.cc \
    src/google/protobuf/compiler/java/java_field.cc \
    src/google/protobuf/compiler/java/java_file.cc \
    src/google/protobuf/compiler/java/java_generator.cc \
    src/google/protobuf/compiler/java/java_generator_factory.cc \
    src/google/protobuf/compiler/java/java_helpers.cc \
    src/google/protobuf/compiler/java/java_lazy_message_field.cc \
    src/google/protobuf/compiler/java/java_message.cc \
    src/google/protobuf/compiler/java/java_message_field.cc \
    src/google/protobuf/compiler/java/java_name_resolver.cc \
    src/google/protobuf/compiler/java/java_primitive_field.cc \
    src/google/protobuf/compiler/java/java_shared_code_generator.cc \
    src/google/protobuf/compiler/java/java_service.cc \
    src/google/protobuf/compiler/java/java_string_field.cc \
    src/google/protobuf/compiler/java/java_doc_comment.cc \
    src/google/protobuf/compiler/javamicro/javamicro_enum.cc \
    src/google/protobuf/compiler/javamicro/javamicro_enum_field.cc \
    src/google/protobuf/compiler/javamicro/javamicro_field.cc \
    src/google/protobuf/compiler/javamicro/javamicro_file.cc \
    src/google/protobuf/compiler/javamicro/javamicro_generator.cc \
    src/google/protobuf/compiler/javamicro/javamicro_helpers.cc \
    src/google/protobuf/compiler/javamicro/javamicro_message.cc \
    src/google/protobuf/compiler/javamicro/javamicro_message_field.cc \
    src/google/protobuf/compiler/javamicro/javamicro_primitive_field.cc \
    src/google/protobuf/compiler/javanano/javanano_enum.cc \
    src/google/protobuf/compiler/javanano/javanano_enum_field.cc \
    src/google/protobuf/compiler/javanano/javanano_extension.cc \
    src/google/protobuf/compiler/javanano/javanano_field.cc \
    src/google/protobuf/compiler/javanano/javanano_file.cc \
    src/google/protobuf/compiler/javanano/javanano_generator.cc \
    src/google/protobuf/compiler/javanano/javanano_helpers.cc \
    src/google/protobuf/compiler/javanano/javanano_message.cc \
    src/google/protobuf/compiler/javanano/javanano_message_field.cc \
    src/google/protobuf/compiler/javanano/javanano_primitive_field.cc \
    src/google/protobuf/compiler/python/python_generator.cc \
    src/google/protobuf/io/coded_stream.cc \
    src/google/protobuf/io/gzip_stream.cc \
    src/google/protobuf/io/printer.cc \
    src/google/protobuf/io/strtod.cc \
    src/google/protobuf/io/tokenizer.cc \
    src/google/protobuf/io/zero_copy_stream.cc \
    src/google/protobuf/io/zero_copy_stream_impl.cc \
    src/google/protobuf/io/zero_copy_stream_impl_lite.cc \
    src/google/protobuf/stubs/atomicops_internals_x86_gcc.cc \
    src/google/protobuf/stubs/atomicops_internals_x86_msvc.cc \
    src/google/protobuf/stubs/common.cc \
    src/google/protobuf/stubs/once.cc \
    src/google/protobuf/stubs/structurally_valid.cc \
    src/google/protobuf/stubs/strutil.cc \
    src/google/protobuf/stubs/substitute.cc \
    src/google/protobuf/stubs/stringprintf.cc


# # C++ full library
# # =======================================================
protobuf_cc_full_src_files := \
    $(CC_LITE_SRC_FILES)                                             \
    src/google/protobuf/stubs/strutil.cc                             \
    src/google/protobuf/stubs/substitute.cc                          \
    src/google/protobuf/stubs/structurally_valid.cc                  \
    src/google/protobuf/descriptor.cc                                \
    src/google/protobuf/descriptor.pb.cc                             \
    src/google/protobuf/descriptor_database.cc                       \
    src/google/protobuf/dynamic_message.cc                           \
    src/google/protobuf/extension_set_heavy.cc                       \
    src/google/protobuf/generated_message_reflection.cc              \
    src/google/protobuf/message.cc                                   \
    src/google/protobuf/reflection_ops.cc                            \
    src/google/protobuf/service.cc                                   \
    src/google/protobuf/text_format.cc                               \
    src/google/protobuf/unknown_field_set.cc                         \
    src/google/protobuf/wire_format.cc                               \
    src/google/protobuf/io/gzip_stream.cc                            \
    src/google/protobuf/io/printer.cc                                \
    src/google/protobuf/io/strtod.cc                                 \
    src/google/protobuf/io/tokenizer.cc                              \
    src/google/protobuf/io/zero_copy_stream_impl.cc                  \
    src/google/protobuf/compiler/importer.cc                         \
    src/google/protobuf/compiler/parser.cc
#
# C++ full library for the NDK.
# =======================================================
include $(CLEAR_VARS)

LOCAL_MODULE := libprotobuf#-cpp-full-ndk
LOCAL_CPP_EXTENSION := .cc
LOCAL_SRC_FILES := $(protobuf_cc_full_src_files)
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/android \
    external/zlib \
    $(LOCAL_PATH)/src

LOCAL_CFLAGS := -DGOOGLE_PROTOBUF_NO_RTTI $(IGNORED_WARNINGS)

# These are the minimum versions and don't need to be updated.
ifeq ($(TARGET_ARCH),arm)
LOCAL_SDK_VERSION := 8
else
# x86/mips support only available from API 9.
LOCAL_SDK_VERSION := 9
endif
LOCAL_NDK_STL_VARIANT := stlport_static

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/src

include $(BUILD_STATIC_LIBRARY)
