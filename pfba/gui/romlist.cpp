//
// Created by cpasjuste on 19/10/16.
//

#include <vector>
#include <string>
#include <algorithm>
#include <skeleton/io.h>

#include "burner.h"
#include "romlist.h"

RomList::RomList(Io *io, std::vector<Hardware> *hwList, const std::vector<std::string> &paths, Renderer *rdr, Skin *skin) {

    hardwareList = hwList;

    printf("RomList: building list...\n");
    clock_t begin = clock();

	char szText[512];
	Rect rcText;
	
	memset(szText, 0, sizeof(szText));

	rcText.x = 20;
	rcText.w = rdr->width - 20 - 10;
	rcText.y = rdr->height - 10 - skin->font->size;
	rcText.h = skin->font->size;

    std::vector<std::string> files[DIRS_MAX];
    for (unsigned int i = 0; i < paths.size(); i++) {
        if (!paths[i].empty()) {
            files[i] = io->GetDirList(paths[i].c_str());
            printf("RomList: found %i files in `%s`\n", (int) files[i].size(), paths[i].c_str());

			sprintf(szText, "Found %i files in %s", (int) files[i].size(), paths[i].c_str());
			rdr->Clear();
			DrawSplashScreen(rdr, skin);
			skin->font->Draw(rcText, C2D_COL_WHITE, szText);
			rdr->Flip();
        }
    }

    char path[MAX_PATH];
    char pathUppercase[MAX_PATH]; // sometimes on FAT32 short files appear as all uppercase
    // TODO: fix sorting speed so we don't skip first 24 roms
    for (UINT32 i = 0/*24*/; i < nBurnDrvCount; i++) {

        nBurnDrvActive = i;

        Rom rom;
        rom.zip = BurnDrvGetTextA(DRV_NAME);
        rom.parent = BurnDrvGetTextA(DRV_PARENT);
        rom.name = BurnDrvGetTextA(DRV_FULLNAME);
        rom.year = BurnDrvGetTextA(DRV_DATE);
        rom.manufacturer = BurnDrvGetTextA(DRV_MANUFACTURER);
        rom.system = BurnDrvGetTextA(DRV_SYSTEM);
        rom.genre = BurnDrvGetGenreFlags();
        rom.flags = BurnDrvGetFlags();
        rom.state = RomState::MISSING;
        rom.hardware = BurnDrvGetHardwareCode();

        // add rom to "ALL" game list
        hardwareList->at(0).supported_count++;
        if (rom.parent) {
            hardwareList->at(0).clone_count++;
        }

        // add rom to specific hardware
        Hardware *hardware = GetHardware(rom.hardware);
        if (hardware) {
            hardware->supported_count++;
            if (rom.parent) {
                hardware->clone_count++;
            }
        }

        for (int j = 0; j < DIRS_MAX; j++) {
            if (files[j].empty()) {
                continue;
            }
            sprintf(path, "%s.zip", rom.zip);
            for (int k=0; k<MAX_PATH; k++) {
                pathUppercase[k] = toupper(path[k]);
            }
            if (std::find(files[j].begin(), files[j].end(), path) != files[j].end() || 
                std::find(files[j].begin(), files[j].end(), pathUppercase) != files[j].end()) {
                rom.state = BurnDrvIsWorking() ? RomState::WORKING : RomState::NOT_WORKING;
                hardwareList->at(0).available_count++;
                if (rom.parent) {
                    hardwareList->at(0).available_clone_count++;
                }
                if (hardware) {
                    hardware->available_count++;
                    if (rom.parent) {
                        hardware->available_clone_count++;
                    }
                }
                break;
            }
        }

        if (rom.state == RomState::MISSING) {
            hardwareList->at(0).missing_count++;
            if (hardware) {
                hardware->missing_count++;
            }
            if (rom.parent) {
                hardwareList->at(0).missing_clone_count++;
                if (hardware) {
                    hardware->missing_clone_count++;
                }
            }
        }

        list.push_back(rom);

		if (hardwareList->at(0).supported_count % 250 == 0)
		{
			sprintf(szText, "Roms: %i/%i | Clones: %i/%i", 
							hardwareList->at(0).available_count, hardwareList->at(0).supported_count,
							hardwareList->at(0).available_clone_count, hardwareList->at(0).clone_count);
			rdr->Clear();
			DrawSplashScreen(rdr, skin);
			skin->font->Draw(rcText, C2D_COL_WHITE, szText);
			rdr->Flip();
		} // if
    }

    /*
    for(int i=0; i<hardwareList->size(); i++) {
        printf("[%s] roms: %i/%i | clones: %i/%i)\n",
               hardwareList->at(i).name.c_str(),
               hardwareList->at(i).available_count, hardwareList->at(i).supported_count,
               hardwareList->at(i).available_clone_count, hardwareList->at(i).clone_count);
    }
    */

    for (int i = 0; i < DIRS_MAX; i++) {
        files[i].clear();
    }

	sprintf(szText, "Roms: %i/%i | Clones: %i/%i", 
					hardwareList->at(0).available_count, hardwareList->at(0).supported_count,
					hardwareList->at(0).available_clone_count, hardwareList->at(0).clone_count);
	rdr->Clear();
	DrawSplashScreen(rdr, skin);
	skin->font->Draw(rcText, C2D_COL_WHITE, szText);
	rdr->Flip();
	rdr->Delay(2000);

    clock_t end = clock();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;
    printf("RomList: list built in %f\n", time_spent);
}

RomList::~RomList() {
    list.clear();
}

void RomList::DrawSplashScreen(Renderer *rdr, Skin *skin)
{
	Rect rcScreen{0, 0, rdr->width, rdr->height};

	rdr->DrawRect(rcScreen, C2D_COL_GRAY);
	rdr->DrawRect(rcScreen, C2D_COL_ORANGE, false);
	if (skin->tex_title->available)
	{
		rcScreen.h -= 32;
		skin->tex_title->Draw(rcScreen, true);
	} // if
} // End of DrawSplashScreen

