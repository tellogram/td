//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2018
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "td/telegram/net/MtprotoHeader.h"

#include "td/utils/tl_helpers.h"

namespace td {

namespace {
class HeaderStorer {
 public:
  HeaderStorer(const MtprotoHeader::Options &options, bool is_anonymous)
      : options(options), is_anonymous(is_anonymous) {
  }
  template <class StorerT>
  void store(StorerT &storer) const {
    constexpr int32 LAYER = 75;

    using td::store;
    // invokeWithLayer#da9b0d0d {X:Type} layer:int query:!X = X;
    store(static_cast<int32>(0xda9b0d0d), storer);
    store(LAYER, storer);
    // initConnection#c7481da6 {X:Type} api_id:int device_model:string system_version:string app_version:string
    // system_lang_code:string lang_pack:string lang_code:string query:!X = X;
    store(static_cast<int32>(0xc7481da6), storer);
    store(options.api_id, storer);
    if (is_anonymous) {
      store(Slice("n/a"), storer);
      store(Slice("n/a"), storer);
    } else {
      store(options.device_model, storer);
      store(options.system_version, storer);
    }
    store(options.application_version, storer);
    store(options.system_language_code, storer);
    store(string(), storer);
    store(string(), storer);
  }

 private:
  const MtprotoHeader::Options &options;
  bool is_anonymous;
};
}  // namespace

string MtprotoHeader::gen_header(const MtprotoHeader::Options &options, bool is_anonymous) {
  HeaderStorer storer(options, is_anonymous);
  return serialize(storer);
}

}  // namespace td
