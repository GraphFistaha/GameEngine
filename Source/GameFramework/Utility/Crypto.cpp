#include "Crypto.hpp"

#include <cryptopp/md5.h>

namespace GameFramework
{
GAME_FRAMEWORK_API Uuid Checksum(const std::filesystem::path & path)
{
  std::array<CryptoPP::byte, CryptoPP::MD5::DIGESTSIZE> digest;
  CryptoPP::MD5 md5;
  md5.CalculateDigest(digest.data(),
                      reinterpret_cast<const CryptoPP::byte *>(path.native().c_str()),
                      path.native().size() *
                        sizeof(std::filesystem::path::string_type::value_type));
  return Uuid(digest);
}
} // namespace GameFramework
