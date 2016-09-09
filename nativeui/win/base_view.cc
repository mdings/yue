// Copyright 2016 Cheng Zhao. All rights reserved.
// Use of this source code is governed by the license that can be found in the
// LICENSE file.

#include "nativeui/win/base_view.h"

namespace nu {

void BaseView::SizeAllocate(const Rect& size_allocation) {
  if (size_allocation == size_allocation_)
    return;

  Invalidate(size_allocation_);  // old
  size_allocation_ = size_allocation;
  Invalidate(size_allocation_);  // new
}

void BaseView::SetParent(BaseView* parent) {
  float old_scale_factor = scale_factor();
  window_ = parent ? parent->window_ : nullptr;
  ParentChanged(old_scale_factor);
}

void BaseView::BecomeContentView(WindowImpl* parent) {
  float old_scale_factor = scale_factor();
  window_ = parent;
  ParentChanged(old_scale_factor);
}

void BaseView::Invalidate(const Rect& dirty) {
  // Nothing to draw?
  if (!window_ || size_allocation_.size().IsEmpty() || dirty.IsEmpty())
    return;

  RECT rect = dirty.ToRECT();
  InvalidateRect(window_->hwnd(), &rect, TRUE);
}

void BaseView::SetFocus(bool focus) {
  is_focused_ = focus;
  Invalidate();
}

bool BaseView::IsFocused() const {
  return is_focused_;
}

void BaseView::Invalidate() {
  Invalidate(size_allocation_);
}

void BaseView::ParentChanged(float old_scale_factor) {
  // Scale the bounds after moving to a new parent.
  if (old_scale_factor != scale_factor()) {
    size_allocation_ = ScaleToEnclosingRect(size_allocation_,
                                            scale_factor() / old_scale_factor);
    preferred_size_ = ScaleToCeiledSize(preferred_size_,
                                        scale_factor() / old_scale_factor);
  }
}

}  // namespace nu
