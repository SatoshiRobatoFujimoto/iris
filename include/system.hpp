#pragma once
#include "bridge.hpp"
#include "config.hpp"
#include "rejector_lpd.hpp"
#include "util.hpp"
#include <memory>
#include <pcl/registration/correspondence_rejection_distance.h>

namespace vllm
{
using pcXYZ = pcl::PointCloud<pcl::PointXYZ>;

class System
{
public:
  System(int argc, char* argv[]);

  int update();
  int optimize(int iteration);

  cv::Mat getFrame() const { return bridge.getFrame(); }

  const pcXYZ::Ptr& getAlignedCloud() const { return aligned_cloud; }
  const pcXYZ::Ptr& getTargetCloud() const { return target_cloud; }
  const std::vector<Eigen::Vector3f>& getTrajectory() const { return vllm_trajectory; }
  const Eigen::Matrix4f& getCamera() const { return vllm_camera; }
  const pcl::CorrespondencesPtr& getCorrespondences() const { return correspondences; }

private:
  int vslam_state;
  Config config;

  // setup for others
  bool first_set = true;
  Eigen::Matrix4f T_init;
  Eigen::Matrix4f T_align = Eigen::Matrix4f::Identity();
  std::vector<Eigen::Vector3f> raw_trajectory;
  std::vector<Eigen::Vector3f> vllm_trajectory;
  Eigen::Matrix4f raw_camera = Eigen::Matrix4f::Identity();
  Eigen::Matrix4f vllm_camera = Eigen::Matrix4f::Identity();

  // Rejector
  vllm::GPD gpd;
  vllm::CorrespondenceRejectorLpd lpd_rejector;
  pcl::registration::CorrespondenceRejectorDistance distance_rejector;

  BridgeOpenVSLAM bridge;

  pcl::CorrespondencesPtr correspondences;

  pcl::PointCloud<pcl::PointXYZ>::Ptr aligned_cloud;
  pcl::PointCloud<pcl::Normal>::Ptr aligned_normals;
  pcl::PointCloud<pcl::PointXYZ>::Ptr source_cloud;
  pcl::PointCloud<pcl::Normal>::Ptr source_normals;

  pcl::PointCloud<pcl::PointXYZ>::Ptr target_cloud;
  pcl::PointCloud<pcl::Normal>::Ptr target_normals;
};

}  // namespace vllm