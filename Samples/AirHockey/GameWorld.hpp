#include <RavEngine/World.hpp>
#include "Puck.hpp"
#include "Table.hpp"
#include "Paddle.hpp"
#include "Player.hpp"
#include <RavEngine/GUI.hpp>

using namespace RavEngine;

class GameWorld : public RavEngine::World {
public:
	GameWorld(int numplayers);
	
	GameWorld(const GameWorld& other);
	
	void OnActivate() override;

	void posttick(float) override;
	
    
protected:
    Ref<Table> hockeytable = std::make_shared<Table>();
    Ref<Puck> puck = std::make_shared<Puck>();
	
	Ref<Entity> cameraBoom = std::make_shared<Entity>();
	
	Ref<Paddle> p1;
	Ref<Paddle> p2;

	int numplayers;
	
	int p1score = 0, p2score = 0, numToWin = 3;
	
	Rml::Element* Scoreboard = nullptr;
	
	void Reset();
	void GameOver();
};
