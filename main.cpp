#include <map>
#include <time.h>
#include <iostream>

#include "parser.h"
#include "gameClock.h"
#include "room.h"
#include "path.h"
#include "container.h"
#include "matchbook.h"
#include "light.h"
#include "food.h"
#include "npc.h"
#include "pc.h"
#include "weapon.h"


/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: main.cpp
*/

//This function is run every round to make sure that the player is still alive
void checkAlive(Player* x, Room* room, Player* player, std::vector<Room*> rooms) 
{

	if (x != nullptr && !x->isAlive()) 
	{
		std::cout << "\n\nYou have died.\nWe will try to fix you up the best we can.\n\n";
		std::string junk;
		getline(std::cin, junk);
		room->removePlayer(player);
		delete player;

		//Reset the health of any players in that room
		for (Player* pl : room->players()) 
		{
			pl->hp(pl->maxHP());
		}

		room = rooms[0];
		player = new Player();
		room->addPlayer(player);
		std::cout << room->description();
	}	
	else if (x != nullptr && x->hp() <= 0) 
	{

		for (Object* o : x->inventory()) 
		{
			room->addObject(o);
		}

		std::cout << "\n\nYou have died.\nWe will try to fix you up the best we can.\n\n";
		std::string junk;
		getline(std::cin, junk);
		room->removePlayer(player);
		delete player;

		//Reset the health of any players in that room
		for (Player* pl : room->players()) 
		{
			pl->hp(pl->maxHP());
		}

		room = rooms[0];
		player = new Player();
		x = player;
		room->addPlayer(player);
		std::cout << room->description();
	}
	
}


int main() 
{
	srand(time(NULL));

	//A vector to hold all the rooms
	std::vector<Room*> rooms = {};
	Object* tempOb;
	Player* player = new PC();

	rooms.push_back(new Room("North of House", "You are on an overgrown lawn. There is a white house in front of you and trees behind you", std::vector<Path*>{}, 
		std::vector<Player*>{}, std::vector<Object*>{}, false, false));
	rooms[0]->addPlayer(player);
	rooms[0]->addObject(new Container("mailbox", 100000, 100000, "The mailbox is black.", 2.5, true));
	tempOb = Object::findObject("mailbox", rooms[0]->objects());

	if (tempOb != nullptr) 
	{
		tempOb->addObject(new Object("letter", .1, .3, 
			"Hello traveler, welcome to Samuel Copeland's Dungeon Crawler version alpha!\n\nTo get instructions type \"man\" in the command line.\n\nHave a great trip through the caverns!\n\t-Samuel Copeland"));
		tempOb->close();
	}

	rooms[0]->addObject(new Object("shovel", 5, 3, "The shovel has a wooden handle and an iron head."));
	rooms[0]->addObject(new Container("canvas_bag", .5, 1, "The bag is made of roughly woven canvas.", 20));
	rooms[0]->addObject(new Light("electric_lamp", 2, 2, "The electric_lamp looks a little rusty.", 2000));
	rooms[0]->addObject(new Matchbook("matchbook", .1, .1, "There is nothing special about the matchbook.", 10, false));
	rooms[0]->addObject(new Light("candle", .5, .5, "The candle is made of yellow wax and has a wick made out of an unknown fibrous material.", 100, true, true));
	rooms[0]->addObject(new Container("handbag", .5, .5, "The handbag has a metal clasp.", 1.5));

	//Add a weapon
	rooms[0]->addObject(new Weapon("sword", 3.5, 2, "The sword glints in the light.", 7.5));

	//Add a food
	rooms[0]->addObject(new Food("orange", .5, .5, "The orange looks perfectly ripe."));
	rooms[0] ->addObject(new Food("apple", .5, .5, "The apple looks perfectly ripe.", 6));
	rooms[0]->addObject(new Food("watermelon", 2, 2, "The watermelon makes a nice hollow sound when you tap it.", 10));
	//Add a food that is max size and weight to test inventory
	rooms[0]->addObject(new Food("hippo_carcass", 50, 20, "The carcass is starting to rot.", -2));
	//Add a poison to test.
	rooms[0]->addObject(new Food("cyanide", .1, .1, "The cyanide looks deadly", -26));


	//Add neuteral npc with a weapon
	rooms[0]->addPlayer(new NPC("miner", 10, 1.2, 10, 25, 58, Aggression::NEUTERAL));
	Player::findPlayer("miner", rooms[0]->players())->addObject(new Object("gold_bar", 10, 2, "The gold_bar is heavy."));
	Player::findPlayer("miner", rooms[0]->players())->addObject(new Weapon("pickaxe", 10, 5, "The pickaxe has a very sharp tip.", 10));
	

	//Add a neuteral npc without a weapon
	rooms[0]->addPlayer(new NPC("ninja", 50, 3.4, 15, 30, 70, Aggression::NEUTERAL));
	Player::findPlayer("ninja", rooms[0]->players())->addObject(new Object("heavy_metal_ball", 50, 2, "The heavy_metal_ball is made out of an incredibly dense unidentifiable metal."));

	//Add a new room and add paths that connects the two rooms
	rooms.push_back(new Room("Inside Kitchen", "You are in a mid 1800s style kitchen.", std::vector<Path*>(), std::vector<Player*>(), std::vector<Object*>(), true, true));

	//Add paths
	rooms[0]->addPath(new Path("open door", Compass::SOUTH, rooms[1]));
	rooms[1]->addPath(new Path("open door", Compass::NORTH, rooms[0]));

	//Add an aggressive npc without a weapon
	rooms[1]->addPlayer(new NPC("cyclops", 60, 5, 2, 100, 250, Aggression::AGGRESSIVE));
	Player::findPlayer("cyclops", rooms[1]->players())->addObject(new Object("gold_fleece", 10, 3, "The gold_fleece appears to cause the plants near it to become greener."));

	Room* room = rooms[0];
	Clock* clock = new Clock();
	std::cout << room->description() << std::endl;

	//Main game loop
	std::string userInput;
	bool cont = true;
	Parser parser;

	while (cont) 
	{
		bool newRoom = false;
		getline(std::cin, userInput);


		//Split the string into a vector of words
		std::vector<std::string> commands = parser.parseInput(userInput);


		//Depending on how many words are in the command, try different possible commands
		switch (commands.size()) 
		{
		//0 words in the command
		case 0:
			std::cout << "Please enter a command:\n";
			continue;
			break;
		//1 word in the command
		case 1:

			//Show the Manual
			if (userInput == "man" || userInput == "manual") 
			{
				std::cout << "Type \"quit\" to end the game.\n";
				std::cout << "More information needs to be added to this.\n";
				continue;
			}

			//Quit the game
			else if (userInput == "quit" || userInput == "q") 
			{
				std::cout << "Are you sure you want to exit (y/n)?\n";
				std::string answer;
				getline(std::cin, answer);

				if (answer == "y" || answer == "Y") 
				{
					std::cout << "Ending game...\n";
					cont = false;
				}
				else 
				{
					std::cout << "Continuing the game...\n";
					continue;
				}

			}
			//Describe what the player can see
			else if (userInput == "look" || userInput == "l") 
			{
				std::cout << room->description() << std::endl;
				continue;
			}
			//Describe the player's inventory
			else if (userInput == "inventory" || userInput == "i") 
			{
				Player* p = Player::findPlayer("you", room->players());
				std::cout << "You are holding the following items: \n" + p->printInventory();
				continue;
			}
			//Describe the player's health
			else if (userInput == "medcheck" || userInput == "health" || userInput == "hp") 
			{
				Player* p = Player::findPlayer("you", room->players());
				std::cout << "You have " << p->hp() << "/" << p->maxHP() << " left.\n";
				continue;
			}
			//Allow time to pass
			else if (userInput == "wait") 
			{
				std::cout << "Time passes...\n";
			}
			//The command wasn't recognized
			else 
			{
				std::cout << "Invalid command\n";
				continue;
			}

			break;
		//Two words in the command
		case 2:

			//Try to open a container object
			if (commands[0] == "open") 
			{
				tempOb = Object::recursiveFindByName(commands[1], room->objects());

				if (tempOb != nullptr) 
				{
					if (tempOb->open()) 
					{
						std::cout << "The " + commands[1] + " is open.\n";
						if(tempOb->inventory() != std::vector<Object*> ())
						{
							std::cout << tempOb->printInventory("") << std::endl;
						}
							
					}
					else 
					{
						std::cout << "You can't open the " + commands[1] + ".\n";
					}
				}
				else 
				{
					tempOb = Object::recursiveFindByName(commands[1], player->inventory());

					if (tempOb != nullptr) 
					{

						if (tempOb->open()) 
						{
							std::cout << "The " + commands[1] + " is open.\n";

							if (tempOb->inventory() != std::vector<Object*>())
							{
								std::cout << tempOb->printInventory("") << std::endl;
							}

						}
						else 
						{
							std::cout << "You can't open the " + commands[1] + ".\n";
						}

					}
					else 
					{
						std::cout << "There is no " + commands[1] + " here.\n";
					}

				}

			}
			//try to close a container object
			else if (commands[0] == "close") 
			{
				tempOb = Object::recursiveFindByName(commands[1], room->objects());

				if (tempOb != nullptr) 
				{

					if (tempOb->close()) 
					{
						std::cout << "The " + commands[1] + " is closed.\n";
					}
					else 
					{
						std::cout << "You can't close the " + commands[1] + ".\n";
					}

				}
				else 
				{
					tempOb = Object::recursiveFindByName(commands[1], player->inventory());

					if (tempOb != nullptr) 
					{
						if (tempOb->close()) 
						{
							std::cout << "The " + commands[1] + " is closed.\n";
						}
						else 
						{
							std::cout << "You can't close the " + commands[1] + ".\n";
						}
					}
					else 
					{
						std::cout << "There is no " + commands[1] + " here.\n";
					}

				}

			}
			//try to take an object
			else if (commands[0] == "take") 
			{
				Player* p = Player::findPlayer("you", room->players());
				Object* container = nullptr;
				tempOb = Object::findObject(commands[1], room->objects());

				if (tempOb != nullptr) 
				{

					if (player->addObject(tempOb)) 
					{
						room->removeObject(tempOb);
						std::cout << "Taken\n";
					}
					else 
					{
						std::cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
					}

				}
				else if (Object::recursiveFindByName(commands[1], room->objects()) != nullptr) 
				{
					tempOb = Object::recursiveFindByName(commands[1], room->objects());

					for (Object* o : room->objects()) 
					{
						container = Object::recursiveFindContainer(tempOb, o);

						if (container != nullptr) 
						{
							std::cout << "Taken\n";
							container->removeObject(tempOb);
							player->addObject(tempOb);
							break;
						}

					}
					if (container == nullptr) 
					{
						std::cout << "There is no " + commands[1] + " here.\n";
					}

				}
				else if (Object::recursiveFindByName(commands[1], player->inventory()) != nullptr) 
				{
					tempOb = Object::recursiveFindByName(commands[1], player->inventory());

					for (Object* o : player->inventory()) 
					{
						container = Object::recursiveFindContainer(tempOb, o);

						if (container != nullptr) 
						{
							std::cout << "Taken\n";
							container->removeObject(tempOb);
							player->addObject(tempOb);
							break;
						}

					}

					if (container == nullptr) 
					{
						std::cout << "There is no " + commands[1] + " here.\n";
					}

				}
				else 
				{
					std::cout << "There is no " + commands[1] + " here.\n";
				}

			}
			//Try to drop an object that the player is holding
			else if (commands[0] == "drop") 
			{
				tempOb = Object::findObject(commands[1], player->inventory());

				if (tempOb != nullptr)
				{
					player->removeObject(tempOb);
					room->addObject(tempOb);
					std::cout << "Dropped\n";
				}
				else
				{
					std::cout << "You are not holding the " + commands[1] + ".\n";
				}

			}
			//Describe an object
			else if (commands[0] == "examine") 
			{
				tempOb = Object::recursiveFindByName(commands[1], room->objects());

				if (tempOb != nullptr) 
				{
					std::cout << tempOb->description() << std::endl;
				}
				else 
				{
					tempOb = Object::recursiveFindByName(commands[1], player->inventory());

					if (tempOb != nullptr) 
					{
						std::cout << tempOb->description() << std::endl;
					}
					else 
					{

						for (Player* p : room->players()) 
						{
							tempOb = Object::recursiveFindByName(commands[1], p->inventory());

							if (tempOb != nullptr) 
							{
								std::cout << tempOb->description() << std::endl;
								break;
							}

						}

						if (tempOb == nullptr) 
						{
							std::cout << "There is no " + commands[1] + " here.\n";
						}

					}	

				}

				continue;
			}
			else if (commands[0] == "eat") 
			{
				Player* p = Player::findPlayer("you", room->players());
				tempOb = Object::findObject(commands[1], p->inventory());

				if (tempOb != nullptr) 
				{

					if (tempOb->eat(p)) 
					{
						std::cout << "You ate the " + tempOb->name() + ".\n";
						p->removeObject(tempOb);
						delete tempOb;
					}
					else 
					{
						std::cout << "You can't eat the " + commands[1] + ".\n";
					}

				}
				else 
				{
					std::cout << "There is no " + commands[1] + " here.\n";
				}

			}
			else if (commands[0] == "light") 
			{
				Player* p = Player::findPlayer("you", room->players());
				tempOb = Object::findObject(commands[1], player->inventory());

				if (tempOb != nullptr) 
				{

					if (tempOb->light(nullptr))
					{
						std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";

						if (room->isDark()) 
						{
							room->isDark(false);
							std::cout << room->description() << std::endl;
						}

					}
					else 
					{
						std::cout << "You can't light the " + tempOb->name() + "\n";
					}

				}
				else
				{
					tempOb = Object::recursiveFindByName(commands[1], room->objects());
						
					if (tempOb != nullptr) 
					{

						if (player->addObject(tempOb)) 
						{
							room->removeObject(tempOb);
							std::cout << "Taken\n";
							Object* container = nullptr;

							for (Object* ob : room->objects()) 
							{
								container = Object::recursiveFindContainer(tempOb, ob);

								if (container != nullptr) 
								{
									container->removeObject(tempOb);
									break;
								}

							}

							if (tempOb->light(nullptr)) 
							{
								std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";

								if (room->isDark()) 
								{
									room->isDark(false);
									std::cout << room->description() << std::endl;
								}

							}
							else 
							{
								std::cout << "You can't light the " + tempOb->name() + "\n";
							}

						}
						else 
						{
							std::cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
						}

					}
					else 
					{
						std::cout << "There is no " + commands[1] + " here.\n";
					}

				}

			}
			else if (commands[0] == "go") 
			{

				if (Path::stringToCompass(commands[1]) != Compass::INVALIDDIRECTION) 
				{
					Room* origRoom = room;

					for (Path* p : room->paths()) 
					{

						if (p->dir() == Path::stringToCompass(commands[1])) 
						{
							room = p->travel();
							room->addPlayer(player);
							origRoom->removePlayer(player);
							clock->updateLights(origRoom, origRoom->players());
							clock->updateLights(room, room->players());
							std::cout << room->description() << std::endl;
							newRoom = true;
							break;
						}

					}

					if (room == origRoom)
					{
						std::cout << "There is no path to your " + commands[1] + ".\n";
					}
				}
				else 
				{
					std::cout << "Invalid direction.\n";
				}

			}
			else if (commands[0] == "attack") 
			{
				std::cout << "Attack with what?" << std::endl;
				std::string weapon;
				getline(std::cin, weapon);
				
				weapon = parser.parseInput(weapon)[0];

				Player* e = Player::findPlayer(commands[1], room->players());
				bool isDark = room->isDark();

				if (e != nullptr) 
				{
					Object* w = Object::findObject(weapon, player->inventory());

					if (w != nullptr) 
					{

						if (!isDark) 
						{
							std::vector<Object*> enemyInventory = player->attack(e, w);

							for (Object* o : enemyInventory) 
							{
								room->addObject(o);
							}

							if (!e->isAlive()) 
							{
								bool isYou = e == player;
								room->removePlayer(e);

								if (isYou) 
								{
									std::cout << "\n\nYou have died.\nWe will try to fix you up the best we can.\n\n";
									std::string junk;
									getline(std::cin, junk);

									//Reset the health of any players in that room
									for (Player* pl : room->players()) 
									{
										pl->hp(pl->maxHP());
									}

									room = rooms[0];
									player = new Player();
									room->addPlayer(player);
									std::cout << room->description();
								}

							}

						}
						else 
						{
							std::cout << "It is too dark, you don't see a " + commands[1] + " here.\n";
						}

					}
					else 
					{
						std::cout << "You are not holding the " + weapon + ".\n";
					}

				}
				else 
				{
					std::cout << "There is no " + commands[1] + " here.\n";
				}

			}
			//The command wasn't recognized
			else 
			{
				std::cout << "Invalid command\n";
				continue;
			}

			break;
		case 3:

			if (commands[0] + " " + commands[1] == "put out")
			{
				tempOb = Object::findObject(commands[2], player->inventory());

				if (tempOb != nullptr) 
				{

					if (tempOb->putOut()) 
					{
						std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now out.\n";
					}
					else 
					{
						std::cout << "The " + commands[2] + " is not lit.\n";
					}

				}
				else 
				{
					tempOb = Object::recursiveFindByName(commands[2], room->objects());

					if (tempOb != nullptr) 
					{

						if (player->addObject(tempOb)) 
						{
							room->removeObject(tempOb);
							std::cout << "Taken\n";
							Object* container = nullptr;

							for (Object* ob : room->objects()) 
							{
								container = Object::recursiveFindContainer(tempOb, ob);

								if (container != nullptr) 
								{
									container->removeObject(tempOb);
									break;
								}

							}

							if (tempOb->putOut())
							{
								std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now out.\n";

								if (!room->isDark()) 
								{
									room->isDark(true);
								}

							}
							else 
							{
								std::cout << "You can't put out the " + tempOb->name() + "\n";
							}

						}
						else 
						{
							std::cout << "You can't pick up the " + commands[2] + " you are holding too much.\n";
						}

					}
					else 
					{
						std::cout << "There is no " + commands[2] + " here.\n";
					}

				}

			}
			//The command wasn't recognized
			else 
			{
				std::cout << "Invalid command\n";
				continue;
			}
			
			break;
		case 4:
			if (commands[0] == "take" && commands[2] == "from") 
			{
				Object* tempContainer = Object::recursiveFindByName(commands[3], room->objects());

				if (tempContainer != nullptr) 
				{
					tempOb = Object::findObject(commands[1], tempContainer->inventory());

					if (tempOb != nullptr) 
					{

						if (player->addObject(tempOb))
						{
							tempContainer->removeObject(tempOb);
							std::cout << "Taken\n";
						}
						else 
						{
							std::cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
						}

					}
					else
					{
						std::cout << "Can't find the " + commands[1] + " in the " + commands[3] + ".\n";
					}

				}
				else 
				{
					tempContainer = Object::recursiveFindByName(commands[3], player->inventory());

					if (tempContainer != nullptr) 
					{
						tempOb = Object::findObject(commands[1], tempContainer->inventory());

						if (tempOb != nullptr) 
						{

							if (player->addObject(tempOb)) 
							{
								tempContainer->removeObject(tempOb);
								std::cout << "Taken\n";
							}
							else 
							{
								std::cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
							}

						}
						else
						{
							std::cout << "Can't find the " + commands[1] + " in the " + commands[3] + ".\n";
						}

					}
					else
					{
						std::cout << "Can't find the " + commands[3] + ".\n";
					}

				}

			}
			else if (commands[0] == "put" && commands[2] == "in") 
			{
				Object* tempContainer = Object::recursiveFindByName(commands[3], room->objects());

				if (tempContainer != nullptr) 
				{
					tempOb = Object::findObject(commands[1], player->inventory());

					if (tempOb != nullptr) 
					{

						if (tempContainer->addObject(tempOb))
						{
							player->removeObject(tempOb);
							std::cout << "The " + commands[1] + " is in the " + commands[3] + "\n";
						}
						else 
						{
							std::cout << "You can't fit the " + commands[1] + " in the " + commands[3] + ".\n";
						}

					}
					else 
					{
						std::cout << "You are not holding the " + commands[1] + ".\n";
					}

				}
				else
				{
					tempContainer = Object::recursiveFindByName(commands[3], player->inventory());

					if (tempContainer != nullptr) 
					{
						tempOb = Object::findObject(commands[1], player->inventory());

						if (tempOb != nullptr) 
						{

							if (tempContainer->addObject(tempOb)) 
							{
								player->removeObject(tempOb);
								std::cout << "The " + commands[1] + " is in the " + commands[3] + "\n";
							}
							else 
							{
								std::cout << "You can't fit the " + commands[1] + " in the " + commands[3] + ".\n";
							}

						}
						else 
						{
							std::cout << "You are not holding the " + commands[1] + ".\n";
						}

					}
					else 
					{
						std::cout << "Can't find the " + commands[3] + ".\n";
					}

				}	

			}
			else if (commands[0] == "attack" && commands[2] == "with") 
			{
				Player* p = Player::findPlayer("you", room->players());

				if (p != nullptr) 
				{
					Player* e = Player::findPlayer(commands[1], room->players());
					bool isDark = room->isDark();

					if (e != nullptr) 
					{
						Object* w = Object::findObject(commands[3], player->inventory());

						if (w != nullptr) 
						{

							if (!isDark) 
							{
								std::vector<Object*> enemyInventory = player->attack(e, w);

								for (Object* o : enemyInventory) 
								{
									room->addObject(o);
								}

								if (!e->isAlive())
								{
									bool isYou = e == player;
									room->removePlayer(e);

									if (isYou) 
									{
										std::cout << "\n\nYou have died.\nWe will try to fix you up the best we can.\n\n";
										std::string junk;
										getline(std::cin, junk);

										//Reset the health of any players in that room
										for (Player* pl : room->players()) 
										{
											pl->hp(pl->maxHP());
										}

										room = rooms[0];
										player = new Player();
										room->addPlayer(player);
										std::cout << room->description();
									}

								}

							}
							else 
							{
								std::cout << "It is too dark, you don't see a " + commands[1] + " here.\n";
							}

						}
						else 
						{
							std::cout << "You are not holding the " + commands[3] + ".\n";
						}

					}
					else
					{
						std::cout << "There is no " + commands[1] + " here.\n";
					}

				}
				else 
				{
					std::cout << "You are dead.\n";
				}

			}
			else if (commands[0] == "light" && commands[2] == "with") 
			{
				Player* p = Player::findPlayer("you", room->players());

				if (p != nullptr) 
				{
					Object* tempContainer = Object::findObject(commands[3], player->inventory());

					if (tempContainer != nullptr) 
					{
						tempOb = Object::findObject(commands[1], player->inventory());

						if (tempOb != nullptr) 
						{

							if (tempOb->light(tempContainer)) 
							{
								std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";

								if (room->isDark()) 
								{
									room->isDark(false);
									std::cout << room->description() << std::endl;
								}

							}
							else 
							{
								std::cout << "You can't light the " + commands[1] + " with the " + commands[3] + ".\n";
							}

						}
						else
						{
							tempOb = Object::findObject(commands[1], tempContainer->inventory());

							if (tempOb != nullptr) 
							{

								if (player->addObject(tempOb))
								{
									Object* containerContainer = nullptr;

									for (Object* obj : player->inventory())
									{
										containerContainer = Object::recursiveFindContainer(tempContainer, obj);

										if (containerContainer != nullptr) 
										{
											break;
										}

									}

									if (containerContainer != nullptr) 
									{
										containerContainer->removeObject(tempContainer);
										player->addObject(tempContainer);
									}
									
									tempContainer->removeObject(tempOb);
									std::cout << "Taken\n";

									if (tempOb->light(tempContainer)) 
									{
										std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";

										if (room->isDark()) 
										{
											room->isDark(false);
											std::cout << room->description() << std::endl;
										}

									}
									else 
									{
										std::cout << "You can't light " + commands[1] + " with " + commands[3] + ".\n";
									}

								}
								else 
								{
									std::cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
								}

							}
							else 
							{
								tempOb = Object::recursiveFindByName(commands[1], room->objects());

								if (tempOb != nullptr) 
								{

									if (player->addObject(tempOb)) 
									{
										std::cout << "Taken.\n";
										Object* container = nullptr;

										for (Object* potC : room->objects()) 
										{
											container = Object::recursiveFindContainer(tempOb, potC);

											if (container != nullptr)
											{
												break;
											}

										}

										if (container != nullptr) 
										{
											container->removeObject(tempOb);
										}
										else 
										{
											room->removeObject(tempOb);
										}

										if (tempOb->light(tempContainer))
										{
											std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";

											if (room->isDark())
											{
												room->isDark(false);
												std::cout << room->description() << std::endl;
											}

										}
										else
										{
											std::cout << "You can't light " + commands[1] + " with " + commands[3] + ".\n";
										}

									}
									else 
									{
										std::cout << "Couldn't pick up " + commands[1] + ". You are holding too much.\n";
									}

								}
								else
								{
									std::cout << "There is no " + commands[1] + " here.\n";
								}

							}

						}
						
					}
					else 
					{
						tempContainer = Object::recursiveFindByName(commands[3], player->inventory());

						if (tempContainer != nullptr) 
						{
							tempOb = Object::findObject(commands[1], player->inventory());

							if (tempOb != nullptr) 
							{

								if (tempOb->light(tempContainer)) 
								{
									std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";

									if (room->isDark()) 
									{
										room->isDark(false);
										std::cout << room->description() << std::endl;
									}

								}
								else 
								{
									std::cout << "You can't light the " + commands[1] + " with the " + commands[3] + ".\n";
								}

							}
							else
							{
								tempOb = Object::findObject(commands[1], tempContainer->inventory());

								if (tempOb != nullptr)
								{

									if (player->addObject(tempOb)) 
									{
										Object* containerContainer = nullptr;

										for (Object* obj : player->inventory())
										{
											containerContainer = Object::recursiveFindContainer(tempContainer, obj);

											if (containerContainer != nullptr) 
											{
												break;
											}

										}

										if (containerContainer != nullptr) 
										{
											containerContainer->removeObject(tempContainer);
											player->addObject(tempContainer);
										}

										tempContainer->removeObject(tempOb);
										std::cout << "Taken\n";

										if (tempOb->light(tempContainer))
										{
											std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";

											if (room->isDark()) 
											{
												room->isDark(false);
												std::cout << room->description() << std::endl;
											}

										}
										else 
										{
											std::cout << "You can't light " + commands[1] + " with " + commands[3] + ".\n";
										}

									}
									else 
									{
										std::cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
									}

								}
								else 
								{
									tempOb = Object::recursiveFindByName(commands[1], room->objects());

									if (tempOb != nullptr) 
									{

										if (player->addObject(tempOb)) 
										{
											std::cout << "Taken.\n";
											Object* container = nullptr;

											for (Object* potC : room->objects()) 
											{
												container = Object::recursiveFindContainer(tempOb, potC);

												if (container != nullptr) 
												{
													break;
												}

											}

											if (container != nullptr) 
											{
												container->removeObject(tempOb);
											}
											else 
											{
												room->removeObject(tempOb);
											}

											if (tempOb->light(tempContainer)) 
											{
												std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";
												if (room->isDark()) 
												{
													room->isDark(false);
													std::cout << room->description() << std::endl;
												}

											}
											else 
											{
												std::cout << "You can't light " + commands[1] + " with " + commands[3] + ".\n";
											}

										}
										else 
										{
											std::cout << "Couldn't pick up " + commands[1] + ". You are holding too much.\n";
										}

									}
									else 
									{
										std::cout << "There is no " + commands[1] + " here.\n";
									}

								}

							}

						}
						else if (Object::findObject(commands[3], room->objects()) != nullptr) 
						{
							tempContainer = Object::findObject(commands[3], room->objects());

							if (player->addObject(tempContainer)) 
							{
								std::cout << "Taken\n";
								Object* containerContainer = nullptr;

								for (Object* obj : room->objects()) 
								{
									containerContainer = Object::recursiveFindContainer(tempContainer, obj);

									if (containerContainer != nullptr) 
									{
										break;
									}

								}

								if (containerContainer != nullptr) 
								{
									containerContainer->removeObject(tempContainer);
								}
								else 
								{
									room->removeObject(tempContainer);
								}

								tempOb = Object::findObject(commands[1], player->inventory());

								if (tempOb != nullptr) 
								{

									if (tempOb->light(tempContainer)) 
									{
										std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";

										if (room->isDark())
										{
											room->isDark(false);
											std::cout << room->description() << std::endl;
										}

									}
									else
									{
										std::cout << "You can't light the " + commands[1] + " with the " + commands[3] + ".\n";
									}

								}
								else 
								{
									tempOb = Object::findObject(commands[1], tempContainer->inventory());

									if (tempOb != nullptr)
									{

										if (player->addObject(tempOb)) 
										{
											tempContainer->removeObject(tempOb);
											std::cout << "Taken\n";

											if (tempOb->light(tempContainer))
											{
												std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";

												if (room->isDark()) 
												{
													room->isDark(false);
													std::cout << room->description() << std::endl;
												}

											}

										}
										else 
										{
											std::cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
										}

									}
									else 
									{
										std::cout << "Can't find the " + commands[1] + ".\n";
									}

								}

							}

						}
						else if (Object::findObject(commands[3], room->objects()) != nullptr) 
						{
							tempContainer = Object::findObject(commands[3], room->objects());

							if (player->addObject(tempContainer)) 
							{
								std::cout << "Taken\n";
								Object* containerContainer = nullptr;

								for (Object* obj : room->objects()) 
								{
									containerContainer = Object::recursiveFindContainer(tempContainer, obj);

									if (containerContainer != nullptr) 
									{
										break;
									}

								}

								if (containerContainer != nullptr) 
								{
									containerContainer->removeObject(tempContainer);
								}
								else 
								{
									room->removeObject(tempContainer);
								}

								tempOb = Object::findObject(commands[1], player->inventory());

								if (tempOb != nullptr) 
								{

									if (tempOb->light(tempContainer)) 
									{
										std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";

										if (room->isDark()) 
										{
											room->isDark(false);
											std::cout << room->description() << std::endl;
										}

									}
									else 
									{
										std::cout << "You can't light the " + commands[1] + " with the " + commands[3] + ".\n";
									}

								}
								else
								{
									tempOb = Object::findObject(commands[1], tempContainer->inventory());

									if (tempOb != nullptr) 
									{

										if (player->addObject(tempOb)) 
										{
											tempContainer->removeObject(tempOb);
											std::cout << "Taken\n";

											if (tempOb->light(tempContainer)) 
											{
												std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";

												if (room->isDark()) 
												{
													room->isDark(false);
													std::cout << room->description() << std::endl;
												}

											}

										}
										else 
										{
											std::cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
										}

									}
									else 
									{
										std::cout << "Can't find the " + commands[1] + ".\n";
									}

								}

							}

						}
						else if (Object::findObject(commands[3], room->objects()) != nullptr) 
						{
							tempContainer = Object::findObject(commands[3], room->objects());

							if (player->addObject(tempContainer)) 
							{
								std::cout << "Taken\n";
								Object* containerContainer = nullptr;

								for (Object* obj : room->objects()) 
								{
									containerContainer = Object::recursiveFindContainer(tempContainer, obj);

									if (containerContainer != nullptr) 
									{
										break;
									}

								}

								if (containerContainer != nullptr) 
								{
									containerContainer->removeObject(tempContainer);
								}
								else 
								{
									room->removeObject(tempContainer);
								}

								tempOb = Object::findObject(commands[1], player->inventory());

								if (tempOb != nullptr) 
								{

									if (tempOb->light(tempContainer)) 
									{
										std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";

										if (room->isDark())
										{
											room->isDark(false);
											std::cout << room->description() << std::endl;
										}

									}
									else 
									{
										std::cout << "You can't light the " + commands[1] + " with the " + commands[3] + ".\n";
									}

								}
								else 
								{
									tempOb = Object::findObject(commands[1], tempContainer->inventory());

									if (tempOb != nullptr) 
									{

										if (player->addObject(tempOb)) 
										{
											tempContainer->removeObject(tempOb);
											std::cout << "Taken\n";

											if (tempOb->light(tempContainer))
											{
												std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";

												if (room->isDark())
												{
													room->isDark(false);
													std::cout << room->description() << std::endl;
												}

											}

										}
										else 
										{
											std::cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
										}

									}
									else 
									{
										std::cout << "Can't find the " + commands[1] + ".\n";
									}

								}

							}

						}
						else if (Object::recursiveFindByName(commands[3], room->objects()) != nullptr) 
						{
							tempContainer = Object::recursiveFindByName(commands[3], room->objects());

							if (player->addObject(tempContainer)) 
							{
								std::cout << "Taken\n";
								Object* containerContainer = nullptr;

								for (Object* obj : room->objects()) 
								{
									containerContainer = Object::recursiveFindContainer(tempContainer, obj);

									if (containerContainer != nullptr) 
									{
										break;
									}

								}

								if (containerContainer != nullptr) 
								{
									containerContainer->removeObject(tempContainer);
								}
								else 
								{
									room->removeObject(tempContainer);
								}
									
								tempOb = Object::findObject(commands[1], player->inventory());

								if (tempOb != nullptr)
								{

									if (tempOb->light(tempContainer))
									{
										std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";

										if (room->isDark()) {
											room->isDark(false);
											std::cout << room->description() << std::endl;
										}

									}
									else
									{
										std::cout << "You can't light the " + commands[1] + " with the " + commands[3] + ".\n";
									}

								}
								else 
								{
									tempOb = Object::findObject(commands[1], tempContainer->inventory());

									if (tempOb != nullptr) 
									{

										if (player->addObject(tempOb)) 
										{
											tempContainer->removeObject(tempOb);
											std::cout << "Taken\n";
	
											if (tempOb->light(tempContainer))
											{
												std::cout << "The " + Parser::split(tempOb->name(), " ")[0] + " is now lit.\n";

												if (room->isDark())
												{
													room->isDark(false);
													std::cout << room->description() << std::endl;
												}

											}
	
										}
										else 
										{
											std::cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
										}

									}
									else 
									{
										std::cout << "Can't find the " + commands[1] + ".\n";
									}

								}

							}

						}
						else 
						{
							std::cout << "There is no " + commands[3] + " here.\n";
						}
					}
				}
				else
				{
					std::cout << "You are dead.\n";
				}

			}
			else 
			{
				std::cout << "Invalid command\n";
				continue;
			}

			break;
		default:
			std::cout << "Too many arguments\n";
		}

		if (!newRoom) 
		{
			clock->clockUpdate(room, room->players());
		}

		Player* x = Player::findPlayer("you", room->players());
		checkAlive(x, room, player, rooms);
	}

}