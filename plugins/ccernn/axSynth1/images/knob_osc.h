const unsigned knob_osc_size = 3767;

const unsigned char knob_osc[] = 
{
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52, 
  0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x01, 0x00, 0x08, 0x06, 0x00, 0x00, 0x00, 0xd2, 0xa1, 0x74, 
  0x76, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47, 0x42, 0x00, 0xae, 0xce, 0x1c, 0xe9, 0x00, 0x00, 
  0x00, 0x04, 0x67, 0x41, 0x4d, 0x41, 0x00, 0x00, 0xb1, 0x8f, 0x0b, 0xfc, 0x61, 0x05, 0x00, 0x00, 
  0x00, 0x20, 0x63, 0x48, 0x52, 0x4d, 0x00, 0x00, 0x7a, 0x26, 0x00, 0x00, 0x80, 0x84, 0x00, 0x00, 
  0xfa, 0x00, 0x00, 0x00, 0x80, 0xe8, 0x00, 0x00, 0x75, 0x30, 0x00, 0x00, 0xea, 0x60, 0x00, 0x00, 
  0x3a, 0x98, 0x00, 0x00, 0x17, 0x70, 0x9c, 0xba, 0x51, 0x3c, 0x00, 0x00, 0x0e, 0x35, 0x49, 0x44, 
  0x41, 0x54, 0x78, 0x5e, 0xed, 0x9d, 0x47, 0x8c, 0x15, 0x49, 0x12, 0x86, 0x9b, 0x3b, 0xa7, 0x3d, 
  0x21, 0x21, 0xcd, 0x8c, 0xb4, 0xa7, 0x11, 0xd2, 0x8c, 0xb4, 0x27, 0x86, 0xcb, 0xde, 0x57, 0x2d, 
  0xe6, 0x04, 0x12, 0x37, 0xbc, 0xf7, 0xde, 0xdb, 0x69, 0xbc, 0x17, 0x8d, 0xf7, 0xde, 0x7b, 0xef, 
  0x04, 0x83, 0xa0, 0x41, 0x18, 0x01, 0x8d, 0x47, 0xb8, 0x03, 0x87, 0x41, 0x9a, 0xd5, 0xee, 0x80, 
  0xe6, 0x32, 0xec, 0xd4, 0xc6, 0x97, 0xf3, 0xb2, 0xc9, 0x97, 0x2f, 0xab, 0x2a, 0xcb, 0xbc, 0xd7, 
  0x48, 0x54, 0x4b, 0xa9, 0xee, 0x7e, 0x2f, 0xab, 0xf2, 0xcf, 0x88, 0xac, 0xcc, 0x88, 0x3f, 0x22, 
  0xb3, 0xda, 0xd4, 0x79, 0xfe, 0xb4, 0x6f, 0xdf, 0xbe, 0x6d, 0xf7, 0xee, 0xdd, 0xff, 0xd5, 0xa1, 
  0x43, 0x87, 0x7f, 0xca, 0x25, 0xff, 0x90, 0xf2, 0x77, 0x29, 0x7f, 0x2b, 0x5d, 0xfe, 0x6f, 0xf9, 
  0xfd, 0x42, 0xca, 0x9d, 0xb7, 0x6f, 0xdf, 0x5e, 0xbe, 0x73, 0xe7, 0xce, 0xc9, 0x5d, 0xbb, 0x76, 
  0x7d, 0xf0, 0xbc, 0x75, 0x74, 0xb5, 0x4e, 0x9d, 0x3a, 0x7d, 0xbb, 0x73, 0xe7, 0xce, 0x46, 0x29, 
  0x1f, 0xa5, 0x04, 0x9e, 0x85, 0xba, 0x8d, 0x13, 0x27, 0x4e, 0xfc, 0x36, 0x13, 0x08, 0xb9, 0x49, 
  0x83, 0x67, 0x83, 0x51, 0xc0, 0x1a, 0x12, 0x83, 0xe8, 0xd8, 0xb1, 0x23, 0xbd, 0x6e, 0xca, 0xa1, 
  0x71, 0x0d, 0xac, 0x69, 0xf1, 0xe2, 0xc5, 0x7e, 0xd2, 0xa8, 0xaf, 0xaf, 0xef, 0x28, 0x0d, 0xbf, 
  0x73, 0x35, 0xbe, 0x63, 0xc7, 0x8e, 0x60, 0xd5, 0xaa, 0x55, 0xc1, 0xd2, 0xa5, 0x4b, 0x83, 0x39, 
  0x73, 0xe6, 0x04, 0x33, 0x67, 0xce, 0x54, 0x85, 0xbf, 0xf9, 0x8c, 0xef, 0xa8, 0x13, 0x02, 0xfc, 
  0xdd, 0x9a, 0x35, 0x6b, 0x3a, 0x46, 0x4a, 0xa3, 0xd4, 0x73, 0x67, 0xe3, 0x8d, 0x8d, 0x8d, 0xc1, 
  0x98, 0x31, 0x63, 0x02, 0x19, 0x88, 0x41, 0xcf, 0x9e, 0x3d, 0x83, 0x1e, 0x3d, 0x7a, 0xa8, 0xbf, 
  0x29, 0xfc, 0xad, 0x3f, 0xa3, 0x0e, 0x40, 0xc2, 0x40, 0x6c, 0xdc, 0xb8, 0x31, 0x5c, 0x12, 0x2e, 
  0xb1, 0x6f, 0xd8, 0xb0, 0x41, 0xf5, 0xb0, 0x57, 0xaf, 0x5e, 0x2d, 0x0d, 0xea, 0x86, 0xc3, 0x7e, 
  0x53, 0x97, 0x6b, 0x36, 0x6f, 0xde, 0xec, 0x02, 0xd2, 0xe4, 0x94, 0x82, 0x6b, 0xc0, 0xad, 0x5e, 
  0xbd, 0x3a, 0xe8, 0xd7, 0xaf, 0x9f, 0xea, 0x5d, 0x5c, 0xa3, 0xf6, 0xf7, 0x5c, 0x33, 0x6c, 0xd8, 
  0xb0, 0x80, 0x0e, 0x38, 0xa4, 0x51, 0x3e, 0x30, 0x4b, 0x8f, 0x5a, 0x59, 0x45, 0x2e, 0xa4, 0xf1, 
  0xa4, 0x0d, 0xdb, 0xf5, 0x47, 0x8e, 0x1c, 0x19, 0x6c, 0xd9, 0xb2, 0xa5, 0x02, 0xc4, 0xf1, 0xe3, 
  0xc7, 0x3f, 0xa9, 0xa2, 0xf4, 0x9c, 0x97, 0x55, 0x42, 0x84, 0x69, 0x7a, 0xee, 0x92, 0x04, 0xf7, 
  0x72, 0x48, 0xa1, 0x51, 0xa9, 0x82, 0x19, 0xce, 0x9e, 0x64, 0x18, 0x70, 0x49, 0x74, 0x1e, 0x27, 
  0x25, 0xee, 0x25, 0x83, 0xcf, 0x06, 0xf1, 0xf1, 0xc6, 0x8d, 0x1b, 0x6d, 0xeb, 0x26, 0x4d, 0x9a, 
  0xd4, 0xd5, 0x44, 0xc7, 0x63, 0x34, 0x7e, 0xfc, 0xf8, 0xcc, 0xa2, 0xb7, 0x41, 0xc9, 0xac, 0x58, 
  0xf1, 0x88, 0x8a, 0x1a, 0xba, 0xd6, 0xd9, 0xe2, 0xe7, 0x11, 0x8a, 0xeb, 0x51, 0x9a, 0xef, 0xfb, 
  0xf4, 0xe9, 0x13, 0x6c, 0xda, 0xb4, 0xc9, 0x96, 0x42, 0x23, 0x00, 0xca, 0x66, 0x3c, 0x26, 0x94, 
  0x3c, 0x74, 0xef, 0x1a, 0x0b, 0xdc, 0xdb, 0x1a, 0x0b, 0x4d, 0x00, 0x28, 0x9b, 0x78, 0x18, 0x30, 
  0xe6, 0x24, 0x93, 0xa6, 0xb7, 0xae, 0x6b, 0xb8, 0xa7, 0x63, 0x30, 0xbe, 0x03, 0x40, 0xd9, 0x2a, 
  0xc7, 0xd4, 0x9a, 0x57, 0xa3, 0xf6, 0x7d, 0xb8, 0xb7, 0x25, 0x81, 0x8f, 0x9f, 0x05, 0x00, 0xa5, 
  0x82, 0x03, 0x07, 0x0e, 0x04, 0xa7, 0x4f, 0x9f, 0x0e, 0x56, 0xae, 0x5c, 0x59, 0x5b, 0x15, 0xdc, 
  0xba, 0x75, 0xeb, 0x97, 0x17, 0x2f, 0x5e, 0x04, 0x6f, 0xde, 0xbc, 0x51, 0xe5, 0xcc, 0x99, 0x33, 
  0x41, 0xef, 0xde, 0xbd, 0x73, 0x57, 0x03, 0x03, 0xdb, 0x39, 0x08, 0x9f, 0x3c, 0x79, 0xd2, 0xac, 
  0x1b, 0xe7, 0xf7, 0xcd, 0x9b, 0x37, 0x03, 0x1e, 0x99, 0xbc, 0xc7, 0x41, 0xff, 0xfe, 0xfd, 0x83, 
  0x6d, 0xdb, 0xb6, 0x55, 0x3e, 0x86, 0x62, 0xbf, 0x2d, 0x31, 0x01, 0xbc, 0x7e, 0xfd, 0x3a, 0x10, 
  0xe3, 0x21, 0x77, 0x00, 0x32, 0xe1, 0x55, 0x4c, 0xc7, 0x97, 0x2e, 0x5d, 0xea, 0x5a, 0xb7, 0x7b, 
  0xf7, 0xee, 0x76, 0x34, 0x6a, 0x4b, 0xa1, 0x6f, 0xdf, 0xbe, 0xb9, 0x81, 0x60, 0x2a, 0x76, 0xac, 
  0x05, 0x1f, 0x3f, 0x7c, 0xf8, 0xd0, 0x56, 0xad, 0x07, 0xb6, 0x1a, 0x00, 0x23, 0xc0, 0x72, 0x51, 
  0x05, 0xba, 0x8f, 0x5c, 0x8c, 0x00, 0x70, 0xee, 0xdc, 0xb9, 0x7a, 0x53, 0x02, 0xfc, 0xfd, 0xf8, 
  0xf1, 0xe3, 0x60, 0xc6, 0x8c, 0x19, 0x99, 0xa5, 0xf0, 0xd3, 0x4f, 0x3f, 0x39, 0xad, 0xa3, 0x57, 
  0xaf, 0x5e, 0x95, 0x5b, 0x46, 0x77, 0xef, 0xde, 0xbd, 0x6a, 0x83, 0xb8, 0x77, 0xef, 0x5e, 0x30, 
  0x6b, 0xd6, 0x2c, 0x56, 0xc6, 0xff, 0x25, 0x1d, 0x94, 0x32, 0xf3, 0xfd, 0x41, 0xe3, 0x21, 0x36, 
  0xa2, 0xdb, 0x52, 0x7e, 0xf8, 0xf0, 0xe1, 0x7b, 0x1b, 0xc4, 0xb5, 0x6b, 0xd7, 0xde, 0x48, 0xe3, 
  0xc3, 0xa5, 0xbc, 0x4f, 0x00, 0x82, 0xba, 0xc3, 0x45, 0xef, 0xbb, 0x1d, 0xba, 0x77, 0x9b, 0x64, 
  0xa8, 0xe2, 0xd8, 0xb1, 0x63, 0xf5, 0xcf, 0x9f, 0x3f, 0xff, 0x53, 0x83, 0x10, 0x40, 0xff, 0xd5, 
  0xf6, 0xdb, 0x90, 0x21, 0x43, 0xbe, 0x11, 0x7d, 0x6e, 0x96, 0x9e, 0xfd, 0x2a, 0x37, 0xff, 0x5d, 
  0xca, 0x1f, 0x52, 0xfe, 0x2c, 0x15, 0xfe, 0xfe, 0x5d, 0xe6, 0x8f, 0x5f, 0xe5, 0x71, 0xdb, 0xbc, 
  0x7f, 0xff, 0xfe, 0x6f, 0xf4, 0x75, 0x02, 0xe0, 0x67, 0x03, 0xc4, 0xbb, 0x0a, 0xd1, 0xdb, 0x06, 
  0xa2, 0xac, 0xd1, 0xbd, 0x01, 0xf1, 0xec, 0xd9, 0xb3, 0x8f, 0x57, 0xae, 0x5c, 0xf9, 0xc1, 0xfc, 
  0x5e, 0xc6, 0x44, 0x9b, 0xd1, 0xa3, 0x47, 0x7f, 0x2d, 0x8d, 0xfd, 0x58, 0x92, 0xca, 0x2c, 0xf9, 
  0x4d, 0x41, 0x42, 0x3f, 0xae, 0x58, 0xb1, 0xe2, 0xeb, 0xcb, 0x97, 0x2f, 0xb7, 0x31, 0xaf, 0x91, 
  0xc6, 0xbf, 0x92, 0xf2, 0x86, 0x45, 0x4f, 0x3a, 0x14, 0x6d, 0x96, 0xeb, 0x0b, 0x0f, 0x1e, 0x3c, 
  0x58, 0x2f, 0x8d, 0x77, 0x4d, 0xec, 0xd1, 0x84, 0x5c, 0x20, 0x8d, 0x7f, 0x1f, 0xdb, 0xf3, 0xbc, 
  0x1a, 0x2b, 0xee, 0x53, 0x35, 0x09, 0x60, 0x3d, 0x97, 0x0c, 0x58, 0x5c, 0x75, 0xcc, 0x38, 0x96, 
  0x71, 0x8c, 0x19, 0x0a, 0x7f, 0xf3, 0x59, 0xe3, 0x82, 0x05, 0x0b, 0xba, 0x76, 0xeb, 0xd6, 0xed, 
  0xaf, 0x29, 0x36, 0x8f, 0x9f, 0x82, 0x1f, 0x08, 0xf1, 0x72, 0x7d, 0x99, 0x92, 0x82, 0x1f, 0xb0, 
  0x25, 0xf5, 0xae, 0xe0, 0x07, 0x4a, 0x63, 0xaa, 0x7c, 0x39, 0xee, 0xdc, 0xb9, 0xf3, 0x77, 0xf6, 
  0x60, 0xab, 0x29, 0x3f, 0x80, 0x49, 0x86, 0x35, 0x7c, 0xf4, 0xe8, 0xd1, 0x16, 0x9d, 0xd5, 0x8c, 
  0x1f, 0x90, 0x99, 0xab, 0xcc, 0x28, 0x15, 0xcb, 0x28, 0xd8, 0xbe, 0x7d, 0x7b, 0xed, 0xf8, 0x81, 
  0xa6, 0xa6, 0xa6, 0x9a, 0x98, 0xe5, 0xa1, 0xfc, 0x80, 0xcb, 0x31, 0xa9, 0x86, 0x67, 0x14, 0xca, 
  0x0f, 0x3c, 0x7a, 0xf4, 0xa8, 0xcc, 0x0e, 0xac, 0xb9, 0x6b, 0x66, 0xda, 0x80, 0xda, 0x1f, 0xa8, 
  0x06, 0x41, 0x11, 0xca, 0x0f, 0xd8, 0x5e, 0x11, 0x64, 0x92, 0x6d, 0xfd, 0x8e, 0x1b, 0x37, 0x4e, 
  0x91, 0x4c, 0x49, 0x0a, 0xd7, 0x78, 0xf1, 0x03, 0x3e, 0x00, 0xa6, 0x4d, 0x9b, 0x16, 0x88, 0xc5, 
  0x9c, 0xa8, 0x70, 0x8d, 0x17, 0x80, 0x56, 0x57, 0x41, 0xab, 0x0f, 0x42, 0xfb, 0x31, 0xbc, 0x70, 
  0xe1, 0x42, 0xae, 0x93, 0x90, 0x56, 0x43, 0x28, 0x3f, 0x20, 0xae, 0xd7, 0xdc, 0xeb, 0xd7, 0xaf, 
  0x2b, 0x7a, 0x66, 0xcf, 0x9e, 0x3d, 0xca, 0x97, 0xab, 0x06, 0x51, 0x19, 0xca, 0x0f, 0x60, 0x40, 
  0xda, 0x4c, 0x19, 0x64, 0x65, 0xde, 0x54, 0x6d, 0x24, 0x3f, 0x60, 0xb3, 0xa5, 0x54, 0xae, 0xd9, 
  0x62, 0x84, 0xe5, 0x4c, 0x74, 0xcb, 0x46, 0x48, 0xb0, 0x01, 0xbe, 0x3f, 0x81, 0x47, 0xec, 0xac, 
  0xeb, 0xcd, 0x0f, 0x08, 0x80, 0x8a, 0x08, 0x19, 0x36, 0x01, 0x7c, 0x3f, 0xbe, 0x7e, 0x52, 0x20, 
  0xa9, 0xf8, 0x01, 0x9b, 0x37, 0x46, 0x2a, 0x32, 0xfb, 0xed, 0xae, 0x19, 0x3f, 0x40, 0x68, 0xcd, 
  0xe2, 0x8e, 0x7f, 0xae, 0x39, 0x3f, 0x40, 0x68, 0xad, 0x04, 0x02, 0x9f, 0xfe, 0xab, 0x56, 0xe1, 
  0x07, 0x30, 0x9d, 0xf1, 0xe9, 0xf3, 0x70, 0xed, 0xb8, 0x47, 0xc1, 0x0f, 0xe4, 0x25, 0xc9, 0xd6, 
  0xbd, 0xcf, 0x97, 0xc9, 0x0f, 0xc8, 0x52, 0x7d, 0x55, 0x68, 0xdc, 0xa4, 0xae, 0xb8, 0x5d, 0x3f, 
  0x79, 0xfe, 0x80, 0x84, 0x5a, 0xeb, 0xb5, 0xa1, 0xf2, 0xf4, 0xe9, 0xd3, 0x40, 0xd2, 0x31, 0xb2, 
  0x82, 0xf0, 0xcf, 0x1f, 0x10, 0x8e, 0x50, 0x11, 0x95, 0x26, 0x65, 0x2b, 0x36, 0x83, 0x02, 0x50, 
  0xf5, 0xfc, 0x01, 0x7a, 0x6e, 0x37, 0xae, 0x81, 0x10, 0x74, 0xac, 0x7a, 0xfe, 0x80, 0x6d, 0x1f, 
  0xd2, 0xb8, 0xd0, 0xab, 0x89, 0x63, 0x07, 0xa9, 0xf2, 0x07, 0x18, 0x70, 0x36, 0x53, 0x7e, 0xfb, 
  0xf6, 0x6d, 0x65, 0x9e, 0xa5, 0x89, 0x21, 0x25, 0xca, 0x1f, 0xd8, 0xba, 0x75, 0x6b, 0x45, 0xc0, 
  0x82, 0x9e, 0xfb, 0xd8, 0x86, 0x71, 0x7e, 0xa4, 0x57, 0xfe, 0x40, 0xda, 0x90, 0xcd, 0x88, 0x11, 
  0x23, 0x02, 0x59, 0x3d, 0x03, 0xa1, 0xf2, 0x43, 0x2d, 0xa7, 0xd8, 0x90, 0x8d, 0xcd, 0x0f, 0xa0, 
  0x06, 0xac, 0x64, 0x9f, 0xa0, 0x15, 0x00, 0x0e, 0x1d, 0x3a, 0xa4, 0x32, 0x24, 0xa2, 0x40, 0x44, 
  0xe6, 0x0f, 0xac, 0x5f, 0xbf, 0xbe, 0xbf, 0x04, 0x2f, 0x83, 0x97, 0x2f, 0x5f, 0xaa, 0xc8, 0x59, 
  0xd2, 0xb0, 0x1d, 0x22, 0x06, 0x04, 0x31, 0xc1, 0x28, 0xd0, 0x5e, 0xf9, 0x03, 0xb8, 0xe6, 0xf8, 
  0x07, 0x71, 0x7a, 0xb5, 0xed, 0x43, 0xc6, 0xca, 0xe1, 0xc3, 0x87, 0x23, 0x41, 0x54, 0x3d, 0x7f, 
  0x60, 0xf2, 0xe4, 0xc9, 0x2d, 0x20, 0x5c, 0xea, 0x08, 0x0d, 0xdd, 0x5a, 0x36, 0xa0, 0xf2, 0x07, 
  0xd2, 0xe6, 0x0f, 0x20, 0x89, 0xb0, 0x31, 0x91, 0x29, 0x7f, 0x20, 0x8a, 0x1f, 0xb0, 0xd5, 0xa1, 
  0xc7, 0x04, 0x4f, 0x87, 0xad, 0xca, 0xd4, 0xf9, 0x03, 0x51, 0xfc, 0x80, 0x0d, 0x60, 0xc0, 0x80, 
  0x01, 0x2a, 0x83, 0x8a, 0x24, 0x28, 0x5f, 0x00, 0xb9, 0xa5, 0x70, 0x0c, 0x1a, 0x34, 0x48, 0xb1, 
  0x28, 0x7b, 0xf7, 0xee, 0x6d, 0x59, 0x37, 0x34, 0xc0, 0x28, 0x15, 0xe4, 0x92, 0xc4, 0xc2, 0xc0, 
  0xe3, 0x51, 0x24, 0x11, 0x42, 0x2f, 0x5a, 0xa6, 0x74, 0xa2, 0x06, 0x21, 0x21, 0x98, 0x96, 0xf5, 
  0x1e, 0xcf, 0x38, 0xe9, 0x20, 0xa4, 0xe7, 0x34, 0xce, 0xa3, 0x18, 0x36, 0x21, 0x85, 0xf2, 0x03, 
  0xc4, 0x78, 0x4c, 0x00, 0x49, 0xf9, 0x01, 0x74, 0x8e, 0xd8, 0xe9, 0x79, 0xd4, 0x6c, 0x58, 0x15, 
  0x7e, 0x80, 0xc9, 0x25, 0x4c, 0xe7, 0xa6, 0xf8, 0x63, 0xf3, 0x07, 0xd2, 0xf2, 0x03, 0x00, 0x90, 
  0xa9, 0xdc, 0xa9, 0x73, 0x0d, 0x20, 0x76, 0x31, 0xca, 0xca, 0x0f, 0xc4, 0x4d, 0xdb, 0x05, 0x3f, 
  0xe0, 0xe0, 0x86, 0xc2, 0xf3, 0x07, 0x0a, 0x7e, 0x40, 0x7b, 0x9f, 0x05, 0x3f, 0xd0, 0xba, 0x7e, 
  0xf8, 0xe7, 0xda, 0x3a, 0xd6, 0x33, 0x01, 0x2e, 0x4c, 0x78, 0x3c, 0x28, 0x5c, 0x38, 0x0c, 0x58, 
  0x8a, 0x24, 0xbe, 0xfc, 0xa7, 0x6a, 0xf9, 0x03, 0x38, 0x2d, 0x34, 0x6a, 0x07, 0x36, 0x4c, 0x2f, 
  0x8a, 0x60, 0x86, 0xf5, 0xbc, 0xe7, 0xb3, 0xbf, 0xc0, 0xe5, 0xae, 0xd9, 0xee, 0x1b, 0xff, 0xb3, 
  0x0c, 0x47, 0x84, 0xf9, 0x93, 0xf3, 0x03, 0x5d, 0xba, 0x74, 0xf9, 0x5e, 0x5c, 0xb3, 0xdf, 0x5c, 
  0x8d, 0xb9, 0x3e, 0x63, 0x29, 0x8e, 0xc9, 0x33, 0xf0, 0xe7, 0x07, 0xf4, 0xfe, 0x02, 0x98, 0x11, 
  0xbb, 0x31, 0xd4, 0x40, 0x88, 0x17, 0xff, 0x81, 0x6c, 0x3b, 0x1d, 0xc4, 0x9a, 0x37, 0x6f, 0x5e, 
  0x75, 0xf6, 0x17, 0xc8, 0xa0, 0x6b, 0x01, 0x81, 0xbb, 0x26, 0x03, 0x51, 0x19, 0x9a, 0x14, 0x33, 
  0xb4, 0x57, 0xd5, 0xfd, 0x05, 0x80, 0xa0, 0xb7, 0x3e, 0xbe, 0xa2, 0xb6, 0x01, 0x52, 0xf1, 0x03, 
  0x2e, 0xba, 0x1e, 0xd3, 0x1a, 0x93, 0x8b, 0x1e, 0x63, 0xf7, 0xc5, 0xad, 0xfd, 0xb6, 0x11, 0x9a, 
  0xeb, 0xfe, 0x02, 0x6c, 0xba, 0xd9, 0xb3, 0x67, 0x27, 0x0e, 0x60, 0x78, 0xf1, 0x03, 0x3e, 0x26, 
  0xd9, 0xdc, 0xb9, 0x73, 0x95, 0xe1, 0x99, 0x34, 0xac, 0x1b, 0x6b, 0x92, 0xf9, 0xec, 0x2f, 0x60, 
  0x90, 0xa1, 0x8e, 0x8b, 0x17, 0x2f, 0x7a, 0xb1, 0x26, 0xb6, 0xb7, 0x94, 0x79, 0x7f, 0xc1, 0xc0, 
  0x81, 0x03, 0xd5, 0x73, 0x2e, 0x69, 0xb8, 0x0a, 0x48, 0xd2, 0xf0, 0x0d, 0xf5, 0xbd, 0xf8, 0x01, 
  0x1a, 0x71, 0xed, 0x2f, 0xc0, 0xd3, 0xd1, 0x24, 0x04, 0xd3, 0x6e, 0xdc, 0xde, 0x13, 0x7a, 0x6c, 
  0xd7, 0xc9, 0xc4, 0x0f, 0x90, 0x5d, 0x8b, 0xbf, 0xc7, 0x40, 0x64, 0x12, 0x9a, 0x3e, 0x7d, 0x7a, 
  0xd9, 0x1e, 0x23, 0x00, 0xf2, 0x94, 0x68, 0xc9, 0x4c, 0x99, 0x32, 0x45, 0x79, 0x4a, 0x26, 0x88, 
  0xd4, 0xfc, 0x80, 0xbe, 0x10, 0xfb, 0x9f, 0x5e, 0xec, 0xdb, 0xb7, 0x4f, 0xa9, 0x41, 0xbb, 0x6f, 
  0x3c, 0x6a, 0x0c, 0x4e, 0x9e, 0x10, 0x3e, 0xa3, 0xcc, 0x9f, 0x3f, 0x5f, 0xf1, 0x46, 0xe6, 0xdc, 
  0x91, 0x9a, 0x1f, 0xe0, 0x26, 0xf4, 0x86, 0xe9, 0x96, 0x9b, 0x2c, 0x5c, 0xb8, 0x50, 0x85, 0x78, 
  0xb5, 0x1b, 0x86, 0xda, 0x18, 0x9c, 0x7c, 0x46, 0x8f, 0x99, 0x2b, 0x00, 0x28, 0xf9, 0xa5, 0x15, 
  0xd1, 0xd7, 0xd4, 0xfc, 0x00, 0x8e, 0xa5, 0xee, 0xcd, 0xe0, 0xc1, 0x83, 0x83, 0x23, 0x47, 0x8e, 
  0x04, 0x88, 0x19, 0x30, 0x27, 0x4f, 0x9e, 0x0c, 0x96, 0x2c, 0x59, 0xa2, 0x52, 0x80, 0x50, 0x0d, 
  0xf5, 0x00, 0xe5, 0x9a, 0x33, 0xc2, 0x00, 0x24, 0xe2, 0x07, 0x68, 0x40, 0x6c, 0x04, 0x55, 0x4e, 
  0x9d, 0x3a, 0xa5, 0x16, 0x21, 0x54, 0x43, 0xaf, 0x51, 0x05, 0xd4, 0x9d, 0x10, 0xde, 0x6a, 0xd4, 
  0x9b, 0x4f, 0x4b, 0x6e, 0xfc, 0x00, 0x37, 0x62, 0x51, 0x22, 0xcc, 0x4f, 0xbe, 0x91, 0xa6, 0x71, 
  0xc7, 0x8e, 0x1d, 0xab, 0x18, 0x36, 0xed, 0x29, 0x33, 0x03, 0x56, 0x8d, 0x1f, 0x98, 0x30, 0x61, 
  0x82, 0x7a, 0x2a, 0xe8, 0xad, 0xc9, 0x80, 0x30, 0x50, 0x19, 0x0f, 0x00, 0x43, 0x6d, 0x26, 0x80, 
  0x5c, 0xf9, 0x01, 0xd4, 0xc0, 0xe8, 0xb7, 0x89, 0x0c, 0x06, 0x26, 0x76, 0x04, 0x04, 0x95, 0x3d, 
  0x59, 0x55, 0x85, 0x1f, 0x70, 0xcd, 0x88, 0x0c, 0xcc, 0x45, 0x8b, 0x16, 0x95, 0x01, 0xa8, 0x1a, 
  0x3f, 0xe0, 0x02, 0x80, 0x74, 0x4c, 0xd5, 0xc4, 0x2e, 0x46, 0x59, 0xf9, 0x81, 0xb8, 0x75, 0xa1, 
  0xe0, 0x07, 0x0a, 0x7e, 0x80, 0x1c, 0x04, 0x29, 0xc9, 0xf6, 0x17, 0x14, 0xfc, 0xc0, 0xe7, 0xea, 
  0xa1, 0xb7, 0x2e, 0x2e, 0x3b, 0x7f, 0x00, 0x4e, 0x40, 0xf3, 0x03, 0x70, 0x05, 0x70, 0x06, 0xb8, 
  0xf1, 0xec, 0x5d, 0x63, 0xfb, 0x58, 0x6e, 0x68, 0xc3, 0xf6, 0x17, 0x60, 0x9c, 0x86, 0x79, 0xcf, 
  0x00, 0x03, 0x0c, 0x3b, 0xb8, 0x32, 0x01, 0x71, 0xb9, 0x6b, 0x7a, 0x62, 0x81, 0x13, 0xf0, 0x71, 
  0xdf, 0xd9, 0xc1, 0x95, 0x18, 0x84, 0xcf, 0xf9, 0x03, 0x58, 0x3f, 0x3e, 0x00, 0xa8, 0xd3, 0xdc, 
  0xdc, 0xfc, 0x9b, 0x58, 0x4c, 0x7e, 0x69, 0x41, 0xbe, 0xe7, 0x0f, 0x60, 0xa4, 0x6a, 0x7e, 0x00, 
  0xef, 0x19, 0x73, 0x1d, 0xee, 0xc0, 0x45, 0xe5, 0x94, 0xb2, 0x30, 0xaa, 0x77, 0xfe, 0x00, 0x4b, 
  0x2e, 0xd6, 0x30, 0xe6, 0x19, 0xe6, 0x1a, 0x40, 0xb4, 0x74, 0x70, 0xef, 0x8d, 0xf5, 0xa0, 0x36, 
  0xfb, 0x0b, 0xb0, 0x09, 0x90, 0x8a, 0xec, 0x2d, 0x6f, 0xbd, 0xf3, 0x07, 0x90, 0x08, 0x86, 0xe9, 
  0x97, 0x7b, 0xfe, 0x80, 0xe6, 0x11, 0x62, 0x4d, 0x32, 0x1f, 0x7e, 0x20, 0xce, 0xec, 0x72, 0x45, 
  0x4f, 0x19, 0x8c, 0xda, 0x9b, 0xca, 0xcc, 0x0f, 0x24, 0x01, 0xa0, 0x63, 0x88, 0x27, 0x4e, 0x9c, 
  0x08, 0xe0, 0x15, 0xf4, 0xb5, 0x99, 0xf8, 0x01, 0x1f, 0x00, 0xf4, 0x92, 0xe8, 0x39, 0x3e, 0x22, 
  0x1e, 0x12, 0x3e, 0xa3, 0x09, 0x20, 0x13, 0x3f, 0x10, 0x07, 0x80, 0x86, 0xd8, 0x2c, 0x2d, 0x0b, 
  0x54, 0xb0, 0x76, 0xed, 0x5a, 0x95, 0x02, 0x00, 0xa1, 0x61, 0x02, 0x48, 0xcd, 0x0f, 0xc4, 0x35, 
  0x3e, 0x7c, 0xf8, 0x70, 0x15, 0x2d, 0x67, 0x6a, 0x6e, 0x68, 0x68, 0x50, 0x13, 0x12, 0xe2, 0xb6, 
  0x01, 0xa4, 0xe6, 0x07, 0xa2, 0x00, 0xe0, 0xa2, 0xd3, 0x6b, 0xb8, 0x01, 0xd9, 0x8b, 0xda, 0xe2, 
  0xaa, 0xb9, 0x00, 0x70, 0x9f, 0xd4, 0xfc, 0x80, 0x0d, 0x02, 0x22, 0x02, 0x4e, 0x00, 0x5d, 0x93, 
  0xe2, 0x65, 0xf3, 0x41, 0x49, 0x01, 0x24, 0xe2, 0x07, 0x98, 0xdd, 0xf0, 0x82, 0x65, 0x6b, 0x6f, 
  0x68, 0xba, 0x47, 0x52, 0x15, 0x78, 0xe5, 0x0f, 0x30, 0x88, 0x10, 0x21, 0x1c, 0x11, 0xee, 0x37, 
  0x69, 0x1d, 0x61, 0x61, 0x7e, 0x17, 0x80, 0x4c, 0xf9, 0x03, 0x3c, 0x42, 0xcb, 0x97, 0x2f, 0x57, 
  0x4b, 0x2e, 0x34, 0x9e, 0xed, 0xfb, 0xdb, 0x2a, 0x72, 0x01, 0x48, 0xcd, 0x0f, 0xd0, 0x4b, 0xd6, 
  0x7e, 0x06, 0x9b, 0x2f, 0x57, 0x8c, 0x74, 0xc8, 0x43, 0x80, 0xe8, 0xd6, 0xe0, 0x32, 0xf1, 0x03, 
  0xcc, 0x6e, 0xb8, 0xdc, 0x49, 0x33, 0x2b, 0x4c, 0xfa, 0xde, 0xe1, 0x23, 0x7e, 0x3a, 0x7f, 0xc0, 
  0x87, 0xac, 0x8e, 0x9b, 0x0f, 0xc2, 0xbe, 0x8f, 0x5d, 0x8c, 0x0a, 0x7e, 0xc0, 0x34, 0x99, 0x8b, 
  0xfd, 0x05, 0x48, 0xa3, 0xd8, 0x5f, 0x90, 0xd8, 0x8d, 0x2a, 0x2e, 0xa8, 0x95, 0x04, 0xbe, 0xcc, 
  0xfd, 0x05, 0x51, 0xfc, 0x40, 0x4c, 0xb8, 0x5e, 0xf9, 0x84, 0x78, 0xc5, 0x55, 0xe3, 0x07, 0xe2, 
  0x00, 0xb0, 0x27, 0x81, 0xbd, 0x09, 0xa5, 0x04, 0xf9, 0xf7, 0x1c, 0xb2, 0xe2, 0x35, 0x5c, 0x7c, 
  0xf9, 0x81, 0xb8, 0xf3, 0x09, 0xd9, 0x93, 0x60, 0x12, 0x18, 0xf0, 0x48, 0x1c, 0xb2, 0x12, 0x09, 
  0x22, 0xaf, 0xf3, 0x09, 0x09, 0xd9, 0xb9, 0xd8, 0x13, 0x40, 0x70, 0xc8, 0x4a, 0x28, 0x08, 0xd7, 
  0x62, 0x94, 0xe6, 0x7c, 0x42, 0x4c, 0x38, 0xf8, 0x01, 0x8e, 0x14, 0xb2, 0x81, 0x3c, 0x78, 0xf0, 
  0xe0, 0xbd, 0x13, 0x40, 0x58, 0xfe, 0x40, 0xda, 0xf3, 0x09, 0x01, 0x41, 0xc4, 0x95, 0x23, 0x85, 
  0x6c, 0x10, 0x62, 0xce, 0x97, 0x13, 0x57, 0xd5, 0x3c, 0x9f, 0x90, 0xf3, 0x8c, 0x84, 0xb2, 0xab, 
  0x00, 0x21, 0x1d, 0xfe, 0xa4, 0x8a, 0x38, 0x93, 0x2c, 0xcb, 0xfe, 0x53, 0xad, 0x0e, 0x5b, 0x0a, 
  0x62, 0x61, 0x37, 0x2b, 0x55, 0xc4, 0xf1, 0x03, 0xf8, 0xf8, 0x04, 0xa2, 0xa2, 0x32, 0x66, 0xe3, 
  0xec, 0x45, 0xee, 0x71, 0xff, 0xfe, 0xfd, 0x32, 0x29, 0xc0, 0xa6, 0x89, 0x7f, 0xd1, 0x2e, 0xf6, 
  0x7c, 0x42, 0xec, 0x79, 0x22, 0xa1, 0x76, 0x9c, 0x30, 0xae, 0x51, 0xfb, 0x7b, 0x22, 0xac, 0x9a, 
  0xc2, 0x63, 0x2f, 0x03, 0x7b, 0x1a, 0x64, 0xa0, 0xf6, 0x8f, 0x3d, 0x9f, 0x10, 0x00, 0x78, 0xba, 
  0xf8, 0x81, 0x3a, 0x74, 0x9f, 0x46, 0x25, 0xf8, 0x08, 0x72, 0x58, 0x92, 0xba, 0x87, 0x31, 0x89, 
  0xc5, 0x9f, 0x4f, 0x08, 0x00, 0xed, 0x07, 0xe2, 0x1d, 0xc1, 0x11, 0x13, 0x19, 0x27, 0x88, 0x9d, 
  0x44, 0x0a, 0xa9, 0xf9, 0x01, 0x0d, 0x80, 0x0c, 0x7b, 0x1c, 0x93, 0xa9, 0x53, 0xa7, 0x2a, 0x75, 
  0xd0, 0x0b, 0xe2, 0xc5, 0xbe, 0x20, 0x52, 0xf3, 0x03, 0x26, 0x00, 0xdd, 0x18, 0x61, 0x5b, 0x66, 
  0x3b, 0xa2, 0xe7, 0x49, 0x36, 0x44, 0xa4, 0xe2, 0x07, 0x5c, 0x00, 0x00, 0xc2, 0x38, 0xe0, 0x34, 
  0x4f, 0xc6, 0x06, 0xa1, 0x7b, 0x93, 0x8e, 0x09, 0x93, 0x4a, 0xaa, 0xfc, 0x81, 0x30, 0x00, 0xba, 
  0x11, 0x66, 0x3b, 0x5c, 0x76, 0xb2, 0x2c, 0xa2, 0x36, 0x46, 0xa5, 0xce, 0x1f, 0x88, 0x03, 0x00, 
  0x10, 0x48, 0x0b, 0x98, 0x73, 0x9e, 0x16, 0xf2, 0x47, 0x5c, 0x4f, 0x49, 0x6a, 0x7e, 0xc0, 0x07, 
  0x00, 0x20, 0x98, 0xf1, 0x88, 0x9a, 0x63, 0x84, 0xc0, 0x94, 0xe5, 0x96, 0x3f, 0xc0, 0x62, 0x44, 
  0xba, 0x06, 0xfe, 0xbd, 0xcf, 0x88, 0x27, 0xb9, 0x81, 0x1c, 0x13, 0x26, 0x1e, 0x53, 0x12, 0x99, 
  0xf8, 0x01, 0x9f, 0x86, 0xcd, 0x3a, 0x43, 0x87, 0x0e, 0x2d, 0x9b, 0xba, 0x6b, 0x9a, 0x3f, 0x60, 
  0x83, 0x2d, 0xf8, 0x81, 0x44, 0xe7, 0x13, 0x16, 0xfc, 0x40, 0xc1, 0x0f, 0x78, 0x79, 0x2e, 0x45, 
  0xa5, 0xd6, 0x92, 0x40, 0xc1, 0x0f, 0xc4, 0xb9, 0xe3, 0xb9, 0xee, 0x2f, 0xf0, 0xc9, 0x1f, 0x48, 
  0x08, 0x28, 0xf9, 0xfe, 0x02, 0x57, 0x03, 0x55, 0x3f, 0x7f, 0x20, 0x2f, 0x7e, 0xa0, 0x78, 0x7f, 
  0x01, 0x46, 0x49, 0xa2, 0xf3, 0x07, 0xaa, 0xc9, 0x0f, 0x84, 0xed, 0x2f, 0x10, 0x87, 0xf7, 0xbb, 
  0x96, 0x39, 0x2d, 0x8e, 0x1f, 0x48, 0x6a, 0x13, 0x9a, 0xf5, 0x6d, 0x93, 0x8c, 0xe0, 0x76, 0x69, 
  0xa3, 0x8c, 0x1f, 0x3f, 0x40, 0x2c, 0x98, 0x98, 0x30, 0x1c, 0x81, 0xab, 0xf8, 0x1c, 0x35, 0x8f, 
  0x51, 0x4a, 0x6c, 0x99, 0xb3, 0x0f, 0x35, 0x51, 0x91, 0x98, 0x1f, 0x38, 0x7f, 0xfe, 0x7c, 0xe0, 
  0x2a, 0xf0, 0x81, 0x3e, 0x12, 0x32, 0xf9, 0x01, 0x0d, 0x42, 0xfc, 0x87, 0x25, 0x5e, 0xfc, 0x00, 
  0xee, 0xb9, 0xeb, 0xac, 0x39, 0xbc, 0x21, 0x02, 0xd7, 0x3e, 0x00, 0xf0, 0x1d, 0x25, 0xa1, 0xa9, 
  0x8c, 0x25, 0x11, 0xff, 0xa1, 0x39, 0xf6, 0xfd, 0x05, 0x51, 0x9e, 0x51, 0x12, 0x00, 0xa8, 0x12, 
  0x72, 0xc2, 0xe4, 0x8a, 0x84, 0xb6, 0x79, 0x1f, 0xfb, 0xfe, 0x82, 0xbc, 0x00, 0x30, 0x18, 0xe1, 
  0x0e, 0x6d, 0xf6, 0x34, 0xf6, 0xf5, 0x01, 0x79, 0x01, 0x40, 0x4d, 0x48, 0xcc, 0x04, 0xc0, 0x40, 
  0x8c, 0x05, 0x10, 0xa5, 0xdf, 0x30, 0x15, 0x84, 0xe5, 0x0f, 0x84, 0x01, 0x48, 0x94, 0x3f, 0x60, 
  0x02, 0x4a, 0x02, 0x20, 0x4a, 0x05, 0x5e, 0xf9, 0x03, 0x2e, 0x49, 0x24, 0x01, 0xe0, 0x1a, 0x84, 
  0x42, 0xe3, 0xaa, 0x41, 0x98, 0xfa, 0xfc, 0x81, 0x24, 0x00, 0x78, 0x0c, 0x49, 0x6e, 0x33, 0xc7, 
  0x80, 0xb8, 0xfd, 0xcd, 0x75, 0x59, 0xce, 0x1f, 0x00, 0xc0, 0xd9, 0xb3, 0x67, 0x15, 0x6b, 0x66, 
  0x16, 0xb2, 0x2d, 0xed, 0x3c, 0x22, 0xcd, 0x0f, 0x30, 0x6f, 0xc0, 0x2b, 0xb1, 0x7d, 0x50, 0x12, 
  0xdf, 0xe7, 0xd6, 0x65, 0x39, 0x9f, 0x10, 0x86, 0x6c, 0xdd, 0xba, 0x75, 0xce, 0xc2, 0xbb, 0x30, 
  0xcc, 0x54, 0xae, 0x22, 0x7f, 0xc0, 0x96, 0x40, 0xc5, 0x51, 0xf2, 0xae, 0x80, 0x45, 0x71, 0x3e, 
  0xa1, 0x48, 0xa5, 0x38, 0x9f, 0x30, 0x17, 0x57, 0x54, 0x24, 0x59, 0xbc, 0xbf, 0x20, 0x17, 0x49, 
  0x56, 0xe7, 0x26, 0x41, 0x10, 0xd4, 0xf9, 0x94, 0xa4, 0xfc, 0x80, 0xcf, 0x3d, 0xa9, 0x13, 0xdb, 
  0x78, 0xd6, 0xf7, 0x17, 0xc4, 0x01, 0x89, 0x04, 0x90, 0x35, 0x7f, 0xa0, 0x34, 0xfd, 0x36, 0x44, 
  0x81, 0x70, 0x02, 0xa8, 0x16, 0x3f, 0xe0, 0x02, 0x52, 0x01, 0x20, 0xaf, 0xfc, 0x01, 0xc7, 0xf2, 
  0xab, 0xf6, 0x17, 0xd8, 0x20, 0xca, 0x00, 0xd4, 0x8a, 0x1f, 0x30, 0x41, 0x94, 0x01, 0xc8, 0x2b, 
  0x7f, 0x20, 0xee, 0xfc, 0x01, 0x27, 0x80, 0xbc, 0xf3, 0x07, 0xe2, 0xf8, 0x01, 0x0d, 0x42, 0x49, 
  0xa0, 0x35, 0xf8, 0x01, 0xde, 0x6f, 0xd8, 0x32, 0x0f, 0xd4, 0x92, 0x1f, 0x30, 0x03, 0xd7, 0x0a, 
  0x40, 0x5c, 0xfe, 0x80, 0xe9, 0x0f, 0x8c, 0x1a, 0x35, 0x4a, 0xed, 0xae, 0x26, 0x32, 0x96, 0x24, 
  0x82, 0x9e, 0xcb, 0xfe, 0x02, 0x52, 0x31, 0x48, 0xed, 0xc5, 0x0c, 0xc7, 0xac, 0x26, 0x9b, 0x3e, 
  0xc9, 0x69, 0x0c, 0x99, 0xf6, 0x17, 0x90, 0x41, 0x8f, 0xdd, 0x0f, 0xfd, 0x46, 0x80, 0x12, 0x30, 
  0x6c, 0x60, 0xf0, 0x39, 0xc3, 0x52, 0x4b, 0x30, 0xf5, 0xfe, 0x02, 0xbd, 0xd5, 0x1b, 0x92, 0x42, 
  0x27, 0x2a, 0xe3, 0x66, 0x11, 0x1d, 0x85, 0xb8, 0xf4, 0x21, 0x27, 0x34, 0x6b, 0x96, 0xea, 0xfd, 
  0x86, 0x34, 0x86, 0x75, 0x6c, 0xbf, 0x78, 0x91, 0x03, 0x11, 0x09, 0xdf, 0x93, 0xd6, 0xef, 0x03, 
  0x22, 0x75, 0xfe, 0x00, 0xa1, 0x5b, 0x78, 0x20, 0xfb, 0x1d, 0x67, 0x00, 0x83, 0xf1, 0x42, 0x12, 
  0x3e, 0x00, 0xa8, 0x93, 0x2a, 0x7f, 0x20, 0xea, 0xe6, 0xa4, 0x74, 0x30, 0x16, 0xec, 0xfd, 0x05, 
  0xf8, 0x81, 0xf6, 0x0e, 0x8b, 0x28, 0x00, 0xa9, 0xf9, 0x01, 0x62, 0xc4, 0xa8, 0xc1, 0x96, 0x0e, 
  0x83, 0x54, 0x9f, 0xc8, 0xa0, 0x3b, 0x80, 0x0a, 0xf0, 0x17, 0x79, 0x82, 0x8c, 0xd3, 0x7b, 0xd4, 
  0x2b, 0x26, 0x53, 0xf3, 0x03, 0xdc, 0x9c, 0x54, 0x0e, 0x36, 0x3c, 0xe8, 0x63, 0x62, 0xf8, 0xad, 
  0x1d, 0x56, 0xf3, 0xe8, 0x18, 0x9b, 0x1f, 0xe0, 0x9d, 0x8a, 0xe2, 0x21, 0xff, 0x92, 0x89, 0x1f, 
  0x00, 0x00, 0xa9, 0xfc, 0xa8, 0x81, 0xe4, 0x16, 0xfe, 0xd7, 0x5b, 0x7d, 0xf4, 0xff, 0x5a, 0x02, 
  0x64, 0xdd, 0xd8, 0xc9, 0x6d, 0x99, 0xf9, 0x01, 0x9d, 0x4b, 0xc2, 0x58, 0x60, 0x92, 0x62, 0xae, 
  0x40, 0xc4, 0x9c, 0x3f, 0x60, 0x1f, 0xb0, 0xbb, 0x6c, 0xd9, 0xb2, 0x8a, 0xbd, 0x88, 0x32, 0xab, 
  0x2e, 0xc9, 0xc4, 0x0f, 0xe8, 0xde, 0x31, 0x08, 0xd9, 0x65, 0x8f, 0x8e, 0xd9, 0x04, 0x61, 0x37, 
  0x0e, 0x4f, 0x20, 0x6f, 0x8a, 0xab, 0x60, 0xc8, 0xa4, 0x7e, 0x3b, 0xb5, 0x1a, 0x56, 0x73, 0x31, 
  0x0a, 0x4b, 0x66, 0x13, 0x76, 0xa5, 0xb9, 0x65, 0x35, 0xac, 0xe6, 0xfb, 0x0b, 0x48, 0xf5, 0x31, 
  0x5f, 0xe2, 0xa9, 0x39, 0x22, 0x21, 0xc0, 0xeb, 0xcb, 0xcc, 0xf2, 0xbc, 0xf9, 0x01, 0xde, 0x89, 
  0x48, 0xe3, 0xb6, 0xe8, 0x01, 0x20, 0x0b, 0xd9, 0xd5, 0x32, 0x83, 0x44, 0xff, 0x93, 0x77, 0xfe, 
  0x80, 0x64, 0xcc, 0x49, 0x7b, 0x7f, 0xbd, 0xbe, 0x54, 0x17, 0xa1, 0xec, 0xdf, 0x87, 0xda, 0x84, 
  0xae, 0xf3, 0x09, 0x75, 0xe5, 0xa4, 0xef, 0x37, 0xd4, 0xd7, 0xf1, 0x8e, 0x44, 0xdd, 0x38, 0x49, 
  0xad, 0x32, 0x1d, 0x2b, 0xd1, 0x3b, 0x25, 0xc0, 0x87, 0xf6, 0xfe, 0x02, 0xb3, 0xb2, 0xef, 0xfb, 
  0x0d, 0xcd, 0x6b, 0x78, 0x47, 0x22, 0xef, 0x4a, 0xa4, 0x71, 0x99, 0xa2, 0x7b, 0x47, 0x9a, 0xe5, 
  0xfa, 0x4b, 0x7d, 0xfe, 0x40, 0x9c, 0x5b, 0xe5, 0xfb, 0xbd, 0xe4, 0x11, 0x76, 0xb5, 0x7b, 0x1e, 
  0x2a, 0x01, 0xdf, 0x9b, 0xe6, 0x55, 0x2f, 0xd6, 0x39, 0xcd, 0xab, 0xa1, 0xb0, 0xfb, 0xb4, 0x3a, 
  0x80, 0xff, 0x03, 0xf2, 0x4a, 0x50, 0x77, 0x95, 0xb8, 0x97, 0x70, 0x00, 0x00, 0x00, 0x00, 0x49, 
  0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};
// 'syn_perc_osc_32x32_8.png' : 3767