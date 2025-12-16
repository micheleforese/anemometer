#pragma once

#include "cJSON.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct AnemometerData {
  uint32_t timestamp;

  double x_vout;
  bool autocalibrazione_asse_x;
  bool autocalibrazione_misura_x;
  double temp_sonica_x;

  double y_vout;
  bool autocalibrazione_asse_y;
  bool autocalibrazione_misura_y;
  double temp_sonica_y;

  double z_vout;
  bool autocalibrazione_asse_z;
  bool autocalibrazione_misura_z;
  double temp_sonica_z;
} AnemometerData;

extern AnemometerData anemometerData;

typedef struct ParticulateMatterData {
  uint32_t timestamp;

  double mass_density_pm_1_0;
  double mass_density_pm_2_5;
  double mass_density_pm_4_0;
  double mass_density_pm_10;

  double particle_count_0_5;
  double particle_count_1_0;
  double particle_count_2_5;
  double particle_count_4_0;
  double particle_count_10;

  double particle_size;

  char mass_density_unit[8];
  char particle_count_unit[8];
  char particle_size_unit[8];
} ParticulateMatterData;

extern ParticulateMatterData particulateMatterData;

typedef struct ImuData {
  double timestamp;

  double acc_top_x;
  double acc_top_y;
  double acc_top_z;
  char acc_top_unit[8];

  double acc_x;
  double acc_y;
  double acc_z;
  char acc_unit[8];

  double mag_x;
  double mag_y;
  double mag_z;
  char mag_unit[8];

  double gyr_x;
  double gyr_y;
  double gyr_z;
  char gyr_unit[8];

} ImuData;

extern ImuData imuData;

typedef enum {
  PRC_PARSING_ERROR,
  PRC_UPDATED_ANEMOMETER,
  PRC_UPDATE_PARTICULATE_MATTER,
  PRC_UPDATE_IMU,
  PRC_STATUS,
} ParseReturnCode;

void anemometer_data_default(AnemometerData *anm_data);
void particulate_matter_data_default(ParticulateMatterData *pm_data);
void imu_data_default(ImuData *imu_data);
bool parse_anemometer_data(cJSON *root, AnemometerData *anm_data);
bool parse_particulate_matter_data(cJSON *root,
                                   ParticulateMatterData *sps_data);
bool parse_imu_data(cJSON *root, ImuData *imu_data);

ParseReturnCode parse_data(cJSON *json, AnemometerData *anm_data,
                           ParticulateMatterData *pm_data, ImuData *imu_data);

void on_json_received(cJSON *json);