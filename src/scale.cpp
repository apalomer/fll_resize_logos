
// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// System
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>

std::vector<std::string> getListOfImages(std::string path){
    // Create output list
    std::vector <std::string> list;

    // Open directory
    struct dirent *de=NULL;
    DIR *d=NULL;
    d=opendir(path.c_str());
    if(d == NULL){
        perror("Couldn't open directory");
        return list;
    } else{
        std::string::iterator it = path.end() - 1;
        if (*it == '/'){
             path.erase(it);
        }
    }
    // Create a list with the list of images.
    while(de = readdir(d)){
        if (de->d_type == DT_REG){		// if entry is a regular file
            std::string fname = std::string(de->d_name);	// filename
            std::string image_fn = std::string(std::string(path + "/" +fname));
            list.push_back( image_fn);
        }
    }
    closedir(d);

    // Sort the list
    sort( list.begin(), list.end() );

    // Exit
    return list;
}

int main(int argc, char** argv)
{
    // Checi inputs
    if (argc != 4)
    {
        std::cout<<"Usage: "<<argv[0]<<" final_width final_height directory"<<std::endl;
        return -1;
    }

    // Get target size
    cv::Size target_size(atoi(argv[1]),atoi(argv[2]));

    // Get images
    std::string path = argv[3];
    if (path.back() != '/')
        path += "/";
    std::vector<std::string> images_names = getListOfImages(path);

    // Set folder to save
    std::string path_to_save = path+"resized";
    mkdir(path_to_save.c_str(),0775);

    // Scale all images
    for (int i = 0;i<images_names.size();i++)
    {
        // Load image
        std::string file_name = images_names[i];
        std::cout<<"Loading "<<file_name<<std::endl;
        cv::Mat img_orig = cv::imread(file_name,CV_LOAD_IMAGE_COLOR);
        if (img_orig.empty())
        {
            std::cout<<"Error opening "<<file_name<<std::endl;
            continue;
        }

        //
        double sh = (double)target_size.height/(double)img_orig.rows;
        double sw = (double)target_size.width/(double)img_orig.cols;
        double scale;
        int scaled_direction;
        if (sh < sw)
        {
            scaled_direction = 1;
            scale = sh;
        }
        else
        {
            scaled_direction = 0;
            scale = sw;
        }

        // Read image size
        cv::Mat scaled;
        cv::resize(img_orig,scaled,cv::Size(scale*img_orig.cols,scale*img_orig.rows));

        // Assign to output
        cv::Mat output(target_size,img_orig.type());
        output = cv::Scalar(255,255,255);
        cv::Mat roi;
        if (scaled_direction == 1)
        {
            roi = cv::Mat(output, cv::Rect(output.cols/2 - scaled.cols/2, 0, scaled.cols, scaled.rows));
        }
        else
        {
            roi = cv::Mat(output, cv::Rect(0, output.rows/2 - scaled.rows/2, scaled.cols, scaled.rows));
        }
        scaled.copyTo(roi);

        // Save
        std::string save_file_name;
        size_t idx = file_name.find_last_of("/");
        save_file_name = path_to_save + file_name.substr(idx);
        std::cout<<"Saving to "<<save_file_name<< " ("<<output.cols<<"x"<<output.rows<<")"<<std::endl;
        cv::imwrite(save_file_name,output);
    }

    // Exit
    return 0;
}

