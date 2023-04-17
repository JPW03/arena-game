#pragma once
#include <SFML\System.hpp>

//Timer is just an upgraded version of sf::Clock

//The primary addition is being able to pause/resume;
//	The timer will store its current value when paused
//	When resumed, the difference between the new time and the stored time is added to the offset, which is taken away from the return of GetElapsedTime


class Timer {

public:
	Timer();
	~Timer();

	double GetElapsedTime();	//Returns the value of the timer in seconds
	void Restart();		//Resets value of timer to 0.
	void Pause();
	void Resume();

private:
	sf::Clock _timer;
	double _timeCache;
	double _timeOffset;
	bool _paused;
};