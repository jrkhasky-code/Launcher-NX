#include <switch.h>  
#include <stdio.h>  
#include "launcher.h"

int main(int argc, char \*\*argv) {  
gfxInitDefault();  
consoleInit(NULL);

// Initialize state engines  
std::vectorstd::string availableGroups = scanBootGroups();  
size\_t activeGroupIdx = 0;  
BootGroup activeGroup = loadConfig(availableGroups\[activeGroupIdx\]);  
size\_t selectedItemIdx = 0;

while (appletMainLoop()) {  
hidScanInput();  
u64 kDown = hidKeysDown(CONTROLLER\_P1\_AUTO);

// Grid boundaries safety checks  
if (activeGroup.items.empty()) selectedItemIdx = 0;  
else if (selectedItemIdx >= activeGroup.items.size()) selectedItemIdx = activeGroup.items.size() - 1;

// Clear screen logic  
printf("\\x1b\[2J\\x1b\[H");

// UI Top Bar Design (Hekate Inspired Theme)  
printf("===================================================================\\n");  
printf(" NX-LAUNCHER v1.0.0 | Active Instance: \[%s\]\\n", activeGroup.groupName.c\_str());  
printf("===================================================================\\n\\n");

printf(" \[L/R\] Cycle Boot Groups | \[D-Pad Up/Down\] Select Target | \[A\] Boot\\n\\n");

// Render the selectable items block  
for (size\_t i = 0; i < activeGroup.items.size(); i++) {  
if (i == selectedItemIdx) {  
printf(" -> \* %s \* (%s) \\n", activeGroup.items\[i\].title.c\_str(), activeGroup.items\[i\].type.c\_str());  
printf(" Path: %s\\n\\n", activeGroup.items\[i\].path.c\_str());  
} else {  
printf(" \[ \] %s \\n\\n", activeGroup.items\[i\].title.c\_str());  
}  
}

// Input Controller Processing Engine  
if (kDown & KEY\_PLUS) break; // Exit application normally to Homebrew Menu

if (kDown & KEY\_DOWN) {  
if (selectedItemIdx < activeGroup.items.size() - 1) selectedItemIdx++;  
}  
if (kDown & KEY\_UP) {  
if (selectedItemIdx > 0) selectedItemIdx--;  
}

// Switch "Instances" / Configuration Profiles  
if (kDown & KEY\_R) {  
activeGroupIdx = (activeGroupIdx + 1) % availableGroups.size();  
activeGroup = loadConfig(availableGroups\[activeGroupIdx\]);  
selectedItemIdx = 0;  
}  
if (kDown & KEY\_L) {  
activeGroupIdx = (activeGroupIdx - 1 + availableGroups.size()) % availableGroups.size();  
activeGroup = loadConfig(availableGroups\[activeGroupIdx\]);  
selectedItemIdx = 0;  
}

// Fire Execution Trigger  
if (kDown & KEY\_A && !activeGroup.items.empty()) {  
bootTarget(activeGroup.items\[selectedItemIdx\]);  
break; // Terminate app context loop to allow target loading  
}

gfxFlushBuffers();  
gfxSwapBuffers();  
gfxWaitForVblank();  
}

gfxExit();  
return 0;  
}
