#include <mbed.h>
#include "image_provider.h"
#include "camera.h"

const uint32_t cImageWidth = 320;
const uint32_t cImageHeigth = 240;

CameraClass cam;
uint8_t buffer[cImageWidth * cImageHeigth];

// Get the camera module ready
void InitCamera(tflite::ErrorReporter* error_reporter) {
  TF_LITE_REPORT_ERROR(error_reporter, "Attempting to start Arducam");

  // Init the cam QVGA, 30FPS
  cam.begin(CAMERA_R320x240, 30);
}

// Get an image from the camera module
TfLiteStatus GetImage(tflite::ErrorReporter* error_reporter, int image_width,
                      int image_height, int channels, int8_t* image_data) {
  static bool g_is_camera_initialized = false;
  if (!g_is_camera_initialized) {
    InitCamera(error_reporter);
    g_is_camera_initialized = true;
  }

  cam.grab(buffer);
  
  auto xOffset = (cImageWidth - image_width) / 2;
  auto yOffset = (cImageHeigth - image_height) / 2;
  
  for(int i = 0; i < image_height; i++) {
    for(int j = 0; j < image_width; j++) {
      image_data[(i * image_width) + j] = buffer[((i + yOffset) * cImageWidth) + (xOffset + j)];
    }
  }

  return kTfLiteOk;
}
