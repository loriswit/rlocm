RAYMAN LEGENDS OFFLINE CHALLENGE MANAGER
Version 1.0.1
http://tinyurl.com/rlocm-download

******* ABOUT ********
RLOCM is a GUI applicaiton which will allow you to change the rules of the offline challenges used in Rayman® Legends.

******* CREDIT ********
Olybri: Author of this program
DJTHED: Discovered a way to modify the seed
CaneofPacci: Unlocked the training room on PC
UsWar: Discovered nice things in the RAM

******* UPDATES ********
Follow the updates here: http://goo.gl/uEN2Wt

WHAT'S NEW?
  -- 1.0.1 - 30th August 2014 --
-Removed the password request when the Bundle_PC is not in the default directory
-Added internal functions to avoid cheating

  -- 1.0.0 - 7th August 2014 --
-Added a GUI to the program.
-Added an "Apply changes" button.
-Added a function to edit the score limit of the challenges.
-Added a "Random seed" button.
-Added a "Reset" button (to reset the modified values).
-Added a menu with save/open states functions (and copy/paste states).
-Removed the "current challenge is a dojo" button.
-Now every challenge rules in every challenges is modifiable.
-Fixed a bug with the password.

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
To start editing the current challenge's rules, launch rlocm.exe (admin rights are maybe needed).
Before doing anythig, please follow these steps:
- Be sure that the training room is installed! (more informations below)
- Start Rayman® Legends and go into a challenge of your choice in the training room.
- Wait until the countdown ("3, 2, 1, GO!") is finished.
- Pause the game and return on RLOCM.
Now you're ready to load the current challenges rules by pressing the "Load challenge" (Ctrl+L) button. This process may take several seconds.

******* CHALLENGE RULES ********
The program is going to look for the current challenge's rules in the game's memory (level, event, goal, score limit, difficulty and seed). If it succeeds, you'll be able to edit these values.
The "Seed" input field is the seed of the current challenge. This number will define the layout of the challenge.
The "Goal" input field is the goal to reach in the challenge. It can be an amount of meters or lums.
The "Score limit" input field is the limit we have to reach (or to not exceed) so that the score is valid. It can be an amount of seconds or meters.

To apply the changes, simply press the "Apply change button" (Ctrl+A). Wait until the program says "Changes saved!". The program also tells you if you have to restart the challenge.
Now go back on Rayman® Legends (and restart the challenge if needed) and play with your new rules!
NOTE: You can reset the changes from the last time you applied the modificaitons with the "Reset" button.

IMPORTANT: If you leave the challenge in-game, don't apply changes anymore! This could corrupt the game's memory.
If you enter another challenge, you'll have to press the "Load challenge" button again!

You can save your rules with a name with "File" > "Save rules..". The rules are separated for each challenge/event/difficulty.
To open the rules again, go in the same challenge/event/difficulty and use "File" > "Open rules...".
NOTE: You can also copy your current rules with "Challenge" > "Copy current rules" (Ctrl+C). You can paste it in RLOCM with "Paste rules" (Ctrl+V) or anywhere else to share your rules with your friends.

******* CHANGING THE SEED ********
Input a new seed in the "Seed" input field, which will replace the one currently in use.
A seed is composed of 8 hexadecimal digits (from 0 to F). Example of seeds: 7C 2E 33 A6, or F9 6C 53 9D, ...
You can press the "Random seed" button to let the program choose a seed for you.

(Note: you can find some of the seeds of the old challenges from the past here: http://goo.gl/Zspo6M )

******* CHANGING THE GOAL ********
Input a new goal in the "Goal" input field, which will replace the one currently in use.
The goal is a floating point value. Example of goals: 500, 32.25, 60500, ...
It can be either an amount of meters or lums.

******* CHANGING THE SCORE LIMIT ********
Input a new goal in the "Score limit" input field, which will replace the one currently in use.
The score limit is a floating point value. Example of limits: 120, 21.45, 3400, ...
It can be either an amount of seconds or meters.

******* THE TRAINING ROOM ********
The training room is a room which is unaccessable in the game normally. It was a room that was likely used by the developers to test the challenges.
However, DJTHED found a glitch on the Wii U version of the game which allows you to access this room.
Since the PC version of this game is easily moddable, CaneofPacci was able to unlock this room on this version of the game.

The training room is called the "training" room because the name of the file it uses is called "training". It is a room that contains four functional paintings: The Neverending Pit, The Land of the Livid Dead, The Dojo and The Infinite Tower. Murfy's Dungeon is inaccessible due to technical reasons we are currently trying to resolve.
Each painting sends you in a challenge, letting you choose the event type ("As far as you can", "Grab them quickly" etc.) and the difficulty ("normal" or "expert"). The list may look like it repeats, and that's because the 2nd occurence of the challenge types are the weekly variants. The first occurence are the daily variants.

To avoid cheating, the training room MUST be installed to use this program for seed modification! The installation will also disable the suitcase menu, which will make you completely unable to access the default online challenges.
To install the training room mod, simply check the "Enable training room" box. To uninstall the training room mod, uncheck this "Enable training room" box. 
Rayman® Legends should not be running during this process in order to avoid data corruption or cheating.
If this succeeds, the program should say "Training room has been (un)installed successfully!".
NOTE: Once in the game, if you can't go into the training room, try to launch RLOCM with admin rights.

******* ERRORS ********
If you have problems, try doing the following.
- If you get errors in RLOCM, be sure you did exactly what is written above!
- Be sure you waited the "3, 2, 1, GO!" countdown before loading the challenge rules.
- Be sure you pressed you load the rules again when you went into a new challenge.
- If your antivirus is telling you that RLOCM is dangerous, it's because the program is messing with the RAM. Trust me, it's not dangerous! Just tell your antivirus that this program is fine.
- For Steam users: if you suddenly can't play Rayman® Legends anymore (that would be really unusual), first contact me and tell me what's happening. (Contact information below)
	Try to uninstall the training room. If it's still not working, go to C:/Program Files (x86)/Steam/SteamApps/common/Rayman Legends/ and delete Bundle_PC.ipk.
	Then open Steam and right-click on Rayman® Legends > 'Properties' > 'Local files' > 'Verify integrity of game cache...'. Steam will then download Bundle_PC.ipk again.
  (For non-Steam users: I'd say backup your game save (My Documents/Rayman Legends/(Uplay Name)) and reinstall the game.)

If you are still having problems, simply go to the contact section below and send me a message. Do not hesitate!

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
