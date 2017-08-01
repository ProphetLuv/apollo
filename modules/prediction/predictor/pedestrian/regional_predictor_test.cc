/******************************************************************************
 * Copyright 2017 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include "modules/prediction/predictor/pedestrian/regional_predictor.h"

#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "modules/perception/proto/perception_obstacle.pb.h"
#include "modules/prediction/proto/prediction_obstacle.pb.h"
#include "modules/prediction/common/prediction_gflags.h"
#include "modules/common/util/file.h"
#include "modules/prediction/container/obstacles/obstacle.h"
#include "modules/prediction/container/obstacles/obstacles_container.h"

namespace apollo {
namespace prediction {

class RegionalPredictorTest : public ::testing::Test {
 public:
  virtual void SetUp() {
    std::string file =
      "modules/prediction/testdata/multiple_perception_pedestrians.pb.txt";
    apollo::common::util::GetProtoFromFile(file, &perception_obstacles_);
    FLAGS_map_file = "modules/prediction/testdata/kml_map.bin";
  }
 protected:
  apollo::perception::PerceptionObstacles perception_obstacles_;
};

TEST_F(RegionalPredictorTest, MovingPedestrian) {
  EXPECT_DOUBLE_EQ(perception_obstacles_.header().timestamp_sec(),
                   1501183430.161906);
  apollo::perception::PerceptionObstacle perception_obstacle =
      perception_obstacles_.perception_obstacle(0);
  EXPECT_EQ(perception_obstacle.id(), 101);
  ObstaclesContainer container;
  container.Insert(perception_obstacles_);
  Obstacle* obstacle_ptr = container.GetObstacle(101);
  EXPECT_TRUE(obstacle_ptr != nullptr);
  RegionalPredictor predictor;
  // predictor.Predict(obstacle_ptr);
  // const PredictionObstacle& prediction_obstacle =
  //     predictor.prediction_obstacle();
  // EXEPECT_EQ(prediction_obstacle.trajectory().trajectory_point_size(), 2);
}

TEST_F(RegionalPredictorTest, StationaryPedestrian) {
  EXPECT_DOUBLE_EQ(perception_obstacles_.header().timestamp_sec(),
                   1501183430.161906);
  apollo::perception::PerceptionObstacle perception_obstacle =
      perception_obstacles_.perception_obstacle(1);
  EXPECT_EQ(perception_obstacle.id(), 102);
  ObstaclesContainer container;
  container.Insert(perception_obstacles_);
  Obstacle* obstacle_ptr = container.GetObstacle(102);
  EXPECT_TRUE(obstacle_ptr != nullptr);
  RegionalPredictor predictor;
  predictor.Predict(obstacle_ptr);
  const PredictionObstacle& prediction_obstacle =
      predictor.prediction_obstacle();
  EXPECT_EQ(prediction_obstacle.trajectory_size(),
             FLAGS_num_trajectory_still_pedestrian);
}

}  // namespace prediction
}  // namespace apollo
