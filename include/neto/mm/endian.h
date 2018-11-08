#ifndef __NETO_MM_ENDIAN_H__
#define __NETO_MM_ENDIAN_H__

#include <inttypes.h>
#include <endian.h>

namespace neto {
namespace mm {

  inline uint16_t hostToBE(uint16_t n) {
    return htobe16(n);
  }
  inline uint32_t hostToBE(uint32_t n) {
    return htobe32(n);
  }
  inline uint64_t hostToBE(uint64_t n) {
    return htobe64(n);
  }

  inline uint16_t hostToLE(uint16_t n) {
    return htole16(n);
  }
  inline uint32_t hostToLE(uint32_t n) {
    return htole32(n);
  }
  inline uint64_t hostToLE(uint64_t n) {
    return htole64(n);
  }

  inline uint16_t BEToHost(uint16_t n) {
    return be16toh(n);
  }
  inline uint32_t BEToHost(uint32_t n) {
    return be32toh(n);
  }
  inline uint64_t BEToHost(uint64_t n) {
    return be64toh(n);
  }

  inline uint16_t LEToHost(uint16_t n) {
    return le16toh(n);
  }
  inline uint32_t LEToHost(uint32_t n) {
    return le32toh(n);
  }
  inline uint64_t LEToHost(uint64_t n) {
    return le64toh(n);
  }

} // namespace mm
} // namespace neto

#endif // __NETO_MM_ENDIAN_H__
