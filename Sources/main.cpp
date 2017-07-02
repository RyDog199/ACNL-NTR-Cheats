#include "cheats.hpp"

namespace CTRPluginFramework
{
    // This function is called on the plugin starts, before main
    void PatchProcess(void)
    {
    }
	
    int main(void)
    {
        PluginMenu* m = new PluginMenu("Animal Crossing New Leaf NTR Cheats", "Ver 4.0_20 Alpha\n\nCheat Plugin by RyDog with the help of Nanquitas, Slattz and others :)");
        PluginMenu& menu = *m;

        u64 tid = Process::GetTitleID();
		u16 ver = Process::GetVersion();

        // Assign globals according to the current game's region
        if (tid == 0x0004000000086300)
		{
			if (ver == 6192)
			{
				assign_region(USA);
			}
			
			else
			{
				MessageBox  msgBox("Your ACNL version isn't\nsupported!\nMake sure you have the\n1.5 update installed!");
				msgBox();
				return (0);
			}
		}
			
        else if (tid == 0x0004000000086400)
		{
			if (ver == 6176)
			{
				assign_region(EUR);
			}
			
			else
			{
				MessageBox  msgBox("Your ACNL version isn't\nsupported!\nMake sure you have the\n1.5 update installed!");
				msgBox();
				return (0);
			}
		}
			
        else if (tid == 0x0004000000086200)
		{
			if (ver == 6272)
			{
				assign_region(JAP);
			}
			
			else
			{
            MessageBox  msgBox("Your ACNL version isn't\nsupported!\nMake sure you have the\n1.5 update installed!");
            msgBox();
            return (0);
			}
		}
			
        else
        {
            // Game not recognised
            MessageBox  msgBox("Error\nGame not supported !\nVisit discord for support.");

            msgBox();
            return (0);
        }

        /*
        ** Garden
        ********************/

        MenuFolder* folder = new MenuFolder("Save File Codes");

        folder->Append(new MenuEntry("Set name to...", SetNameTo));
        folder->Append(new MenuEntry("Garden Dumper", nullptr, GardenDumper));
        folder->Append(new MenuEntry("Garden Restore", nullptr, GardenRestore));
        folder->Append(new MenuEntry("Inject TCP Picture", InjectTCP));

        menu.Append(folder);

        /*
        ** Movement
        ********************/

        folder = new MenuFolder("Movement Codes");

        folder->Append(new MenuEntry("Coordinates Modifier", CoordinateModifier, "Press \uE000 and D Pad in the direction that you want to move."));
        folder->Append(new MenuEntry("Touch Coordinates", TouchCoordinates, "Touch the map to teleport your character there."));
        folder->Append(new MenuEntry("Teleport", Teleporter, "Press \uE001 and \uE079 to save your location, \uE001 and \uE07A to teleport back to the location. Use \uE052 or \uE053 to use multiple locations!"));
        folder->Append(new MenuEntry("Walk Over Things", WalkOverThings, "Press \uE052 and \uE079 to enable walking through stuff, \uE052 and \uE07A to disable walking through stuff."));
        folder->Append(new MenuEntry("Speed Hack", SpeedHack, SpeedHackEditor));
        folder->Append(new MenuEntry("Moon Jump", MoonJump, "Press \uE052 and \uE079 to go higher and \uE07A to go lower."));
        
        menu.Append(folder);


        folder = new MenuFolder("Main Street Codes");

        folder->Append(new MenuEntry("Nookling Upgrades", nullptr, NooklingKeyboard));
        folder->Append(new MenuEntry("Fill out Catalog", FillCatalog));
        folder->Append(new MenuEntry("Fill out Main Street", FillMainStreet, "Unlocks all the Main Street buildings except Leif + Nooklings"));
        menu.Append(folder);

        /*
        ** Inventory
        ********************/

        folder = new MenuFolder("Inventory");

        folder->Append(new MenuEntry("Text to Item", Text2Item, "Press " FONT_X " and " FONT_DR " to open the keyboard to enter in the ID you want to recieve."));
        folder->Append(new MenuEntry("Duplicate", Duplication, "Press " FONT_R " to duplicate the item that is slot 01 to slot 02."));
        folder->Append(new MenuEntry("Show names of buried items", ShowBuriedItems));
        folder->Append(new MenuEntry("Pick up buried items", PickBuriedItems));
        folder->Append(new MenuEntry("Set bells", nullptr, SetBells));
        folder->Append(new MenuEntry("Inventory box extender", ExtendedInventoryBox, "This allows you to create 10 additionals boxes to store your items.\nHold Start 1 second to choose which one to open."));
        folder->Append(new MenuEntry("Fossil Inspector", GenerateFossils, "Press " FONT_X " and " FONT_A " to process all fossils\nas if you talked to Blathers"));

        menu.Append(folder);

        /*
        ** Environment
        ********************/

        folder = new MenuFolder("Enviroment");

        /* Subfolder of Enviroment
        ** These codes only execute when R+A is pressed, so I only want 1 enabled. 
        ** I use the radio group parameter to prevent multiple from being enabled.
        *****************************************************************************/
        MenuFolder  *ra = new MenuFolder("R + A Codes");

        ra->Append(new MenuEntry(1, "Remove All Items", RemoveAllItems, "Press " FONT_R " and " FONT_A " to execute... Beware as there is no going back if you save."));
        ra->Append(new MenuEntry(1, "Remove All Weeds", RemoveAllWeeds, "Press " FONT_R " and " FONT_A " to execute."));
        ra->Append(new MenuEntry(1, "Water All Flowers", WaterAllFlowers, "Press " FONT_R " and " FONT_A "to execute."));
        
        folder->Append(ra);
        folder->Append(new MenuEntry("Real Time World Edit", WorldEdit, "Press " FONT_R " and " FONT_DL " to open the keyboard to store the item. " FONT_R " and " FONT_DU " to store the item that you're standing on. And " FONT_R " + " FONT_DD " to write the item to the place that you're standing on."));

        menu.Append(folder);

        /*
        ** Time Travel
        ********************/

        folder = new MenuFolder("Time Travel Codes");

        folder->Append(new MenuEntry("Time Travel", TimeTravel, "Press either " FONT_R " or " FONT_B " and " FONT_DR " to travel forward or " FONT_DL " to retwind time or " FONT_B " and " FONT_DD " to set ingame time back to your 3DS's clock."));
        folder->Append(new MenuEntry("Time Machine", TimeMachine, "Press " FONT_Y " and " FONT_DR " to start time traveling."));
        
        menu.Append(folder);

        /*
        ** Misc.
        ********************/

        folder = new MenuFolder("Misc.");

        folder->Append(new MenuEntry("Real Time Building Placer", BuildingPlacer));
        folder->Append(new MenuEntry("Ghost Mode", GhostMode));
        folder->Append(new MenuEntry("Camera Mod", CameraMod));
        folder->Append(new MenuEntry("Keyboard Extender", KeyboardExtender, "This extends the max characters that you can type into chat to 54 characters. Now you can type short stories into chat :)"));
        
        menu.Append(folder);

        /*
        ** Callbacks
        ********************/

        // Add Text2Cheat to plugin's main loop
        menu.Callback(SleepThread);
        menu.Callback(CheatsKeyboard);
        menu.Callback(PlayerUpdateCallback);
        Assign();
        Game::Initialize();

        // Launch menu and mainloop
        menu.Run();

        // Exit plugin
        return (0);
    }
}
