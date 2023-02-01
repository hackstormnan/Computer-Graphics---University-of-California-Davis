#pragma once
#include "GL_Dependencies.h"
#include "EntityInstance.h"
#include "Camera.h"
#include "Singleton.h"

enum KnotType {
	OPEN_END,
	FLOATING_END
};

class Engine : public Singleton<Engine>
{
	friend class Singleton<Engine>;
public:
	Engine();
	~Engine();
	void Update(float deltaTime);
	void Render();

	std::vector<std::vector<glm::vec2>> beziers;

	std::vector<std::vector<glm::vec2>> bspines;
	std::vector<std::vector<float>> knotsList;
	std::vector<int> degrees;
	std::vector<KnotType> knotTypes;

	//Bspine
	int AbleToChangeDegree(int index); //1 for can minus, 2 for can add, 3 for can add and minus, 0 for cannot change
	void AddDegree(int index, int num);
	void ResetKnots(int index, KnotType type = KnotType::FLOATING_END);
	void CreateNewBspine();
	void DeleteBspine(int index);
	void CreateBspinePoint(int index);
	void InsertBspinePoint(int index, int pointIndex);
	void DeleteBspinePoint(int index, int pointIndex);

	//bezier
	void CreateNewBezier();
	void DeleteBeizer(int index);
	void CreateBeizerPoint(int index);
	void InsertBeizerPoint(int index, int pointIndex);
	void DeleteBeizerPoint(int index, int pointIndex);

	void Save(std::string path);
	void Load(std::string path);
	void Reset();
private:
};