#include "RoomManager.h"
#include "SqliteDatabase.h"

/*
	Function creates room, adds given user to it and adds it to room map.
	Input: usr (const LoggedUser&), data (const RoomData&)
	Output: None (void)
*/

void RoomManager::createRoom(const LoggedUser& usr, const RoomData& data)
{
	std::lock_guard<std::mutex> lock(this->m_roomsMutex);

	this->m_rooms.try_emplace(data.id, data);
	this->m_rooms.at(data.id).addUser(usr);
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

RoomStatus RoomManager::getRoomState(int id)
{
	std::lock_guard<std::mutex> lock(this->m_roomsMutex);

	auto it = this->m_rooms.find(id);

	if (it == this->m_rooms.end())
	{
		return Closed; 
	}

	return it->second.getData().status;
}

std::vector<RoomData> RoomManager::getRooms()
{
	return std::vector<RoomData>();
}


