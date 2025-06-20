// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "InputEvents.hpp"
#include "Language/Language.hpp"
#include "Message.hpp"
#include "Interface.hpp"
#include "ActionInterface.hpp"
#include "Profile/Profile.hpp"
#include "Profile/Keys.hpp"
#include "UIGlobals.hpp"
#include "MapWindow/GlueMapWindow.hpp"
#include "Units/Units.hpp"
#include "UIState.hpp"
#include "Pan.hpp"
#include "PageActions.hpp"
#include "Math/Constants.hpp"
#include "Screen/Layout.hpp"

#include <algorithm> // for std::clamp()

// eventAutoZoom - Turn on|off|toggle AutoZoom
// misc:
//	auto on - Turn on if not already
//	auto off - Turn off if not already
//	auto toggle - Toggle current full screen status
//	auto show - Shows autozoom status
//	+	- Zoom in
//	++	- Zoom in near
//	-	- Zoom out
//	--	- Zoom out far
//	n.n	- Zoom to a set scale
//	show - Show current zoom scale
void
InputEvents::eventZoom(const TCHAR* misc)
{
  // JMW pass through to handler in MapWindow
  // here:
  // -1 means toggle
  // 0 means off
  // 1 means on

  MapSettings &settings_map = CommonInterface::SetMapSettings();

  if (StringIsEqual(misc, _T("auto toggle")))
    sub_AutoZoom(-1);
  else if (StringIsEqual(misc, _T("auto on")))
    sub_AutoZoom(1);
  else if (StringIsEqual(misc, _T("auto off")))
    sub_AutoZoom(0);
  else if (StringIsEqual(misc, _T("auto show"))) {
    if (settings_map.auto_zoom_enabled)
      Message::AddMessage(_("Auto. zoom on"));
    else
      Message::AddMessage(_("Auto. zoom off"));
  } else if (StringIsEqual(misc, _T("slowout")))
    sub_ScaleZoom(-1);
  else if (StringIsEqual(misc, _T("slowin")))
    sub_ScaleZoom(1);
  else if (StringIsEqual(misc, _T("out")))
    sub_ScaleZoom(-1);
  else if (StringIsEqual(misc, _T("in")))
    sub_ScaleZoom(1);
  else if (StringIsEqual(misc, _T("-")))
    sub_ScaleZoom(-1);
  else if (StringIsEqual(misc, _T("+")))
    sub_ScaleZoom(1);
  else if (StringIsEqual(misc, _T("--")))
    sub_ScaleZoom(-2);
  else if (StringIsEqual(misc, _T("++")))
    sub_ScaleZoom(2);
  else if (StringIsEqual(misc, _T("circlezoom toggle"))) {
    settings_map.circle_zoom_enabled = !settings_map.circle_zoom_enabled;
  } else if (StringIsEqual(misc, _T("circlezoom on"))) {
    settings_map.circle_zoom_enabled = true;
  } else if (StringIsEqual(misc, _T("circlezoom off"))) {
    settings_map.circle_zoom_enabled = false;
  } else if (StringIsEqual(misc, _T("circlezoom show"))) {
    if (settings_map.circle_zoom_enabled)
      Message::AddMessage(_("Circling zoom on"));
    else
      Message::AddMessage(_("Circling zoom off"));
  } else {
    TCHAR *endptr;
    double zoom = _tcstod(misc, &endptr);
    if (endptr == misc)
      return;

    sub_SetZoom(Units::ToSysDistance(zoom));
  }

  XCSoarInterface::SendMapSettings(true);
}

/**
 * This function handles all "pan" input events
 * @param misc A string describing the desired pan action.
 *  on             Turn pan on
 *  off            Turn pan off
 *  toogle         Toogles pan mode
 *  up             Pan up
 *  down           Pan down
 *  left           Pan left
 *  right          Pan right
 *  @todo feature: n,n Go that direction - +/-
 *  @todo feature: ??? Go to particular point
 *  @todo feature: ??? Go to waypoint (eg: next, named)
 */
void
InputEvents::eventPan(const TCHAR *misc)
{
  if (StringIsEqual(misc, _T("toggle")))
    TogglePan();

  else if (StringIsEqual(misc, _T("on")))
    EnterPan();

  else if (StringIsEqual(misc, _T("off")))
    LeavePan();

  else if (StringIsEqual(misc, _T("up")))
    sub_PanCursor(0, 1);

  else if (StringIsEqual(misc, _T("down")))
    sub_PanCursor(0, -1);

  else if (StringIsEqual(misc, _T("left")))
    sub_PanCursor(1, 0);

  else if (StringIsEqual(misc, _T("right")))
    sub_PanCursor(-1, 0);

  XCSoarInterface::SendMapSettings(true);
}

void
InputEvents::sub_PanCursor(int dx, int dy)
{
  GlueMapWindow *map_window = UIGlobals::GetMapIfActive();
  if (map_window == NULL || !map_window->IsPanning())
    return;

  const WindowProjection &projection = map_window->VisibleProjection();
  if (!projection.IsValid())
    return;

  auto pt = projection.GetScreenOrigin();
  pt.x -= dx * Layout::FastScale(40);
  pt.y -= dy * Layout::FastScale(40);
  map_window->SetLocation(projection.ScreenToGeo(pt));

  map_window->QuickRedraw();
}

// called from UI or input event handler (same thread)
void
InputEvents::sub_AutoZoom(int vswitch)
{
  MapSettings &settings_map = CommonInterface::SetMapSettings();

  if (vswitch == -1)
    settings_map.auto_zoom_enabled = !settings_map.auto_zoom_enabled;
  else
    settings_map.auto_zoom_enabled = (vswitch != 0); // 0 off, 1 on

  Profile::Set(ProfileKeys::AutoZoom, settings_map.auto_zoom_enabled);

  if (settings_map.auto_zoom_enabled &&
      UIGlobals::GetMap() != NULL)
    UIGlobals::GetMap()->SetPan(false);

  ActionInterface::SendMapSettings(true);
}

void
InputEvents::sub_SetZoom(double value)
{
  MapSettings &settings_map = CommonInterface::SetMapSettings();
  GlueMapWindow *map_window = PageActions::ShowMap();
  if (map_window == NULL)
    return;

  const DisplayMode displayMode = CommonInterface::GetUIState().display_mode;
  if (settings_map.auto_zoom_enabled &&
      !(displayMode == DisplayMode::CIRCLING && settings_map.circle_zoom_enabled) &&
      !IsPanning()) {
    settings_map.auto_zoom_enabled = false;  // disable autozoom if user manually changes zoom
    Profile::Set(ProfileKeys::AutoZoom, false);
    Message::AddMessage(_("Auto. zoom off"));
  }

  const double scale_100m = 10;
  const double scale_1600km = 1600*100;
  auto minreasonable = scale_100m;
  value = std::clamp(value, minreasonable, scale_1600km);
  map_window->SetMapScale(value);
  map_window->QuickRedraw();
}

void
InputEvents::sub_ScaleZoom(int vswitch)
{
  const GlueMapWindow *map_window = PageActions::ShowMap();
  if (map_window == NULL)
    return;

  const MapWindowProjection &projection =
      map_window->VisibleProjection();
  if (!projection.IsValid())
    return;

  auto value = projection.GetMapScale();

  if (projection.HaveScaleList()) {
    value = projection.StepMapScale(value, -vswitch);
  } else {
    if (vswitch == 1)
      // zoom in a little
      value /= M_SQRT2;
    else if (vswitch == -1)
      // zoom out a little
      value *= M_SQRT2;
    else if (vswitch == 2)
      // zoom in a lot
      value /= 2;
    else if (vswitch == -2)
      // zoom out a lot
      value *= 2;
  }

  sub_SetZoom(value);
}
