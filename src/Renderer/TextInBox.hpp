// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "LabelShape.hpp"


struct PixelPoint;
struct PixelSize;
struct PixelRect;
class Canvas;
class LabelBlock;
void
RenderShadowedText(Canvas &canvas, const char *text,
                   PixelPoint p,
                   bool inverted, bool vibrant) noexcept;

struct TextInBoxMode {
  enum Alignment : uint8_t {
    LEFT,
    CENTER,
    RIGHT,
  };

  enum VerticalPosition : uint8_t {
    ABOVE,
    CENTERED,
    BELOW,
  };

  LabelShape shape = LabelShape::SIMPLE;
  Alignment align = Alignment::LEFT;
  VerticalPosition vertical_position = VerticalPosition::BELOW;
  bool move_in_view = false;
};

bool
TextInBox(Canvas &canvas, const char *value, PixelPoint p,
          TextInBoxMode mode, const PixelRect &map_rc,
          LabelBlock *label_block=nullptr) noexcept;

bool
TextInBox(Canvas &canvas, const char *value, PixelPoint p,
          TextInBoxMode mode,
          PixelSize screen_size,
          LabelBlock *label_block=nullptr) noexcept;
