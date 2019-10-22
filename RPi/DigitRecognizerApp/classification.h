#ifndef CLASSIFICATION_H
#define CLASSIFICATION_H

#include <vector>
#include <iostream>
#include <caffe/caffe.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

using namespace caffe;

class Classifier
{
  public:
      Classifier();
      Classifier(string model_file, string trained_file);
      ~Classifier();

      /* 이미지 처리를 위한 메소드 */
      void wrapInputLayer(const vector<Mat> imgs, vector<Mat> *input_channels);
      void wrapInputLayer(const Mat imgs, vector<Mat> *input_channels);
      void preprocess(const cv::Mat& img, cv::Mat& grayImg);
      int predictNumber(Mat img); 			/* 분석한 숫자를 반환 */

      std::shared_ptr<Net<float>> m_net; 		/* 네트워크 */
};

#endif /* CLASSIFICATION_H */
