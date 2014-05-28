SEED MANAGER, by Olybri
Beta version 1.1.1, build #0007
http://tinyurl.com/seed-manager

******* ABOUT ********
The "Seed Manager" is a console application. It's currently in beta, so you might expect some bugs that will be fixed in later updates, as well as an update for a graphical user interface.
This will allow you to change the properties of the challenges used in Rayman® Legends.

******* CREDIT ********
Olybri: Author of this program
DJTHED: Discovered a way to modify the seed
CaneofPacci: Unlocked the training room on PC

******* UPDATES ********
The program has an auto-update feature, which needs an internet connexion.
Follow more informations about updates here: http://goo.gl/uEN2Wt

WHAT'S NEW?
   -- Beta 1.1.1 - 28th May 2014 --
-Deleted the server connexion when entering the training room (users can play offline now).
-Added an option to change the time limit of a challenge.
-Added a mod for the Dojo (used in the tournament).

   -- Beta 1.1.0 - 22th May 2014 --
-Added a new menu system.
-Added an option to change the challenge's goal (meters/lums) of the LotLD, the pit and the tower.
-Now the user needs to press the 'C' key to load the current challenge properties.
-Added a sub-menu for the challenge properties.
-Added the "change seed", "change goal" and "back to menu" options in the sub-menu.
-Decreased the processing time to change the seed for the LotLD, the pit and the tower.

   -- Beta 1.0.3 - 2nd May 2014 --
-Fixed a bug which made the user unable to change the seed if he was owning the non-Steam™ version of Rayman® Legends.
-Added an option which allows the user to specify the Bundle_PC path if it's not at its default location (it needs a password, contact me if needed).

  -- Beta 1.0.2 - 28th April 2014 --
-Fixed a bug which made the user unable to install the training room if he was owning the non-Steam™ version of Rayman® Legends.

  -- Beta 1.0.1 - 28th April 2014 --
-Fixed a bug which made the user unable to install the training room if a window was named "Rayman Legends" (even if the game wasn't running).
-Fixed a similar bug which allowed the user to locate the seed even if the game wasn't running.
  
   -- Beta 1.0 - 25th April 2014 --
This is the first public release of this program.

******* USAGE ********
To start editing the current challenge's properties, launch seed_manager.exe (admin rights are maybe needed).
You have 4 hotkeys you can use:
	Press the 'C' key to load the current challenge properties (more informations below).
	Press the 'T' key to install the training room mod (more informations below).
	Press the 'D' key to install the mod for the Dojo (used in the tournament).
	Press the 'H' key to open this file.
	Press the 'Q' key to exit the program.
	
******* CHALLENGE PROPERTIES ********
Before pressing the 'C' key, follow these steps:
- Be sure that the training room mod is installed! (more informations below)
- Start Rayman® Legends and go into a challenge in the training room.
- Wait until the countdown ("3, 2, 1, GO!") is finished.
- Pause the game, return on Seed Manager and NOW press the 'C' key.

The program is going to ask you if the running challenge is in the Dojo or not. Answer by pressing 'Y' (for yes) or 'N' (for no).
(Note: that's because we have not been able to find all the properties of the dojo in the RAM yet.)

Now the program is going to look for the current challenge's properties in the game's memory (level, event, goal, difficulty and seed). If it succeeds, the program will open a new menu with new options.
You have now 5 hotkeys you can use:
	Press the 'S' key to start changing the seed (more informations below).
	Press the 'G' key to start changing the goal (more informations below).
	Press the 'L' key to start changing the time limit (more informations below).
	Press the 'B' key to go back to the main menu.
	Press the 'H' key to open this file.
	Press the 'Q' key to exit the program.
	
Note: Before going in another challenge, press the 'B' key when you leave the challenge in the game.
	
******* CHANGING THE SEED ********
After pressing the 'S' key, the program will ask you to input the new seed that will replace the one currently in use.
A seed is composed of 8 hexadecimal digits (from 0 to F). Example of seeds: 7C 2E 33 A6, or F9 6C 53 9D, ...
Press the 'Enter' key to apply the changes. Wait until the program says "Seed has been changed successfully!".
Then go back on Rayman® Legends and restart the challenge. You should now be playing in the same challenge type, but with a different layout.

(Note: you can find some of the seeds of the old challenges from the past here: http://goo.gl/Zspo6M )
TIP: If you want to paste a seed you have copied, right-click on the window then click 'Paste'.

******* CHANGING THE GOAL ********
After pressing the 'G' key, the program will ask you to input the new goal (meters or lums) that will replace the one currently in use.
The goal is a floating point value. Example of goals: 500, 32.25, 60500, ...
Press the 'Enter' key to apply the changes. Wait until the program says "Goal has been changed successfully!".
The modifications are made immediately. When you'll go back on Rayman® Legends, you won't need to restart the challenge.

******* CHANGING THE TIME LIMIT ********
After pressing the 'L' key, the program will ask you to input the new time limit (in seconds) that will replace the one currently in use.
The time limit is a floating point value. Example of time limits: 120, 21.45, 3400, ...
Press the 'Enter' key to apply the changes. Wait until the program says "Time limit has been changed successfully!".
The modifications are made immediately. When you'll go back on Rayman® Legends, you won't need to restart the challenge.

******* THE TRAINING ROOM ********
The training room is a room which is unaccessable in the game normally. It was a room that was likely used by the developers to test the challenges.
However, DJTHED found a glitch on the Wii U version of the game which allows you to access this room.
Since the PC version of this game is easily moddable, CaneofPacci was able to unlock this room on this version of the game.

The training room is called the "training" room because the name of the file it uses is called "training". It is a room that contains four functional paintings: The Neverending Pit, The Land of the Livid Dead, The Dojo and The Infinite Tower. Murfy's Dungeon is inaccessible due to technical reasons we are currently trying to resolve.
Each painting sends you in a challenge, letting you choose the event type ("As far as you can", "Grab them quickly" etc.) and the difficulty ("normal" or "expert"). The list may look like it repeats, and that's because the 2nd occurence of the challenge types are the weekly variants. The first occurence are the daily variants.

To avoid cheating, the training room MUST be installed to use this program for seed modification! The installation will also disable the suitcase menu, which will make you completely unable to access the default online challenges.
To install the training room mod, simply press the 'T' key in Seed Manager (Rayman® Legends should not be running in order to avoid data corruption or cheating).
To uninstall the training room mod, press 'T' again in Seed Manager. If it succeeds, the program should say "Training room has been (un)installed successfully!".
NOTE: Once in the game, if you can't go into the training room, try to launch Seed Manager with admin rights.

******* ERRORS ********
If you have problems, try doing the following.
- If you get errors in Seed Manager, be sure you did exactly what is written above!
- Be sure you waited the "3, 2, 1, GO!" countdown before pressing the 'C' key.
- Be sure you pressed the 'B' when you left a challenge and want to go in another one.
- If your antivirus is telling you that Seed Manager is dangerous, it's because the program is messing with the RAM. Trust me, it's not dangerous! Just tell your antivirus that this program is fine.
- For Steam users: if you suddenly can't play Rayman® Legends anymore (that would be really unusual), first contact me and tell me what's happening. (Contact information below)
	Try to uninstall the training room. If it's still not working, go to C:/Program Files (x86)/Steam/SteamApps/common/Rayman Legends/ and delete Bundle_PC.ipk.
	Then open Steam and right-click on Rayman® Legends > 'Properties' > 'Local files' > 'Verify integrity of game cache...'. Steam will then download Bundle_PC.ipk again.
  (For non-Steam users: I'd say backup your game save (My Documents/Rayman Legends/(Uplay Name)) and reinstall the game.)

If you are still having problems, simply go to the contact section below and send me a message. Do not hesitate! My program is in beta so it might not work all of the time. 

******* CONTACT ********
For any problem, bug or suggestion, please contact me:
Olybri:
	olibrius.media@gmail.com
	Twitter: @olybri
	
Other contacts:
DJTHED:
	raymancustomchallenges@gmail.com
	
CaneofPacci:
	caneofpacci@gmail.com
	Twitter: @CaneofPacci
