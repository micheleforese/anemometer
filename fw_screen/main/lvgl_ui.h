#pragma once

#include "data.h"
#include "esp_log.h"
#include "lvgl.h"
#include "screen.h"

typedef struct WindLabels {
  lv_obj_t *timestamp;

  lv_obj_t *x_vout;
  lv_obj_t *autocalibrazione_asse_x;
  lv_obj_t *autocalibrazione_misura_x;
  lv_obj_t *temp_sonica_x;

  lv_obj_t *y_vout;
  lv_obj_t *autocalibrazione_asse_y;
  lv_obj_t *autocalibrazione_misura_y;
  lv_obj_t *temp_sonica_y;

  lv_obj_t *z_vout;
  lv_obj_t *autocalibrazione_asse_z;
  lv_obj_t *autocalibrazione_misura_z;
  lv_obj_t *temp_sonica_z;
} WindLabels;

extern WindLabels windLabels;

typedef struct ParticulateMatterLabels {
  lv_obj_t *timestamp;

  lv_obj_t *mass_density_pm_1_0;
  lv_obj_t *mass_density_pm_2_5;
  lv_obj_t *mass_density_pm_4_0;
  lv_obj_t *mass_density_pm_10;

  lv_obj_t *particle_count_0_5;
  lv_obj_t *particle_count_1_0;
  lv_obj_t *particle_count_2_5;
  lv_obj_t *particle_count_4_0;
  lv_obj_t *particle_count_10;

  lv_obj_t *particle_size;
} ParticulateMatterLabels;

extern ParticulateMatterLabels particulateMatterLabels;

typedef struct ImuLabels {
  lv_obj_t *timestamp;

  lv_obj_t *acc_top_x;
  lv_obj_t *acc_top_y;
  lv_obj_t *acc_top_z;

  lv_obj_t *acc_x;
  lv_obj_t *acc_y;
  lv_obj_t *acc_z;

  lv_obj_t *mag_x;
  lv_obj_t *mag_y;
  lv_obj_t *mag_z;

  lv_obj_t *gyr_x;
  lv_obj_t *gyr_y;
  lv_obj_t *gyr_z;

} ImuLabels;

extern ImuLabels imuLabels;

#define MAX_MESSAGES 4

void lvgl_update_anemometer_data(const AnemometerData *anm_data);
void lvgl_update_particulate_matter_data(const ParticulateMatterData *pm_data);
void lvgl_update_imu_data(const ImuData *imu_data);
void lvgl_anemometer_ui_init(lv_obj_t *parent);
void add_text_to_status_list(const char *text);