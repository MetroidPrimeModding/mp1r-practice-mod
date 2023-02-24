#pragma once

namespace CLZSSCompressor {
  bool CompressBuffer8(
      unsigned char const *src,
      unsigned int src_size,
      unsigned int out_buff_size,
      unsigned char *out_buff,
      unsigned int &compressed_size);
  bool CompressBuffer8_3Byte(
      unsigned char const *src,
      unsigned int src_size,
      unsigned int out_buff_size,
      unsigned char *out_buff,
      unsigned int &compressed_size);
  bool CompressBuffer16(
      unsigned char const *src,
      unsigned int src_size,
      unsigned int out_buff_size,
      unsigned char *out_buff,
      unsigned int &compressed_size);
  bool CompressBuffer16_3Byte(
      unsigned char const *src,
      unsigned int src_size,
      unsigned int out_buff_size,
      unsigned char *out_buff,
      unsigned int &compressed_size);
  bool CompressBuffer32(
      unsigned char const *src,
      unsigned int src_size,
      unsigned int out_buff_size,
      unsigned char *out_buff,
      unsigned int &compressed_size);
  bool CompressBuffer32_3Byte(
      unsigned char const *src,
      unsigned int src_size,
      unsigned int out_buff_size,
      unsigned char *out_buff,
      unsigned int &compressed_size);
}