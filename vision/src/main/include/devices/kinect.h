#pragma once

#include <libfreenect.h>
#include <opencv2/opencv.hpp>

#include <stdint.h>
#include <functional>

class kinect_device;

/**
 * Kinect Driver - Manages all kinect instances, and the main kinect capture loop
 */
class kinect_driver {
 public:
  /**
   * Get the current instance of the kinect driver.
   */
  static kinect_driver *instance();
  
  /**
   * @return the number of kinects detected.
   */
  int num_devices();

  /**
   * Create a Kinect on the given ID.
   */
  kinect_device *create(int id);

  /**
   * Run the main Kinect capture loop. This function will loop itself until an error is detected.
   */
  void run();
  
  /**
   * Single-shot capture. Does not loop.
   * 
   * @return 0 if successful, non-0 if error
   */
  int run_single();

 private:
  kinect_driver();
  freenect_context *_ctx;
  int _err;
};

/**
 * Kinect Device - A single instance of a Microsoft Xbox 360 Kinect.
 * 
 * By default, it is configured to start a video and depth stream, at medium resolution, in RGB + Millimetres.
 */
class kinect_device {
 public:
  using led_mode = freenect_led_options;
  using resolution = freenect_resolution;
  using depth_mode = freenect_depth_format;

  enum class video_mode {
    IR,   // Infrared monochrome capture
    RGB   // RGB (Full Colour) capture
  };

  /**
   * Get the ID of the Kinect sensor, as passed to @ref kinect_driver::create(int)
   */
  int get_id();

  /**
   * Open the Kinect device. Must be called before any set / capture functions.
   * 
   * @return 0 on success, non-0 on error.
   */
  int open();

  /**
   * Close the Kinect device. Use this to discard the Kinect device.
   * 
   * @return 0 on success, non-0 on error.
   */
  int close();

  /**
   * Is the Kinect currently open and active?
   */
  bool is_open();

  /**
   * Set the tilt of the Kinect motor, in degrees.
   * 
   * @param degrees The tilt angle in degrees, between -27 and 27.
   */
  void set_tilt(double degrees);

  /**
   * Set the LED pattern of the Kinect LED
   */
  void set_led(led_mode mode);

  /**
   * Set the video mode of the Kinect.
   * 
   * @param mode The video mode, either RGB (Full Colour), or IR (Monochrome)
   * @param res The resolution. Low = 320x240@30Hz, Med = 640x480@30Hz, High = 1280x1024@10Hz
   */
  void set_video(video_mode mode, resolution res = resolution::FREENECT_RESOLUTION_MEDIUM);

  /**
   * Set the depth mode of the Kinect.
   * 
   * @param depth_mode The depth mode. Recommended MM (millimetres aligned with IR), or Registered (millimeters aligned with RGB)
   */
  void set_depth(depth_mode mode);

  /**
   * Set the depth callback, called when a new depth frame is available.
   */
  void set_depth_callback(std::function<void(kinect_device &)> cb);

  /**
   * Set the video callback, called when a new video frame is available.
   */
  void set_video_callback(std::function<void(kinect_device &)> cb);

  /**
   * @return The video frame, as an OpenCV Mat. May be 3 channel (for RGB) or 1 channel (for IR)
   */
  cv::Mat *video();

  /**
   * @return The depth data, as an array of uint16_t. Resolution is 640x480, units depend on active depth mode.
   */
  uint16_t *depth();

  // Internal, do not call
  void trigger_video_callback();
  // Internal, do not call
  void trigger_depth_callback();

 private:
  kinect_device(int id, freenect_context *ctx);
  ~kinect_device();

  cv::Size get_size(resolution res);

  freenect_device *_dev;
  freenect_context *_ctx;

  int _id;
  bool _open = false;
  bool _depth_running = false;
  bool _video_running = false;

  cv::Mat *_video_mat = nullptr;
  cv::Mat *_depth_mat = nullptr;

  std::function<void(kinect_device &)> _depth_callback = nullptr;
  std::function<void(kinect_device &)> _video_callback = nullptr;

  friend class kinect_driver;
};