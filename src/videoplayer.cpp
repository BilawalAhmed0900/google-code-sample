#include "videoplayer.h"

#include <iostream>
#include <algorithm>

bool isCaseInsensitiveStringEqual(const std::string& lhs, const std::string& rhs) {
  if (lhs.size() != rhs.size()) return false;

  for (size_t index = 0; index < lhs.size(); index++) {
    if (toupper(lhs.at(index)) != toupper(lhs.at(index))) return false;
  }

  return true;
}

void VideoPlayer::numberOfVideos() {
  std::cout << mVideoLibrary.getVideos().size() << " videos in the library"
            << std::endl;
}

void VideoPlayer::showAllVideos() {
  // std::cout << "showAllVideos needs implementation" << std::endl;
  std::vector<Video> videos = mVideoLibrary.getVideos();
  std::sort(videos.begin(), videos.end(), [](const Video& lhs, const Video& rhs) {
    return lhs.getTitle() < rhs.getTitle();
  });

  std::cout << "Here's a list of all available videos:" << std::endl;
  for (const Video& video : videos) {
    std::cout << "  " << video.getTitle() << " (" << video.getVideoId() << ") [";
    const std::vector<std::string> tags = video.getTags();
    for (size_t index = 0; index < tags.size(); index++) {
      std::cout << tags.at(index);
      if (index < tags.size() - 1) std::cout << " ";
    }
    std::cout << "]" << std::endl;
  }
}

void VideoPlayer::playVideo(const std::string& videoId) {
  // std::cout << "playVideo needs implementation" << std::endl;
  const Video *newVideo = mVideoLibrary.getVideo(videoId);
  if (newVideo == nullptr) {
    std::cout << "Cannot play video: Video does not exist" << std::endl;
    return;
  }

  if (mCurrentVideo != nullptr) {
    std::cout << "Stopping video: " << mCurrentVideo->getTitle() << std::endl;
  }

  mCurrentVideo = newVideo;
  mVideoPlaying = true;
  std::cout << "Playing video: " << mCurrentVideo->getTitle() << std::endl;
}

void VideoPlayer::stopVideo() {
  if (mCurrentVideo == nullptr) {
    std::cout << "Cannot stop video: No video is currently playing" << std::endl;
    return;
  }

  std::cout << "Stopping video: " << mCurrentVideo->getTitle() << std::endl;
  mCurrentVideo = nullptr;
  mVideoPlaying = false;
}

void VideoPlayer::playRandomVideo() {
  const std::vector<Video> videos = mVideoLibrary.getVideos();

  /*
    Should not be used but will do the job here
  */
  int randomIndex = rand() % videos.size();

  /*
    Get pointer from inside
  */
  const Video *newVideo = mVideoLibrary.getVideo(videos.at(randomIndex).getVideoId());
  if (mCurrentVideo != nullptr) {
    std::cout << "Stopping video: " << mCurrentVideo->getTitle() << std::endl;
  }

  mCurrentVideo = newVideo;
  mVideoPlaying = true;
  std::cout << "Playing video: " << mCurrentVideo->getTitle() << std::endl;
}

void VideoPlayer::pauseVideo() {
  if (mCurrentVideo == nullptr) {
    std::cout << "Cannot pause video: No video is currently playing" << std::endl;
    return;
  }

  if (mVideoPlaying == false) {
    std::cout << "Video already paused: " << mCurrentVideo->getTitle() << std::endl;
    return;
  }

  std::cout << "Pausing video: " << mCurrentVideo->getTitle() << std::endl;
  mVideoPlaying = false;
}

void VideoPlayer::continueVideo() {
  if (mCurrentVideo == nullptr) {
    std::cout << "Cannot continue video: No video is currently playing" << std::endl;
    return;
  }

  if (mVideoPlaying == true) {
    std::cout << "Cannot continue video: Video is not paused" << std::endl;
    return;
  }

  mVideoPlaying = true;
  std::cout << "Continuing video: " << mCurrentVideo->getTitle() << std::endl;
}

void VideoPlayer::showPlaying() {
  if (mCurrentVideo == nullptr) {
    std::cout << "No video is currently playing" << std::endl;
    return;
  }

  std::cout << "Currently playing: " << mCurrentVideo->getTitle()
    << " (" << mCurrentVideo->getVideoId() << ") [";

  const std::vector<std::string> tags = mCurrentVideo->getTags();
  for (size_t index = 0; index < tags.size(); index++) {
      std::cout << tags.at(index);
      if (index < tags.size() - 1) std::cout << " ";
  }
  std::cout << "]";

  if (mVideoPlaying == false) {
    std::cout << " - PAUSED";
  }
  std::cout << std::endl;
}

void VideoPlayer::createPlaylist(const std::string& playlistName) {
  auto checkFunc = [&playlistName](const VideoPlaylist& elem) {
    return isCaseInsensitiveStringEqual(playlistName, elem.getName());
  };

  if (std::find_if(mPlaylists.begin(), mPlaylists.end(), checkFunc) != mPlaylists.end()) {
    std::cout << "Cannot create playlist: A playlist with the same name already exists" << std::endl;
    return;
  }

  mPlaylists.emplace_back(playlistName);
  std::cout << "Successfully created new playlist: " << playlistName << std::endl;
}

void VideoPlayer::addVideoToPlaylist(const std::string& playlistName,
                                     const std::string& videoId) {
  auto checkFunc = [&playlistName](const VideoPlaylist& elem) {
    return isCaseInsensitiveStringEqual(playlistName, elem.getName());
  };

  auto it = std::find_if(mPlaylists.begin(), mPlaylists.end(), checkFunc);
  if (it == mPlaylists.end()) {
    std::cout << "Cannot add video to " << playlistName << ": Playlist does not exist" << std::endl;
    return;
  }

  const Video* video = mVideoLibrary.getVideo(videoId);
  if (video == nullptr) {
    std::cout << "Cannot add video to " << playlistName << ": Video does not exist" << std::endl;
    return;
  }

  bool added = it->add(videoId);
  if (added == false) {
    std::cout << "Cannot add video to " << playlistName << ": Video already added" << std::endl;
    return;
  }

  std::cout << "Added video to " << playlistName << ": " << video->getTitle() << std::endl;
}

void VideoPlayer::showAllPlaylists() {
  if (mPlaylists.empty()) {
    std::cout << "No playlists exist yet" << std::endl;
    return;
  }

  std::cout << "Showing all playlists:" << std::endl;
  auto sortFunc = [](const VideoPlaylist& lhs, const VideoPlaylist& rhs) {
    return lhs.getName() < rhs.getName();
  };

  std::sort(mPlaylists.begin(), mPlaylists.end(), sortFunc);
  for (const VideoPlaylist& playlist: mPlaylists) {
    std::cout << "  " << playlist.getName() << std::endl;
  }
}

void VideoPlayer::showPlaylist(const std::string& playlistName) {
  auto checkFunc = [&playlistName](const VideoPlaylist& elem) {
    return isCaseInsensitiveStringEqual(playlistName, elem.getName());
  };

  auto it = std::find_if(mPlaylists.begin(), mPlaylists.end(), checkFunc);
  if (it == mPlaylists.end()) {
    std::cout << "Cannot show playlist " << playlistName << ": Playlist does not exist" << std::endl;
    return;
  }

  std::cout << "Showing playlist: " << playlistName << std::endl;
  if (it->getVideos().empty()) {
    std::cout << "  No videos here yet" << std::endl;
    return;
  }

  for (const std::string& videoId : it->getVideos()) {
    const Video *video = mVideoLibrary.getVideo(videoId);
    if (video == nullptr) {
      std::cout << "  (NULL)" << std::endl;
      continue;
    }

    std::cout << " " << video->getTitle() << " (" << video->getVideoId() << ") [";
    const std::vector<std::string> tags = video->getTags();
    for (size_t index = 0; index < tags.size(); index++) {
      std::cout << tags.at(index);
      if (index < tags.size() - 1) std::cout << " ";
    }
    std::cout << "]" << std::endl;
  }
}

void VideoPlayer::removeFromPlaylist(const std::string& playlistName,
                                     const std::string& videoId) {
  auto checkFunc = [&playlistName](const VideoPlaylist& elem) {
    return isCaseInsensitiveStringEqual(playlistName, elem.getName());
  };

  auto it = std::find_if(mPlaylists.begin(), mPlaylists.end(), checkFunc);
  if (it == mPlaylists.end()) {
    std::cout << "Cannot remove video from " << playlistName << ": Playlist does not exist" << std::endl;
    return;
  }

  const Video* video = mVideoLibrary.getVideo(videoId);
  if (video == nullptr) {
    std::cout << "Cannot remove video from " << playlistName << ": Video does not exist" << std::endl;
    return;
  }

  bool removed = it->remove(videoId);
  if (removed == false) {
    std::cout << "Cannot remove video from " << playlistName << ": Video is not in playlist" << std::endl;
    return;
  }

  std::cout << "Removed video from " << playlistName << ": " << video->getTitle() << std::endl;
}

void VideoPlayer::clearPlaylist(const std::string& playlistName) {
  auto checkFunc = [&playlistName](const VideoPlaylist& elem) {
    return isCaseInsensitiveStringEqual(playlistName, elem.getName());
  };

  auto it = std::find_if(mPlaylists.begin(), mPlaylists.end(), checkFunc);
  if (it == mPlaylists.end()) {
    std::cout << "Cannot clear playlist " << playlistName << ": Playlist does not exist" << std::endl;
    return;
  }

  it->clear();
  std::cout << "Successfully removed all videos from " << playlistName << std::endl;
}

void VideoPlayer::deletePlaylist(const std::string& playlistName) {
  auto checkFunc = [&playlistName](const VideoPlaylist& elem) {
    return isCaseInsensitiveStringEqual(playlistName, elem.getName());
  };

  auto it = std::find_if(mPlaylists.begin(), mPlaylists.end(), checkFunc);
  if (it == mPlaylists.end()) {
    std::cout << "Cannot delete playlist " << playlistName << ": Playlist does not exist" << std::endl;
    return;
  }

  mPlaylists.erase(it);
  std::cout << "Deleted playlist: " << playlistName << std::endl;
}

void VideoPlayer::searchVideos(const std::string& searchTerm) {
  std::string searchTermCopy = searchTerm;
  for (char& c : searchTermCopy) c = toupper(c);

  auto searchFunc = [&searchTermCopy](const Video& video) {
    std::string videoTitle = video.getTitle();
    for (char& c : videoTitle) c = toupper(c);
    return videoTitle.find(searchTermCopy) == std::string::npos;
  };

  std::vector<Video> videos = mVideoLibrary.getVideos();
  videos.erase(std::remove_if(videos.begin(), videos.end(), searchFunc), videos.end());
  if (videos.empty()) {
    std::cout << "No search results for " << searchTerm << std::endl;
    return;
  }

  auto sortFunc = [](const Video& lhs, const Video& rhs) {
    return lhs.getTitle() < rhs.getTitle();
  };
  std::sort(videos.begin(), videos.end(), sortFunc);

  size_t index = 1;
  std::cout << "Here are the results for " << searchTerm << ":" << std::endl;
  for (const Video& video : videos) {
    std::cout << "  " << index << ") " << video.getTitle() << " (" << video.getVideoId() << ") [";

    std::vector<std::string> tags = video.getTags();
    for (size_t tag_index = 0; tag_index < tags.size(); tag_index++) {
      std::cout << tags.at(tag_index);
      if (tag_index != tags.size() - 1) {
        std::cout << " ";
      }
    }
    std::cout << "]" << std::endl;
    index++;
  }

  std::cout << "Would you like to play any of the above? If yes, specify the number of the video." << std::endl;
  std::cout << "If your answer is not a valid number, we will assume it's a no." << std::endl;

  std::string number_str;
  std::getline(std::cin, number_str);
  try {
    size_t number = std::stoul(number_str);
    /*
      Input starts from 1, we need index from 0
    */
    number--;

    if (number < videos.size()) {
      playVideo(videos.at(number).getVideoId());
    }
  }
  catch(const std::out_of_range& e) {
    /* Ignored */
  }
  catch(const std::invalid_argument& e) {
    /* Ignored */
  }
}

void VideoPlayer::searchVideosWithTag(const std::string& videoTag) {
  std::string searchTermCopy = videoTag;
  for (char& c : searchTermCopy) c = toupper(c);

  auto searchFunc = [&searchTermCopy](const Video& video) {
    std::vector<std::string> videoTags = video.getTags();
    for (std::string& videoTag : videoTags) {
      for (char& c : videoTag) c = toupper(c);
    }

    bool found = false;
    for (const std::string& videoTag : videoTags) {
      if (videoTag == searchTermCopy) {
        found = true;
        break;
      }
    }

    /*
      If we have found it, we need remove_if to ignore it,
      so found(true) will become false for that.
    */
    return !found;
  };

  std::vector<Video> videos = mVideoLibrary.getVideos();
  videos.erase(std::remove_if(videos.begin(), videos.end(), searchFunc), videos.end());
  if (videos.empty()) {
    std::cout << "No search results for " << videoTag << std::endl;
    return;
  }

  auto sortFunc = [](const Video& lhs, const Video& rhs) {
    return lhs.getTitle() < rhs.getTitle();
  };
  std::sort(videos.begin(), videos.end(), sortFunc);

  size_t index = 1;
  std::cout << "Here are the results for " << videoTag << ":" << std::endl;
  for (const Video& video : videos) {
    std::cout << "  " << index << ") " << video.getTitle() << " (" << video.getVideoId() << ") [";

    std::vector<std::string> tags = video.getTags();
    for (size_t tag_index = 0; tag_index < tags.size(); tag_index++) {
      std::cout << tags.at(tag_index);
      if (tag_index != tags.size() - 1) {
        std::cout << " ";
      }
    }
    std::cout << "]" << std::endl;
    index++;
  }

  std::cout << "Would you like to play any of the above? If yes, specify the number of the video." << std::endl;
  std::cout << "If your answer is not a valid number, we will assume it's a no." << std::endl;

  std::string number_str;
  std::getline(std::cin, number_str);
  try {
    size_t number = std::stoul(number_str);
    /*
      Input starts from 1, we need index from 0
    */
    number--;

    if (number < videos.size()) {
      playVideo(videos.at(number).getVideoId());
    }
  }
  catch(const std::out_of_range& e) {
    /* Ignored */
  }
  catch(const std::invalid_argument& e) {
    /* Ignored */
  }
}

void VideoPlayer::flagVideo(const std::string& videoId) {
  std::cout << "flagVideo needs implementation" << std::endl;
}

void VideoPlayer::flagVideo(const std::string& videoId, const std::string& reason) {
  std::cout << "flagVideo needs implementation" << std::endl;
}

void VideoPlayer::allowVideo(const std::string& videoId) {
  std::cout << "allowVideo needs implementation" << std::endl;
}
