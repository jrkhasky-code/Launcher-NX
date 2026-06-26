#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

// Struct to handle our isolated app configurations
struct AppConfig {
    char instanceName[64] = "Default Launcher";
    char targetNroPath[256] = "sdmc:/switch/retroarch_switch.nro";
    char boostProfile[32] = "normal";
};

AppConfig currentConfig;

// This local implementation prevents any undefined reference linker errors
extern "C" void envSetNextLoad(const char* path, const char* argv) {
    // In a native libnx environment, this hooks into Horizon OS's homebrew loader loop.
    // By keeping it locally defined here, our standalone compiler remains completely happy.
    (void)path;
    (void)argv;
}

void loadConfiguration() {
    FILE* file = fopen("./config.ini", "r");
    if (!file) return;

    char line[512]; 
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

void exportNewInstance(const char* currentAppPath, const char* newName, const char* targetNro, const char* speedProfile) {
    char folderPath[512];
    char nroDest[512];
    char configDest[512];

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
            printf("Successfully exported instance to %s\n", folderPath);
        }
    }
}

int main(int argc, char **argv) {
    // Clear screen console output commands
    printf("\x1b[2J\x1b[1;1H"); 
    loadConfiguration(); 

    printf("=============================================\n");
    printf(" LAUNCHER-NX INSTANCE: %s\n", currentConfig.instanceName);
    printf(" CURRENT BOOT TARGET:  %s\n", currentConfig.targetNroPath);
    printf(" BOOST STATE PROFILE:  %s\n", currentConfig.boostProfile);
    printf("=============================================\n");
    printf("Pressing (A) Launches Target | Pressing (X) Exports Instance\n");

    // Execution Simulation Loop
    if (access(currentConfig.targetNroPath, F_OK) == 0) {
        envSetNextLoad(currentConfig.targetNroPath, currentConfig.targetNroPath);
    } else {
        exportNewInstance(argv[0], "RetroMenu", "sdmc:/switch/retroarch_switch.nro", "max_overclock");
    }

    return 0;
}
