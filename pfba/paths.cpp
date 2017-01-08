
// Paths module
#include <unistd.h>
#include <sys/stat.h>
#include "burner.h"

#ifdef __PSP2__
#include <psp2/io/dirent.h>
char szAppHomePath[MAX_PATH] = "ux0:/data/vfba";
char szAppSavePath[MAX_PATH] = "ux0:/data/vfba/saves";
char szAppConfigPath[MAX_PATH] = "ux0:/data/vfba/configs";
char szAppHiscorePath[MAX_PATH] = "ux0:/data/vfba/hiscore";
char szAppSamplesPath[MAX_PATH] = "ux0:/data/vfba/samples";
char szAppPreviewPath[MAX_PATH] = "ux0:/data/vfba/previews";
char szAppTitlePath[MAX_PATH] = "ux0:/data/vfba/titles";
char szAppBlendPath[MAX_PATH] = "ux0:/data/vfba/blend";
char szAppSkinPath[MAX_PATH] = "app0:/skin";
#else
char szAppHomePath[MAX_PATH];
char szAppSavePath[MAX_PATH];
char szAppConfigPath[MAX_PATH];
char szAppHiscorePath[MAX_PATH];
char szAppSamplesPath[MAX_PATH];
char szAppPreviewPath[MAX_PATH];
char szAppTitlePath[MAX_PATH];
char szAppBlendPath[MAX_PATH];
//char szAppRomPaths[DIRS_MAX][MAX_PATH];
char szAppSkinPath[MAX_PATH];
#endif

void BurnPathsInit()
{
#ifndef __PSP2__ // TODO : crash on psp2 ?!
    getcwd(szAppHomePath, MAX_PATH);
    strcat(szAppHomePath, "/");
    mkdir(szAppHomePath, 0777);
    //printf("szAppHomePath: %s\n", szAppHomePath);

    snprintf(szAppSavePath, MAX_PATH, "%s%s", szAppHomePath, "saves");
    mkdir(szAppSavePath, 0777);
    //printf("szAppSavePath: %s\n", szAppSavePath);

    snprintf(szAppConfigPath, MAX_PATH, "%s%s", szAppHomePath, "configs");
    mkdir(szAppConfigPath, 0777);
    //printf("szAppConfigPath: %s\n", szAppConfigPath);

    snprintf(szAppHiscorePath, MAX_PATH, "%s%s", szAppHomePath, "hiscore");
    mkdir(szAppHiscorePath, 0777);
    //printf("szAppHiscorePath: %s\n", szAppHiscorePath);

    snprintf(szAppSamplesPath, MAX_PATH, "%s%s", szAppHomePath, "samples");
    mkdir(szAppSamplesPath, 0777);
    //printf("szAppSamplesPath: %s\n", szAppSamplesPath);

    snprintf(szAppPreviewPath, MAX_PATH, "%s%s", szAppHomePath, "previews");
    mkdir(szAppPreviewPath, 0777);
    //printf("szAppPreviewPath: %s\n", szAppPreviewPath);

    snprintf(szAppTitlePath, MAX_PATH, "%s%s", szAppHomePath, "titles");
    mkdir(szAppTitlePath, 0777);
    //printf("szAppTitlePath: %s\n", szAppTitlePath);

    snprintf(szAppBlendPath, MAX_PATH, "%s%s", szAppHomePath, "blend");
    mkdir(szAppBlendPath, 0777);
    //printf("szAppBlendPath: %s\n", szAppBlendPath);

    snprintf(szAppSkinPath, MAX_PATH, "%s%s", szAppHomePath, "skin");
    mkdir(szAppSkinPath, 0777);
    //printf("szAppSkinPath: %s\n", szAppSkinPath);

    //snprintf(szAppRomPaths[0], MAX_PATH, "%s%s", szAppHomePath, "roms/");
    //mkdir(szAppRomPaths[0], 0777);
    //printf("szAppRomPaths[0]: %s\n", szAppRomPaths[0]);
#endif
}
