#ifndef CERT_H_
#define CERT_H_
unsigned char example_crt_DER[] = {
  0x30, 0x82, 0x02, 0x18, 0x30, 0x82, 0x01, 0x81, 0x02, 0x01, 0x02, 0x30,
  0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b,
  0x05, 0x00, 0x30, 0x54, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
  0x06, 0x13, 0x02, 0x44, 0x45, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55,
  0x04, 0x08, 0x0c, 0x02, 0x42, 0x45, 0x31, 0x0f, 0x30, 0x0d, 0x06, 0x03,
  0x55, 0x04, 0x07, 0x0c, 0x06, 0x42, 0x65, 0x72, 0x6c, 0x69, 0x6e, 0x31,
  0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x09, 0x4d, 0x79,
  0x43, 0x6f, 0x6d, 0x70, 0x61, 0x6e, 0x79, 0x31, 0x13, 0x30, 0x11, 0x06,
  0x03, 0x55, 0x04, 0x03, 0x0c, 0x0a, 0x6d, 0x79, 0x63, 0x61, 0x2e, 0x6c,
  0x6f, 0x63, 0x61, 0x6c, 0x30, 0x1e, 0x17, 0x0d, 0x32, 0x32, 0x30, 0x31,
  0x31, 0x30, 0x31, 0x36, 0x34, 0x33, 0x35, 0x34, 0x5a, 0x17, 0x0d, 0x33,
  0x32, 0x30, 0x31, 0x30, 0x38, 0x31, 0x36, 0x34, 0x33, 0x35, 0x34, 0x5a,
  0x30, 0x55, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
  0x02, 0x44, 0x45, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x08,
  0x0c, 0x02, 0x42, 0x45, 0x31, 0x0f, 0x30, 0x0d, 0x06, 0x03, 0x55, 0x04,
  0x07, 0x0c, 0x06, 0x42, 0x65, 0x72, 0x6c, 0x69, 0x6e, 0x31, 0x12, 0x30,
  0x10, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x09, 0x4d, 0x79, 0x43, 0x6f,
  0x6d, 0x70, 0x61, 0x6e, 0x79, 0x31, 0x14, 0x30, 0x12, 0x06, 0x03, 0x55,
  0x04, 0x03, 0x0c, 0x0b, 0x65, 0x73, 0x70, 0x33, 0x32, 0x2e, 0x6c, 0x6f,
  0x63, 0x61, 0x6c, 0x30, 0x81, 0x9f, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86,
  0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x81, 0x8d,
  0x00, 0x30, 0x81, 0x89, 0x02, 0x81, 0x81, 0x00, 0xa1, 0xfb, 0xf2, 0xe2,
  0xe9, 0x96, 0xa8, 0x89, 0x78, 0x70, 0x28, 0xe6, 0xb9, 0x2d, 0xf4, 0x57,
  0x0c, 0xf1, 0x43, 0x80, 0x12, 0xd0, 0x89, 0xc3, 0x41, 0x5b, 0xe2, 0xff,
  0xa2, 0xaa, 0x26, 0xd9, 0x5b, 0xd0, 0x4c, 0xd7, 0x20, 0x7e, 0x14, 0x16,
  0x5f, 0xb3, 0xb3, 0x7c, 0x27, 0x8b, 0x87, 0x4f, 0x01, 0x88, 0xc2, 0x9d,
  0x01, 0x47, 0x0e, 0x9a, 0x01, 0x8f, 0x6f, 0xb0, 0xb6, 0xfc, 0x70, 0x1b,
  0x59, 0x3c, 0x81, 0x3e, 0xca, 0xc7, 0x27, 0x9b, 0x67, 0x42, 0x71, 0x7a,
  0xfe, 0xa0, 0xf3, 0x3d, 0x35, 0x57, 0x83, 0xfb, 0x8a, 0x7d, 0x71, 0x93,
  0x6d, 0x3f, 0x0b, 0x03, 0x1c, 0x99, 0x9d, 0xa3, 0x8c, 0x1d, 0x73, 0xf4,
  0xf0, 0x61, 0xaa, 0xa5, 0x26, 0x8e, 0xfb, 0xb7, 0xc5, 0x03, 0x45, 0xad,
  0x6d, 0xa9, 0xb2, 0x6a, 0xfc, 0x46, 0xbc, 0xf6, 0x65, 0xff, 0xf4, 0xb7,
  0xdf, 0xbd, 0xb2, 0x91, 0x02, 0x03, 0x01, 0x00, 0x01, 0x30, 0x0d, 0x06,
  0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00,
  0x03, 0x81, 0x81, 0x00, 0xa6, 0xb4, 0xe6, 0xe9, 0x0c, 0x02, 0x31, 0x07,
  0x3e, 0x6c, 0x0f, 0x7e, 0xad, 0xd2, 0x34, 0x8e, 0xb3, 0xe8, 0x16, 0xd6,
  0x82, 0x52, 0xc7, 0xde, 0x47, 0xf5, 0x92, 0xea, 0x85, 0xc8, 0x1a, 0x40,
  0x9d, 0x68, 0x3c, 0x16, 0x3a, 0xe7, 0x77, 0xca, 0x17, 0x44, 0xe5, 0x1c,
  0xec, 0x72, 0xf6, 0xc7, 0xfb, 0x7a, 0x85, 0x3e, 0x9d, 0x24, 0x16, 0x13,
  0xa9, 0x30, 0x84, 0xb2, 0xfe, 0xf6, 0x68, 0xa7, 0xb3, 0xe3, 0xf0, 0x11,
  0x5e, 0x2f, 0x94, 0x0c, 0x01, 0x67, 0xdd, 0xee, 0x43, 0x49, 0xe1, 0x57,
  0xec, 0x86, 0x25, 0x01, 0xe9, 0x71, 0x02, 0xc3, 0x1f, 0xf7, 0xe2, 0x95,
  0x03, 0x4d, 0x9a, 0x5b, 0xe2, 0xa8, 0xd5, 0x4d, 0xdf, 0xea, 0x7d, 0x0b,
  0xbe, 0x2f, 0x37, 0x0b, 0xac, 0x65, 0x77, 0xaf, 0x32, 0x50, 0x8b, 0x74,
  0x36, 0x38, 0x5b, 0xd9, 0xb8, 0x9a, 0x28, 0xf9, 0x62, 0x26, 0xd6, 0x67
};
unsigned int example_crt_DER_len = 540;
#endif