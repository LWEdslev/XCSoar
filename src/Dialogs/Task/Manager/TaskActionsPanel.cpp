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

#include "TaskActionsPanel.hpp"
#include "TaskMiscPanel.hpp"
#include "TaskListPanel.hpp"
#include "Internal.hpp"
#include "Dialogs/Message.hpp"
#include "Dialogs/Task/dlgTaskHelpers.hpp"
#include "Components.hpp"
#include "Logger/ExternalLogger.hpp"
#include "Simulator.hpp"
#include "Language/Language.hpp"
#include "Interface.hpp"
#include "Device/Declaration.hpp"
#include "Task/ValidationErrorStrings.hpp"
#include "Engine/Task/Ordered/OrderedTask.hpp"
#include "Engine/Task/Factory/AbstractTaskFactory.hpp"
#include "Engine/Waypoint/Waypoints.hpp"
#include "system/Path.hpp"
#include "system/FileUtil.hpp"

#include "Cloud/weglide/DownloadTask.hpp"
#include "Cloud/weglide/WeGlideObjects.hpp"
#include "Dialogs/Contest/WeGlide/TaskDownloadDialog.hpp"

TaskActionsPanel::TaskActionsPanel(TaskManagerDialog &_dialog,
                                   TaskMiscPanel &_parent,
                                   std::unique_ptr<OrderedTask> &_active_task,
                                   bool *_task_modified) noexcept
  :RowFormWidget(_dialog.GetLook()),
   dialog(_dialog), parent(_parent),
   active_task(_active_task), task_modified(_task_modified) {}

void
TaskActionsPanel::SaveTask()
{
  AbstractTaskFactory &factory = active_task->GetFactory();
  factory.UpdateStatsGeometry();
  if (factory.CheckAddFinish())
    factory.UpdateGeometry();

  const auto errors = active_task->CheckTask();
  if (!IsError(errors)) {
    if (!OrderedTaskSave(*active_task))
      return;

    *task_modified = true;
    dialog.UpdateCaption();
    DirtyTaskListPanel();
  } else {
    ShowMessageBox(getTaskValidationErrors(errors), _("Task not saved"),
        MB_ICONEXCLAMATION);
  }
}

inline void
TaskActionsPanel::OnBrowseClicked()
{
  parent.SetCurrent(1);
}

inline void
TaskActionsPanel::OnNewTaskClicked()
{
  if ((active_task->TaskSize() < 2) ||
      (ShowMessageBox(_("Create new task?"), _("Task New"),
                   MB_YESNO|MB_ICONQUESTION) == IDYES)) {
    active_task->Clear();
    active_task->SetFactory(CommonInterface::GetComputerSettings().task.task_type_default);
    *task_modified = true;
    dialog.SwitchToPropertiesPanel();
  }
}

inline void
TaskActionsPanel::OnDeclareClicked()
{
  const auto errors = active_task->CheckTask();
  if (IsError(errors)) {
    ShowMessageBox(getTaskValidationErrors(errors), _("Declare task"),
                MB_ICONEXCLAMATION);
    return;
  }

  const ComputerSettings &settings = CommonInterface::GetComputerSettings();
  Declaration decl(settings.logger, settings.plane, active_task.get());
  ExternalLogger::Declare(decl, way_points.GetHome().get());
}

inline void
TaskActionsPanel::OnOwnWeGlideClicked()
{
  auto pilot =  CommonInterface::GetComputerSettings().weglide.pilot; // the preset value
  TaskDownloadDialog(pilot, nullptr);
//  const auto task_file = WeGlide::DownloadTaskFile(pilot);
//  // August2111:    if (WeGlide::DownloadTaskDialog()) {
//  if (File::Exists(task_file))
//    DirtyTaskListPanel();
}

inline void
TaskActionsPanel::OnUserWeGlideClicked()
{
  const auto task_file = WeGlide::DownloadTaskFile(WeGlide::User(1752)); // zum Test: Thomas Melde...
// August2111:    if (WeGlide::DownloadTaskDialog()) {
  if (File::Exists(task_file))
    DirtyTaskListPanel();
}

void
TaskActionsPanel::ReClick() noexcept
{
  dialog.TaskViewClicked();
}

void
TaskActionsPanel::Prepare([[maybe_unused]] ContainerWindow &parent,
                          [[maybe_unused]] const PixelRect &rc) noexcept
{
  const auto &settings = CommonInterface::GetComputerSettings();

  AddButton(_("New Task"), [this](){ OnNewTaskClicked(); });
  AddButton(_("Declare"), [this](){ OnDeclareClicked(); });
  AddButton(_("Browse"), [this](){ OnBrowseClicked(); });
  AddButton(_("Save"), [this](){ SaveTask(); });
  AddButton(_("Own WeGlide Task"), [this]() { OnOwnWeGlideClicked(); });
  AddButton(_("User WeGlide Task"), [this]() { OnUserWeGlideClicked(); });

  if (settings.weglide.pilot_id != 0)
    AddButton(_("Download WeGlide task"),
              [this](){ OnDownloadClicked(); });

  if (is_simulator())
    /* cannot communicate with real devices in simulator mode */
    SetRowEnabled(DECLARE, false);
}
