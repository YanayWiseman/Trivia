#include "RoomManager.h"
#include "SqliteDatabase.h"

/*
	Function creates room, adds given user to it and adds it to room map.
	Input: usr (const LoggedUser&), data (const RoomData&)
	Output: Succuss or Failute
*/

Status RoomManager::createRoom(const LoggedUser& usr, const RoomData& data)
{
	std::lock_guard<std::mutex> lock(this->m_roomsMutex);

	this->m_rooms.try_emplace(data.id, data);
	this->m_rooms.at(data.id).addUser(usr);

	return Success;
}


/*
	Function deletes room, removing it from room map using id.
	Input: id (int)
	Output: None (void)
*/

void RoomManager::deleteRoom(int id)
{
	std::lock_guard<std::mutex> lock(this->m_roomsMutex);

	this->m_rooms.erase(id);
}


/*
	Function return room status using given ID.
	Input: id (int)
	Output: Room status (RoomStatus)
*/

RoomStatus RoomManager::getRoomState(int id) const
{
	std::lock_guard<std::mutex> lock(this->m_roomsMutex);

	auto it = this->m_rooms.find(id);

	if (it == this->m_rooms.end())
	{
		return Closed; 
	}

	return it->second.getData().status;
}


/*
	Function returns vector of data of all rooms.
	Input: None
	Output: Vector of RoomData of all rooms (std::vector<RoomData>)
*/

std::vector<RoomData> RoomManager::getRooms()
{
	std::lock_guard<std::mutex> lock(this->m_roomsMutex);
	std::vector<RoomData> rooms;
	RoomData data;

	for (auto it = this->m_rooms.begin(); it != this->m_rooms.end(); it++)
	{
		data = it->second.getData();

		if (data.status == Open)
		{
			rooms.push_back(data);
		}
	}

	return rooms;
}

/*
	Method to get a room by its id
	input: the id
	output: the room
*/

Room& RoomManager::getRoom(int id)
{
	std::lock_guard<std::mutex> lock(this->m_roomsMutex);

	auto it = this->m_rooms.find(id);

	if (it == this->m_rooms.end())
	{
		throw std::exception("Room not found.");
	}

	return it->second;
}


/*
	Function creates room with random unique id, adds given user to it and adds it to room map.
	Input: usr (const LoggedUser&), data (RoomData)
	Output: Created room id (unsigned int)
*/

unsigned int RoomManager::createRoomWithRandomId(const LoggedUser& usr, RoomData data)
{
	std::lock_guard<std::mutex> lock(this->m_roomsMutex);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(100000, 999999);

	int roomId = 0;

	do
	{
		roomId = distr(gen);
	} while (this->m_rooms.find(roomId) != this->m_rooms.end());

	data.id = roomId;

	this->m_rooms.try_emplace(data.id, data);
	this->m_rooms.at(data.id).addUser(usr);

	return data.id;
}

/*
	Function creates single instance of RoomManager.
	Input: None
	Output: New RoomManager instance (RoomManager&)
*/

RoomManager& RoomManager::getInstance()
{
	static RoomManager instance;
	return instance;
}

/*
	Method to check if a room exists 
	input: the room's id
	output: true if it exists and false otherwise
*/

bool RoomManager::checkIfRoomExists(int id) const
{
	std::lock_guard<std::mutex> lock(this->m_roomsMutex);
	auto it = this->m_rooms.find(id);
	return it != this->m_rooms.end();
}