#ifndef LAUNCHER\_H  
#define LAUNCHER\_H

#include  
#include

// Represents an individual boot choice (Game, Emulator, Tool)  
struct BootItem {  
std::string title;  
std::string path;  
std::string type; // "NRO", "PAYLOAD", or "TITLEID"  
};

// Represents a standalone group configuration file (An "Instance")  
struct BootGroup {  
std::string groupName;  
std::vector items;  
};

// Core functions  
std::vectorstd::string scanBootGroups();  
BootGroup loadConfig(const std::string& fileName);  
void bootTarget(const BootItem& item);

#endif
