/**
 * VideoPlayer.hpp
 */

#pragma once

#include <iostream>
#include <string>

enum class State
{
  Stopped,
  Playing,
  Paused
};

class VideoPlayer
{
public:
  VideoPlayer();

  void play();
  void pause();
  void stop();

private:
  State state;
};
