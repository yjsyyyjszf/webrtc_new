/*
 *  Copyright 2016 The WebRTC Project Authors. All rights reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_AUDIO_DEVICE_IOS_AUDIO_SESSION_OBSERVER_H_
#define WEBRTC_MODULES_AUDIO_DEVICE_IOS_AUDIO_SESSION_OBSERVER_H_

#include "webrtc/base/asyncinvoker.h"
#include "webrtc/base/thread.h"

namespace webrtc {

// Observer interface for listening to AVAudioSession events.
class AudioSessionObserver {
 public:
  // Called when audio session interruption begins.
  virtual void OnInterruptionBegin() = 0;

  // Called when audio session interruption ends.
  virtual void OnInterruptionEnd() = 0;

  // Called when audio route changes.
  virtual void OnValidRouteChange() = 0;

 protected:
  virtual ~AudioSessionObserver() {}
};

}  // namespace webrtc

#endif  //  WEBRTC_MODULES_AUDIO_DEVICE_IOS_AUDIO_SESSION_OBSERVER_H_