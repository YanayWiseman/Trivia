#pragma once

#include <map>
#include <vector>
#include <mutex>
#include <exception>
#include "Room.h"

class RoomManager
{
private:
	// fields
	std::map<int, Room> m_rooms;
	mutable std::mutex m_roomsMutex;

	// methods
	RoomManager() = default;

public:
	// methods
	Status createRoom(const LoggedUser& usr, const RoomData& data);
	void deleteRoom(int id);
	RoomStatus getRoomState(int id) const;
	std::vector<RoomData> getRooms();
	bool checkIfRoomExists(int id) const;
	Room& getRoom(int id);

	static RoomManager& getInstance();

	RoomManager(const RoomManager& other) = delete;
	RoomManager& operator=(const RoomManager& other) = delete;
};