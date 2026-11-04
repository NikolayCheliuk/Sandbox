/**
 * VideoPlayer.cpp
 */

#include "VideoPlayer.hpp"

VideoPlayer::VideoPlayer() :
  state(State::Stopped)
{
}

void VideoPlayer::play()
{
  switch (state)
  {
    case State::Stopped:
    case State::Paused:
    {
      state = State::Playing;
      std::cout << "Playing video\n";
      break;
    }

    case State::Playing:
    {
      std::cout << "Already playing\n";
      break;
    }
  }
}

void VideoPlayer::pause()
{
  switch (state)
  {
    case State::Playing:
    {
      state = State::Paused;
      std::cout << "Video paused\n";
      break;
    }

    case State::Stopped:
    {
      std::cout << "Can't pause, video stopped\n";
      break;
    }

    case State::Paused:
    {
      std::cout << "Already paused\n";
      break;
    }
  }
}

void VideoPlayer::stop()
{
  switch (state)
  {
    case State::Playing:
    case State::Paused:
    {
      state = State::Stopped;
      std::cout << "Video stopped\n";
      break;
    }

    case State::Stopped:
    {
      std::cout << "Already stopped\n";
      break;
    }
  }
}
