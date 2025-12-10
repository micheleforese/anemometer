#include "data.h"
#include "cJSON.h"
#include "esp_log.h"
#include "lvgl.h"
#include "lvgl_ui.h"
#include "lvgl_utils.h"
#include "string.h"

static const char *TAG = "DATA";

AnemometerData anemometerData;
ParticulateMatterData particulateMatterData;
ImuData imuData;

void anemometer_data_default(AnemometerData *anm_data) {
  anm_data->x_vout = 0;
  anm_data->y_vout = 0;
  anm_data->z_vout = 0;

  anm_data->autocalibrazione_asse_x = false;
  anm_data->autocalibrazione_asse_y = false;
  anm_data->autocalibrazione_asse_z = false;

  anm_data->autocalibrazione_misura_x = false;
  anm_data->autocalibrazione_misura_y = false;
  anm_data->autocalibrazione_misura_z = false;

  anm_data->temp_sonica_x = 0;
  anm_data->temp_sonica_y = 0;
  anm_data->temp_sonica_z = 0;
}

void particulate_matter_data_default(ParticulateMatterData *pm_data) {
  pm_data->timestamp = 0;

  pm_data->mass_density_pm_1_0 = 0;
  pm_data->mass_density_pm_1_0 = 0;
  pm_data->mass_density_pm_2_5 = 0;
  pm_data->mass_density_pm_10 = 0;

  pm_data->particle_count_0_5 = 0;
  pm_data->particle_count_1_0 = 0;
  pm_data->particle_count_2_5 = 0;
  pm_data->particle_count_4_0 = 0;
  pm_data->particle_count_10 = 0;

  pm_data->particle_size = 0;

  strcpy(pm_data->mass_density_unit, "");
  strcpy(pm_data->particle_size_unit, "");
  strcpy(pm_data->particle_count_unit, "");
}

void imu_data_default(ImuData *imu_data) {
  imu_data->timestamp = 0;

  imu_data->acc_top_x = 0;
  imu_data->acc_top_y = 0;
  imu_data->acc_top_z = 0;

  imu_data->acc_x = 0;
  imu_data->acc_y = 0;
  imu_data->acc_z = 0;

  imu_data->mag_x = 0;
  imu_data->mag_y = 0;
  imu_data->mag_z = 0;

  imu_data->gyr_x = 0;
  imu_data->gyr_y = 0;
  imu_data->gyr_z = 0;

  strcpy(imu_data->acc_top_unit, "");
  strcpy(imu_data->acc_unit, "");
  strcpy(imu_data->mag_unit, "");
  strcpy(imu_data->gyr_unit, "");
}

bool parse_anemometer_data(cJSON *root, AnemometerData *anm_data) {
  cJSON *cjson_timestamp = cJSON_GetObjectItem(root, "timestamp");
  cJSON *cjson_x_vout = cJSON_GetObjectItem(root, "x_vout");
  cJSON *cjson_y_vout = cJSON_GetObjectItem(root, "y_vout");
  cJSON *cjson_z_vout = cJSON_GetObjectItem(root, "z_vout");
  cJSON *cjson_autocalibrazione_asse_x =
      cJSON_GetObjectItem(root, "autocalibrazione_asse_x");
  cJSON *cjson_autocalibrazione_asse_y =
      cJSON_GetObjectItem(root, "autocalibrazione_asse_y");
  cJSON *cjson_autocalibrazione_asse_z =
      cJSON_GetObjectItem(root, "autocalibrazione_asse_z");
  cJSON *cjson_autocalibrazione_misura_x =
      cJSON_GetObjectItem(root, "autocalibrazione_misura_x");
  cJSON *cjson_autocalibrazione_misura_y =
      cJSON_GetObjectItem(root, "autocalibrazione_misura_y");
  cJSON *cjson_autocalibrazione_misura_z =
      cJSON_GetObjectItem(root, "autocalibrazione_misura_z");

  cJSON *cjson_temp_sonica_x = cJSON_GetObjectItem(root, "temp_sonica_x");
  cJSON *cjson_temp_sonica_y = cJSON_GetObjectItem(root, "temp_sonica_y");
  cJSON *cjson_temp_sonica_z = cJSON_GetObjectItem(root, "temp_sonica_z");

  if (!cJSON_IsNumber(cjson_timestamp)) {
    ESP_LOGI(TAG, "ROOT->timestamp: NOT FOUND");
    return false;
  }

  if (!cJSON_IsNumber(cjson_x_vout)) {
    ESP_LOGI(TAG, "ROOT->x_vout: NOT FOUND");
    return false;
  }
  if (!cJSON_IsNumber(cjson_y_vout)) {
    ESP_LOGI(TAG, "ROOT->y_vout: NOT FOUND");
    return false;
  }
  if (!cJSON_IsNumber(cjson_z_vout)) {
    ESP_LOGI(TAG, "ROOT->z_vout: NOT FOUND");
    return false;
  }

  if (!cJSON_IsBool(cjson_autocalibrazione_asse_x)) {
    ESP_LOGI(TAG, "ROOT->autocalibrazione_asse_x: NOT FOUND");
    return false;
  }
  if (!cJSON_IsBool(cjson_autocalibrazione_asse_y)) {
    ESP_LOGI(TAG, "ROOT->autocalibrazione_asse_y: NOT FOUND");
    return false;
  }
  if (!cJSON_IsBool(cjson_autocalibrazione_asse_z)) {
    ESP_LOGI(TAG, "ROOT->autocalibrazione_asse_z: NOT FOUND");
    return false;
  }

  if (!cJSON_IsBool(cjson_autocalibrazione_misura_x)) {
    ESP_LOGI(TAG, "ROOT->autocalibrazione_misura_x: NOT FOUND");
    return false;
  }
  if (!cJSON_IsBool(cjson_autocalibrazione_misura_y)) {
    ESP_LOGI(TAG, "ROOT->autocalibrazione_misura_y: NOT FOUND");
    return false;
  }
  if (!cJSON_IsBool(cjson_autocalibrazione_misura_z)) {
    ESP_LOGI(TAG, "ROOT->autocalibrazione_misura_z: NOT FOUND");
    return false;
  }

  if (!cJSON_IsNumber(cjson_temp_sonica_x)) {
    ESP_LOGI(TAG, "ROOT->temp_sonica_x: NOT FOUND");
    return false;
  }
  if (!cJSON_IsNumber(cjson_temp_sonica_y)) {
    ESP_LOGI(TAG, "ROOT->temp_sonica_y: NOT FOUND");
    return false;
  }
  if (!cJSON_IsNumber(cjson_temp_sonica_z)) {
    ESP_LOGI(TAG, "ROOT->temp_sonica_z: NOT FOUND");
    return false;
  }

  anm_data->timestamp = (double)cjson_timestamp->valuedouble;
  anm_data->x_vout = (double)cjson_x_vout->valuedouble;
  anm_data->y_vout = (double)cjson_y_vout->valuedouble;
  anm_data->z_vout = (double)cjson_z_vout->valuedouble;

  anm_data->autocalibrazione_asse_x =
      cJSON_IsTrue(cjson_autocalibrazione_asse_x);
  anm_data->autocalibrazione_asse_y =
      cJSON_IsTrue(cjson_autocalibrazione_asse_y);
  anm_data->autocalibrazione_asse_z =
      cJSON_IsTrue(cjson_autocalibrazione_asse_z);

  anm_data->autocalibrazione_misura_x =
      cJSON_IsTrue(cjson_autocalibrazione_misura_x);
  anm_data->autocalibrazione_misura_y =
      cJSON_IsTrue(cjson_autocalibrazione_misura_y);
  anm_data->autocalibrazione_misura_z =
      cJSON_IsTrue(cjson_autocalibrazione_misura_z);

  anm_data->temp_sonica_x = (double)cjson_temp_sonica_x->valuedouble;
  anm_data->temp_sonica_y = (double)cjson_temp_sonica_y->valuedouble;
  anm_data->temp_sonica_z = (double)cjson_temp_sonica_z->valuedouble;

  ESP_LOGI(TAG, "ANEMOMETER DATA PARSED OK.");
  return true;
}

bool parse_particulate_matter_data(cJSON *root,
                                   ParticulateMatterData *sps_data) {

  cJSON *cjson_timestamp = cJSON_GetObjectItem(root, "timestamp");

  if (!cJSON_IsNumber(cjson_timestamp)) {
    ESP_LOGI(TAG, "ROOT->timestamp: NOT FOUND");
    return false;
  }

  cJSON *cjson_sensor_data = cJSON_GetObjectItem(root, "sensor_data");

  if (!cJSON_IsObject(cjson_sensor_data)) {
    ESP_LOGI(TAG, "ROOT->timestamp: NOT FOUND");
    return false;
  }

  cJSON *cjson_mass_density =
      cJSON_GetObjectItem(cjson_sensor_data, "mass_density");

  if (!cJSON_IsObject(cjson_mass_density)) {
    ESP_LOGI(TAG, "ROOT->sensor_data->mass_density: NOT FOUND");
    return false;
  }

  cJSON *cjson_md_pm1_0 = cJSON_GetObjectItem(cjson_mass_density, "pm1.0");
  cJSON *cjson_md_pm2_5 = cJSON_GetObjectItem(cjson_mass_density, "pm2.5");
  cJSON *cjson_md_pm4_0 = cJSON_GetObjectItem(cjson_mass_density, "pm4.0");
  cJSON *cjson_md_pm10 = cJSON_GetObjectItem(cjson_mass_density, "pm10");

  if (!cJSON_IsNumber(cjson_md_pm1_0) || !cJSON_IsNumber(cjson_md_pm2_5) ||
      !cJSON_IsNumber(cjson_md_pm4_0) || !cJSON_IsNumber(cjson_md_pm10)) {
    ESP_LOGI(TAG, "ROOT->sensor_data->mass_density->pm(?): NOT FOUND");
    return false;
  }

  cJSON *cjson_particle_count =
      cJSON_GetObjectItem(cjson_sensor_data, "particle_count");

  if (!cJSON_IsObject(cjson_particle_count)) {
    ESP_LOGI(TAG, "ROOT->sensor_data->particle_count: NOT FOUND");
    return false;
  }

  cJSON *cjson_pc_pm0_5 = cJSON_GetObjectItem(cjson_particle_count, "pm0.5");
  cJSON *cjson_pc_pm1_0 = cJSON_GetObjectItem(cjson_particle_count, "pm1.0");
  cJSON *cjson_pc_pm2_5 = cJSON_GetObjectItem(cjson_particle_count, "pm2.5");
  cJSON *cjson_pc_pm4_0 = cJSON_GetObjectItem(cjson_particle_count, "pm4.0");
  cJSON *cjson_pc_pm10 = cJSON_GetObjectItem(cjson_particle_count, "pm10");

  if (!cJSON_IsNumber(cjson_pc_pm0_5) || !cJSON_IsNumber(cjson_pc_pm1_0) ||
      !cJSON_IsNumber(cjson_pc_pm2_5) || !cJSON_IsNumber(cjson_pc_pm4_0) ||
      !cJSON_IsNumber(cjson_pc_pm10)) {
    ESP_LOGI(TAG, "ROOT->sensor_data->particle_count->pm(?): NOT FOUND");
    return false;
  }

  cJSON *cjson_particle_size =
      cJSON_GetObjectItem(cjson_sensor_data, "particle_size");
  cJSON *cjson_mass_density_unit =
      cJSON_GetObjectItem(cjson_sensor_data, "mass_density_unit");
  cJSON *cjson_particle_count_unit =
      cJSON_GetObjectItem(cjson_sensor_data, "particle_count_unit");
  cJSON *cjson_particle_size_unit =
      cJSON_GetObjectItem(cjson_sensor_data, "particle_size_unit");

  if (!cJSON_IsNumber(cjson_particle_size)) {
    ESP_LOGI(TAG, "ROOT->particle_size: NOT FOUND");
    return false;
  }

  if (!cJSON_IsString(cjson_mass_density_unit)) {
    ESP_LOGI(TAG, "ROOT->mass_density_unit: NOT FOUND");
    return false;
  }
  if (!cJSON_IsString(cjson_particle_count_unit)) {
    ESP_LOGI(TAG, "ROOT->particle_count_unit: NOT FOUND");
    return false;
  }
  if (!cJSON_IsString(cjson_particle_size_unit)) {
    ESP_LOGI(TAG, "ROOT->particle_size_unit: NOT FOUND");
    return false;
  }

  particulateMatterData.timestamp = (double)cjson_timestamp->valuedouble;
  particulateMatterData.mass_density_pm_1_0 =
      (double)cjson_md_pm1_0->valuedouble;
  particulateMatterData.mass_density_pm_2_5 =
      (double)cjson_md_pm2_5->valuedouble;
  particulateMatterData.mass_density_pm_4_0 =
      (double)cjson_md_pm4_0->valuedouble;
  particulateMatterData.mass_density_pm_10 = (double)cjson_md_pm10->valuedouble;

  particulateMatterData.particle_count_0_5 =
      (double)cjson_pc_pm0_5->valuedouble;
  particulateMatterData.particle_count_1_0 =
      (double)cjson_pc_pm1_0->valuedouble;
  particulateMatterData.particle_count_2_5 =
      (double)cjson_pc_pm2_5->valuedouble;
  particulateMatterData.particle_count_4_0 =
      (double)cjson_pc_pm4_0->valuedouble;
  particulateMatterData.particle_count_10 = (double)cjson_pc_pm10->valuedouble;

  particulateMatterData.particle_size =
      (double)cjson_particle_size->valuedouble;

  strcpy(particulateMatterData.mass_density_unit,
         cjson_mass_density_unit->valuestring);

  strcpy(particulateMatterData.particle_count_unit,
         cjson_particle_count_unit->valuestring);

  strcpy(particulateMatterData.particle_size_unit,
         cjson_particle_size_unit->valuestring);

  ESP_LOGI(TAG, "SPS DATA PARSED OK.");
  return true;
}

bool parse_imu_data(cJSON *root, ImuData *imu_data) {

  cJSON *cjson_timestamp = cJSON_GetObjectItem(root, "timestamp");

  if (!cJSON_IsNumber(cjson_timestamp)) {
    ESP_LOGI(TAG, "ROOT->timestamp: NOT FOUND");
    return false;
  }

  imu_data->timestamp = (double)cjson_timestamp->valuedouble;

  cJSON *cjson_sensor_data_array = cJSON_GetObjectItem(root, "sensor_data");

  if (!cJSON_IsArray(cjson_sensor_data_array)) {
    ESP_LOGI(TAG, "ROOT->sensor_data: NOT FOUND");
    return false;
  }

  int sensor_data_array_size = cJSON_GetArraySize(cjson_sensor_data_array);

  for (size_t index = 0; index < sensor_data_array_size; index++) {
    cJSON *cjson_sensor_data =
        cJSON_GetArrayItem(cjson_sensor_data_array, index);

    if (!cJSON_IsObject(cjson_sensor_data)) {
      ESP_LOGI(TAG, "ROOT->sensor_data[%d]: NOT FOUND", index);
      return false;
    }
    cJSON *dev = cJSON_GetObjectItem(cjson_sensor_data, "dev");

    if (strcmp(dev->valuestring, "acctop") == 0) {
      cJSON *cjson_acctop_unit = cJSON_GetObjectItem(cjson_sensor_data, "unit");
      cJSON *cjson_acctop_x = cJSON_GetObjectItem(cjson_sensor_data, "x");
      cJSON *cjson_acctop_y = cJSON_GetObjectItem(cjson_sensor_data, "y");
      cJSON *cjson_acctop_z = cJSON_GetObjectItem(cjson_sensor_data, "z");

      if (!cJSON_IsString(cjson_acctop_unit)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->acctop: unit NOT FOUND", index);
        return false;
      }

      if (!cJSON_IsNumber(cjson_acctop_x)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->acctop: x NOT FOUND", index);
        return false;
      }

      if (!cJSON_IsNumber(cjson_acctop_y)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->acctop: y NOT FOUND", index);
        return false;
      }

      if (!cJSON_IsNumber(cjson_acctop_z)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->acctop: z NOT FOUND", index);
        return false;
      }

      strcpy(imu_data->acc_top_unit, cjson_acctop_unit->valuestring);
      imu_data->acc_top_x = cjson_acctop_x->valuedouble;
      imu_data->acc_top_y = cjson_acctop_y->valuedouble;
      imu_data->acc_top_z = cjson_acctop_z->valuedouble;
    } else if (strcmp(dev->valuestring, "acc") == 0) {
      cJSON *cjson_acc_unit = cJSON_GetObjectItem(cjson_sensor_data, "unit");
      cJSON *cjson_acc_x = cJSON_GetObjectItem(cjson_sensor_data, "x");
      cJSON *cjson_acc_y = cJSON_GetObjectItem(cjson_sensor_data, "y");
      cJSON *cjson_acc_z = cJSON_GetObjectItem(cjson_sensor_data, "z");

      if (!cJSON_IsString(cjson_acc_unit)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->acc: unit NOT FOUND", index);
        return false;
      }

      if (!cJSON_IsNumber(cjson_acc_x)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->acc: x NOT FOUND", index);
        return false;
      }

      if (!cJSON_IsNumber(cjson_acc_y)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->acc: y NOT FOUND", index);
        return false;
      }

      if (!cJSON_IsNumber(cjson_acc_z)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->acc: z NOT FOUND", index);
        return false;
      }

      strcpy(imu_data->acc_unit, cjson_acc_unit->valuestring);
      imu_data->acc_x = (double)cjson_acc_x->valuedouble;
      imu_data->acc_y = (double)cjson_acc_y->valuedouble;
      imu_data->acc_z = (double)cjson_acc_z->valuedouble;
    } else if (strcmp(dev->valuestring, "mag") == 0) {
      cJSON *cjson_mag_unit = cJSON_GetObjectItem(cjson_sensor_data, "unit");
      cJSON *cjson_mag_x = cJSON_GetObjectItem(cjson_sensor_data, "x");
      cJSON *cjson_mag_y = cJSON_GetObjectItem(cjson_sensor_data, "y");
      cJSON *cjson_mag_z = cJSON_GetObjectItem(cjson_sensor_data, "z");

      if (!cJSON_IsString(cjson_mag_unit)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->mag: unit NOT FOUND", index);
        return false;
      }

      if (!cJSON_IsNumber(cjson_mag_x)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->mag: x NOT FOUND", index);
        return false;
      }

      if (!cJSON_IsNumber(cjson_mag_y)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->mag: y NOT FOUND", index);
        return false;
      }

      if (!cJSON_IsNumber(cjson_mag_z)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->mag: z NOT FOUND", index);
        return false;
      }

      strcpy(imu_data->mag_unit, cjson_mag_unit->valuestring);
      imu_data->mag_x = (double)cjson_mag_x->valuedouble;
      imu_data->mag_y = (double)cjson_mag_y->valuedouble;
      imu_data->mag_z = (double)cjson_mag_z->valuedouble;
    } else if (strcmp(dev->valuestring, "gyr") == 0) {
      cJSON *cjson_gyr_unit = cJSON_GetObjectItem(cjson_sensor_data, "unit");
      cJSON *cjson_gyr_x = cJSON_GetObjectItem(cjson_sensor_data, "x");
      cJSON *cjson_gyr_y = cJSON_GetObjectItem(cjson_sensor_data, "y");
      cJSON *cjson_gyr_z = cJSON_GetObjectItem(cjson_sensor_data, "z");

      if (!cJSON_IsString(cjson_gyr_unit)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->gyr: unit NOT FOUND", index);
        strcpy(imu_data->gyr_unit, "");
      } else {
        strcpy(imu_data->gyr_unit, cjson_gyr_unit->valuestring);
      }

      if (!cJSON_IsNumber(cjson_gyr_x)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->gyr: x NOT FOUND", index);
      } else {
        imu_data->gyr_x = (double)cjson_gyr_x->valuedouble;
      }

      if (!cJSON_IsNumber(cjson_gyr_y)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->gyr: y NOT FOUND", index);
      } else {
        imu_data->gyr_y = (double)cjson_gyr_y->valuedouble;
      }

      if (!cJSON_IsNumber(cjson_gyr_z)) {
        ESP_LOGI(TAG, "ROOT->sensor_data[%d]->gyr: z NOT FOUND", index);
      } else {
        imu_data->gyr_z = (double)cjson_gyr_z->valuedouble;
      }
    }
  }

  ESP_LOGI(TAG, "IMU DATA PARSED OK.");
  return true;
}

ParseReturnCode parse_data(cJSON *json, AnemometerData *anm_data,
                           ParticulateMatterData *pm_data, ImuData *imu_data) {
  static const char *TAG = "PARSE_DATA";

  ESP_LOGI(TAG, "JSON RECEIVED.");

  cJSON *topic = cJSON_GetObjectItem(json, "topic");

  if (!cJSON_IsString(topic)) {
    ESP_LOGI(TAG, "JSON topic missing.");
    return PRC_PARSING_ERROR;
  }

  if (strcmp(topic->valuestring, "anm") == 0) {
    if (parse_anemometer_data(json, anm_data))
      return PRC_UPDATED_ANEMOMETER;
  }

  if (strcmp(topic->valuestring, "sps") == 0) {
    if (parse_particulate_matter_data(json, pm_data))
      return PRC_UPDATE_PARTICULATE_MATTER;
  }

  if (strcmp(topic->valuestring, "imu") == 0) {
    if (parse_imu_data(json, imu_data))
      return PRC_UPDATE_IMU;
  }

  if (strcmp(topic->valuestring, "type") == 0) {
    ESP_LOGI(TAG, "COMMAND");
  }

  ESP_LOGI(TAG, "Unknown topic: %s", topic->valuestring);
  return PRC_PARSING_ERROR;
}

void on_json_received(cJSON *json) {

  switch (parse_data(json, &anemometerData, &particulateMatterData, &imuData)) {
  case PRC_UPDATED_ANEMOMETER:
    lvgl_update_anemometer_data(&anemometerData);
    break;
  case PRC_UPDATE_PARTICULATE_MATTER:
    lvgl_update_particulate_matter_data(&particulateMatterData);
    break;
  case PRC_UPDATE_IMU:
    lvgl_update_imu_data(&imuData);
    break;
  case PRC_PARSING_ERROR:
    ESP_LOGW(TAG, "Failed to parse data");
    break;

  default:
    ESP_LOGE(TAG, "WTF!");
    break;
  }
}