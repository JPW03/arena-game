#include "Timer.h"
#include <iostream>


Timer::Timer() : _paused(false), _timeOffset(0) {
	Restart();
}

Timer::~Timer() {}


double Timer::GetElapsedTime() {
	if (!_paused) {
		return _timer.getElapsedTime().asSeconds() + _timeOffset;
	}
	else {
		return _timeCache;
	}
}


void Timer::Restart() {
	_timer.restart();
	_timeOffset = 0;
	_timeCache = 0;
}


void Timer::Pause() {
	if (!_paused) {
		_paused = true;
		_timeCache = _timer.getElapsedTime().asSeconds() + _timeOffset;
	}
}


void Timer::Resume() {
	if (_paused) {
		_paused = false;
		_timeOffset += _timeCache;
		_timer.restart();
	}
}