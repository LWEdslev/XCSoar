/*
  Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2021 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "WeatherWidget.hpp"
#include "UIGlobals.hpp"
#include "Look/Look.hpp"
#include "Interface.hpp"
#include "ui/window/AntiFlickerWindow.hpp"
#include "ui/canvas/Canvas.hpp"
#include "Renderer/WeatherRenderer.hpp"

#include "MergeThread.hpp"
#include "Components.hpp"

/**
 * A Window which renders a terrain and airspace cross-section
 */
class WeatherWindow : public AntiFlickerWindow {
  const WeatherLook &look;
  const bool& inverse;

  AttitudeState attitude;

public:
  /**
   * Constructor. Initializes most class members.
   */
  WeatherWindow(const WeatherLook &_look, const bool &_inverse) noexcept
    :look(_look),inverse(_inverse)
  {
    attitude.Reset();
  }

//  void ReadBlackboard(const AttitudeState _attitude) noexcept {
//    attitude = _attitude;
//    Invalidate();
//  }

protected:
  /* virtual methods from AntiFlickerWindow */
  void OnPaintBuffer(Canvas &canvas) noexcept override {
    if (inverse)
      canvas.Clear(COLOR_BLACK);
    else
      canvas.ClearWhite();

    if (!attitude.bank_angle_available && !attitude.pitch_angle_available)
      // TODO: paint "no data" hint
      return;

    WeatherRenderer::Draw(canvas, canvas.GetRect(), look, attitude);
  }
};

void
WeatherWidget::Update([[maybe_unused]]const MoreData &basic) noexcept  // TODO(August2111): basic is unused?
{
  WeatherWindow &w = (WeatherWindow &)GetWindow();
//  w.ReadBlackboard(basic.attitude);
  w.Invalidate();
}

#include "Input/InputEvents.hpp"  // TODO(August2111) move
void
WeatherWidget::InternetUpdate() noexcept
{
  InputEvents::eventBeep(nullptr);
}

void
WeatherWidget::Prepare(ContainerWindow &parent, const PixelRect &rc) noexcept
{
  const Look &look = UIGlobals::GetLook();

  WindowStyle style;
  style.Hide();
  style.Disable();

  auto w = std::make_unique<WeatherWindow>(look.weather, look.info_box.inverse);
  w->Create(parent, rc, style);
  SetWindow(std::move(w));

  merge_thread->SetWeatherWidget(this);
}

#if 0
void 
WeatherWidget::OnPaint(Canvas &canvas)
{
  canvas.ClearWhite();

  if (bitmap == nullptr)
    return;

  const PixelSize bitmap_size = bitmap->GetSize();
  const PixelRect rc = GetClientRect();
  const PixelSize window_size = rc.GetSize();

  PixelSize fit_size(window_size.width, window_size.width * bitmap_size.height /
                                            bitmap_size.width);
  if (fit_size.height > window_size.height) {
    fit_size.height = window_size.height;
    fit_size.width =
        window_size.height * bitmap_size.width / bitmap_size.height;
  }

  canvas.Stretch(rc.CenteredTopLeft(fit_size), fit_size, *bitmap);
}
#endif

void
WeatherWidget::Show(const PixelRect &rc) noexcept
{
  Update(CommonInterface::Basic());
  CommonInterface::GetLiveBlackboard().AddListener(*this);

  WindowWidget::Show(rc);
}

void
WeatherWidget::Hide() noexcept
{
  WindowWidget::Hide();

  CommonInterface::GetLiveBlackboard().RemoveListener(*this);
}
