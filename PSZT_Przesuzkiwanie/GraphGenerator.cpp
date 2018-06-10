#include <time.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <set>
#include <tuple>
#include <cmath>

typedef std::tuple<int, int, int> trio;
typedef std::set<trio> trios;

int* generateSetOfNumbers(int range, int elements_count);
float getDistance(trios::iterator first, trios::iterator second);

struct SortOrder
{
	bool operator()(const std::tuple<int, int, float> &first, const std::tuple<int, int, float> &second)
	{
		return (std::get<2>(first) < std::get<2>(second));
	}
};

struct SortOrder2
{
	bool operator()(const std::tuple<int, int, float> &first, const std::tuple<int, int, float> &second)
	{
		if (std::get<2>(first) < std::get<2>(second))
			return true;
		if (std::get<2>(first) > std::get<2>(second))
			return false;
		if (std::get<0>(first) == std::get<0>(second) && std::get<1>(first) == std::get<1>(second))
			return false;
		if (std::get<0>(first) == std::get<1>(second) && std::get<0>(first) == std::get<1>(second))
			return false;
		return (first < second);
	}
};





void generateGraph(int cities_count)
{
	std::set<std::tuple<int, int, int>> cities; // cities = nodes
	std::set<std::tuple<int, int, int>> connected_cities;
	std::vector<std::tuple<int, int, float>> cities_connections; // connections = edges
	std::set<std::tuple<int, int, float>, SortOrder2> cities_connections_set;

	std::ofstream result_file;
	result_file.open("generated_graph.txt", std::ios::trunc);

	srand(10); // change to time, 10 for debug

	std::pair<int*, int*> coords;
	coords.first = generateSetOfNumbers(cities_count * 10, cities_count);
	coords.second = generateSetOfNumbers(cities_count * 10, cities_count);
	for (int i = 0; i < cities_count; i++)
		cities.insert(std::make_tuple(i, coords.first[i], coords.second[i]));
	delete[] coords.first;
	delete[] coords.second;

	std::set<std::tuple<int, int, int>>::iterator it = cities.begin();
	//std::set<std::tuple<int, int, int>>::iterator min_ptr = ++cities.begin();
	//float result_dis = getDistance(it, min_ptr);
	//std::cout << ""

	trios::iterator current_itr = cities.begin();
	trios::iterator best_next_itr = cities.begin();

	while (true)
	{
		if (best_next_itr == current_itr)
			best_next_itr++;
		if (best_next_itr == cities.end())
		{
			best_next_itr--;
			float min_value = getDistance(current_itr, best_next_itr);
			connected_cities.insert(*current_itr);
			cities = connected_cities;
			break;
		}
		float min_value = getDistance(current_itr, best_next_itr);
		for (trios::iterator itr = cities.begin(); itr != cities.end(); itr++)
		{
			if (itr == current_itr)
				++itr;
			if (itr == cities.end())
				break;
			float current_distance = getDistance(current_itr, itr);
			if (current_distance < min_value)
			{
				min_value = current_distance;
				best_next_itr = itr;
			}
		}
		cities_connections.push_back(std::make_tuple(std::get<0>(*current_itr), std::get<0>(*best_next_itr), min_value));
		connected_cities.insert(*current_itr);
		cities.erase(*current_itr);
		current_itr = best_next_itr;
		best_next_itr = cities.begin();
		
	}

	std::set<std::tuple<int, int, float>, SortOrder> temp_connections;
	//for (std::vector<std::tuple<int, int, float>>::iterator it = cities_connections.begin(); it != cities_connections.end(); it++)
		//temp_connections.insert(*it);


	std::set<trio> temp_full_cities;
	for (trios::iterator itr = cities.begin(); itr != cities.end(); itr++)
		temp_full_cities.insert(*itr);

	for (it = cities.begin(); it != cities.end(); it++)
	{
		std::set<trio> temp_cities = temp_full_cities;
		temp_cities.erase(*it);
		for (std::set<std::tuple<int, int, int>>::iterator it2 = temp_cities.begin(); it2 != temp_cities.end(); it2++)
		{
			float current_distance = getDistance(it, it2);
			temp_connections.insert(std::make_tuple(std::get<0>(*it), std::get<0>(*it2), current_distance));
		}
		for (int i = 0; i < 3 && i < cities.size(); i++)
		{
			cities_connections.push_back(*(temp_connections.begin()));
			temp_connections.erase(temp_connections.begin());
		}
		temp_connections.clear();
		
		cities_connections.size();
	}



	//trios::iterator some_other = (++cities.begin());
	bool first_one = false;
	for (it = cities.begin(); it != cities.end(); it++)
	{
		if (first_one == false)
		{
			trios::iterator some_other = (++cities.begin());
			std::cout << "it: " << std::get<0>(*it) << ';' << std::get<1>(*it) << ';' << std::get<2>(*it) << std::endl;
			std::cout << "some_other: " << std::get<0>(*some_other) << ';' << std::get<1>(*some_other) << ';' << std::get<2>(*some_other) << std::endl;
			first_one = true;

			float test_distance = getDistance(it, some_other);
			std::cout << "Test distance equals: " << test_distance << std::endl;
		}
		std::cout << std::get<0>(*it) << ';' << std::get<1>(*it) << ';' << std::get<2>(*it) << std::endl;
		result_file << std::get<0>(*it) << ';' << std::get<1>(*it) << ';' << std::get<2>(*it) << std::endl;
	}
	result_file << '\n';


	for (auto it3 = cities_connections.begin(); it3 != cities_connections.end(); it3++)
		cities_connections_set.insert(*it3);

	for (auto it = cities_connections_set.begin(); it != cities_connections_set.end(); it++)
	{
		result_file << std::get<0>(*it) << ';' << std::get<1>(*it) << ';' << floor(std::get<2>(*it) * ((rand() % 50 + 100) / 100.0)) << std::endl;
	}

	result_file.close();

}

int* generateSetOfNumbers(int range, int elements_count)
{
	if (elements_count > range)
	{
		std::cout << "Range value have to be higher than elements_count.";
		return nullptr;
	}
	int* result = new int[elements_count];
	int* number_pool = new int[range];
	for (int i = 0; i < range; i++)
		number_pool[i] = i;
	
	//shuffle number_pool
	for (int i = range - 1; i>0; --i)
	{
		//get swap index
		int j = rand() % i;
		//swap number_pool[i] with number_pool[j]
		int temp = number_pool[i];
		number_pool[i] = number_pool[j];
		number_pool[j] = temp;
	}

	for (int i = 0; i < elements_count; ++i)
		result[i] = number_pool[i];

	return result;
}

float getDistance(trios::iterator one, trios::iterator other)
{
	std::pair<int, int> one_cords;
	std::pair<int, int> other_cords;
	one_cords.first = std::get<1>(*one);
	one_cords.second = std::get<2>(*one);
	std::cout << "x1 = " << one_cords.first << " y1 = " << one_cords.second << std::endl;
	other_cords.first = std::get<1>(*other);
	other_cords.second = std::get<2>(*other);
	std::cout << "x2 = " << other_cords.first << " y2 = " << other_cords.second << std::endl;
	float x_distance = abs(one_cords.first - other_cords.first);
	float y_distance = abs(one_cords.second - other_cords.second);
	float distance = sqrt(pow(x_distance, 2) + pow(y_distance, 2));
	return distance;

}