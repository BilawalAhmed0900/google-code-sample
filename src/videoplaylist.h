#pragma once

#include <string>
#include <vector>

/** A class used to represent a Playlist */

class VideoPlaylist {
  std::string mName;
  std::vector<std::string> mVideos;

public:
  explicit VideoPlaylist(const std::string& name) : mName(name) { }

  VideoPlaylist(const VideoPlaylist& other) = delete;
  VideoPlaylist& operator=(const VideoPlaylist& other) = delete;

  VideoPlaylist(VideoPlaylist&& other) = default;
  VideoPlaylist& operator=(VideoPlaylist&& other) = default;

  /*
    False: already exists
    True: successfully added
  */
  bool add(const std::string& videoId);

  /*
    False: does not exists
    True: successfully removed
  */
  bool remove(const std::string& videoId);

  void clear();

  std::string getName() const { return mName; }
  const std::vector<std::string>& getVideos() const { return mVideos; }
};
