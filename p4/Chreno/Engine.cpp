#include "Engine.h"
#include "Renderer.h"
#include <algorithm>

Engine::Engine()
{
	CreateNewBspine();
	CreateNewBezier();
}

Engine::~Engine()
{

}

void Engine::Update(float deltaTime)
{

}

void Engine::Render()
{
	for (int i = 0; i < (int)beziers.size(); i++)
	{
		Renderer::Get().DrawBezierCurve(beziers[i], 50);
	}

	for (int i = 0; i < (int)bspines.size(); i++)
	{
		Renderer::Get().DrawBspineCurve(bspines[i], knotsList[i], degrees[i], 50);
	}
}

int Engine::AbleToChangeDegree(int index)
{
	int maxDegree = (int)bspines[index].size() - 1; //max degree
	int minDegree = 1;
	int k = degrees[index];

	bool canAdd = k < maxDegree;
	bool canMinus = k > minDegree;

	if (canAdd && canMinus) {
		return 3;
	}
	else if (canAdd && canMinus == false) {
		return 2;
	}
	else if (canAdd == false && canMinus) {
		return 1;
	}
	else {
		return 0;
	}
}

void Engine::AddDegree(int index, int num)
{
	int maxDegree = (int)bspines[index].size() - 1; //max degree
	int minDegree = 1;

	degrees[index] += num;
	if (degrees[index] > maxDegree)
		degrees[index] = maxDegree;
	else if(degrees[index] < minDegree)
		degrees[index] = minDegree;
	ResetKnots(index, knotTypes[index]);
}

void Engine::ResetKnots(int index, KnotType type)
{
	knotsList[index].clear();

	int n = (int)bspines[index].size() - 1;
	int k = degrees[index];
	int m = n + k + 1;

	if (type == KnotType::OPEN_END) {
		int remain = m;
		for (int i = 0; i < k; i++)
		{
			knotsList[index].push_back(0);
			knotsList[index].push_back((float)m);
			remain -= 2;
		}

		float interval = (float)m / (float)remain;
		for (int i = 0; i <= remain; i++)
		{
			knotsList[index].push_back((float)i * interval);
		}
		std::sort(knotsList[index].begin(), knotsList[index].end());
	}
	else if (type == KnotType::FLOATING_END) {
		for (int i = 0; i <= m; i++)
		{
			knotsList[index].push_back((float)i);
		}
	}
}

void Engine::CreateNewBspine()
{
	std::vector<glm::vec2> bspine;

	bspine.push_back({ -100 + 300,  100 });
	bspine.push_back({ -100 + 300, -100 });
	bspine.push_back({  100 + 300, -100 });
	bspine.push_back({  100 + 300,  100 });

	degrees.push_back(2);
	bspines.push_back(bspine);

	std::vector<float> knots;
	knotsList.push_back(knots);
	knotTypes.push_back(KnotType::FLOATING_END);
	ResetKnots((int)knotsList.size() - 1, KnotType::FLOATING_END);
}

void Engine::DeleteBspine(int index)
{
	bspines.erase(bspines.begin() + index);
	knotsList.erase(knotsList.begin() + index);
	degrees.erase(degrees.begin() + index);
	knotTypes.erase(knotTypes.begin() + index);
}

void Engine::CreateBspinePoint(int index)
{
	bspines[index].push_back(glm::vec2(300, 0));
	knotsList[index].push_back((float)(knotsList.size() - 1));
	ResetKnots(index, knotTypes[index]);
}

void Engine::InsertBspinePoint(int index, int pointIndex)
{
	bspines[index].insert(bspines[index].begin() + pointIndex, glm::vec2(300, 0));
	knotsList[index].insert(knotsList[index].begin() + pointIndex, (float)(knotsList.size() - 1));
	if (degrees[index] < 1)
		degrees[index] = 1;
	ResetKnots(index, knotTypes[index]);
}

void Engine::DeleteBspinePoint(int index, int pointIndex)
{
	bspines[index].erase(bspines[index].begin() + pointIndex);
	knotsList[index].erase(knotsList[index].begin() + pointIndex);
	if (degrees[index] > (int)bspines[index].size() - 1)
		degrees[index] = (int)bspines[index].size() - 1;
	ResetKnots(index, knotTypes[index]);
}

void Engine::CreateNewBezier()
{
	std::vector<glm::vec2> bezier;

	bezier.push_back({ -100 - 300,  100 });
	bezier.push_back({ -100 - 300, -100 });
	bezier.push_back({  100 - 300, -100 });
	bezier.push_back({  100 - 300,  100 });

	beziers.push_back(bezier);
}

void Engine::DeleteBeizer(int index)
{
	beziers.erase(beziers.begin() + index);
}

void Engine::CreateBeizerPoint(int index)
{
	beziers[index].push_back(glm::vec2(-300, 0));
}

void Engine::InsertBeizerPoint(int index, int pointIndex)
{
	beziers[index].insert(beziers[index].begin() + pointIndex, glm::vec2(-300,0));
}

void Engine::DeleteBeizerPoint(int index, int pointIndex)
{
	beziers[index].erase(beziers[index].begin() + pointIndex);
}

void Engine::Save(std::string path)
{
	std::ofstream out(path);

	if (!out.is_open())
	{
		std::cout << path << " load failed!";
		return;
	}

	//Bezier curve data
	out << "# Bezier curve data:\n";
	out << "# A of total Bezier curve, followed by number of control points:\n";
	out << "b " << beziers.size() << "\n";
	for (int i = 0; i < (int)beziers.size(); i++)
	{
		out << beziers[i].size() << "\n";
		for (int j = 0; j < (int)beziers[i].size(); j++)
		{
			out << beziers[i][j].x << " " << beziers[i][j].y << "\n";
		}
	}
	out << "\n";

	//B-spline curve data
	out << "# B-spline curve data:\n";
	out << "# First the number of b-spline curves, first property degree, second knot type, then control points data, followed by knots data:\n";
	out << "s " << bspines.size() << "\n";		//degree
	for (int i = 0; i < (int)bspines.size(); i++)
	{
		out << degrees[i] << "\n";		//degree

		int type = knotTypes[i] == KnotType::FLOATING_END ? 1 : 0;
		out << type << "\n";		//knot type

		out << bspines[i].size() << "\n";		//control point number
		for (int j = 0; j < (int)bspines[i].size(); j++)//control points data
		{
			out << bspines[i][j].x << " " << bspines[i][j].y << "\n";
		}

		out << knotsList[i].size() << "\n";		//control point number
		for (int j = 0; j < (int)knotsList[i].size(); j++)//control points data
		{
			out << knotsList[i][j] << "\n";
		}
	}
}

void Engine::Load(std::string path)
{
	std::stringstream ss;
	std::ifstream in_file(path);
	std::string line = "";
	std::string prefix = "";

	if (!in_file.is_open()) {
		std::cout << path << " load failed!";
		return;
	}
	beziers.clear();
	bspines.clear();
	knotsList.clear();
	degrees.clear();
	knotTypes.clear();

	while (std::getline(in_file, line)) {
		prefix = "";

		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "#") { //do nothing when meet #
			continue;
		}
		else if (prefix == "b") {	//b for bezier curve
			int size = 0;
			ss >> size;
			for (int i = 0; i < size; i++)
			{
				std::vector<glm::vec2> b;
				std::getline(in_file, line);
				ss.clear();
				ss.str(line);
				int pointCount = 0;
				ss >> pointCount;
				for (int j = 0; j < pointCount; j++)
				{
					glm::vec2 vec = glm::vec2(0);
					std::getline(in_file, line);
					ss.clear();
					ss.str(line);
					ss >> vec.x >> vec.y;
					
					b.push_back(vec);
				}
				beziers.push_back(b);
			}
		}
		else if (prefix == "s") {	//s for b-Spline curve
			int size = 0;
			ss >> size;
			for (int i = 0; i < size; i++)
			{
				std::getline(in_file, line);
				ss.clear();
				ss.str(line);
				int degree = 0;
				ss >> degree;
				degrees.push_back(degree);

				std::getline(in_file, line);
				ss.clear();
				ss.str(line);
				KnotType type = KnotType::FLOATING_END;
				int t = 0;
				ss >> t;
				type = t == 1 ? KnotType::FLOATING_END : KnotType::OPEN_END;
				knotTypes.push_back(type);

				std::vector<glm::vec2> b;
				std::getline(in_file, line);
				ss.clear();
				ss.str(line);
				int pointCount = 0;
				ss >> pointCount;
				for (int j = 0; j < pointCount; j++)
				{
					glm::vec2 vec = glm::vec2(0);
					std::getline(in_file, line);
					ss.clear();
					ss.str(line);
					ss >> vec.x >> vec.y;

					b.push_back(vec);
				}
				bspines.push_back(b);

				std::getline(in_file, line);
				ss.clear();
				ss.str(line);
				int knotsCount = 0;
				ss >> knotsCount;

				std::vector<float> knots;
				for (int j = 0; j < knotsCount; j++)
				{
					float knot = 0;
					std::getline(in_file, line);
					ss.clear();
					ss.str(line);
					ss >> knot;

					knots.push_back(knot);
				}
				knotsList.push_back(knots);
			}
		}
	}
}

void Engine::Reset()
{
	beziers.clear();
	bspines.clear();
	knotsList.clear();
	degrees.clear();
	knotTypes.clear();

	CreateNewBspine();
	CreateNewBezier();
}