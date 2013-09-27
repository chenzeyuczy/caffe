// Copyright 2013 Yangqing Jia

#include <stdint.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <algorithm>
#include <string>

#include "caffe/common.hpp"
#include "caffe/util/io.hpp"
#include "caffe/proto/caffe.pb.h"

using cv::Mat;
using cv::Vec3b;
using std::max;
using std::string;

namespace caffe {

void ReadImageToProto(const string& filename, BlobProto* proto) {
  Mat cv_img;
  cv_img = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
  CHECK(cv_img.data) << "Could not open or find the image.";
  DCHECK_EQ(cv_img.channels(), 3);
  proto->set_num(1);
  proto->set_channels(3);
  proto->set_height(cv_img.rows);
  proto->set_width(cv_img.cols);
  proto->clear_data();
  proto->clear_diff();
  for (int c = 0; c < 3; ++c) {
    for (int h = 0; h < cv_img.rows; ++h) {
      for (int w = 0; w < cv_img.cols; ++w) {
        proto->add_data(static_cast<float>(cv_img.at<Vec3b>(h, w)[c]) / 255.);
      }
    }
  }
}

void WriteProtoToImage(const string& filename, const BlobProto& proto) {
  CHECK_EQ(proto.num(), 1);
  CHECK(proto.channels() == 3 || proto.channels() == 1);
  CHECK_GT(proto.height(), 0);
  CHECK_GT(proto.width(), 0);
  Mat cv_img(proto.height(), proto.width(), CV_8UC3);
  for (int c = 0; c < 3; ++c) {
    int source_c = max(c, proto.channels() - 1);
    for (int h = 0; h < cv_img.rows; ++h) {
      for (int w = 0; w < cv_img.cols; ++w) {
        cv_img.at<Vec3b>(h, w)[c] =
            uint8_t(proto.data((source_c * cv_img.rows + h) * cv_img.cols + w)
                * 255.);
      }
    }
  }
  CHECK(cv::imwrite(filename, cv_img));
}


}  // namespace caffe
