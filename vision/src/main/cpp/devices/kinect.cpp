#include "devices/kinect.h"

#include <iostream>

// Driver //

static kinect_driver *_driver_instance;

kinect_driver* kinect_driver::instance() {
  if (_driver_instance == nullptr) {
    _driver_instance = new kinect_driver();
  }
  return _driver_instance;
}

kinect_driver::kinect_driver() {
  freenect_init(&_ctx, nullptr);
  // freenect_select_subdevices(_ctx, (freenect_device_flags)(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA));
}

int kinect_driver::num_devices() {
  return freenect_num_devices(_ctx);
}

kinect_device *kinect_driver::create(int id) {
  if (id > num_devices()) {
    std::cout << "No such Kinect: " << id << " (" << num_devices() << " kinect(s) exist" << std::endl;
    return nullptr;
  }

  return new kinect_device(id, _ctx);
}

void kinect_driver::run() {
  while (freenect_process_events(_ctx) >= 0);
}

int kinect_driver::run_single() {
  return freenect_process_events(_ctx);
}

// Device //

kinect_device::kinect_device(int id, freenect_context *ctx) : _id(id), _ctx(ctx) { }

kinect_device::~kinect_device() {
  freenect_stop_video(_dev);
  freenect_stop_depth(_dev);

  if (_video_mat) {
    _video_mat->release();
    delete _video_mat;
  }

  if (_depth_mat) {
    _depth_mat->release();
    delete _depth_mat;
  }

  if (is_open()) {
    close();
  }
}

int kinect_device::get_id() {
  return _id;
}

int kinect_device::open() {
  if (is_open()) {
    return 0;
  }

  int code;
  if ((code = freenect_open_device(_ctx, &_dev, _id)) < 0) {
    std::cout << "Could not open Kinect: " << _id << ": " << code << std::endl;
  } else {
    _open = true;
    set_video(video_mode::RGB);
    set_depth(depth_mode::FREENECT_DEPTH_MM);

    freenect_set_user(_dev, (void *)this);
    freenect_set_video_callback(_dev, [](freenect_device *dev, void *video, uint32_t timestamp) {
      ((kinect_device *)freenect_get_user(dev))->trigger_video_callback();
    });
    freenect_set_depth_callback(_dev, [](freenect_device *dev, void *depth, uint32_t timestamp) {
      ((kinect_device *)freenect_get_user(dev))->trigger_depth_callback();
    });
  }
  return code;
}

int kinect_device::close() {
  if (!is_open()) {
    return 0;
  }

  int code;
  if ((code = freenect_close_device(_dev)) < 0) {
    std::cout << "Could not close Kinect: " << _id << ": " << code << std::endl;
  } else {
    _open = false;
  }
  return code;
}

bool kinect_device::is_open() {
  return _open;
}

void kinect_device::set_tilt(double degrees) {
  freenect_set_tilt_degs(_dev, degrees);
}

void kinect_device::set_led(led_mode mode) {
  freenect_set_led(_dev, mode);
}

void kinect_device::set_video(video_mode mode, resolution res) {
  if (_video_running)
    freenect_stop_video(_dev);
  if (_video_mat)
    _video_mat->release();

  _video_mat = new cv::Mat{ get_size(res), mode == video_mode::IR ? CV_8UC1 : CV_8UC3 };
  freenect_set_video_mode(_dev, freenect_find_video_mode(res, mode == video_mode::IR ? FREENECT_VIDEO_IR_8BIT : FREENECT_VIDEO_RGB));
  freenect_set_video_buffer(_dev, (void *)_video_mat->data);

  freenect_start_video(_dev);
  _video_running = true;
}

void kinect_device::set_depth(depth_mode mode) {
  if (_depth_running)
    freenect_stop_depth(_dev);
  if (_depth_mat)
    _depth_mat->release();

  _depth_mat = new cv::Mat{ get_size(resolution::FREENECT_RESOLUTION_MEDIUM), CV_8UC2 };
  freenect_set_depth_mode(_dev, freenect_find_depth_mode(resolution::FREENECT_RESOLUTION_MEDIUM, mode));
  freenect_set_depth_buffer(_dev, (void *)_depth_mat->data);

  freenect_start_depth(_dev);
  _depth_running = true;
}

cv::Mat *kinect_device::video() {
  return _video_mat;
}

uint16_t *kinect_device::depth() {
  return (uint16_t *)_depth_mat->data;
}

cv::Size kinect_device::get_size(resolution res) {
  int width, height;
  if (res == FREENECT_RESOLUTION_LOW) {
    width = 320;
    height = 240;
  } else if (res == FREENECT_RESOLUTION_MEDIUM) {
    width = 640;
    height = 480;
  } else {
    width = 1280;
    height = 1024;
  }
  return cv::Size{width, height};
}

void kinect_device::set_depth_callback(std::function<void(kinect_device &)> cb) {
  _depth_callback = cb;
}

void kinect_device::set_video_callback(std::function<void(kinect_device &)> cb) {
  _video_callback = cb;
}

void kinect_device::trigger_video_callback() {
  if (_video_callback != nullptr)
    _video_callback(*this);
}

void kinect_device::trigger_depth_callback() {
  if (_depth_callback != nullptr)
    _depth_callback(*this);
}