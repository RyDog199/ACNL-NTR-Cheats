#include "cheats.hpp"
#include "Helpers.hpp"
#include "3ds.h"

extern "C" void LoadBottomUI(void);

u32     g_bottomUI = 0;
u32     g_bottomReturn;

namespace CTRPluginFramework
{

    struct NPC
    {
        const char *File;
        std::string Name;
    };

    const std::vector<NPC> villagers =
    {
        { "alp.bcres", "Cyrus" },
        { "alw.bcres", "Reese" },
        { "bev.bcres", "Chip" },
        { "bln.bcres", "Phineas" },
        { "boa.bcres", "Joan" },
        { "bpt.bcres", "Katrina" },
        { "chm.bcres", "Nat" },
        { "cml.bcres", "Sahara" },
        { "end.bcres", "Totakeke" },
        { "enj.bcres", "Totakeke" },
        { "fob.bcres", "Redd" },
        { "fox.bcres", "Red" },
        { "grf.bcres", "Grace" },
        { "hgc.bcres", "Labelle" },
        { "hgh.bcres", "Mable" },
        { "hgs.bcres", "Sable" },
        { "kpg.bcres", "Grams" },
        { "kpm.bcres", "Leilani" },
        { "kpp.bcres", "Kapp'n" },
        { "kps.bcres", "Leila" },
        { "liz.bcres", "Isabelle" },
        { "lom.bcres", "Katie" },
        { "lrc.bcres", "Timmy/Tommy" },
        { "lrd.bcres", "Timmy/Tommy" },
        { "lrh.bcres", "Timmy/Tommy" },
        { "lrn.bcres", "Timmy/Tommy" },
        { "lrs.bcres", "Timmy/Tommy" },
        { "mka.bcres", "Blanca?" },
        { "mnk.bcres", "Porter" },
        { "moc.bcres", "Don" },
        { "mod.bcres", "Don (No hat)" },
        { "mof.bcres", "Resetti (No Hat)" },
        { "moo.bcres", "Resetti" },
        { "ott.bcres", "Lyle" },
        { "owl.bcres", "Blathers" },
        { "ows.bcres", "Celeste" },
        { "pck.bcres", "Pave" },
        { "pga.bcres", "Pelly" },
        { "pgb.bcres", "Phyllis" },
        { "pge.bcres", "Brewster" },
        { "pkn.bcres", "Jack" },
        { "pla.bcres", "Booker" },
        { "plb.bcres", "Pete" },
        { "plc.bcres", "Copper" },
        { "poo.bcres", "Shampoodle" },
        { "pyn.bcres", "Zipper T." },
        { "rci.bcres", "Tom Nook" },
        { "rcn.bcres", "Isabelle" },
        { "rco.bcres", "Tom Nook" },
        { "seg.bcres", "Gulliver" },
        { "seo.bcres", "Pascal" },
        { "skk.bcres", "Kicks" },
        { "slo.bcres", "Lief" },
        { "snt.bcres", "Jingle" },
        { "sza.bcres", "Isabelle" },
        { "szo.bcres", "Digby" },
        { "szr.bcres", "Digby (Rain Jacket)" },
        { "tap.bcres", "Luna" },
        { "ttl.bcres", "Tortimer" },
        { "tuk.bcres", "Franklin" },
        { "upa.bcres", "Shrunk" },
        { "wrl.bcres", "Wendell" },
        { "xct.bcres", "Rover" },
    };

    void    GhostMode(MenuEntry *entry)
    {
        u32   patch = 0xE38110FF;
        u32   original = 0xE3811004;

        if (entry->Hotkeys[0].IsDown())
            Process::Patch(Game::Visibility, (u8 *)&patch, 4);
        if (entry->Hotkeys[1].IsDown())
            Process::Patch(Game::Visibility, (u8 *)&original, 4);
    }

    void    FastGameSpeed(MenuEntry *entry)
    {
        static const u32    nop = 0xE1A00000;
        static const u32    original = 0xE58450A0;
        static u32 offset = reinterpret_cast<u32>(Game::GameSpeed);
        if (entry->WasJustActivated())
            Process::Patch(offset, &nop, 4);
        else if (!entry->WasJustActivated())
            Process::Patch(offset, &original, 4);
    }


    void    CameraMod(MenuEntry *entry)
    {
        //pointers & addresses
        static const u32    cameraAsm = AutoRegion(USA_CAMERA_ASM_ADDR, EUR_CAMERA_ASM_ADDR, JAP_CAMERA_ASM_ADDR, USA_WA_CAMERA_ASM_ADDR, EUR_WA_CAMERA_ASM_ADDR, JAP_WA_CAMERA_ASM_ADDR)();
        static const u32    rotationAsm = AutoRegion(USA_CAMERA_ROT_ASM, EUR_CAMERA_ROT_ASM, JAP_CAMERA_ROT_ASM, USA_WA_CAMERA_ROT_ASM, EUR_WA_CAMERA_ROT_ASM, JAP_WA_CAMERA_ROT_ASM)();
        static u32  * cameraPointer = reinterpret_cast<u32 * const>(AutoRegion(USA_CAMERA_POINTER, EUR_CAMERA_POINTER, JAP_CAMERA_POINTER, USA_WA_CAMERA_POINTER, EUR_WA_CAMERA_POINTER, JAP_WA_CAMERA_POINTER)());
        static Coordinates * const cameraCoordinates = reinterpret_cast<Coordinates * const>(AutoRegion(USA_CAMERA_X_ADDR, EUR_CAMERA_X_ADDR, JAP_CAMERA_X_ADDR, USA_WA_CAMERA_X_ADDR, EUR_WA_CAMERA_X_ADDR, JAP_WA_CAMERA_X_ADDR)());

        //variables
        static const u32    patch = 0xEA000020;
        static const u32    nop = 0xE1A00000;
        static const u32    originalRotation = 0xE18020B4;
        static const u32    original = 0x2A000020;
        static bool         isPatched = false;
        static bool         rotationPatch = false;
        static bool         followRotation = false;

        if (*cameraPointer)
        {
            //check if you're outside
            if (*Game::Location == -1)
            {
                if (*Game::Room == 1)
                {
                    Process::Patch(rotationAsm, (u8 *)&originalRotation, 4);
                    Process::Patch(rotationAsm + 0xC, (u8 *)&originalRotation, 4);
                }
                else
                {
                    Process::Patch(rotationAsm, (u8 *)&nop, 4);
                    Process::Patch(rotationAsm + 0xC, (u8 *)&nop, 4);
                }
            }
            else
            {
                Process::Patch(rotationAsm, (u8 *)&originalRotation, 4);
                Process::Patch(rotationAsm + 0xC, (u8 *)&originalRotation, 4);
            }
            if (followRotation)
            {
                u32 rotation = Player::GetInstance()->GetRotation();
                Process::Write16(*cameraPointer + 0x12E, (rotation >> 16) ^ 0x8000); //get the opposite of the rotation value in order to face opposite of player
            }
            if (entry->Hotkeys[0].IsDown())
            {
                if (Controller::IsKeyDown(Key::CPadUp))
                    ADD16((*cameraPointer + 0x12C), 0x10);
                if (Controller::IsKeyDown(Key::CPadDown))
                    SUB16((*cameraPointer + 0x12C), 0x10);
                if (Controller::IsKeyDown(Key::CPadLeft))
                    ADD16((*cameraPointer + 0x12E), 0x10);
                if (Controller::IsKeyDown(Key::CPadRight))
                    SUB16((*cameraPointer + 0x12E), 0x10);
            }
            if (entry->Hotkeys[1].IsDown()) // Stop camera from moving
                goto patch;
            if (entry->Hotkeys[2].IsDown()) // Make camera move again
                goto unpatch;
            if (entry->Hotkeys[3].IsDown())
            {
                if (!followRotation)
                    followRotation = true;
                else
                    followRotation = false;
                while (entry->Hotkeys[3].IsDown())
                    Controller::Update();
            }

            if (entry->Hotkeys[4].IsDown())
            {
                cameraCoordinates->z -= 0.1f;
                goto patch;
            }

            if (entry->Hotkeys[5].IsDown())
            {
                cameraCoordinates->x += 0.1f;
                goto patch;
            }

            if (entry->Hotkeys[6].IsDown())
            {
                cameraCoordinates->z += 0.1f;
                goto patch;
            }

            if (entry->Hotkeys[7].IsDown())
            {
                cameraCoordinates->x -= 0.1f;
                goto patch;
            }

            if (entry->Hotkeys[8].IsDown())
            {
                cameraCoordinates->y -= 0.1f;
                goto patch;
            }

            if (entry->Hotkeys[9].IsDown())
            {
                cameraCoordinates->y += 0.1f;
                goto patch;
            }
            return;
        patch:
            if (!isPatched)
            {
                // Change the asm instruction to b, allows overwriting camera coordinates
                Process::Patch(cameraAsm, (u8 *)&patch, 4);
                isPatched = true;
            }
            return;
        unpatch:
            if (isPatched)
            {
                Process::Patch(cameraAsm, (u8 *)&original, 4);
                isPatched = false;
            }
        }
    }

    void    KeyboardExtender(MenuEntry *entry)
    {
        u32 offset = 0;

        offset = *Game::KeyboardText;
        if (offset != 0)
        {
            Process::Write32(0xC + offset, 0x36);
            Process::Write32(0x12B + offset, 0x45);
            *Game::EnterBool = 1;
        }
    }

    void    ItemFormChanger(MenuEntry *entry)
    {
        u16     item = *GetArg<u16>(entry);

        if (*Game::Location == -1)
            Process::Write16(Game::ItemForm, 0x334F + item);
    }

    void    ItemFormEditor(MenuEntry *entry)
    {
        u16   *item = GetArg<u16>(entry);

        *item = ItemChangerKeyboard();
    }

    void    ItemEffectChanger(MenuEntry *entry)
    {
        u16     item = *GetArg<u16>(entry);

        if (*Game::Location == -1)
            Process::Write16(Game::ItemForm - 0x3AD8, 0x334F + item);
    }

    void    ItemEffectEditor(MenuEntry *entry)
    {
        u16     *item = GetArg<u16>(entry);

        *item = ItemChangerKeyboard();
    }

    int     ItemChangerKeyboard(void)
    {
        Keyboard keyboard("Item Effect Changer\nWhat form would you like ?");
        static std::vector<std::string> list =
        {
            "Axe",
            "Net",
            "Rod",
            "Shovel",
            "Watering Can",
            "Slingshot",
            "Hammer",
            "MegaPhone",
            "Sparkler",
            "Roman Candle",
            "Party Popper",
            "Bubble Wand",
            "Balloon",
            "Pinwheel",
            "Drink",
            "Beans",
            "Good Luck Roll",
            "Ice Cream",
            "Wand",
            "Tweeter",
            "Frying Pan"
        };

        keyboard.Populate(list);
        keyboard.CanAbort(false);

        int userChoice = keyboard.Open();

        if (userChoice < 5)
            return (userChoice * 4);
        else if (userChoice == 5)
            return (20);
        else if (userChoice > 5 && userChoice < 8)
            return (16 + userChoice);
        else if (userChoice >= 8 && userChoice < 13)
            return (17 + userChoice);
        else if (userChoice == 13)
            return (53);
        else if (userChoice > 13 && userChoice < 18)
            return (48 + userChoice);
        else if (userChoice == 18)
            return (73);
        else if (userChoice == 19)
            return (79);

        return (986); //userChoice == 20
    }   


    void ChangeAnimal(const char* name)
    {
        static u32 isabelle = Game::DynamicNPC;
        static u32 offset = Game::StaticNPC;
        const char * original = "%s.bcres";
        if (name != "")
        {
            if (*Game::Room != 0x27)
            {
                if (*Game::Room == 0x63)
                {
                    Process::CopyMemory(reinterpret_cast<void *>(offset), (void*)original, 9);
                    Process::CopyMemory(reinterpret_cast<void *>(isabelle), (void*)name, 3);
                }
                else if (*Game::Room == 0)
                {
                    Process::CopyMemory(reinterpret_cast<void *>(offset), (void*)original, 9);
                    Process::CopyMemory(reinterpret_cast<void *>(isabelle + 0x1180), (void*)name, 3);
                }
                else
                {
                    Process::CopyMemory(reinterpret_cast<void *>(offset), (void*)name, 9);
                }
            }
            else
            {
                svcSleepThread(6000000000);
            }
        }
    }

    using StringVector = std::vector<std::string>;

    void    AnimalChangerKeyboard(MenuEntry *entry)
    {
        int *index = GetArg<int>(entry);
        Keyboard    keyboard("What would you like to change all Special NPCS to?");
        StringVector    entryNames;

        for (const NPC &entryName : villagers)
            entryNames.push_back(entryName.Name);

        keyboard.Populate(entryNames);

        int userChoice = keyboard.Open();


        *index = userChoice;

    }

    void    AnimalChanger(MenuEntry *entry)
    {
        int index = *GetArg<int>(entry);
        const NPC &NPCPicked = villagers[index];
        ChangeAnimal(NPCPicked.File);
    }

    void    StorageEverywhere(MenuEntry *entry)
    {
        static Hook hook;
        static Clock clock;
        u32 addr = AutoRegion(USA_BOTTOM_ASM -4, EUR_BOTTOM_ASM -4, JAP_BOTTOM_ASM -4, USA_WA_BOTTOM_ASM - 4, EUR_WA_BOTTOM_ASM - 4, JAP_WA_BOTTOM_ASM - 4)();
        
        if (entry->Hotkeys[0].IsDown())
            *Game::BottomScreen = 0x3D;

        if (entry->Hotkeys[1].IsDown())
            *Game::BottomScreen = 0x89;

        if (entry->Hotkeys[2].IsDown())
            *Game::BottomScreen = 0x7C;

        if (entry->Hotkeys[3].IsDown())
        {
            g_bottomUI = *GetArg<u8>(entry, 0);
            g_bottomReturn = addr + 8;
            hook.Initialize(addr, (u32)LoadBottomUI);
            clock.Restart();
            hook.Enable();
        }
        if (hook.isEnabled && clock.HasTimePassed(Seconds(1.f)))
        {
            hook.Disable();
            clock.Restart();
        }

    }


    void    StorageEverywhereSettings(MenuEntry *entry)
    {
        u8 *arg = GetArg<u8>(entry);
        u8 id;
        Keyboard keyboard("Which ID would you like?");
        keyboard.IsHexadecimal(true);
        if (keyboard.Open(id) != -1)
            *arg = id;
    }

    void    Faint(MenuEntry *entry)
    {
        if (entry->Hotkeys[0].IsDown())
        {
            if (*Game::Location == -1)
                *Game::Consciousness = 0x0100;
        }
            
    }

    void    Corrupter(MenuEntry *entry)
    {
        u32 address = 0x30000000;
        float value;
        int     *corruption = GetArg<int>(entry);

        while (Process::ReadFloat(address, value))
        {
            switch (*corruption)
            {
                case 0:
                    if (value == 1.0f)
                    {
                        int rng = Utils::Random(0, 10);
                        if (rng == 5)
                        {
                            rng = Utils::Random(1, 2);
                            if (rng = 1)
                                Process::WriteFloat(address, value + 0.1f);
                            else
                                Process::WriteFloat(address, value - 0.1f);
                        }
                    }
                    break;
                case 1:
                    if (value >= 1.0f && value < 2.0f)
                    {
                        int rng = Utils::Random(0, 10);
                        if (rng == 5)
                        {
                            rng = Utils::Random(1, 2);
                            if (rng = 1)
                                Process::WriteFloat(address, value + 0.1f);
                            else
                                Process::WriteFloat(address, value - 0.1f);
                        }
                    }
                    break;
                case 2:
                    if (value >= 1.0f && value < 2.0f)
                    {
                        int rng = Utils::Random(0, 10);
                        if (rng != 5)
                        {
                            rng = Utils::Random(1, 2);
                            if (rng = 1)
                                Process::WriteFloat(address, value + 0.1f);
                            else
                                Process::WriteFloat(address, value - 0.1f);
                        }
                    }
                    break;
                case 3:
                    if (value >= 1.0f && value < 2.0f)
                    {
                        int rng = Utils::Random(1, 2);
                        if (rng = 1)
                            Process::WriteFloat(address, value + 0.1f);
                        else
                            Process::WriteFloat(address, value - 0.1f);
                    }
                    break;

                case 4:
                    if (value >= 1.0f && value < 2.0f)
                    {
                        int rng = Utils::Random(0, 20);
                        Process::WriteFloat(address, 0.1f * rng);
                    }
                    break;
                default:
                    if (value >= 1.0f && value < 2.0f)
                    {
                        int rng = Utils::Random(0, 20);
                        Process::WriteFloat(address, 0.1f * rng);
                    }
                    break;
            }
            address += 4;
        }
    }

    void    CorrupterSettings(MenuEntry *entry)
    {
        int     *corruption = GetArg<int>(entry);

        Keyboard keyboard("Corruption Editor\nWhat level of corruption would you like?");
        static std::vector<std::string> list =
        {
            "0 - Minimal",
            "1 - Low",
            "2 - Medium",
            "3 - High",
            "4 - Extreme !Dangerous!",
        };
        keyboard.Populate(list);
        keyboard.CanAbort(false);

        *corruption = keyboard.Open();

    }

    std::vector<u8> FindItemCoordinates(std::vector<u16> & id, bool isFlag) //isFlag is used to check the flag of items ex: buried items
    {
        int size = id.size();
        u32 offset = reinterpret_cast<u32>(Game::TownItem);
        static u32 counter = 0;
        if (isFlag)
            offset += 2;
        
        while (counter < 0x5000) //loop through town data
        {
            for (int i = 0; i < size; i++)
            {
                if (*(u16 *)(offset + counter) == id[i]) //check if the offset has a weed
                {
                    u8 acre;
                    std::vector <u8> coordinates = { 0, 0 };
                    acre = counter / 0x400; //each acre contains 0x100 bytes of data
#ifdef DEBUG
                    OSD::Notify(Utils::Format("acre id: %i", acre));
#endif
                    coordinates[0] = acre % 5; //get the remmainder of the row to find x
                    coordinates[1] = acre / 5; //5 acres per row
#ifdef DEBUG
                    OSD::Notify(Utils::Format("acre coords: (%i,%i)", coordinates[0], coordinates[1]));
#endif
                    /*
                    * We now know which acre we're in, so we multiply 16 to x and y to get to (0,0) of the acre
                    */
                    coordinates[0] = (coordinates[0] + 1) * 16;
                    coordinates[1] = (coordinates[1] + 1) * 16;
#ifdef DEBUG
                    OSD::Notify(Utils::Format("coords: (%i,%i)", coordinates[0], coordinates[1]));
#endif
                    int tmp = counter - (acre * 0x400);
                    coordinates[0] += (tmp / 4) % 16;
                    coordinates[1] += tmp / 0x40;
                    

                    return (coordinates);
                }

            }
            counter += 4;
        }
        counter = 0;
        std::vector<u8>noItems = { 0, 0 }; //return 0, 0 if no items are found, which is out of bounds
        return(noItems);
    }

    void    UltimateWeedPuller(MenuEntry *entry)
    {
        static bool execution = false;
        std::vector <u16> weeds = { 0x007C, 0x007D, 0x007E, 0x007F, 0x00CC, 0x00F8 };
        std::vector<u8> coordinates = { 0, 0 };

        if (entry->Hotkeys[0].IsDown())
        {
            if (execution)
                execution = false;
            else
                execution = true;
            while (entry->Hotkeys[0].IsDown())
                Controller::Update();
        }
        if (execution)
        {
            coordinates = FindItemCoordinates(weeds, false);
#ifdef DEBUG
            OSD::Notify(Utils::Format("coords: (%i,%i)", coordinates[0], coordinates[1]));
#endif
            if (coordinates[0] + coordinates[1] == 0)
            {
                execution == false;
                return;
            }
            Player::GetInstance()->SetFloatCoordinates(coordinates[0] + 0.1f, coordinates[1] + 0.1f);
            Player::GetInstance()->SetRotation(0xEB00000);
            Controller::InjectKey(Key::Y);
        }
    }

    void    UnBuryItems(MenuEntry *entry)
    {
        std::vector<u8> coordiantes = { 0, 0 };
        static bool execution = false;

        if (entry->Hotkeys[0].IsDown())
        {
            if (execution)
                execution = false;
            else
                execution = true;
            while (entry->Hotkeys[0].IsDown())
                Controller::Update();
        }
        if (execution)
        {
            std::vector<u16> buriedFlag = { 0x8000 };
            coordiantes = FindItemCoordinates(buriedFlag, true);
            if (coordiantes[0] + coordiantes[1] == 0)
            {
                execution = false;
                return;
            }
            u16 heldItem;
            if (Player::GetInstance()->Read16(0x26, heldItem) && heldItem < 0x3357 || heldItem > 0x335C) //check if the player doesn't have a shovel
            {
                MessageBox(Color::Yellow << "Info", "Please equip a shovel and press the hotkey again.")();
                execution = false;
                return;
            }
            if (*Game::MapBool == 0)
                return;
            Player::GetInstance()->SetFloatCoordinates(coordiantes[0] + 0.5f, coordiantes[1] - 0.01f);
            Player::GetInstance()->SetRotation(0);

            Sleep(Seconds(0.1f)); //sleep in order to give the game time to update the coordinates
            Controller::InjectKey(Key::A);
        }
    }


    void    EnableAllTours(MenuEntry *entry) //Thanks to Wii8461!
    {
        if (*Game::Tours != 0)
        {
            for(int i = 0; i < 64; i++)
                Process::Write8(*Game::Tours + 10 + i, 1); //Mark every tours as enabled
        }
    }
}