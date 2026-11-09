/**
* @file Serialization.h
* @brief Tools for saving data in json file
*
* @version 1.1
* @date 2026-10-03
*
* @copyright idk bro
* @author MgPhenix (https://github.com/MgPhenix) & Marien-Garon (https://github.com/Marien-Garon)
*/
#pragma once
#include <string>
#include "json.hpp"
/**
* @brief nlohmann json data type
*/
using json = nlohmann::json;

/**
* @brief A simple Serializer for write/read data to/from a json file
*/
class Serializer
{
protected:

	json m_data;

public:

	Serializer() = default;

	/**
	* @brief Write data in a json file
	* @param std::string& id : Id of the data you wanna save
	* @tparam T : Value you wanna save
	*/
	template<typename T>
	void Write(const std::string& id, T value)
	{
		m_data[id] = value;
	}
	/**
	* @brief Read data from a json file
	* @param std::string& id : Id of the data you wanna read
	* @tparam T& : Value you wanna read
	*/
	template<typename T>
	void Read(const std::string& id, T& value)
	{
		value = m_data.value(id, T());
	}
	/**
	* @brief Read data from a json file (use Get<type>(id) )
	* @param std::string& id : Id of the data you wanna read
	* @return T : value you wanna read
	*/
	template<typename T>
	T Get(const std::string& id)
	{
		return m_data.value(id, T());
	}

public:

	/**
	* @brief Get raw json data (nlohmann json)
	* @return json& : reference to the raw json data
	*/
	json& GetData();
	/**
	* @brief Save Data to a file
	* @param const std::string& path : Path to your json file
	*/
	void Save(const std::string& path);
	/**
	* @brief Load Data from a file
	* @param const std::string& path : Path to your json file
	*/
	void Load(const std::string& path);

	//For what we lost
	//template<typename T>
	//T operator[](const std::string& key)
	//auto operator[](const std::string& key) -> std::invoke_result_t<decltype(Get<T>), std::string, T> // Same same but different
	//{
	//	return m_data.value(key, T());
	//}

	auto& operator[](const std::string& key)
	{
		return m_data[key];
	}
};