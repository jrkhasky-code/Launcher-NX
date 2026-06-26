#include <switch.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

// Global structure to hold our current launcher instance configuration settings
struct AppConfig {
    char instanceName[64] = "Default Launcher";
    char targetNroPath[256] = "sdmc:/switch/retroarch_switch.nro";
    char boostProfile[32] = "normal";
};

AppConfig currentConfig;

// Reads text configs locally from the app's current directory
void loadConfiguration() {
    FILE* file = fopen("./config.ini", "r");
    if (!file) return;

    char line[256]; 
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0; 
        if (line[0] == ';' || line[0] == '#' || line[0] == '[') continue; 

        char* eq = strchr(line, '=');
        if (eq) {
            *eq = '\0';
            char* key = line;
            char* value = eq + 1;

            if (strcmp(key, "name") == 0) strncpy(currentConfig.instanceName, value, sizeof(currentConfig.instanceName)-1);
            if (strcmp(key, "target_boot_path") == 0) strncpy(currentConfig.targetNroPath, value, sizeof(currentConfig.targetNroPath)-1);
            if (strcmp(key, "boost_profile") == 0) strncpy(currentConfig.boostProfile, value, sizeof(currentConfig.boostProfile)-1);
        }
    }
    fclose(file);
}

// Low-level function to clone the binary stream safely
bool copyFile(const char* src, const char* dest) {
    FILE* source = fopen(src, "rb");
    FILE* target = fopen(dest, "wb");
    if (!source || !target) {
        if (source) fclose(source);
        if (target) fclose(target);
        return false;
    }
    char buffer[4096]; 
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        fwrite(buffer, 1, bytesRead, target);
    }
    fclose(source);
    fclose(target);
    return true;
}

// Clones this exact running binary and outputs a standalone target workspace configuration under Launcher-NX layout rules
void exportNewInstance(const char* currentAppPath, const char* newName, const char* targetNro, const char* speedProfile) {
    char folderPath[256];
    char nroDest[256];
    char configDest[256];

    snprintf(folderPath, sizeof(folderPath), "sdmc:/switch/Launcher-NX_%s", newName);
    snprintf(nroDest, sizeof(nroDest), "sdmc:/switch/Launcher-NX_%s/Launcher-NX_%s.nro", newName, newName);
    snprintf(configDest, sizeof(configDest), "sdmc:/switch/Launcher-NX_%s/config.ini", newName);

    mkdir(folderPath, 0777); 

    if (copyFile(currentAppPath, nroDest)) { 
        FILE* configFile = fopen(configDest, "w");
        if (configFile) {
            fprintf(configFile, "[InstanceSettings]\n");
            fprintf(configFile, "name=%s\n", newName);
            fprintf(configFile, "target_boot_path=%s\n", targetNro);
            fprintf(configFile, "boost_profile=%s\n", speedProfile);
            fclose(configFile);
            printf("\x1b[12;1HSuccessfully exported instance to %s!", folderPath);
        }
    } else {
        printf("\x1b[12;1HFailed to export instance binaries.");
    }
}

int main(int argc, char **argv) {
    consoleInit(NULL); 
    loadConfiguration(); 

    PadState pad;
    padInitializeDefault(&pad);

    printf("\x1b[1;1H=============================================");
    printf("\x1b[2;1H LAUNCHER-NX INSTANCE: %s", currentConfig.instanceName);
    printf("\x1b[3;1H CURRENT BOOT TARGET:  %s", currentConfig.targetNroPath);
    printf("\x1b[4;1H BOOST STATE PROFILE:  %s", currentConfig.boostProfile);
    printf("\x1b[5;1H=============================================");
    
    printf("\x1b[7;1HPress (A) to LAUNCH Target App with Boost Profile");
    printf("\x1b[8;1HPress (X) to EXPORT a brand new 'Retro' Instance");
    printf("\x1b[10;1HPress (+) to Exit back to Homebrew Menu");

    bool launchTriggered = false;

    while(appletMainLoop()) {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus) break; 

        if (kDown & HidNpadButton_A) {
            launchTriggered = true; 
            break; 
        }

        if (kDown & HidNpadButton_X) {
            exportNewInstance(argv[0], "RetroMenu", "sdmc:/switch/retroarch_switch.nro", "max_overclock");
        }

        consoleUpdate(NULL); 
    }

    if (launchTriggered) {
        if (access(currentConfig.targetNroPath, F_OK) == 0) {
            envSetNextLoad(currentConfig.targetNroPath, currentConfig.targetNroPath);
        } else {
            consoleInit(NULL);
            printf("Launcher-NX Error: Target path not found!\n%s\n", currentConfig.targetNroPath);
            printf("\nPress (+) to return to system.");
            while(appletMainLoop()) {
                padUpdate(&pad);
                if (padGetButtonsDown(&pad) & HidNpadButton_Plus) break;
                consoleUpdate(NULL);
            }
        }
    }

    consoleExit(NULL);
    return 0;
}
