# Q-RTSP-Viewer

A Qt-based cross-platform RTSP stream viewer.

## Screenshot

<img src="./images/screenshot.png">

## OpenCV Installation and Reference

It turns out that installaton of OpenCV is a big headache.

### References

#### Windows

The exact process is not exactly the same as any of the links...

* [Installation in Windows](https://docs.opencv.org/4.5.5/d3/d52/tutorial_windows_install.html)
* [How to setup Qt and openCV on Windows](https://wiki.qt.io/How_to_setup_Qt_and_openCV_on_Windows)
* [Windows10下QT6.0(C++)+OpenCV4环境配置及安装](https://blog.csdn.net/u011826081/article/details/113081099)


#### Linux

* [Installation in Linux](https://docs.opencv.org/3.4/d7/d9f/tutorial_linux_install.html)

Check

To check available libraries installed, run 

```
pkg-config --libs opencv4
```

Sample results

```
-lopencv_stitching -lopencv_alphamat -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dnn_objdetect -lopencv_dnn_superres -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_hdf -lopencv_hfs -lopencv_img_hash -lopencv_intensity_transform -lopencv_line_descriptor -lopencv_mcc -lopencv_quality -lopencv_rapid -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_shape -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_superres -lopencv_optflow -lopencv_surface_matching -lopencv_tracking -lopencv_highgui -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_ml -lopencv_videostab -lopencv_videoio -lopencv_viz -lopencv_ximgproc -lopencv_video -lopencv_dnn -lopencv_xobjdetect -lopencv_objdetect -lopencv_calib3d -lopencv_imgcodecs -lopencv_features2d -lopencv_flann -lopencv_xphoto -lopencv_photo -lopencv_imgproc -lopencv_core
```
