#pragma once
enum EVENT_TYPE : int
{
	EMPTY = 0,
	WALL = 1
};

class Event : public ResourceObj
{
private:
	EVENT_TYPE m_event_type;
public:
	Event(EVENT_TYPE _event_type) : ResourceObj(EVENT), m_event_type(_event_type) {};

	EVENT_TYPE GetEventType() { return m_event_type; }

};

