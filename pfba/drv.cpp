// Driver Init module
#include "gui.h"
#include "run.h"

extern Gui *gui;
extern UINT8 NeoSystem;
int bDrvOkay = 0;                        // 1 if the Driver has been initted okay, and it's okay to use the BurnDrv functions

static int ProgressCreate();

static UINT8 NeoSystemList[] = {0x0f, 0x0c, 0x0b, 0x0d, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
                                0x0a, 0x0e};

static int DoLibInit()                    // Do Init of Burn library driver
{
    int nRet;

    ProgressCreate();

    nRet = BzipOpen(false);
    printf("DoLibInit: BzipOpen = %i\n", nRet);
    if (nRet) {
        BzipClose();
        return 1;
    }

    NeoSystem &= ~(UINT8) 0x1f;
    NeoSystem |= NeoSystemList[gui->GetConfig()->GetRomValue(Option::Index::ROM_NEOBIOS)];

    nRet = BurnDrvInit();
    printf("DoLibInit: BurnDrvInit = %i\n", nRet);

    BzipClose();

    if (nRet) {
        return 1;
    } else {
        return 0;
    }
}

// Catch calls to BurnLoadRom() once the emulation has started;
// Intialise the zip module before forwarding the call, and exit cleanly.
static int DrvLoadRom(unsigned char *Dest, int *pnWrote, int i) {
    int nRet;

    BzipOpen(false);

    char *pszFilename;
    BurnDrvGetRomName(&pszFilename, i, 0);
    printf("DrvLoadRom: BurnExtLoadRom(%s)\n", pszFilename);
    nRet = BurnExtLoadRom(Dest, pnWrote, i);
    printf("DrvLoadRom: BurnExtLoadRom = %i\n", nRet);

    if (nRet != 0) {
        char szText[256] = "";
        sprintf(szText,
                "Error loading %s, requested by %s.\n"
                        "The emulation will likely suffer problems.",
                pszFilename, BurnDrvGetTextA(0));
        printf("DrvLoadRom: %s\n", szText);
    }

    BzipClose();

    BurnExtLoadRom = DrvLoadRom;

    return nRet;
}

int DrvInit(int nDrvNum, bool bRestore) {

    printf("DrvInit(%i, %i)\n", nDrvNum, bRestore);
    DrvExit();

    nBurnDrvSelect[0] = (UINT32) nDrvNum;

    // Define nMaxPlayers early; GameInpInit() needs it (normally defined in DoLibInit()).
    nMaxPlayers = BurnDrvGetMaxPlayers();

    printf("DrvInit: DoLibInit()\n");
    if (DoLibInit()) {                // Init the Burn library's driver
        //char szTemp[512];
        //_stprintf(szTemp, _T("Error starting '%s'.\n"), BurnDrvGetText(DRV_FULLNAME));
        //AppError(szTemp, 1);
        return 1;
    }

    printf("DrvInit: BurnExtLoadRom = DrvLoadRom\n");
    BurnExtLoadRom = DrvLoadRom;

    char path[MAX_PATH];
    snprintf(path, MAX_PATH, "%s%s.fs", szAppEEPROMPath, BurnDrvGetTextA(DRV_NAME));
    BurnStateLoad(path, 0, NULL);

    bDrvOkay = 1;                    // Okay to use all BurnDrv functions

    nBurnLayer = 0xFF;                // show all layers

    // Reset the speed throttling code, so we don't 'jump' after the load
    RunReset();
    return 0;
}

int DrvInitCallback() {
    return DrvInit(nBurnDrvSelect[0], false);
}

int DrvExit() {
    if (bDrvOkay) {
        if (nBurnDrvSelect[0] < nBurnDrvCount) {
            char path[MAX_PATH];
            snprintf(path, MAX_PATH, "%s%s.fs", szAppEEPROMPath, BurnDrvGetTextA(DRV_NAME));
            BurnStateSave(path, 0);
            BurnDrvExit();                // Exit the driver
        }
    }

    BurnExtLoadRom = NULL;
    bDrvOkay = 0;                    // Stop using the BurnDrv functions
    nBurnDrvSelect[0] = ~0U;            // no driver selected

    return 0;
}

static double nProgressPosBurn = 0;

static int ProgressCreate() {
    nProgressPosBurn = 0;
    return 0;
}

int ProgressUpdateBurner(double dProgress, const TCHAR *pszText, bool bAbs) {
    gui->Clear();
    gui->DrawBg();
    gui->DrawRomList();

    Rect window{
            gui->GetRenderer()->width / 4,
            gui->GetRenderer()->height / 4,
            gui->GetRenderer()->width / 2,
            gui->GetRenderer()->height / 2
    };

    gui->GetRenderer()->DrawRect(window, C2D_COL_GRAY);
    gui->GetRenderer()->DrawRect(window, C2D_COL_GREEN, false);

    if (pszText) {
        nProgressPosBurn += dProgress;

        Rect r = {window.x + 16, window.y + 32, window.w - 32, 32};
        gui->GetSkin()->font->Draw(r, C2D_COL_WHITE, false, true, BurnDrvGetTextA(DRV_FULLNAME));
        r.y += 64;
        gui->GetSkin()->font->Draw(r, C2D_COL_WHITE, false, true, "Please wait...");
        r.y += 32;
        gui->GetSkin()->font->Draw(r, C2D_COL_WHITE, false, true, "%s", pszText);

        int x = window.x + 16;
        int w = window.w - 32;
        gui->GetRenderer()->DrawRect(x - 1, window.y + window.h - 65, w + 2, 34, 255, 255, 255, 255, false);

        int progress_y = (int) (nProgressPosBurn * (double) w);
        if (progress_y > w) {
            progress_y = w;
        }
        gui->GetRenderer()->DrawRect(x, window.y + window.h - 64, progress_y, 32, 255, 255, 0, 255);
    } else {
        gui->GetSkin()->font->Draw(window.x + 16, window.y + 96, "Please wait...");
    }
    gui->Flip();

    return 0;
}

int AppError(TCHAR *szText, int bWarning) {

    gui->GetRenderer()->Delay(500);

    Rect window{
            gui->GetRenderer()->width / 4,
            gui->GetRenderer()->height / 4,
            gui->GetRenderer()->width / 2,
            gui->GetRenderer()->height / 2
    };

    while (!gui->GetInput()->Update(0)[0].state) {

        gui->DrawRomList();

        gui->GetRenderer()->DrawRect(window, C2D_COL_GRAY);
        gui->GetRenderer()->DrawRect(window, C2D_COL_GREEN, false);

        Rect dst = window;
        int height = window.h / 3;
        dst.h = height;

        gui->GetSkin()->font->Draw(dst, C2D_COL_WHITE, true, true, "WARNING");
        dst.y += height;

        if (szText) {
            gui->GetSkin()->font->Draw(dst, C2D_COL_WHITE, true, true, "%s", szText);
            dst.y += height;
        }

        gui->GetSkin()->font->Draw(dst, C2D_COL_WHITE, true, true, "PRESS A KEY TO CONTINUE", szText);

        gui->Flip();
    }

    GameLooping = false;

    gui->GetRenderer()->Delay(500);

    return 1;
}
