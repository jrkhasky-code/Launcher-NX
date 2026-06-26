#include "launcher.h"  
#include <switch.h>  
#include  
#include <dirent.h>  
#include

// Scans the config folder for all available custom boot groups (.ini files)  
std::vectorstd::string scanBootGroups() {  
std::vectorstd::string files;  
DIR\* dir = opendir("sdmc:/switch/NX-LAUNCHER/configs");  
if (!dir) return {"default.ini"}; // Fallback if directory isn't made yet

struct dirent\* ent;  
while ((ent = readdir(dir)) != NULL) {  
std::string name = ent->d\_name;  
if (name.find(".ini") != std::string::npos) {  
files.push\_back(name);  
}  
}  
closedir(dir);  
if (files.empty()) files.push\_back("default.ini");  
return files;  
}

// Simple parsing engine to convert INI data into a usable UI structure  
BootGroup loadConfig(const std::string& fileName) {  
BootGroup group;  
group.groupName = fileName.substr(0, fileName.find(".ini")); // Use file name as header

std::ifstream file("sdmc:/switch/NX-LAUNCHER/configs/" + fileName);  
if (!file.is\_open()) {  
// Create a default placeholder item if the file is missing  
group.items.push\_back({"Default Homebrew Menu", "sdmc:/hbmenu.nro", "NRO"});  
return group;  
}

std::string line;  
BootItem currentItem;  
bool buildingItem = false;

while (std::getline(file, line)) {  
// Strip white space or handle comments  
line.erase(std::remove(line.begin(), line.end(), '\\r'), line.end());  
if (line.empty() || line\[0\] == ';') continue;

if (line\[0\] == '\[' && line\[line.length() - 1\] == '\]') {  
if (buildingItem) {  
group.items.push\_back(currentItem);  
}  
currentItem = BootItem();  
currentItem.title = line.substr(1, line.length() - 2);  
buildingItem = true;  
} else {  
size\_t delim = line.find('=');  
if (delim != std::string::npos) {  
std::string key = line.substr(0, delim);  
std::string val = line.substr(delim + 1);

if (key == "path") currentItem.path = val;  
if (key == "type") currentItem.type = val;  
}  
}  
}  
if (buildingItem) {  
group.items.push\_back(currentItem);  
}  
return group;  
}

// Handoff logic that tells the Switch operating system what to execute next  
void bootTarget(const BootItem& item) {  
if (item.type == "NRO") {  
// Overrides the homebrew environment next-load register  
envSetNextLoad(item.path.c\_str(), item.path.c\_str());  
}  
else if (item.type == "PAYLOAD") {  
// Advanced: Requires bpc service to reboot hardware into a payload binary  
// Commonly used for Hekate/Atmosphere chaining  
bpcInitialize();  
// Custom implementations write payload to IRAM memory space here  
bpcRebootToPayload();  
}  
}
