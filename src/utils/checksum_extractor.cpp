/*
 * This File is part of Davix, The IO library for HTTP based protocols
 * Copyright (C) CERN 2018
 * Author: Georgios Bitzes <georgios.bitzes@cern.ch>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <string_utils/stringutils.hpp>
#include <alibxx/crypto/base64.hpp>
#include "checksum_extractor.hpp"
#include <ctype.h>
#include <sstream>
#include <iomanip>

#define SSTR(message) static_cast<std::ostringstream&>(std::ostringstream().flush() << message).str()

namespace Davix {

static bool startsWithNoCase(const std::string &str, const std::string &prefix) {
  if(prefix.size() > str.size()) return false;

  for(size_t i = 0; i < prefix.size(); i++) {
    if(tolower(str[i]) != tolower(prefix[i])) return false;
  }

  return true;
}

static bool equalsNoCase(const std::string &s1, const std::string &s2) {
  if(s1.size() != s2.size()) return false;

  for(size_t i = 0; i < s1.size(); i++) {
    if(tolower(s1[i]) != tolower(s2[i])) return false;
  }

  return true;
}

static std::string hexEncode(const std::string &input, const std::string &separator="") {
    std::ostringstream ss;
    for(std::string::const_iterator it = input.begin(); it != input.end(); it++) {
        ss << std::setw(2) << std::setfill('0') << std::hex << (unsigned int) ( (unsigned char) *it) << separator;
    }
    return ss.str();
}

bool ChecksumExtractor::extractChecksum(const HeaderVec &headers,
    const std::string &desiredChecksum, std::string &checksum) {

  std::string expectedPrefix = SSTR(desiredChecksum << "=");

  for(HeaderVec::const_iterator it = headers.begin(); it != headers.end(); it++) {
    if(equalsNoCase(it->first, "Digest")) {
      if(startsWithNoCase(it->second, expectedPrefix)) {
        // We have a match. Are we supposed to base64 decode this?
        checksum = it->second.substr(expectedPrefix.size());

        if(StrUtil::compare_ncase(desiredChecksum, "UNIXcksum") == 0 ||
           StrUtil::compare_ncase(desiredChecksum, "CRC32c") == 0    ||
           StrUtil::compare_ncase(desiredChecksum, "ADLER32") == 0   ||
           StrUtil::compare_ncase(desiredChecksum, "UNIXsum") == 0
         ) {

          // Nope, just extract the value.
          return true;
        }

        if(StrUtil::compare_ncase(desiredChecksum, "md5") == 0) {
          // Maybe.. older versions of DPM don't base64 encode their output.
          if(checksum.size() != 32) {
            checksum = hexEncode(Base64::base64_decode(checksum));
          }
          return true;
        }

        // All other checksums should be base64 decoded.
        checksum = hexEncode(Base64::base64_decode(checksum));
        return true;
      }
    }
  }

  return false;
}

}
