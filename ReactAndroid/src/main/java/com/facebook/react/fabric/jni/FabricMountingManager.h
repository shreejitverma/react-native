/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "FabricMountItem.h"

#include <react/config/ReactNativeConfig.h>
#include <react/renderer/animations/LayoutAnimationDriver.h>
#include <react/renderer/mounting/MountingCoordinator.h>
#include <react/renderer/mounting/ShadowView.h>
#include <react/renderer/uimanager/LayoutAnimationStatusDelegate.h>
#include <react/utils/ContextContainer.h>

#include <fbjni/fbjni.h>

#include <mutex>

namespace facebook {
namespace react {

class FabricMountingManager {
 public:
  constexpr static auto UIManagerJavaDescriptor =
      "com/facebook/react/fabric/FabricUIManager";

  constexpr static auto ReactFeatureFlagsJavaDescriptor =
      "com/facebook/react/config/ReactFeatureFlags";

  FabricMountingManager(
      std::shared_ptr<const ReactNativeConfig> &config,
      jni::global_ref<jobject> &javaUIManager);

  void onSurfaceStart(SurfaceId surfaceId);

  void onSurfaceStop(SurfaceId surfaceId);

  void preallocateShadowView(SurfaceId surfaceId, ShadowView const &shadowView);

  void executeMount(MountingCoordinator::Shared const &mountingCoordinator);

  void dispatchCommand(
      ShadowView const &shadowView,
      std::string const &commandName,
      folly::dynamic const &args);

  void sendAccessibilityEvent(
      const ShadowView &shadowView,
      std::string const &eventType);

  void setIsJSResponder(
      ShadowView const &shadowView,
      bool isJSResponder,
      bool blockNativeResponder);

  void onAnimationStarted();

  void onAllAnimationsComplete();

  virtual ~FabricMountingManager() = default;

 private:
  jni::global_ref<jobject> javaUIManager_;

  std::recursive_mutex commitMutex_;

  butter::map<SurfaceId, butter::set<Tag>> allocatedViewRegistry_{};
  std::recursive_mutex allocatedViewsMutex_;

  bool enableEventEmitterRawPointer_{false};
  bool enableEarlyEventEmitterUpdate_{false};
  bool disablePreallocateViews_{false};
  bool disableRevisionCheckForPreallocation_{false};
  bool useOverflowInset_{false};
  bool shouldRememberAllocatedViews_{false};
};

} // namespace react
} // namespace facebook
