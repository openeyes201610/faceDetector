
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "face_detector.h"

int read_image_list_from_file(const char *filePath, std::vector<std::string> &imgList)
{
    char line[512];
    FILE *fin = fopen(filePath, "r");

    if(fin == NULL){
        printf("Can't open file: %s\n", filePath);
        return -1;
    }

    while(fscanf(fin, "%s\n", line) != EOF){
        imgList.push_back(line);
    }

    fclose(fin);

    return 0;
}


void split_file_path(const char* filePath, char *rootDir, char *fileName, char *ext)
{
    int len = strlen(filePath);
    int idx = len - 1, idx2 = 0;

    while(idx >= 0){
        if(filePath[idx] == '.')
            break;
        idx--;
    }

    if(idx >= 0){
        strcpy(ext, filePath + idx + 1);
        ext[len - idx] = '\0';
    }
    else {
        ext[0] = '\0';
        idx = len - 1;
    }

    idx2 = idx;
    while(idx2 >= 0){
#if defined(WIN32)
        if(filePath[idx2] == '\\')
#elif defined(linux)
        if(filePath[idx2] == '/')
#endif
            break;
        idx2 --;
    }

    if(idx2 > 0){
        strncpy(rootDir, filePath, idx2);
        rootDir[idx2] = '\0';
    }
    else{
        rootDir[0] = '.';
        rootDir[1] = '\0';
    }

    strncpy(fileName, filePath + idx2 + 1, idx - idx2 - 1);
    fileName[idx - idx2 - 1] = '\0';
}


int main_detect_video(int argc, char **argv);
int main_detect_images(int argc, char **argv);


int main(int argc, char **argv)
{

    main_detect_video(argc, argv);
    //main_detect_images(argc, argv);



    return 0;
}


int main_detect_video(int argc, char **argv)
{
    if(argc < 3){
        printf("Usage: %s [model] [video]\n", argv[0]);
        return 1;
    }

    cv::VideoCapture cap;
    cv::Mat frame, gray;
    int fps;
    int frameW;
    int frameH;
    long totalFrameNumber;
    int ret;


    char outname[128];

    ret = load_model(argv[1]);
    if(ret != 0){
        printf("Can't load model %s\n", argv[1]);
        return 1;
    }

    cap.open(argv[2]);
    if(!cap.isOpened()){
        printf("Can't open video %s\n", argv[2]);
        return 1;
    }

    fps = cap.get(CV_CAP_PROP_FPS);
    frameW = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    frameH = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);

    

    for(long frameNumber = 0; frameNumber < totalFrameNumber; frameNumber++){
        std::vector<Rect> rects;

        cap >> frame;
        if(frame.empty()) continue;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        detect_face(gray.data, gray.cols, gray.rows, gray.step, rects);
        

        int rsize = rects.size();

        for(int i = 0; i < rsize; i++){
            Rect rect = rects[i];
            cv::rectangle(frame, cv::Rect(rect.x, rect.y, rect.width, rect.height), cv::Scalar(255, 255, 0), 3);
        }

        cv::imshow("frame", frame);
        cv::waitKey(1);
    }

    cap.release();
    release_data();

    return 0;
}


int main_detect_images(int argc, char **argv)
{
    if(argc < 3){
        printf("Usage: %s [model] [image list] [out dir]\n", argv[0]);
        return 1;
    }

    int ret, size;
    std::vector<std::string> imageList;
    char rootDir[256], fileName[256], ext[20], outPath[512];

    ret = load_model(argv[1]);
    if(ret != 0){
        printf("Can't load model %s\n", argv[1]);
        return 1;
    }

    read_image_list_from_file(argv[2], imageList);

    size = imageList.size();


    for(int i = 0; i < size; i++){
        const char *imgPath = imageList[i].c_str();
        cv::Mat img = cv::imread(imgPath);
        cv::Mat gray;

        std::vector<Rect> rects;

        int rsize = 0;

        if(img.empty()){
            printf("Can't open image %s\n", imgPath);
            break;
        }

        printf("%d\r", i);
        fflush(stdout);


        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

        detect_face(gray.data, gray.cols, gray.rows, gray.step, rects);

        split_file_path(imgPath, rootDir, fileName, ext);

        rsize = rects.size();

        if(rsize == 0) {
            continue;
        }

        for(int j = 0; j < rsize; j++){
            Rect rect = rects[j];
            cv::rectangle(img, cv::Rect(rect.x, rect.y, rect.width, rect.height), cv::Scalar(255, 0, 0), 2);
        }
//            continue;

        sprintf(outPath, "%s/%s.jpg", argv[3], fileName );
        if(!cv::imwrite(outPath, img) ){
            printf("Can't write image %s\n", outPath);
            break;
        }
    }

    release_data();
    return 0;
}
