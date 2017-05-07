#include "Timer.h"
#include <iostream>
#include <iomanip>

high_resolution_clock::time_point& Timer::StartTimer()
{
	m_startPoint = high_resolution_clock::now();
	return m_startPoint;
}

high_resolution_clock::time_point& Timer::StopTimer()
{
	m_stopPoint = high_resolution_clock::now();
	m_elapsedTime = duration_cast<duration<float>>(m_stopPoint - m_startPoint);
	return m_stopPoint;
}

duration<float>& Timer::GetElapsedTime()
{
	return m_elapsedTime;
}

void Timer::PrintElapsedTime() const
{
	namespace cr = std::chrono;
	// auto diff = end - start;

	auto hours = cr::duration_cast<cr::hours>(m_elapsedTime);
	m_elapsedTime -= hours;

	auto mins = cr::duration_cast<cr::minutes>(m_elapsedTime);
	m_elapsedTime -= mins;

	auto secs = cr::duration_cast<cr::seconds>(m_elapsedTime);
	m_elapsedTime -= secs;

	auto millis = cr::duration_cast<cr::milliseconds>(m_elapsedTime);
	m_elapsedTime -= millis;

	auto nanos = cr::duration_cast<cr::microseconds>(m_elapsedTime);

	std::cout << std::setfill('0');
	std::cout << std::setw(2) << hours.count() << ':'
		<< std::setw(2) << mins.count() << ':'
		<< std::setw(2) << secs.count() << ':'
		<< std::setw(3) << millis.count() << "."
		<< std::setw(3) << nanos.count() << ";";
	std::cout << std::endl;
}

void Timer::PrintMilliSeconds() const
{
	// std::cout << "Measurement (ms) : " << m_elapsedTime.count() << std::endl;
	printf("%.15f", m_elapsedTime.count());
}
