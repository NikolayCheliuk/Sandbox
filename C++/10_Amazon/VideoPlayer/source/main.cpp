/**
 * Video Player
 */

#include "VideoPlayer.hpp"

#include <iostream>
#include <string>

int main(int argc, const char * argv[])
{
  VideoPlayer player;

  player.play();   // Stopped → Playing
  player.pause();  // Playing → Paused
  player.pause();  // Playing → Paused
  player.stop();   // Playing → Stopped
  player.pause();  // Playing → Paused
  player.play();   // Paused → Playing
  player.stop();   // Playing → Stopped
  player.pause();  // Playing → Paused

  return 0;
}
