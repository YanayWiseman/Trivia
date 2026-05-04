#pragma once

#include <map>
#include <vector>
#include <mutex>
#include "Room.h"


class RoomManager
{
private:
	std::map<int, Room> m_rooms;
	std::mutex m_roomsMutex;

public:
	void createRoom(const LoggedUser& usr, const RoomData& data);
	void deleteRoom(int id);
	RoomStatus getRoomState(int id);
	std::vector<RoomData> getRooms();
	Room& getRoom(int id);
};