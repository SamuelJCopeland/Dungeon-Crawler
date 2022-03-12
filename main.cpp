#define NORTH 0
#define NORTHEAST 1
#define EAST 2
#define SOUTHEAST 3
#define SOUTH 4
#define SOUTHWEST 5
#define WEST 6
#define NORTHWEST 7
#define UP 8
#define DOWN 9

#include "gameClock.h"
#include <map>

/*
* Copyright: Samuel Copeland
* Date: 3/15/2021
*
* File: main.cpp
*/

using namespace std;

//NEED TO MAKE IT SO THAT WHEN A COMMAND FAILS, THE GAME CLOCK DOES NOT TICK

//This function is run every round to make sure that the player is still alive
void checkAlive(Player* x, Room* room, Player* player, vector<Room*> rooms) {
	if (x != nullptr && !x->isAlive()) {
		cout << "\n\nYou have died.\nWe will try to fix you up the best we can.\n\n";
		string junk;
		getline(cin, junk);
		room->removePlayer(player);
		delete player;
		//Reset the health of any players in that room
		for (Player* pl : room->getPlayers()) {
			pl->setHP(pl->getMaxHealth());
		}

		room = rooms[0];
		player = new Player();
		room->addPlayer(player);
		cout << room->getDescription();
	}
	
	else if (x != nullptr && x->getHP() <= 0) {
		for (Object* o : x->getInventory()) {
			room->addObject(o);
		}
		cout << "\n\nYou have died.\nWe will try to fix you up the best we can.\n\n";
		string junk;
		getline(cin, junk);
		room->removePlayer(player);
		delete player;
		//Reset the health of any players in that room
		for (Player* pl : room->getPlayers()) {
			pl->setHP(pl->getMaxHealth());
		}

		room = rooms[0];
		player = new Player();
		x = player;
		room->addPlayer(player);
		cout << room->getDescription();
	}
	
}


int main() {
	map<string, int> dirMap;
	dirMap.insert(pair<string, int>("north", 0));
	dirMap.insert(pair<string, int>("northeast", 1));
	dirMap.insert(pair<string, int>("east", 2));
	dirMap.insert(pair<string, int>("southeast", 3));
	dirMap.insert(pair<string, int>("south", 4));
	dirMap.insert(pair<string, int>("southwest", 5));
	dirMap.insert(pair<string, int>("west", 6));
	dirMap.insert(pair<string, int>("northwest", 7));
	dirMap.insert(pair<string, int>("up", 8));
	dirMap.insert(pair<string, int>("down", 9));

	//A vector to hold all the rooms
	vector<Room*> rooms = {};
	Object* tempOb;
	Player* player = new PC();

	rooms.push_back(new Room("North of House", "You are on an overgrown lawn. There is a white house in front of you and trees behind you", vector<Path*>{}, vector<Player*>{}, vector<Object*>{}, false, false));
	rooms[0]->addPlayer(player);
	rooms[0]->addObject(new Container("mailbox", 100000, 100000, "The mailbox is black.", 2.5, true));
	tempOb = findObject("mailbox", rooms[0]->getObjects());
	if (tempOb != nullptr) {
		tempOb->putIn(new Object("letter", .1, .3,
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
	rooms[0]->addPlayer(new NPC("miner", 10, 1.2, 10, 25, 58, NEUTERAL));
	findPlayer("miner", rooms[0]->getPlayers())->addObject(new Object("gold_bar", 10, 2, "The gold_bar is heavy."));
	findPlayer("miner", rooms[0]->getPlayers())->addObject(new Weapon("pickaxe", 10, 5, "The pickaxe has a very sharp tip.", 10));
	

	//Add a neuteral npc without a weapon
	rooms[0]->addPlayer(new NPC("ninja", 50, 3.4, 15, 30, 70, NEUTERAL));
	findPlayer("ninja", rooms[0]->getPlayers())->addObject(new Object("heavy_metal_ball", 50, 2, "The heavy_metal_ball is made out of an incredibly dense unidentifiable metal."));

	//Add a new room and add paths that connects the two rooms
	rooms.push_back(new Room("Inside Kitchen", "You are in a mid 1800s style kitchen.", vector<Path*>(), vector<Player*>(), vector<Object*>(), true, true));

	//Add paths
	rooms[0]->addPath(new Path("open door", SOUTH, rooms[1]));
	rooms[1]->addPath(new Path("open door", NORTH, rooms[0]));

	//Add an aggressive npc without a weapon
	rooms[1]->addPlayer(new NPC("cyclops", 60, 5, 2, 100, 250, AGGRESSIVE));
	findPlayer("cyclops", rooms[1]->getPlayers())->addObject(new Object("gold_fleece", 10, 3, "The gold_fleece appears to cause the plants near it to become greener."));

	Room* room = rooms[0];
	Clock* clock = new Clock();
	cout << room->getDescription() << endl;

	//Main game loop
	string userInput;
	bool cont = true;
	while (cont) {
		bool newRoom = false;
		getline(cin, userInput);

		//Convert the user input to lowercase
		for (int i = 0; i < userInput.size(); i++) {
			userInput[i] = tolower(userInput[i]);
		}

		//Split the string into a vector of words
		vector<string> commands = splitString(userInput, ' ');

		//Depending on how many words are in the command, try different possible commands
		switch (commands.size()) {
		//0 words in the command
		case 0:
			cout << "Please enter a command:\n";
			break;
		//1 word in the command
		case 1:
			//Show the Manual
			if (userInput == "man" || userInput == "manual") {
				cout << "Type \"quit\" to end the game.\n";
				cout << "More information needs to be added to this.\n";
				continue;
			}
			//Quit the game
			else if (userInput == "quit" || userInput == "q") {
				cout << "Are you sure you want to exit (y/n)?\n";
				string answer;
				getline(cin, answer);
				if (answer == "y" || answer == "Y") {
					cout << "Ending game...\n";
					cont = false;
				}
				else {
					cout << "Continuing the game...\n";
					continue;
				}
			}
			//Describe what the player can see
			else if (userInput == "look" || userInput == "l") {
				Player* p = findPlayer("you", room->getPlayers());
				if (p != nullptr) {
					cout << room->getDescription() << endl;
				}
				else {
					cout << "You are dead.\n";
				}
			}
			//Describe the player's inventory
			else if (userInput == "inventory" || userInput == "i") {
				Player* p = findPlayer("you", room->getPlayers());
				if (p != nullptr) {
					cout << "You are holding the following items: \n" + p->printInventory();
				}
				else {
					cout << "You are dead.\n";
				}
			}
			//Describe the player's health
			else if (userInput == "medcheck" || userInput == "health" || userInput == "hp") {
				Player* p = findPlayer("you", room->getPlayers());
				if (p != nullptr) {
					cout << "You have " << p->getHP() << "/" << p->getMaxHealth() << " left.\n";
				}
				else {
					cout << "You are dead.\n";
				}
			}
			//Allow time to pass
			else if (userInput == "wait") {
				cout << "Time passes...\n";
			}
			//The command wasn't recognized
			else {
				cout << "Invalid command\n";
				continue;
			}
			break;
		//Two words in the command
		case 2:
			//Try to open a container object
			if (commands[0] == "open") {
				Player* p = findPlayer("you", room->getPlayers());
				if (p != nullptr) {
					tempOb = recursiveFindByName(commands[1], room->getObjects());
					if (tempOb != nullptr) {
						if (tempOb->open()) {
							cout << "The " + commands[1] + " is open.\n";
							if(tempOb->getInventory() != vector<Object*> ()){
								cout << tempOb->printInventory("") << endl;
							}
							
						}
						else {
							cout << "You can't open the " + commands[1] + ".\n";
						}
					}
					else {
						tempOb = recursiveFindByName(commands[1], player->getInventory());
						if (tempOb != nullptr) {
							if (tempOb->open()) {
								cout << "The " + commands[1] + " is open.\n";
								if (tempOb->getInventory() != vector<Object*>()) {
									cout << tempOb->printInventory("") << endl;
								}
							}
							else {
								cout << "You can't open the " + commands[1] + ".\n";
							}
						}
						else {
							cout << "There is no " + commands[1] + " here.\n";
						}
					}
				}
				else {
					cout << "You are dead.\n";
				}

			}
			//try to close a container object
			else if (commands[0] == "close") {
				Player* p = findPlayer("you", room->getPlayers());
				if (p != nullptr) {
					tempOb = recursiveFindByName(commands[1], room->getObjects());
					if (tempOb != nullptr) {
						if (tempOb->close()) {
							cout << "The " + commands[1] + " is closed.\n";
						}
						else {
							cout << "You can't close the " + commands[1] + ".\n";
						}
					}
					else {
						tempOb = recursiveFindByName(commands[1], player->getInventory());
						if (tempOb != nullptr) {
							if (tempOb->close()) {
								cout << "The " + commands[1] + " is closed.\n";
							}
							else {
								cout << "You can't close the " + commands[1] + ".\n";
							}
						}
						else {
							cout << "There is no " + commands[1] + " here.\n";
						}
					}
				}
				else {
					cout << "You are dead.\n";
				}
			}
			//try to take an object
			else if (commands[0] == "take") {
				Player* p = findPlayer("you", room->getPlayers());
				if (p != nullptr) {
					Object* container = nullptr;
					tempOb = findObject(commands[1], room->getObjects());
					if (tempOb != nullptr) {
						if (player->addObject(tempOb)) {
							room->removeObject(tempOb);
							cout << "Taken\n";
						}
						else {
							cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
						}
					}
					else if (recursiveFindByName(commands[1], room->getObjects()) != nullptr) {
						tempOb = recursiveFindByName(commands[1], room->getObjects());
						for (Object* o : room->getObjects()) {
							container = recursiveFindContainer(tempOb, o);
							if (container != nullptr) {
								cout << "Taken\n";
								container->takeOut(tempOb);
								player->addObject(tempOb);
								break;
							}
						}
						if (container == nullptr) {
							cout << "There is no " + commands[1] + " here.\n";
						}
					}
					else if (recursiveFindByName(commands[1], player->getInventory()) != nullptr) {
						tempOb = recursiveFindByName(commands[1], player->getInventory());
						for (Object* o : player->getInventory()) {
							container = recursiveFindContainer(tempOb, o);
							if (container != nullptr) {
								cout << "Taken\n";
								container->takeOut(tempOb);
								player->addObject(tempOb);
								break;
							}
						}
						if (container == nullptr) {
							cout << "There is no " + commands[1] + " here.\n";
						}
					}
					else {
						cout << "There is no " + commands[1] + " here.\n";
					}
				}
				else {
					cout << "You are dead.\n";
				}
			}
			//Try to drop an object that the player is holding
			else if (commands[0] == "drop") {
				Player* p = findPlayer("you", room->getPlayers());
				if (p != nullptr) {
					tempOb = findObject(commands[1], player->getInventory());
					if (tempOb != nullptr) {
						player->removeObject(tempOb);
						room->addObject(tempOb);
						cout << "Dropped\n";
					}
					else {
						cout << "You are not holding the " + commands[1] + ".\n";
					}
				}
				else {
					cout << "You are dead.\n";
				}
			}
			//Describe an object
			else if (commands[0] == "examine") {
				Player* p = findPlayer("you", room->getPlayers());
				if (p != nullptr) {
					tempOb = recursiveFindByName(commands[1], room->getObjects());
					if (tempOb != nullptr) {
						cout << tempOb->getDescription() << endl;
					}
					else {
						tempOb = recursiveFindByName(commands[1], player->getInventory());
						if (tempOb != nullptr) {
							cout << tempOb->getDescription() << endl;
						}
						else {
							for (Player* p : room->getPlayers()) {
								tempOb = recursiveFindByName(commands[1], p->getInventory());
								if (tempOb != nullptr) {
									cout << tempOb->getDescription() << endl;
									break;
								}
							}
							if (tempOb == nullptr) {
								cout << "There is no " + commands[1] + " here.\n";
							}

						}
						
					}
				}
				else {
					cout << "You are dead.\n";
				}
			}
			else if (commands[0] == "eat") {
				Player* p = findPlayer("you", room->getPlayers());
				if (p != nullptr) {
					tempOb = findObject(commands[1], p->getInventory());
					if (tempOb != nullptr) {
						if (tempOb->eat(p)) {
							cout << "You ate the " + tempOb->getName() + ".\n";
							p->removeObject(tempOb);
							delete tempOb;
						}
						else {
							cout << "You can't eat the " + commands[1] + ".\n";
						}
					}
					else {
						cout << "There is no " + commands[1] + " here.\n";
					}
				}
				else {
					cout << "You are dead.\n";
				}
			}
			else if (commands[0] == "light") {
				Player* p = findPlayer("you", room->getPlayers());
				if (p != nullptr) {
					tempOb = findObject(commands[1], player->getInventory());
					if (tempOb != nullptr) {
						if (tempOb->light(nullptr)) {
							cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
							if (room->is_dark()) {
								room->setDark(false);
								cout << room->getDescription() << endl;
							}
						}
						else {
							cout << "You can't light the " + tempOb->getName() + "\n";
						}
					}
					else {
						tempOb = recursiveFindByName(commands[1], room->getObjects());
						
						if (tempOb != nullptr) {
							if (player->addObject(tempOb)) {
								room->removeObject(tempOb);
								cout << "Taken\n";
								Object* container = nullptr;
								for (Object* ob : room->getObjects()) {
									container = recursiveFindContainer(tempOb, ob);
									if (container != nullptr) {
										container->takeOut(tempOb);
										break;
									}
								}
								if (tempOb->light(nullptr)) {
									cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
									if (room->is_dark()) {
										room->setDark(false);
										cout << room->getDescription() << endl;
									}
								}
								else {
									cout << "You can't light the " + tempOb->getName() + "\n";
								}
							}
							else {
								cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
							}
						}
						else {
							cout << "There is no " + commands[1] + " here.\n";
						}
					}
				}
				else {
					cout << "You are dead.\n";
				}
			}
			else if (commands[0] == "go") {
				Player* p = findPlayer("you", room->getPlayers());
				if (p != nullptr) {
					if (dirMap.find(commands[1]) != dirMap.end()) {
						Room* origRoom = room;
						for (Path* p : room->getPaths()) {
							if (p->getDir() == dirMap[commands[1]]) {
								room = p->travel();
								room->addPlayer(player);
								origRoom->removePlayer(player);
								clock->updateLights(origRoom, origRoom->getPlayers());
								clock->updateLights(room, room->getPlayers());
								cout << room->getDescription() << endl;
								newRoom = true;
								break;
							}
						}
						if (room == origRoom) {
							cout << "There is no path to your " + commands[1] + ".\n";
						}
					}
					else {
						cout << "Invalid direction.\n";
					}
				}
			}
			else {
				cout << "Invalid command\n";
				continue;
			}
			break;
		case 3:
			if (commands[0] + " " + commands[1] == "put out") {
				if (findPlayer("you", room->getPlayers()) != nullptr) {
					tempOb = findObject(commands[2], player->getInventory());
					if (tempOb != nullptr) {
						if (tempOb->putOut()) {
							cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now out.\n";
						}
						else {
							cout << "The " + commands[2] + " is not lit.\n";
						}
					}
					else {
						tempOb = recursiveFindByName(commands[2], room->getObjects());

						if (tempOb != nullptr) {
							if (player->addObject(tempOb)) {
								room->removeObject(tempOb);
								cout << "Taken\n";
								Object* container = nullptr;
								for (Object* ob : room->getObjects()) {
									container = recursiveFindContainer(tempOb, ob);
									if (container != nullptr) {
										container->takeOut(tempOb);
										break;
									}
								}
								if (tempOb->putOut()) {
									cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now out.\n";
									if (!room->is_dark()) {
										room->setDark(true);
									}
								}
								else {
									cout << "You can't put out the " + tempOb->getName() + "\n";
								}
							}
							else {
								cout << "You can't pick up the " + commands[2] + " you are holding too much.\n";
							}
						}
						else {
							cout << "There is no " + commands[2] + " here.\n";
						}
					}
				}
			}
			
			break;
		case 4:
			if (commands[0] == "take" && commands[2] == "from") {
				Player* p = findPlayer("you", room->getPlayers());
				if (p != nullptr) {
					Object* tempContainer = recursiveFindByName(commands[3], room->getObjects());
					if (tempContainer != nullptr) {
						tempOb = findObject(commands[1], tempContainer->getInventory());
						if (tempOb != nullptr) {
							if (player->addObject(tempOb)) {
								tempContainer->takeOut(tempOb);
								cout << "Taken\n";
							}
							else {
								cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
							}
						}
						else {
							cout << "Can't find the " + commands[1] + " in the " + commands[3] + ".\n";
						}
					}
					else {
						tempContainer = recursiveFindByName(commands[3], player->getInventory());
						if (tempContainer != nullptr) {
							tempOb = findObject(commands[1], tempContainer->getInventory());
							if (tempOb != nullptr) {
								if (player->addObject(tempOb)) {
									tempContainer->takeOut(tempOb);
									cout << "Taken\n";
								}
								else {
									cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
								}
							}
							else {
								cout << "Can't find the " + commands[1] + " in the " + commands[3] + ".\n";
							}
						}
						else {
							cout << "Can't find the " + commands[3] + ".\n";
						}
					}
				}
				else {
					cout << "You are dead.\n";
				}
			}
			else if (commands[0] == "put" && commands[2] == "in") {
				Player* p = findPlayer("you", room->getPlayers());
				if (p != nullptr) {
					Object* tempContainer = recursiveFindByName(commands[3], room->getObjects());
					if (tempContainer != nullptr) {
						tempOb = findObject(commands[1], player->getInventory());
						if (tempOb != nullptr) {
							if (tempContainer->putIn(tempOb)) {
								player->removeObject(tempOb);
								cout << "The " + commands[1] + " is in the " + commands[3] + "\n";
							}
							else {
								cout << "You can't fit the " + commands[1] + " in the " + commands[3] + ".\n";
							}
						}
						else {
							cout << "You are not holding the " + commands[1] + ".\n";
						}
					}
					else {
						tempContainer = recursiveFindByName(commands[3], player->getInventory());
						if (tempContainer != nullptr) {
							tempOb = findObject(commands[1], player->getInventory());
							if (tempOb != nullptr) {
								if (tempContainer->putIn(tempOb)) {
									player->removeObject(tempOb);
									cout << "The " + commands[1] + " is in the " + commands[3] + "\n";
								}
								else {
									cout << "You can't fit the " + commands[1] + " in the " + commands[3] + ".\n";
								}
							}
							else {
								cout << "You are not holding the " + commands[1] + ".\n";
							}
						}
						else {
							cout << "Can't find the " + commands[3] + ".\n";
						}
					}
				}
				else {
					cout << "You are dead.\n";
				}
			}
			else if (commands[0] == "attack" && commands[2] == "with") {
				Player* p = findPlayer("you", room->getPlayers());
				if (p != nullptr) {
					Player* e = findPlayer(commands[1], room->getPlayers());
					bool isDark = room->is_dark();
					if (e != nullptr) {
						Object* w = findObject(commands[3], player->getInventory());

						if (w != nullptr) {
							if (!isDark) {
								vector<Object*> enemyInventory = player->attack(e, w);
								for (Object* o : enemyInventory) {
									room->addObject(o);
								}
								if (!e->isAlive()) {
									bool isYou = e == player;
									room->removePlayer(e);
									if (isYou) {
										cout << "\n\nYou have died.\nWe will try to fix you up the best we can.\n\n";
										string junk;
										getline(cin, junk);
										//Reset the health of any players in that room
										for (Player* pl : room->getPlayers()) {
											pl->setHP(pl->getMaxHealth());
										}
										room = rooms[0];
										player = new Player();
										room->addPlayer(player);
										cout << room->getDescription();
									}
								}
							}
							else {
								cout << "It is too dark, you don't see a " + commands[1] + " here.\n";
							}
						}
						else {
							cout << "You are not holding the " + commands[3] + ".\n";
						}
					}
					else {
						cout << "There is no " + commands[1] + " here.\n";
					}
				}
				else {
					cout << "You are dead.\n";
				}
			}
			else if (commands[0] == "light" && commands[2] == "with") {
				Player* p = findPlayer("you", room->getPlayers());
				if (p != nullptr) {
					Object* tempContainer = findObject(commands[3], player->getInventory());
					if (tempContainer != nullptr) {

						tempOb = findObject(commands[1], player->getInventory());
						if (tempOb != nullptr) {
							if (tempOb->light(tempContainer)) {
								cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
								if (room->is_dark()) {
									room->setDark(false);
									cout << room->getDescription() << endl;
								}
							}
							else {
								cout << "You can't light the " + commands[1] + " with the " + commands[3] + ".\n";
							}
						}
						else{
							tempOb = findObject(commands[1], tempContainer->getInventory());
							if (tempOb != nullptr) {
								if (player->addObject(tempOb)) {
									Object* containerContainer = nullptr;
									for (Object* obj : player->getInventory()) {
										containerContainer = recursiveFindContainer(tempContainer, obj);
										if (containerContainer != nullptr) {
											break;
										}
									}
									if (containerContainer != nullptr) {
										containerContainer->takeOut(tempContainer);
										player->addObject(tempContainer);
									}
									
									tempContainer->takeOut(tempOb);
									cout << "Taken\n";

									if (tempOb->light(tempContainer)) {
										cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
										if (room->is_dark()) {
											room->setDark(false);
											cout << room->getDescription() << endl;
										}
									}
									else {
										cout << "You can't light " + commands[1] + " with " + commands[3] + ".\n";
									}

								}
								else {
									cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
								}
							}
							else {
								tempOb = recursiveFindByName(commands[1], room->getObjects());
								if (tempOb != nullptr) {
									if (player->addObject(tempOb)) {
										cout << "Taken.\n";
										Object* container = nullptr;
										for (Object* potC : room->getObjects()) {
											container = recursiveFindContainer(tempOb, potC);
											if (container != nullptr) {
												break;
											}
										}
										if (container != nullptr) {
											container->takeOut(tempOb);
										}
										else {
											room->removeObject(tempOb);
										}
										if (tempOb->light(tempContainer)) {
											cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
											if (room->is_dark()) {
												room->setDark(false);
												cout << room->getDescription() << endl;
											}
										}
										else {
											cout << "You can't light " + commands[1] + " with " + commands[3] + ".\n";
										}
									}
									else {
										cout << "Couldn't pick up " + commands[1] + ". You are holding too much.\n";
									}
								}
								else {
									cout << "There is no " + commands[1] + " here.\n";
								}
							}
						}
						
					}
					else {
						tempContainer = recursiveFindByName(commands[3], player->getInventory());
						if (tempContainer != nullptr) {
							tempOb = findObject(commands[1], player->getInventory());
							if (tempOb != nullptr) {
								if (tempOb->light(tempContainer)) {
									cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
									if (room->is_dark()) {
										room->setDark(false);
										cout << room->getDescription() << endl;
									}
								}
								else {
									cout << "You can't light the " + commands[1] + " with the " + commands[3] + ".\n";
								}
							}
							else {
								tempOb = findObject(commands[1], tempContainer->getInventory());
								if (tempOb != nullptr) {
									if (player->addObject(tempOb)) {
										Object* containerContainer = nullptr;
										for (Object* obj : player->getInventory()) {
											containerContainer = recursiveFindContainer(tempContainer, obj);
											if (containerContainer != nullptr) {
												break;
											}
										}
										if (containerContainer != nullptr) {
											containerContainer->takeOut(tempContainer);
											player->addObject(tempContainer);
										}

										tempContainer->takeOut(tempOb);
										cout << "Taken\n";

										if (tempOb->light(tempContainer)) {
											cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
											if (room->is_dark()) {
												room->setDark(false);
												cout << room->getDescription() << endl;
											}
										}
										else {
											cout << "You can't light " + commands[1] + " with " + commands[3] + ".\n";
										}

									}
									else {
										cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
									}
								}
								else {
									tempOb = recursiveFindByName(commands[1], room->getObjects());
									if (tempOb != nullptr) {
										if (player->addObject(tempOb)) {
											cout << "Taken.\n";
											Object* container = nullptr;
											for (Object* potC : room->getObjects()) {
												container = recursiveFindContainer(tempOb, potC);
												if (container != nullptr) {
													break;
												}
											}
											if (container != nullptr) {
												container->takeOut(tempOb);
											}
											else {
												room->removeObject(tempOb);
											}
											if (tempOb->light(tempContainer)) {
												cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
												if (room->is_dark()) {
													room->setDark(false);
													cout << room->getDescription() << endl;
												}
											}
											else {
												cout << "You can't light " + commands[1] + " with " + commands[3] + ".\n";
											}
										}
										else {
											cout << "Couldn't pick up " + commands[1] + ". You are holding too much.\n";
										}
									}
									else {
										cout << "There is no " + commands[1] + " here.\n";
									}
								}
							}
						}
						else if (findObject(commands[3], room->getObjects()) != nullptr) {
							tempContainer = findObject(commands[3], room->getObjects());
							if (player->addObject(tempContainer)) {
								cout << "Taken\n";
								Object* containerContainer = nullptr;
								for (Object* obj : room->getObjects()) {
									containerContainer = recursiveFindContainer(tempContainer, obj);
									if (containerContainer != nullptr) {
										break;
									}
								}
								if (containerContainer != nullptr) {
									containerContainer->takeOut(tempContainer);
								}
								else {
									room->removeObject(tempContainer);
								}

								tempOb = findObject(commands[1], player->getInventory());
								if (tempOb != nullptr) {
									if (tempOb->light(tempContainer)) {
										cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
										if (room->is_dark()) {
											room->setDark(false);
											cout << room->getDescription() << endl;
										}
									}
									else {
										cout << "You can't light the " + commands[1] + " with the " + commands[3] + ".\n";
									}
								}
								else {
									tempOb = findObject(commands[1], tempContainer->getInventory());
									if (tempOb != nullptr) {
										if (player->addObject(tempOb)) {
											tempContainer->takeOut(tempOb);
											cout << "Taken\n";

											if (tempOb->light(tempContainer)) {
												cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
												if (room->is_dark()) {
													room->setDark(false);
													cout << room->getDescription() << endl;
												}
											}

										}
										else {
											cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
										}
									}
									else {
										cout << "Can't find the " + commands[1] + ".\n";
									}
								}
							}

						}
						else if (findObject(commands[3], room->getObjects()) != nullptr) {
							tempContainer = findObject(commands[3], room->getObjects());
							if (player->addObject(tempContainer)) {
								cout << "Taken\n";
								Object* containerContainer = nullptr;
								for (Object* obj : room->getObjects()) {
									containerContainer = recursiveFindContainer(tempContainer, obj);
									if (containerContainer != nullptr) {
										break;
									}
								}
								if (containerContainer != nullptr) {
									containerContainer->takeOut(tempContainer);
								}
								else {
									room->removeObject(tempContainer);
								}

								tempOb = findObject(commands[1], player->getInventory());
								if (tempOb != nullptr) {
									if (tempOb->light(tempContainer)) {
										cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
										if (room->is_dark()) {
											room->setDark(false);
											cout << room->getDescription() << endl;
										}
									}
									else {
										cout << "You can't light the " + commands[1] + " with the " + commands[3] + ".\n";
									}
								}
								else {
									tempOb = findObject(commands[1], tempContainer->getInventory());
									if (tempOb != nullptr) {
										if (player->addObject(tempOb)) {
											tempContainer->takeOut(tempOb);
											cout << "Taken\n";

											if (tempOb->light(tempContainer)) {
												cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
												if (room->is_dark()) {
													room->setDark(false);
													cout << room->getDescription() << endl;
												}
											}

										}
										else {
											cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
										}
									}
									else {
										cout << "Can't find the " + commands[1] + ".\n";
									}
								}
							}

						}
						else if (findObject(commands[3], room->getObjects()) != nullptr) {
							tempContainer = findObject(commands[3], room->getObjects());
							if (player->addObject(tempContainer)) {
								cout << "Taken\n";
								Object* containerContainer = nullptr;
								for (Object* obj : room->getObjects()) {
									containerContainer = recursiveFindContainer(tempContainer, obj);
									if (containerContainer != nullptr) {
										break;
									}
								}
								if (containerContainer != nullptr) {
									containerContainer->takeOut(tempContainer);
								}
								else {
									room->removeObject(tempContainer);
								}

								tempOb = findObject(commands[1], player->getInventory());
								if (tempOb != nullptr) {
									if (tempOb->light(tempContainer)) {
										cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
										if (room->is_dark()) {
											room->setDark(false);
											cout << room->getDescription() << endl;
										}
									}
									else {
										cout << "You can't light the " + commands[1] + " with the " + commands[3] + ".\n";
									}
								}
								else {
									tempOb = findObject(commands[1], tempContainer->getInventory());
									if (tempOb != nullptr) {
										if (player->addObject(tempOb)) {
											tempContainer->takeOut(tempOb);
											cout << "Taken\n";

											if (tempOb->light(tempContainer)) {
												cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
												if (room->is_dark()) {
													room->setDark(false);
													cout << room->getDescription() << endl;
												}
											}

										}
										else {
											cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
										}
									}
									else {
										cout << "Can't find the " + commands[1] + ".\n";
									}
								}
							}

						}
						else if (recursiveFindByName(commands[3], room->getObjects()) != nullptr) {
							tempContainer = recursiveFindByName(commands[3], room->getObjects());
							if (player->addObject(tempContainer)) {
								cout << "Taken\n";
								Object* containerContainer = nullptr;
								for (Object* obj : room->getObjects()) {
									containerContainer = recursiveFindContainer(tempContainer, obj);
									if (containerContainer != nullptr) {
										break;
									}
								}
								if (containerContainer != nullptr) {
									containerContainer->takeOut(tempContainer);
								}
								else {
									room->removeObject(tempContainer);
								}
									
								tempOb = findObject(commands[1], player->getInventory());
								if (tempOb != nullptr) {
									if (tempOb->light(tempContainer)) {
										cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
										if (room->is_dark()) {
											room->setDark(false);
											cout << room->getDescription() << endl;
										}
									}
									else {
										cout << "You can't light the " + commands[1] + " with the " + commands[3] + ".\n";
									}
								}
								else {
									tempOb = findObject(commands[1], tempContainer->getInventory());
									if (tempOb != nullptr) {
										if (player->addObject(tempOb)) {
											tempContainer->takeOut(tempOb);
											cout << "Taken\n";
	
											if (tempOb->light(tempContainer)) {
												cout << "The " + splitString(tempOb->getName(), ' ')[0] + " is now lit.\n";
												if (room->is_dark()) {
													room->setDark(false);
													cout << room->getDescription() << endl;
												}
											}
	
										}
										else {
											cout << "You can't pick up the " + commands[1] + " you are holding too much.\n";
										}
									}
									else {
										cout << "Can't find the " + commands[1] + ".\n";
									}
								}
							}

						}
						else {
							cout << "There is no " + commands[3] + " here.\n";
						}
					}
				}
				else {
					cout << "You are dead.\n";
				}
			}
			else {
				cout << "Invalid command\n";
				continue;
			}
			break;
		default:
			cout << "Too many arguments\n";

		}
		if (!newRoom) {
			clock->clockUpdate(room, room->getPlayers());
		}
		Player* x = findPlayer("you", room->getPlayers());
		checkAlive(x, room, player, rooms);
		
		
	}
}