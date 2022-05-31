#ifndef H_lib_txbench_pq_Network
#define H_lib_txbench_pq_Network
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
namespace ByteOrder {
/// Are we using a big endian machine?
constexpr const bool bigEndian =
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
   true
#else
   false
#endif
   ;
}
//---------------------------------------------------------------------------
/// Helper functions for network communication
struct Network {
   /// Convert from host to network byte order
   static int host2be(int arg) {
      if constexpr (ByteOrder::bigEndian)
         return arg;
      else
         return __builtin_bswap32(arg);
   }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif