#include "../../include/window/fileDialog.hpp"

#include <AppKit/AppKit.h>

// Opens MacOS file dialog to pick a file to load
std::string FileDialog::openDialog() {

  NSApplication *application = [NSApplication sharedApplication];
  [application setActivationPolicy:NSApplicationActivationPolicyAccessory];

  NSOpenPanel *openDlg = [NSOpenPanel openPanel];
  [openDlg setCanChooseFiles:YES];
  [openDlg setFloatingPanel:YES];

  if ([openDlg runModal] == NSModalResponseOK) {
    for (NSURL *URL in [openDlg URLs]) {
      NSLog(@"%@", [URL path]);
      return [URL.path UTF8String];
    }
  }

  return "";
}
