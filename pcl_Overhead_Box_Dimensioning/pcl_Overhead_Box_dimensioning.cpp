//
//    Copyright 2023 Viraj V. Sabhaya
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "CloudVisualizer.h"

// using this for: Creating point clouds and using those PCL
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

// using this for: Reading PLY file
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>

// using this for: to measure time
#include <pcl/common/time.h>

// using this for: to find min and max points from point cloud
#include <pcl/common/common.h>

// using this for: plane segmentation
#include <pcl/sample_consensus/model_types.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/sac_model_plane.h>

// using this for: to seperate out the clusters formed.
// Clusters here are point clouds of each box respectivley.
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>

// using this for: to store the data points
#include <pcl/ModelCoefficients.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/features/normal_3d.h>

// using this for: used for tracking the points quickly
#include <pcl/kdtree/kdtree.h>

#define NUM_COMMAND_ARGS 1

using namespace std;
using namespace pcl;

/***********************************************************************************************************************
 * @brief Opens a point cloud file
 *
 * Opens a point cloud file in either PCD or PLY format
 *
 * @param[out] cloudOut pointer to opened point cloud
 * @param[in] filename path and name of input file
 * @return false if an error occurred while opening file
 * @author Christopher D. McMurrough
 **********************************************************************************************************************/
bool openCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr &cloudOut, const char *fileName)
{
    // convert the file name to string
    string fileNameStr(fileName);

    // handle various file types
    string fileExtension = fileNameStr.substr(fileNameStr.find_last_of(".") + 1);
    if (fileExtension.compare("pcd") == 0)
    {
        // attempt to open the file
        if (pcl::io::loadPCDFile<pcl::PointXYZRGBA>(fileNameStr, *cloudOut) == -1)
        {
            PCL_ERROR("error while attempting to read pcd file: %s \n", fileNameStr.c_str());
            return false;
        }
        else
        {
            return true;
        }
    }
    else if (fileExtension.compare("ply") == 0)
    {
        // attempt to open the file
        if (pcl::io::loadPLYFile<pcl::PointXYZRGBA>(fileNameStr, *cloudOut) == -1)
        {
            PCL_ERROR("error while attempting to read pcl file: %s \n", fileNameStr.c_str());
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        PCL_ERROR("error while attempting to read unsupported file: %s \n", fileNameStr.c_str());
        return false;
    }
}

/*******************************************************************************************************************/ /**
                                                                                                                       * @brief Locate a plane in the cloud
                                                                                                                       *
                                                                                                                       * Perform planar segmentation using RANSAC, returning the plane parameters and point indices
                                                                                                                       *
                                                                                                                       * @param[in] cloudIn pointer to input point cloud
                                                                                                                       * @param[out] inliers list containing the point indices of inliers
                                                                                                                       * @param[in] distanceThreshold maximum distance of a point to the planar model to be considered an inlier
                                                                                                                       * @param[in] maxIterations maximum number of iterations to attempt before returning
                                                                                                                       * @return the number of inliers
                                                                                                                       * @author Christopher D. McMurrough
                                                                                                                       **********************************************************************************************************************/
void segmentPlane(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloudIn, pcl::PointIndices::Ptr &inliers, double distanceThreshold, int maxIterations)
{
    // store the model coefficients
    pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);

    // Create the segmentation object for the planar model and set the parameters
    pcl::SACSegmentation<pcl::PointXYZRGBA> seg;
    seg.setOptimizeCoefficients(true);
    seg.setModelType(pcl::SACMODEL_PLANE);
    seg.setMethodType(pcl::SAC_RANSAC);
    seg.setMaxIterations(maxIterations);
    seg.setDistanceThreshold(distanceThreshold);

    // Segment the largest planar component from the remaining cloud
    seg.setInputCloud(cloudIn);
    seg.segment(*inliers, *coefficients);
}

/***********************************************************************************************************************
 * @brief callback function for handling a point picking event
 * @param[in] event handle generated by the visualization window
 * @param[in] cookie user data passed by the event
 * @author Christoper D. McMurrough
 **********************************************************************************************************************/
void pointPickingCallback(const pcl::visualization::PointPickingEvent &event, void *cookie)
{
    static int pickCount = 0;
    static pcl::PointXYZRGBA lastPoint;

    pcl::PointXYZRGBA p;
    event.getPoint(p.x, p.y, p.z);

    cout << "POINT CLICKED: " << p.x << " " << p.y << " " << p.z << endl;

    // if we have picked a point previously, compute the distance
    if (pickCount % 2 == 1)
    {
        double d = sqrt((p.x - lastPoint.x) * (p.x - lastPoint.x) + (p.y - lastPoint.y) * (p.y - lastPoint.y) + (p.z - lastPoint.z) * (p.z - lastPoint.z));
        cout << "DISTANCE BETWEEN THE POINTS: " << d << endl;
    }

    // update the last point and pick count
    lastPoint.x = p.x;
    lastPoint.y = p.y;
    lastPoint.z = p.z;
    pickCount++;
}

/***********************************************************************************************************************
 * @brief callback function for handling a keypress event
 * @param[in] event handle generated by the visualization window
 * @param[in] viewer_void user data passed by the event
 * @author Christoper D. McMurrough
 **********************************************************************************************************************/
void keyboardCallback(const pcl::visualization::KeyboardEvent &event, void *viewer_void)
{
    // handle key down events
    if (event.keyDown())
    {
        // handle any keys of interest
        switch (event.getKeyCode())
        {
        case 'a':
            cout << "KEYPRESS DETECTED: '" << event.getKeySym() << "'" << endl;
            break;
        default:
            break;
        }
    }
}

int main(int argc, char **argv)
{
    // validate and parse the command line arguments
    if (argc != NUM_COMMAND_ARGS + 1)
    {
        printf("USAGE: %s <file_name>\n", argv[0]);
        return 0;
    }

    // parse the command line arguments
    char *fileName = argv[1];

    // create a stop watch for measuring time
    StopWatch watch;

    // initialize the cloud viewer
    CloudVisualizer CV("Rendering Window");

    // start timing the processing step
    watch.reset();

    // open the point cloud
    PointCloud<PointXYZRGBA>::Ptr cloud(new PointCloud<PointXYZRGBA>);
    openCloud(cloud, fileName);

    // creating a copy of the cloud so that we can save the table which is later filtered out to obtain the boxes
    PointCloud<PointXYZRGBA>::Ptr copiedCloud(new PointCloud<PointXYZRGBA>(*cloud));

    // creating a point cloud to merge both views
    PointCloud<PointXYZRGBA>::Ptr mergedCloud(new PointCloud<PointXYZRGBA>); 

    // segment a plane
    const float distanceThreshold = 0.02;
    const int maxIterations = 5000;
    PointIndices::Ptr inliers(new PointIndices);
    segmentPlane(copiedCloud, inliers, distanceThreshold, maxIterations);
    cout << "Segmentation result: " << inliers->indices.size() << " points" << endl;

    // coloring the tabletop to blue color
    for (int i = 0; i < copiedCloud->points.size(); ++i)
    {
        copiedCloud->points[i].r = 0;
        copiedCloud->points[i].g = 0;
        copiedCloud->points[i].b = 255;
    }

    // merging the table to the point cloud
    *mergedCloud = *copiedCloud;

    // removing the planar inliers from input cloud
    ExtractIndices<PointXYZRGBA> extract;
    extract.setInputCloud(cloud);
    extract.setIndices(inliers);
    extract.setNegative(true);
    extract.filter(*cloud);

    // Create the KdTree object for the search method of the extraction
    search::KdTree<PointXYZRGBA>::Ptr tree(new search::KdTree<PointXYZRGBA>);
    tree->setInputCloud(cloud);

    vector<PointIndices> clusterIndices;

    // creating the euclidian cluster extraction object
    EuclideanClusterExtraction<PointXYZRGBA> ec;
    ec.setClusterTolerance(0.02); // distance between two clusters
    ec.setMinClusterSize(5000);
    ec.setMaxClusterSize(100000);
    ec.setSearchMethod(tree);
    ec.setInputCloud(cloud);

    ec.extract(clusterIndices);
    std::cout << "Clusters identified: " << clusterIndices.size() << std::endl;

    int j = 0;
    for (vector<PointIndices>::const_iterator it = clusterIndices.begin(); it != clusterIndices.end(); ++it)
    {
        PointCloud<PointXYZRGBA>::Ptr cloud_cluster(new PointCloud<PointXYZRGBA>);
        for (vector<int>::const_iterator pit = it->indices.begin(); pit != it->indices.end(); ++pit)
        {
            cloud_cluster->points.push_back(cloud->points[*pit]); 
            if (j == 0) // FIRST BOX
            {
                // Color the first box green
                cloud->points[*pit].r = 0;
                cloud->points[*pit].g = 255;
                cloud->points[*pit].b = 0;
            }
            else if (j > 0 && j <= 1) //
            {
                cloud->points[*pit].r = 255;
                cloud->points[*pit].g = 0;
                cloud->points[*pit].b = 0;
            }
        }

        // finding the minimum and max. point on each side
        PointXYZRGBA minPt, maxPt;
        getMinMax3D(*cloud_cluster, minPt, maxPt);

        float length = maxPt.x - minPt.x;
        float width = maxPt.y - minPt.y;
        float height = maxPt.z - minPt.z;

        cout << "BOX " << j + 1 << ": " << length << " " << width << " " << height << endl;

        // merging the colored boxes to the point cloud
        *mergedCloud += *cloud;

        j++;
    }

    // get the elapsed time
    double elapsedTime = watch.getTimeSeconds();
    cout << elapsedTime << " seconds passed " << endl;

    // render the scene
    // CV.addCloud(cloud);
    // CV.addCloud(copiedCloud);
    CV.addCloud(mergedCloud);
    CV.addCoordinateFrame(mergedCloud->sensor_origin_, mergedCloud->sensor_orientation_);

    // register mouse and keyboard event callbacks
    CV.registerPointPickingCallback(pointPickingCallback, cloud);
    CV.registerKeyboardCallback(keyboardCallback);

    // enter visualization loop
    while (CV.isRunning())
    {
        CV.spin(100);
    }

    // exit program
    return 0;
}