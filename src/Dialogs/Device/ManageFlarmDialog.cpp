/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2022 The XCSoar Project
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

#include "ManageFlarmDialog.hpp"
#include "FLARM/ConfigWidget.hpp"
#include "Dialogs/WidgetDialog.hpp"
#include "Dialogs/Error.hpp"
#include "Widget/RowFormWidget.hpp"
#include "UIGlobals.hpp"
#include "Language/Language.hpp"
#include "Operation/MessageOperationEnvironment.hpp"
#include "Device/Driver/FLARM/Device.hpp"
#include "FLARM/Version.hpp"
#include "FLARM/DeviceInformation.hpp"

class ManageFLARMWidget final
  : public RowFormWidget {
  enum Controls {
    Setup,
    Reboot,
  };

  FlarmDevice &device;
  const FlarmVersion version;  // TODO: delete this! and call with DeviceInformation
  FLARM::DeviceInformation flarm_info;
  const unsigned device_index;

public:
  ManageFLARMWidget(const DialogLook &look, FlarmDevice &_device,
                    const unsigned _device_index,
                    const FlarmVersion &version)
      : RowFormWidget(look), device(_device), device_index(_device_index),
        version(version) {}

  /* virtual methods from Widget */
  void Prepare(ContainerWindow &parent, const PixelRect &rc) noexcept override;
};

void
ManageFLARMWidget::Prepare([[maybe_unused]] ContainerWindow &parent,
                           [[maybe_unused]] const PixelRect &rc) noexcept
{
  MessageOperationEnvironment env;
  device.CollectDeviceInformation(flarm_info, env);

  if (!flarm_info.device_type.empty()) {
    AddReadOnly(_("Device Type"), nullptr, flarm_info.device_type.c_str());
  }

  if (!flarm_info.bootloader_version.empty()) {
    AddReadOnly(_("Flarm Version"), nullptr,
                flarm_info.bootloader_version.c_str());
  }

  if (!flarm_info.software_version.empty()) {
    AddReadOnly(_("Firmware Version"), nullptr,
                flarm_info.software_version.c_str());
  }

  if (!flarm_info.fw_expiry_date.empty()) {
    AddReadOnly(_("FW Expiry Date"), nullptr,
                flarm_info.fw_expiry_date.c_str());
  }

  if (!flarm_info.radio_id.empty()) {
    AddReadOnly(_("Radio ID"), nullptr, flarm_info.radio_id.c_str());
  }

  if (!flarm_info.serial_id.empty()) {
    AddReadOnly(_("Serial ID"), nullptr, flarm_info.serial_id.c_str());
  }

  if (!flarm_info.obstacle_version.empty()) {
    AddReadOnly(_("Obstacle database"), nullptr, flarm_info.obstacle_version.c_str());
  }


  AddButton(_("Setup"), [this](){
    FLARMConfigWidget widget(GetLook(), device, device_index);
    DefaultWidgetDialog(UIGlobals::GetMainWindow(), GetLook(),
                        _T("FLARM"), widget);
  });

  AddButton(_("Reboot"), [this](){
    try {
      MessageOperationEnvironment env;
      device.Restart(env);
    } catch (OperationCancelled) {
    } catch (...) {
      ShowError(std::current_exception(), _("Error"));
    }
  });
}

void ManageFlarmDialog(Device &device, const unsigned device_index,
                       const FlarmVersion &version) {
  WidgetDialog dialog(WidgetDialog::Auto{}, UIGlobals::GetMainWindow(),
                      UIGlobals::GetDialogLook(),
                      _T("FLARM"),
                      new ManageFLARMWidget(UIGlobals::GetDialogLook(),
                        (FlarmDevice &)device,
                        device_index, version));
  dialog.AddButton(_("Close"), mrCancel);
  dialog.ShowModal();

//   MessageOperationEnvironment env;
//   device.EnableNMEA(env);
}
