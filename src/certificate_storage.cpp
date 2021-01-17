#include "certificate_storage.hpp"
#include "generated/certificates.h"
#include "utils.hpp"

namespace BearSSL {

void CertStore::installCertStore(br_x509_minimal_context *ctx) {
  // NOLINTNEXTLINE *-pro-type-reinterpret-cast
  br_x509_minimal_set_dynamic(ctx, reinterpret_cast<void *>(this), findHashedTA,
                              freeHashedTA);
}

auto CertStore::findHashedTA(void *ctx, void *hashed_dn, size_t len)
    -> const br_x509_trust_anchor * {
  constexpr const uint8_t hashSize = 32;
  if (ctx == nullptr || len != hashSize)
    return nullptr;

  auto *cs = static_cast<CertStore *>(ctx);

  for (int i = 0; i < numberOfCertificates; i++) {
    // NOLINTNEXTLINE cppcoreguidelines-pro-bounds-constant-array-index
    if (memcmp_P(hashed_dn, indices[i], hashSize) == 0) {

      // NOLINTNEXTLINE cppcoreguidelines-pro-bounds-constant-array-index
      auto der = try_make_unique<uint8_t[]>(certSizes[i]);
      // NOLINTNEXTLINE cppcoreguidelines-pro-bounds-constant-array-index
      memcpy_P(der.get(), certificates[i], certSizes[i]);

      // NOLINTNEXTLINE cppcoreguidelines-pro-bounds-constant-array-index
      cs->_x509 = try_make_unique<X509List>(der.get(), certSizes[i]);
      der.reset(nullptr);

      if (!cs->_x509)
        return nullptr;

      const auto *taTmp = cs->_x509->getTrustAnchors();
      // _x509 is heap allocated so it's mutable
      // NOLINTNEXTLINE cppcoreguidelines-pro-type-const-cast
      auto *ta = const_cast<br_x509_trust_anchor *>(taTmp);
      // NOLINTNEXTLINE cppcoreguidelines-pro-bounds-constant-array-index
      memcpy_P(ta->dn.data, indices[i], hashSize);
      ta->dn.len = hashSize;

      return ta;
    }
  }

  return nullptr;
}

void CertStore::freeHashedTA(void *ctx, const br_x509_trust_anchor *ta) {
  auto *cs = static_cast<CertStore *>(ctx);
  cs->_x509.reset(nullptr);
  (void)ta; // Unused
}

} // namespace BearSSL
