#include "videoplaylist.h"

#include <algorithm>

bool VideoPlaylist::add(const std::string& videoId) {
  if (std::find(mVideos.begin(), mVideos.end(), videoId) != mVideos.end()) {
    return false;
  }

  mVideos.push_back(videoId);
  return true;
}

bool VideoPlaylist::remove(const std::string& videoId) {
  auto it = std::find(mVideos.begin(), mVideos.end(), videoId);
  if (it == mVideos.end()) {
    return false;
  }

  mVideos.erase(it);
  return true;
}

void VideoPlaylist::clear() {
  mVideos.clear();
}
