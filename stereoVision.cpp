/*****************************************************************************
** DARWIN: A FRAMEWORK FOR MACHINE LEARNING RESEARCH AND DEVELOPMENT
** Distributed under the terms of the BSD license (see the LICENSE file)
** Copyright (c) 2007-2013, Stephen Gould
** All rights reserved.
**
******************************************************************************
** FILENAME:    stereoVision.cpp
** AUTHOR(S):   Ashutosh Mishra <ashutoshdtu@gmail.com>
**              Stephen Gould <stephen.gould@anu.edu.au>
**
*****************************************************************************/

// c++ standard headers
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <iomanip>

// opencv library headers
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

// darwin library headers
#include "drwnBase.h"
#include "drwnVision.h"

using namespace std;

// usage ---------------------------------------------------------------------

void usage()
{
    cerr << DRWN_USAGE_HEADER << endl;
    cerr << "USAGE: ./stereoVision [OPTIONS] <img>\n";
    cerr << "OPTIONS:\n"
         << "  -l <filename>     :: left image filename\n"
         << "  -r <filename>     :: right image filename\n"
         << "  -g <grid>         :: set grid size (default: 10)\n"
         << "  -o <filename>     :: save segmentation to file <filename>\n"
         << "  -x                :: visualize\n"
         << DRWN_STANDARD_OPTIONS_USAGE
	 << endl;
}

// main ----------------------------------------------------------------------

int main(int argc, char *argv[])
{
    unsigned gridSize = 10;
    const char *leftFile = NULL;
    const char *rightFile = NULL;
    const char *outFile = NULL;
    bool bVisualize = false;

    // process commandline arguments

    DRWN_BEGIN_CMDLINE_PROCESSING(argc, argv)
        DRWN_CMDLINE_STR_OPTION("-l", leftFile)
        DRWN_CMDLINE_STR_OPTION("-r", rightFile)
        DRWN_CMDLINE_INT_OPTION("-g", gridSize)
        DRWN_CMDLINE_STR_OPTION("-o", outFile)
        DRWN_CMDLINE_BOOL_OPTION("-x", bVisualize)
    DRWN_END_CMDLINE_PROCESSING(usage());

    if (argc < 5) {
        usage();
        return -1;
    }

    drwnCodeProfiler::tic(drwnCodeProfiler::getHandle("main"));

    // load left and right images
    cv::Mat leftImg = cv::imread(leftFile, CV_LOAD_IMAGE_COLOR);
    cv::Mat rightImg = cv::imread(rightFile, CV_LOAD_IMAGE_COLOR);
    DRWN_ASSERT_MSG(leftImg.data != NULL, leftFile);
    DRWN_ASSERT_MSG(rightImg.data != NULL, rightFile);

    // run superpixel algorithm
    cv::Mat seg = drwnFastSuperpixels(leftImg, gridSize);

    // visualize
    if (bVisualize) {
        cv::Mat canvas = leftImg.clone();
        drwnAverageRegions(canvas, seg);
        drwnDrawRegionBoundaries(canvas, seg, CV_RGB(0, 0, 0));
        drwnShowDebuggingImage(canvas, string("superpixels"), true);
    }

    // save segmentation
    if (outFile != NULL) {
        ofstream ofs(outFile);
        for (int y = 0; y < seg.rows; y++) {
            for (int x = 0; x < seg.cols; x++) {
                if (x != 0) ofs << " ";
                ofs << seg.at<int>(y, x);
            }
            ofs << "\n";
        }
        ofs.close();
    }

    // clean up
    cv::destroyAllWindows();
    drwnCodeProfiler::toc(drwnCodeProfiler::getHandle("main"));
    drwnCodeProfiler::print();
    return 0;
}
