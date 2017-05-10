#include "cheats.hpp"

namespace CTRPluginFramework
{
    bool    CheckItemInput(const void *input, std::string &error)
    {
        // Cast the input into the appropriate type (must match the type provided to Open)
        u32 in = *static_cast<const u32 *>(input);

        // Check the value
        if (in < 0x1000 || in > 0xFFFF)
        {
            error = "The value must be between 1000 - FFFF";
            // Return that the value isn't valid
            return (false);
        }

        // The value is valid
        return (true);
    }

    void    Text2Item(MenuEntry *entry)
    {
        if (Controller::IsKeysDown(X + DPadRight))
        {
            u32 output;
            // New keyboard, hint being:
            Keyboard keyboard("What item would you like ?");

            // Add the function to check the input entered by the user
            keyboard.SetCompareCallback(CheckItemInput);

            // If the function return -1, then the user canceled the keyboard, so do nothing 
            if (keyboard.Open(output) != -1)
            {
                Player::GetInstance()->WriteInventorySlot(0, output);
            }
        }
    }

    void    Duplication(MenuEntry   *entry)
    {
        if (Controller::IsKeyDown(R))
        {
            u32 item;

            if (Player::GetInstance()->ReadInventorySlot(0, item))
                Player::GetInstance()->WriteInventorySlot(1, item);
        }
    }

    void    ShowBuriedItems(MenuEntry *entry)
    {
        u32* item = Game::GetItem();

        if (item != nullptr)
        {
            // Check to see if you're on a buried spot
            if (*item >> 16 == 0x8000)
                Player::GetInstance()->ThinkTo(*item & 0xFFFF);
            else
                Player::GetInstance()->UnThink();
        }
    }

    void    PickBuriedItems(MenuEntry *entry)
    {
        if (!Controller::IsKeysDown(Y))
            return;

        u32* item = Game::GetItem();

        if (item != nullptr)
        {
            // Check to see if you're on a buried spot
            if (*item >> 16 == 0x8000)
                *item &= 0xFFFF;
        }
    }

    void    ShowBells(MenuEntry *entry)
    {
        u64 money;
        u32 result;
        char buffer[0x100];

        Player::GetInstance()->Read64(0x6f08, money);
        result = DecryptACNLMoney(money);

        sprintf(buffer, "Bells %i", result);
        OSD::WriteLine(1, buffer, 0, 0);
    }

    void    SetBells(MenuEntry *entry)
    {
        u64 money;
        u32 output;

        Keyboard keyboard("How much money would you like?");

        keyboard.IsHexadecimal(false);

        // If the function return -1, then the user canceled the keyboard, so do nothing 
        if (keyboard.Open(output) != -1)
        {
            money = EncryptACNLMoney(output);

            Player::GetInstance()->Write64(0x6f08, money);
        }
    }

    // Inventory has 16 slots
    // File struct:
    // 0 - 3: current opened box
    // 4 - end: box item's
    #define BOX_SIZE 64

    struct InventoryBox
    {
        int currentBox;
        int lastBox;
        File file;
    };

    void    OpenBox(MenuEntry *entry, int id)
    {
        InventoryBox* box = static_cast<InventoryBox *>(entry->GetArg());
        File& file = box->file;

        // If file is not open, something's wrong
        if (!file.IsOpen())
        {
            OSD::Notify("An error occured !", Color::Red);
            OSD::Notify("Try to re-enable the cheat.");
            entry->Disable();
            return;
        }

        u32 inventory = Player::GetInstance()->GetInventoryAddress();
        char buffer[0x100] = {0};

        // Save current box
        {
            // Go to current box's offset in file
            file.Seek(4 + (box->currentBox * BOX_SIZE), File::SET);

            // Now save current items
            file.Write(reinterpret_cast<void *>(inventory), BOX_SIZE);
        }

        // Open new box
        {
            // Go to wanted box's offset in file
            file.Seek(4 + (id * BOX_SIZE), File::SET);

            // Read items from file and write them in ram

            file.Read(buffer, BOX_SIZE);
            Process::CopyMemory(reinterpret_cast<void *>(inventory), buffer, BOX_SIZE);
        }

        // Update lastBox
        box->lastBox = box->currentBox;

        // Update current box
        box->currentBox = id;
        file.Seek(0, File::SET);
        file.Write(static_cast<void *>(&id), 4);

        // A little notification is always nice :)
        sprintf(buffer, "Opened box %d", id + 1);
        OSD::Notify(buffer, Color::LimeGreen);
    }

    void    ExtendedInventoryBox(MenuEntry *entry)
    {
        // If entry is disabled, properly release InventoryBox
        if (!entry->IsActivated())
        {
            InventoryBox    *box = static_cast<InventoryBox *>(entry->GetArg());

            if (box != nullptr)
            {
                box->file.Close();
                delete box;
                entry->SetArg(nullptr);
            }
            return;
        }

        // If just enabled the entry, create InventoryBox and open the file
        if (entry->WasJustActivated())
        {
            InventoryBox    *box = new InventoryBox;

            box->currentBox = 0;
            box->lastBox = 0;
            entry->SetArg(box);

            // If file don't exist, create it and init it
            if (!File::Exists("InventoryBoxs.bin"))
            {
                File    &file = box->file;

                // Open with create flag
                int flags = File::READ | File::WRITE | File::CREATE | File::SYNC;
                if (File::Open(file, "InventoryBoxs.bin", flags) == 0)
                {
                    int size = 4 + (BOX_SIZE * 10);
                    u8  buffer[4 + (BOX_SIZE * 10)] = { 0 };

                    file.Write(buffer, size);
                }
                else
                {
                    OSD::Notify("InventoryBox: An error occurred.", Color::Red);
                    entry->Disable();
                    return;
                }
            }
            // Else open it and get current box's index
            else
            {
                File    &file = box->file;

                if (File::Open(file, "InventoryBoxs.bin") == 0)
                {
                    u32  index = 0;

                    file.Read(&index, 4);
                    box->currentBox = index;
                }
                else
                {
                    OSD::Notify("InventoryBox: An error occurred.", Color::Red);
                    entry->Disable();
                    return;
                }
            }
        }

        // When Start is held for 2 seconds, open the menu to change the box
        static bool     startIsPressed = false;
        static Clock    timer;

        // If it's been 2 seconds that start is pressed
        if (startIsPressed && timer.HasTimePassed(Seconds(2.f)))
            goto showMenu;

        // When start is released reset our boolean
        if (Controller::IsKeyReleased(Start))
            startIsPressed = false;

        // If start isn't pressed , no need to go further
        if (!startIsPressed && !Controller::IsKeyPressed(Start))
            return;

        // If start just got pressed, start the timer
        if (!startIsPressed)
        {
            startIsPressed = true;
            timer.Restart();
        }

        return;

    showMenu:
        // Reset start press state
        startIsPressed = false;

        using StringVector = std::vector<std::string>;

        char            buffer[0x100] = { 0 };
        InventoryBox    *box = static_cast<InventoryBox *>(entry->GetArg());
        StringVector    boxList;
        std::string     keyboardHint = "Inventory Box\n\nWhich box to open ?\n";

        sprintf(buffer, "Currently opened: [Box %d]", box->currentBox + 1);
        keyboardHint += buffer;

        Keyboard        keyboard(keyboardHint);
        
        // Init my list
        sprintf(buffer, "Last: %d", box->lastBox + 1);
        boxList.push_back(buffer);
        
        for (int i = 1; i < 11; i++)
        {
            sprintf(buffer, "Box %d", i);
            boxList.push_back(buffer);
        }

        // Init my keyboard with my list
        keyboard.Populate(boxList);

        // Show keyboard and get the box id

        int  id = keyboard.Open();

        // User did B, abort
        if (id == -1)
            return;

        // If user decided to open the last opened box
        if (id == 0)
            OpenBox(entry, box->lastBox);
        else
            OpenBox(entry, id - 1);
    }
}
