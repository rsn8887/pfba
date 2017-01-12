#include <SDL2/SDL.h>
#include <skeleton/input.h>

#include "burner.h"


//TODO: extern unsigned char ServiceRequest;
//TODO: extern unsigned char P1P2Start;
unsigned char ServiceRequest;
unsigned char P1P2Start;

#define MAX_INPUT_inp (19)

struct GameInput {
    //unsigned char *pVal;  // Destination for the Input Value
    union {
        UINT8 *pVal;                    // Most inputs use a char*
        UINT16 *pShortVal;                // All analog inputs use a short*
    };
    //int *ipVal;			 //for x axis steering
    unsigned char nType;  // 0=binary (0,1) 1=analog (0x01-0xFF) 2=dip switch
    unsigned char nConst;
    int nBit;   // bit offset of Keypad data
};

struct DIPInfo {
    unsigned char nDIP;
    unsigned short nFirstDIP;
    struct GameInput *DIPData;
} DIPInfo;
// Mapping of PC inputs to game inputs
struct GameInput GameInput[4][MAX_INPUT_inp];
unsigned int nGameInpCount = 0;
static bool bInputOk = false;
unsigned char *ServiceDip = 0;
unsigned char *P1Start = 0;
unsigned char *P2Start = 0;

int DoInputBlank(int /*bDipSwitch*/) {
    int iJoyNum = 0;
    unsigned int i = 0;
    // Reset all inputs to undefined (even dip switches, if bDipSwitch==1)
    char controlName[MAX_INPUT_inp];


    DIPInfo.nDIP = 0;
    // Get the targets in the library for the Input Values
    for (i = 0; i < nGameInpCount; i++) {
        struct BurnInputInfo bii;
        memset(&bii, 0, sizeof(bii));
        BurnDrvGetInputInfo(&bii, i);

        printf("c %s\n", bii.szInfo);

        //if (bDipSwitch==0 && bii.nType==2) continue; // Don't blank the dip switches

        if (bii.nType == BIT_DIPSWITCH) {
            if (DIPInfo.nDIP == 0) {
                DIPInfo.nFirstDIP = i;
                DIPInfo.nDIP = nGameInpCount - i;
                DIPInfo.DIPData = (struct GameInput *) malloc(DIPInfo.nDIP * sizeof(struct GameInput));
                memset(DIPInfo.DIPData, 0, DIPInfo.nDIP * sizeof(struct GameInput));
            }
            DIPInfo.DIPData[i - DIPInfo.nFirstDIP].pVal = bii.pVal;
            DIPInfo.DIPData[i - DIPInfo.nFirstDIP].nType = bii.nType;
            DIPInfo.DIPData[i - DIPInfo.nFirstDIP].nConst = 0;
            DIPInfo.DIPData[i - DIPInfo.nFirstDIP].nBit = 0;
        }

        if ((bii.szInfo[0] == 'p') || (bii.szInfo[0] == 'm'))
            if (bii.szInfo[0] == 'm') iJoyNum = 0; else iJoyNum = bii.szInfo[1] - '1';
        else {
            if (strcmp(bii.szInfo, "diag") == 0 || strcmp(bii.szInfo, "test") == 0) {
                ServiceDip = bii.pVal;
            }
            continue;
        }

        sprintf(controlName, "p%i coin", iJoyNum + 1);
        if (strcmp(bii.szInfo, controlName) == 0) {
            GameInput[iJoyNum][0].nBit = 4;
            GameInput[iJoyNum][0].pVal = bii.pVal;
            GameInput[iJoyNum][0].nType = bii.nType;
        } else {
            sprintf(controlName, "p%i start", iJoyNum + 1);
            if (strcmp(bii.szInfo, controlName) == 0) {
                GameInput[iJoyNum][1].nBit = 5;
                GameInput[iJoyNum][1].pVal = bii.pVal;
                GameInput[iJoyNum][1].nType = bii.nType;
                switch (iJoyNum) {
                    case 0:
                        P1Start = bii.pVal;
                        break;
                    case 1:
                        P2Start = bii.pVal;
                        break;
                }
            } else {
                sprintf(controlName, "p%i up", iJoyNum + 1);
                if (strcmp(bii.szInfo, controlName) == 0) {
                    GameInput[iJoyNum][2].nBit = 0;
                    GameInput[iJoyNum][2].pVal = bii.pVal;
                    GameInput[iJoyNum][2].nType = bii.nType;
                } else {
                    sprintf(controlName, "p%i down", iJoyNum + 1);
                    if (strcmp(bii.szInfo, controlName) == 0) {
                        GameInput[iJoyNum][3].nBit = 1;
                        GameInput[iJoyNum][3].pVal = bii.pVal;
                        GameInput[iJoyNum][3].nType = bii.nType;
                    } else {
                        sprintf(controlName, "p%i left", iJoyNum + 1);
                        if (strcmp(bii.szInfo, controlName) == 0) {
                            GameInput[iJoyNum][4].nBit = 2;
                            GameInput[iJoyNum][4].pVal = bii.pVal;
                            GameInput[iJoyNum][4].nType = bii.nType;
                        } else {
                            sprintf(controlName, "p%i right", iJoyNum + 1);
                            if (strcmp(bii.szInfo, controlName) == 0) {
                                GameInput[iJoyNum][5].nBit = 3;
                                GameInput[iJoyNum][5].pVal = bii.pVal;
                                GameInput[iJoyNum][5].nType = bii.nType;
                            } else {
                                sprintf(controlName, "p%i x-axis", iJoyNum + 1);
                                if (strcmp(bii.szInfo, controlName) == 0) {
                                    GameInput[iJoyNum][12].nBit = 2;
                                    GameInput[iJoyNum][12].pShortVal = bii.pShortVal;
                                    GameInput[iJoyNum][12].nType = bii.nType;
                                    GameInput[iJoyNum][13].nBit = 3;
                                    GameInput[iJoyNum][13].pShortVal = bii.pShortVal;
                                    GameInput[iJoyNum][13].nType = bii.nType;
                                } else {
                                    sprintf(controlName, "mouse x-axis");
                                    if (strcmp(bii.szInfo, controlName) == 0) {
                                        GameInput[iJoyNum][12].nBit = 2;
                                        GameInput[iJoyNum][12].pShortVal = bii.pShortVal;
                                        GameInput[iJoyNum][12].nType = bii.nType;
                                        GameInput[iJoyNum][13].nBit = 3;
                                        GameInput[iJoyNum][13].pShortVal = bii.pShortVal;
                                        GameInput[iJoyNum][13].nType = bii.nType;
                                    } else {
                                        sprintf(controlName, "p%i y-axis", iJoyNum + 1);
                                        if (strcmp(bii.szInfo, controlName) == 0) {
                                            GameInput[iJoyNum][14].nBit = 0;
                                            GameInput[iJoyNum][14].pShortVal = bii.pShortVal;
                                            GameInput[iJoyNum][14].nType = bii.nType;
                                            GameInput[iJoyNum][15].nBit = 1;
                                            GameInput[iJoyNum][15].pShortVal = bii.pShortVal;
                                            GameInput[iJoyNum][15].nType = bii.nType;
                                        } else {
                                            sprintf(controlName, "mouse y-axis");
                                            if (strcmp(bii.szInfo, controlName) == 0) {
                                                GameInput[iJoyNum][14].nBit = 0;
                                                GameInput[iJoyNum][14].pShortVal = bii.pShortVal;
                                                GameInput[iJoyNum][14].nType = bii.nType;
                                                GameInput[iJoyNum][15].nBit = 1;
                                                GameInput[iJoyNum][15].pShortVal = bii.pShortVal;
                                                GameInput[iJoyNum][15].nType = bii.nType;
                                            } else {
                                                sprintf(controlName, "p%i z-axis", iJoyNum + 1);
                                                if (strcmp(bii.szInfo, controlName) == 0) {
                                                    GameInput[iJoyNum][16].nBit = 11;
                                                    GameInput[iJoyNum][16].pShortVal = bii.pShortVal;
                                                    GameInput[iJoyNum][16].nType = bii.nType;
                                                } else {
                                                    sprintf(controlName, "p%i fire 1", iJoyNum + 1);
                                                    if (strcmp(bii.szInfo, controlName) == 0) {
                                                        GameInput[iJoyNum][6].nBit = 6;
                                                        GameInput[iJoyNum][6].pVal = bii.pVal;
                                                        GameInput[iJoyNum][6].nType = bii.nType;
                                                    } else {
                                                        sprintf(controlName, "p%i fire 2", iJoyNum + 1);
                                                        if (strcmp(bii.szInfo, controlName) == 0) {
                                                            GameInput[iJoyNum][7].nBit = 7;
                                                            GameInput[iJoyNum][7].pVal = bii.pVal;
                                                            GameInput[iJoyNum][7].nType = bii.nType;
                                                        } else {
                                                            sprintf(controlName, "mouse button 1");
                                                            if (strcmp(bii.szInfo, controlName) == 0) {
                                                                GameInput[iJoyNum][6].nBit = 6;
                                                                GameInput[iJoyNum][6].pVal = bii.pVal;
                                                                GameInput[iJoyNum][6].nType = bii.nType;
                                                            } else {
                                                                sprintf(controlName, "mouse button 2");
                                                                if (strcmp(bii.szInfo, controlName) == 0) {
                                                                    GameInput[iJoyNum][7].nBit = 7;
                                                                    GameInput[iJoyNum][7].pVal = bii.pVal;
                                                                    GameInput[iJoyNum][7].nType = bii.nType;
                                                                } else {
                                                                    sprintf(controlName, "p%i fire 3", iJoyNum + 1);
                                                                    if (strcmp(bii.szInfo, controlName) == 0) {
                                                                        GameInput[iJoyNum][8].nBit = 8;
                                                                        GameInput[iJoyNum][8].pVal = bii.pVal;
                                                                        GameInput[iJoyNum][8].nType = bii.nType;
                                                                    } else {
                                                                        sprintf(controlName, "p%i fire 4", iJoyNum + 1);
                                                                        if (strcmp(bii.szInfo, controlName) == 0) {
                                                                            GameInput[iJoyNum][9].nBit = 9;
                                                                            GameInput[iJoyNum][9].pVal = bii.pVal;
                                                                            GameInput[iJoyNum][9].nType = bii.nType;
                                                                        } else {
                                                                            sprintf(controlName, "p%i fire 5",
                                                                                    iJoyNum + 1);
                                                                            if (strcmp(bii.szInfo, controlName) == 0) {
                                                                                GameInput[iJoyNum][10].nBit = 10;
                                                                                GameInput[iJoyNum][10].pVal = bii.pVal;
                                                                                GameInput[iJoyNum][10].nType = bii.nType;
                                                                            } else {
                                                                                sprintf(controlName, "p%i fire 6",
                                                                                        iJoyNum + 1);
                                                                                if (strcmp(bii.szInfo, controlName) ==
                                                                                    0) {
                                                                                    GameInput[iJoyNum][11].nBit = 11;
                                                                                    GameInput[iJoyNum][11].pVal = bii.pVal;
                                                                                    GameInput[iJoyNum][11].nType = bii.nType;
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

#if 0
        if (pgi->pVal != NULL)
    printf("GI(%02d): %-12s 0x%02x 0x%02x %-12s, [%d]\n", i, bii.szName, bii.nType, *(pgi->pVal), bii.szInfo, pgi->nBit );
else
    printf("GI(%02d): %-12s 0x%02x N/A  %-12s, [%d]\n", i, bii.szName, bii.nType, bii.szInfo, pgi->nBit );
#endif

    }
    return 0;
}

int InpInit() {
    unsigned int i = 0;
    int nRet = 0;

    bInputOk = false;
    // Count the number of inputs
    nGameInpCount = 0;
    for (i = 0; i < 0x1000; i++) {
        nRet = BurnDrvGetInputInfo(NULL, i);
        if (nRet != 0) {   // end of input list
            nGameInpCount = i;
            break;
        }
    }

    memset(GameInput, 0, MAX_INPUT_inp * 4 * sizeof(struct GameInput));
    DoInputBlank(1);

    bInputOk = true;

    return 0;
}

int InpExit() {
    bInputOk = false;
    nGameInpCount = 0;
    if (DIPInfo.nDIP) free(DIPInfo.DIPData);
    return 0;
}

int InpMake(Input::Player *players) {
    if (!bInputOk) return 1;

    static int skip = 0;
    skip++;
    if (skip > 1) skip = 0;
    if (skip != 1) return 1;

    unsigned int i = 0;
    unsigned int down = 0;
    if (ServiceDip) {
        *(ServiceDip) = ServiceRequest;
    }
    int nJoy;
    for (short joyNum = 0; joyNum < PLAYER_COUNT; joyNum++) {
        if (!players[joyNum].enabled) {
            continue;
        }
        for (i = 0; i < MAX_INPUT_inp; i++) {
            nJoy = 0;
            if (GameInput[joyNum][i].pVal == NULL) continue;

            if (GameInput[joyNum][i].nBit >= 0) {
                down = players[joyNum].state & (1U << GameInput[joyNum][i].nBit);

                if (GameInput[joyNum][i].nType != 1) {
                    // Set analog controls to full

                    if (i < 12) {
                        if (down) *(GameInput[joyNum][i].pVal) = 0xff; else *(GameInput[joyNum][i].pVal) = 0x01;
                    }

                    /* TODO
                    if (i == 12) //analogue x
                    {
                        nJoy = SDL_JoystickGetAxis(joys[joyNum], 0) << 1;
                        if (down) nJoy = -32768 << 1;
                        nJoy *= nAnalogSpeed;
                        nJoy >>= 13;

                        // Clip axis to 8 bits
                        if (nJoy < -32768) {
                            nJoy = -32768;
                        }
                        if (nJoy > 32767) {
                            nJoy = 32767;
                        }

                        *(GameInput[joyNum][i].pShortVal) = nJoy;
                    }
                    if (i == 13) //analogue right
                    {
                        if (down) {
                            nJoy = 32768 << 1;
                            nJoy *= nAnalogSpeed;
                            nJoy >>= 13;

                            // Clip axis to 8 bits
                            if (nJoy < -32768) {
                                nJoy = -32768;
                            }
                            if (nJoy > 32767) {
                                nJoy = 32767;
                            }
                            *(GameInput[joyNum][i].pShortVal) = nJoy;
                        }
                    }
                    if (i == 14) //analogue y
                    {
                        nJoy = SDL_JoystickGetAxis(joys[joyNum], 1) << 1;
                        if (down) nJoy = -32768 << 1;
                        nJoy *= nAnalogSpeed;
                        nJoy >>= 13;

                        // Clip axis to 8 bits
                        if (nJoy < -32768) {
                            nJoy = -32768;
                        }
                        if (nJoy > 32767) {
                            nJoy = 32767;
                        }
                        *(GameInput[joyNum][i].pShortVal) = nJoy;
                    }
                    if (i == 15) //analogue down
                    {
                        if (down) {
                            nJoy = 32768 << 1;
                            nJoy *= nAnalogSpeed;
                            nJoy >>= 13;

                            // Clip axis to 8 bits
                            if (nJoy < -32768) {
                                nJoy = -32768;
                            }
                            if (nJoy > 32767) {
                                nJoy = 32767;
                            }
                            *(GameInput[joyNum][i].pShortVal) = nJoy;
                        }
                    }
                    if (i == 16) //analogue z
                    {
                        nJoy = (-SDL_JoystickGetAxis(joys[joyNum + 1], 1)) << 1;
                        //printf("%d\n",nJoy);
                        if (down) nJoy = 32768 << 1;
                        nJoy *= nAnalogSpeed;
                        nJoy >>= 13;

                        // Clip axis to 8 bits
                        if (nJoy < -32768) {
                            nJoy = -32768;
                        }
                        if (nJoy > 32767) {
                            nJoy = 32767;
                        }
                        *(GameInput[joyNum][i].pShortVal) = nJoy;
                    }
                    */
                } else {
                    // Binary controls
                    if (down) *(GameInput[joyNum][i].pVal) = 1; else *(GameInput[joyNum][i].pVal) = 0;
                    //(GameInput[joyNum][i].pVal)=0;
                }
            }
        }
    }
    for (i = 0; i < (int) DIPInfo.nDIP; i++) {
        if (DIPInfo.DIPData[i].pVal == NULL)
            continue;
        *(DIPInfo.DIPData[i].pVal) = DIPInfo.DIPData[i].nConst;
    }
    if (P1P2Start) {
        *(P1Start) = *(P2Start) = 1;
    }
    return 0;
}

int GameScreenMode;

void InpDIP() {
    struct BurnDIPInfo bdi;
    struct GameInput *pgi;
    int i, j;
    int nDIPOffset = 0;

    // get dip switch offset
    for (i = 0; BurnDrvGetDIPInfo(&bdi, i) == 0; i++)
        if (bdi.nFlags == 0xF0) {
            nDIPOffset = bdi.nInput;
            break;
        }

    // set DIP to default
    i = 0;
    bool bDifficultyFound = false;
    while (BurnDrvGetDIPInfo(&bdi, i) == 0) {

        //printf("%2d. %02x '%s'\n", bdi.nInput, bdi.nFlags, bdi.szText);

        if (bdi.nFlags == 0xFF) {
            pgi = DIPInfo.DIPData + (bdi.nInput + nDIPOffset - DIPInfo.nFirstDIP);
            pgi->nConst = (pgi->nConst & ~bdi.nMask) | (bdi.nSetting & bdi.nMask);
        } else if (bdi.nFlags == 0xFE) {
            if (bdi.szText)
                if ((strcmp(bdi.szText, "Difficulty") == 0) ||
                    (strcmp(bdi.szText, "Game Level") == 0)

                        )
                    bDifficultyFound = true;
        } else {
            if (bDifficultyFound) {
                if (bdi.nFlags == 0x01) {

                    // use GameScreenMode store
                    pgi = DIPInfo.DIPData + (bdi.nInput + nDIPOffset - DIPInfo.nFirstDIP);
                    for (j = 0; j < 8; j++)
                        if ((1U << j) & bdi.nMask)
                            break;
                    pgi->nConst = (pgi->nConst & ~bdi.nMask) | ((GameScreenMode << j) & bdi.nMask);

                    printf("Set DIP Difficulty [%d] = 0x%02x\n", bdi.nInput, (GameScreenMode << j) & bdi.nMask);
                }
                bDifficultyFound = false;
            }
        }
        i++;
    }
    for (i = 0, pgi = DIPInfo.DIPData; i < (int) DIPInfo.nDIP; i++, pgi++) {
        if (pgi->pVal == NULL)
            continue;
        *(pgi->pVal) = pgi->nConst;
    }
}