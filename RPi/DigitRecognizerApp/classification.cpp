#include <QtMath>

#include "classification.h"

Classifier::Classifier()
{
}

Classifier::~Classifier()
{
}

Classifier::Classifier(std::string model_file,std::string trained_file)
{
    Caffe::set_mode(Caffe::CPU);
    m_net.reset(new Net<float>(trained_file, TEST)); 	/* 분석을 위해 사용할 네트워크 설정 */
    m_net->CopyTrainedLayersFrom(model_file); 		/* 가중치 파일 설정 */
}

/* 필기체 인식을 위해 폭(w)*높이(h)*3을 3*h*w으로 변환 */
void Classifier::wrapInputLayer(const vector<cv::Mat> imgs, std::vector<cv::Mat> *input_channels)
{
    Blob<float> *input_layer = m_net->input_blobs()[0]; /* 데이터 영역(블랍) */
    int width = input_layer->width(); 			/* 블랍의 폭 */
    int height = input_layer->height(); 		/* 블랍의 높이 */
    int num = input_layer->num(); 			/* 블랍의 수 */

    float *input_data = input_layer->mutable_cpu_data(); /* 블랍에 대한 포인터 */

    for (int j = 0; j < num; j++) {
        for (int k = 0; k < input_layer->channels(); k++) {
            cv::Mat channel(height, width, CV_32FC1, input_data);
            input_channels->push_back(channel);
            input_data += width * height;
        }
        cv::Mat img = imgs[j];
        cv::split(img, *input_channels); 		/* r, g, b 채널로 분리 */
        input_channels->clear();
    }
}

void Classifier::wrapInputLayer(const cv::Mat imgs, std::vector<cv::Mat> *input_channels)
{
    Blob<float> *input_layer = m_net->input_blobs()[0]; /* 데이터 영역(블랍) */
    int width = input_layer->width(); 			/* 블랍의 폭 */
    int height = input_layer->height(); 		/* 블랍의 높이 */
    float *input_data = input_layer->mutable_cpu_data(); /* 블랍에 대한 포인터 */

    for (int k = 0; k < input_layer->channels(); k++) {
        cv::Mat channel(height, width, CV_32FC1, input_data);
        input_channels->push_back(channel);
        input_data += width * height;
    }

    cv::split(imgs, *input_channels); 			/* r, g, b 채널로 분리 */
}

/* 선처리 과정 진행 */
void Classifier::preprocess(const cv::Mat& img, cv::Mat& grayImg)
{
    /* 입력 이미지를 네트워크에 맞는 이미지 포맷으로 변경 */
    cv::Mat sample;

    if(img.channels() == 3) /* 컬러인 경우 흑백 이미지로 변환 */
        cv::cvtColor(img, sample, cv::COLOR_BGR2GRAY);
    else if(img.channels() == 4)
        cv::cvtColor(img, sample, cv::COLOR_BGRA2GRAY);
    else
        sample = img;

    cv::threshold(sample, sample, 250, 255, THRESH_BINARY | THRESH_OTSU);
//    cv::Mat mask = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3), cv::Point(1, 1));
//    cv::erode(sample, sample, mask, cv::Point(-1, -1), 3);

    cv::bitwise_not(sample, sample); /* 흑백의 색상을 치환 */

    resize(sample, grayImg, cv::Size(28, 28), cv::INTER_CUBIC); /* 28 × 28 크기로 변환 */
    grayImg.convertTo(grayImg, CV_32FC1, 0.00390625); /* 정규화 1/256단계*/
}

/* 예측한 숫자를 반환 */
int Classifier::predictNumber(cv::Mat img)
{
    cv::Mat grayImg;
    preprocess(img, grayImg); /* 선처리 과정 */

    /* 네트워크 입력 크기 설정 */
    Blob<float> *input_layer = m_net->input_blobs()[0];
    input_layer->Reshape(1, 1, grayImg.rows, grayImg.cols);
    m_net->Reshape();

    /* 채널 변경 */
    std::vector<cv::Mat>channels;
    wrapInputLayer(grayImg, &channels);

    /* 순방향 네트워크(forword net) 설정 */
    m_net->Forward();

    /* 네트워크에서 레이어 계산 */
    Blob<float>* out_layer = m_net->output_blobs()[0];
    int count = out_layer->count();
    const float* start_feature = out_layer->cpu_data();
    const float* end_feature = out_layer->cpu_data() + count;
    std::vector<float> softMax = std::vector<float>(start_feature, end_feature);

    /* 가장 큰 값의 색인(예측한 숫자) 가져오기 */
    float result = softMax[0], min = 1.0;
    int index = 0, minIndex = 0;
    cout << "sfds" << softMax.size() << ":" << softMax[0] << endl;
    for(int i = 1; i < softMax.size(); i++) {
        if(result < softMax[i]) {
            result = softMax[i];
            index = i;
        }
        if(min > softMax[i]) {
            min = softMax[i];
            minIndex = i;
        }
    }

    /* 예측한 값 반환 */
    return (index == 0)?minIndex:index;
}
