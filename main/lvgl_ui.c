#include "lvgl_ui.h"
#include "data.h"
#include <time.h>
// #include "lv_conf.h"
#include "lvgl.h"
#include "tusb_cdc.h"

WindLabels windLabels;
ParticulateMatterLabels particulateMatterLabels;
ImuLabels imuLabels;

void lvgl_update_anemometer_data(const AnemometerData *anm_data) {
  if (lvgl_lock(-1)) {
    static char buffer[64];

    time_t timestamp = anm_data->timestamp;
    struct tm timeinfo;
    localtime_r(&timestamp, &timeinfo);
    char time_buffer[64];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    snprintf(buffer, 64, "%s", time_buffer);
    lv_label_set_text(windLabels.timestamp, buffer);

    snprintf(buffer, 64, "X Vento: %.03f m/s", anm_data->x_vout);
    lv_label_set_text(windLabels.x_vout, buffer);

    snprintf(buffer, 64, "X Cal Asse: %s",
             anm_data->autocalibrazione_asse_x ? "True" : "False");
    lv_label_set_text(windLabels.autocalibrazione_asse_x, buffer);

    snprintf(buffer, 64, "X Cal Misura: %s",
             anm_data->autocalibrazione_misura_x ? "True" : "False");
    lv_label_set_text(windLabels.autocalibrazione_misura_x, buffer);

    snprintf(buffer, 64, "X Temp Sonica: %.02f C", anm_data->temp_sonica_x);
    lv_label_set_text(windLabels.temp_sonica_x, buffer);

    snprintf(buffer, 64, "Y Vento: %.03f m/s", anm_data->y_vout);
    lv_label_set_text(windLabels.y_vout, buffer);

    snprintf(buffer, 64, "Y Cal Asse: %s",
             anm_data->autocalibrazione_asse_y ? "True" : "False");
    lv_label_set_text(windLabels.autocalibrazione_asse_y, buffer);

    snprintf(buffer, 64, "Y Cal Misura: %s",
             anm_data->autocalibrazione_misura_y ? "True" : "False");
    lv_label_set_text(windLabels.autocalibrazione_misura_y, buffer);

    snprintf(buffer, 64, "Y Temp Sonica: %.02f C", anm_data->temp_sonica_y);
    lv_label_set_text(windLabels.temp_sonica_y, buffer);

    snprintf(buffer, 64, "Z Vento: %.03f m/s", anm_data->z_vout);
    lv_label_set_text(windLabels.z_vout, buffer);

    snprintf(buffer, 64, "Z Cal Asse: %s",
             anm_data->autocalibrazione_asse_z ? "True" : "False");
    lv_label_set_text(windLabels.autocalibrazione_asse_z, buffer);

    snprintf(buffer, 64, "Z Cal Misura: %s",
             anm_data->autocalibrazione_misura_z ? "True" : "False");
    lv_label_set_text(windLabels.autocalibrazione_misura_z, buffer);

    snprintf(buffer, 64, "Z Temp Sonica: %.02f C", anm_data->temp_sonica_z);
    lv_label_set_text(windLabels.temp_sonica_z, buffer);

    ESP_LOGI("UART", "WIND UPDATED");

    lvgl_unlock();
  }
}

void lvgl_update_particulate_matter_data(const ParticulateMatterData *pm_data) {

  if (lvgl_lock(-1)) {
    static char buffer[64];

    time_t timestamp = pm_data->timestamp;
    struct tm timeinfo;
    localtime_r(&timestamp, &timeinfo);
    char time_buffer[64];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    snprintf(buffer, 64, "%s", time_buffer);
    lv_label_set_text(particulateMatterLabels.timestamp, buffer);

    snprintf(buffer, 64, "Mass PM1.0 %.02f %s", pm_data->mass_density_pm_1_0,
             pm_data->mass_density_unit);
    lv_label_set_text(particulateMatterLabels.mass_density_pm_1_0, buffer);

    snprintf(buffer, 64, "Mass PM2.5 %.02f %s", pm_data->mass_density_pm_2_5,
             pm_data->mass_density_unit);
    lv_label_set_text(particulateMatterLabels.mass_density_pm_2_5, buffer);

    snprintf(buffer, 64, "Mass PM4.0 %.02f %s", pm_data->mass_density_pm_4_0,
             pm_data->mass_density_unit);
    lv_label_set_text(particulateMatterLabels.mass_density_pm_4_0, buffer);

    snprintf(buffer, 64, "Mass PM10 %.02f %s", pm_data->mass_density_pm_10,
             pm_data->mass_density_unit);
    lv_label_set_text(particulateMatterLabels.mass_density_pm_10, buffer);

    snprintf(buffer, 64, "PM0.5 %.02f %s", pm_data->particle_count_0_5,
             pm_data->particle_count_unit);
    lv_label_set_text(particulateMatterLabels.particle_count_0_5, buffer);

    snprintf(buffer, 64, "PM1.0 %.02f %s", pm_data->particle_count_1_0,
             pm_data->particle_count_unit);
    lv_label_set_text(particulateMatterLabels.particle_count_1_0, buffer);

    snprintf(buffer, 64, "PM2.5 %.02f %s", pm_data->particle_count_2_5,
             pm_data->particle_count_unit);
    lv_label_set_text(particulateMatterLabels.particle_count_2_5, buffer);

    snprintf(buffer, 64, "PM4.0 %.02f %s", pm_data->particle_count_4_0,
             pm_data->particle_count_unit);
    lv_label_set_text(particulateMatterLabels.particle_count_4_0, buffer);

    snprintf(buffer, 64, "PM10 %.02f %s", pm_data->particle_count_10,
             pm_data->particle_count_unit);
    lv_label_set_text(particulateMatterLabels.particle_count_10, buffer);

    snprintf(buffer, 64, "P. Size: %.03f %s", pm_data->particle_size,
             pm_data->particle_size_unit);
    lv_label_set_text(particulateMatterLabels.particle_size, buffer);

    lvgl_unlock();
  }

  ESP_LOGI("UART", "PARTICULATE MATTER UPDATED");
}

void lvgl_update_imu_data(const ImuData *imu_data) {

  if (lvgl_lock(-1)) {
    static char buffer[64];

    time_t timestamp = imu_data->timestamp;
    struct tm timeinfo;
    localtime_r(&timestamp, &timeinfo);
    char time_buffer[64];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    snprintf(buffer, 64, "%s", time_buffer);
    lv_label_set_text(imuLabels.timestamp, buffer);

    snprintf(buffer, 64, "Acc TOP X: %.02f %s", imu_data->acc_top_x,
             imu_data->acc_top_unit);
    lv_label_set_text(imuLabels.acc_top_x, buffer);
    snprintf(buffer, 64, "Acc TOP Y: %.02f %s", imu_data->acc_top_y,
             imu_data->acc_top_unit);
    lv_label_set_text(imuLabels.acc_top_y, buffer);
    snprintf(buffer, 64, "Acc TOP Z: %.02f %s", imu_data->acc_top_z,
             imu_data->acc_top_unit);
    lv_label_set_text(imuLabels.acc_top_z, buffer);

    snprintf(buffer, 64, "MAG X: %.02f %s", imu_data->mag_x,
             imu_data->mag_unit);
    lv_label_set_text(imuLabels.mag_x, buffer);
    snprintf(buffer, 64, "MAG Y: %.02f %s", imu_data->mag_y,
             imu_data->mag_unit);
    lv_label_set_text(imuLabels.mag_y, buffer);
    snprintf(buffer, 64, "MAG Z: %.02f %s", imu_data->mag_z,
             imu_data->mag_unit);
    lv_label_set_text(imuLabels.mag_z, buffer);

    lvgl_unlock();
  }

  ESP_LOGI("UART", "PARTICULATE MATTER UPDATED");
}

static void btn_power_off_handler(lv_event_t *e) {
  static const char *TAG = "EVENT - BTN POWER OFF";
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_CLICKED) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "type", "command");
    cJSON_AddStringToObject(json, "command", "poweroff");
    tusb_json_write(json);
    cJSON_Delete(json);
  }
}

static void btn_measure_start_handler(lv_event_t *e) {
  static const char *TAG = "EVENT - BTN MEASURE START";
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_CLICKED) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "type", "command");
    cJSON_AddStringToObject(json, "command", "start");
    tusb_json_write(json);
    cJSON_Delete(json);
  }
}

static void btn_measure_stop_handler(lv_event_t *e) {
  static const char *TAG = "EVENT - BTN MEASURE STOP";
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_CLICKED) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "type", "command");
    cJSON_AddStringToObject(json, "command", "stop");
    tusb_json_write(json);
    cJSON_Delete(json);
  }
}

static void btn_restart_handler(lv_event_t *e) {
  static const char *TAG = "EVENT - BTN RESTART";
  lv_event_code_t code = lv_event_get_code(e);

  if (code == LV_EVENT_CLICKED) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "type", "command");
    cJSON_AddStringToObject(json, "command", "restart");
    tusb_json_write(json);
    cJSON_Delete(json);
  }
}

void lvgl_anemometer_ui_init(lv_obj_t *parent) {
  static const char *TAG = "LVGL";

  // Set the default theme with dark mode
  lv_theme_t *theme = lv_theme_default_init(lv_disp_get_default(),
                                            lv_palette_main(LV_PALETTE_BLUE),
                                            lv_palette_main(LV_PALETTE_RED),
                                            false, // Light mode
                                            LV_FONT_DEFAULT);

  lv_disp_set_theme(lv_disp_get_default(), theme);

  lv_obj_t *tabview;
  tabview = lv_tabview_create(parent, LV_DIR_BOTTOM, 50);

  lv_obj_t *tab_wind = lv_tabview_add_tab(tabview, "WIND");
  lv_obj_t *tab_sps = lv_tabview_add_tab(tabview, "SPS30");
  lv_obj_t *tab_imu = lv_tabview_add_tab(tabview, "IMU");
  lv_obj_t *tab_cmd = lv_tabview_add_tab(tabview, "CMD");

  // -------------------------------
  // TAB WIND
  // -------------------------------

  lv_obj_set_flex_flow(tab_wind, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_row(tab_wind, 10, 0);

  lv_obj_t *time_container = lv_obj_create(tab_wind);
  lv_obj_set_width(time_container, lv_pct(100));      // Full width
  lv_obj_set_height(time_container, LV_SIZE_CONTENT); // Height fits content
  lv_obj_set_flex_flow(time_container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(time_container, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_all(time_container, 8, 0); // Internal padding
  lv_obj_set_style_pad_column(time_container, 10, 0);
  lv_obj_set_style_pad_row(time_container, 5,
                           0); // Space between rows if wrapped

  windLabels.timestamp = lv_label_create(time_container);

  lv_obj_t *x_container = lv_obj_create(tab_wind);
  lv_obj_set_width(x_container, lv_pct(100));      // Full width
  lv_obj_set_height(x_container, LV_SIZE_CONTENT); // Height fits content
  lv_obj_set_flex_flow(x_container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(x_container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_set_style_pad_all(x_container, 8, 0); // Internal padding
  lv_obj_set_style_pad_column(x_container, 10, 0);
  lv_obj_set_style_pad_row(x_container, 5, 0); // Space between rows if wrapped
  lv_obj_set_scroll_dir(x_container, LV_DIR_NONE);

  windLabels.x_vout = lv_label_create(x_container);
  windLabels.autocalibrazione_asse_x = lv_label_create(x_container);
  windLabels.autocalibrazione_misura_x = lv_label_create(x_container);
  windLabels.temp_sonica_x = lv_label_create(x_container);

  lv_obj_t *y_container = lv_obj_create(tab_wind);
  lv_obj_set_width(y_container, lv_pct(100));      // Full width
  lv_obj_set_height(y_container, LV_SIZE_CONTENT); // Height fits content
  lv_obj_set_flex_flow(y_container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(y_container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_set_style_pad_all(y_container, 8, 0); // Internal padding
  lv_obj_set_style_pad_column(y_container, 10, 0);
  lv_obj_set_style_pad_row(y_container, 5, 0); // Space between rows if wrapped
  lv_obj_set_scroll_dir(y_container, LV_DIR_NONE);

  windLabels.y_vout = lv_label_create(y_container);
  windLabels.autocalibrazione_asse_y = lv_label_create(y_container);
  windLabels.autocalibrazione_misura_y = lv_label_create(y_container);
  windLabels.temp_sonica_y = lv_label_create(y_container);

  lv_obj_t *z_container = lv_obj_create(tab_wind);
  lv_obj_set_width(z_container, lv_pct(100));      // Full width
  lv_obj_set_height(z_container, LV_SIZE_CONTENT); // Height fits content
  lv_obj_set_flex_flow(z_container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(z_container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_set_style_pad_all(z_container, 8, 0); // Internal padding
  lv_obj_set_style_pad_column(z_container, 10, 0);
  lv_obj_set_style_pad_row(z_container, 5, 0); // Space between rows if wrapped
  lv_obj_set_scroll_dir(z_container, LV_DIR_NONE);

  windLabels.z_vout = lv_label_create(z_container);
  windLabels.autocalibrazione_asse_z = lv_label_create(z_container);
  windLabels.autocalibrazione_misura_z = lv_label_create(z_container);
  windLabels.temp_sonica_z = lv_label_create(z_container);

  anemometer_data_default(&anemometerData);

  lvgl_update_anemometer_data(&anemometerData);

  // -------------------------------
  // TAB SPS
  // -------------------------------

  lv_obj_set_flex_flow(tab_sps, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_row(tab_sps, 10, 0);

  lv_obj_t *tstamp_container = lv_obj_create(tab_sps);
  lv_obj_set_width(tstamp_container, lv_pct(100));      // Full width
  lv_obj_set_height(tstamp_container, LV_SIZE_CONTENT); // Height fits content
  lv_obj_set_flex_flow(tstamp_container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(tstamp_container, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_all(tstamp_container, 8, 0); // Internal padding
  lv_obj_set_style_pad_column(tstamp_container, 10, 0);
  lv_obj_set_style_pad_row(tstamp_container, 5,
                           0); // Space between rows if wrapped
  lv_obj_set_scroll_dir(tstamp_container, LV_DIR_NONE);

  particulateMatterLabels.timestamp = lv_label_create(tstamp_container);

  lv_obj_t *md_container = lv_obj_create(tab_sps);
  lv_obj_set_width(md_container, lv_pct(100));      // Full width
  lv_obj_set_height(md_container, LV_SIZE_CONTENT); // Height fits content
  lv_obj_set_flex_flow(md_container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(md_container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_set_style_pad_all(md_container, 8, 0); // Internal padding
  lv_obj_set_style_pad_column(md_container, 10, 0);
  lv_obj_set_style_pad_row(md_container, 5,
                           0); // Space between rows if wrapped
  lv_obj_set_scroll_dir(md_container, LV_DIR_NONE);

  particulateMatterLabels.mass_density_pm_1_0 = lv_label_create(md_container);
  particulateMatterLabels.mass_density_pm_2_5 = lv_label_create(md_container);
  particulateMatterLabels.mass_density_pm_4_0 = lv_label_create(md_container);
  particulateMatterLabels.mass_density_pm_10 = lv_label_create(md_container);

  lv_obj_t *pc_container = lv_obj_create(tab_sps);
  lv_obj_set_width(pc_container, lv_pct(100));      // Full width
  lv_obj_set_height(pc_container, LV_SIZE_CONTENT); // Height fits content
  lv_obj_set_flex_flow(pc_container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(pc_container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_set_style_pad_all(pc_container, 8, 0); // Internal padding
  lv_obj_set_style_pad_column(pc_container, 10, 0);
  lv_obj_set_style_pad_row(pc_container, 5,
                           0); // Space between rows if wrapped
  lv_obj_set_scroll_dir(pc_container, LV_DIR_NONE);

  particulateMatterLabels.particle_count_0_5 = lv_label_create(pc_container);
  particulateMatterLabels.particle_count_1_0 = lv_label_create(pc_container);
  particulateMatterLabels.particle_count_2_5 = lv_label_create(pc_container);
  particulateMatterLabels.particle_count_4_0 = lv_label_create(pc_container);
  particulateMatterLabels.particle_count_10 = lv_label_create(pc_container);
  particulateMatterLabels.particle_size = lv_label_create(pc_container);

  particulate_matter_data_default(&particulateMatterData);
  lvgl_update_particulate_matter_data(&particulateMatterData);

  // -------------------------------
  // TAB IMU
  // -------------------------------

  lv_obj_set_flex_flow(tab_imu, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_row(tab_imu, 10, 0);

  lv_obj_t *imu_tstamp_container = lv_obj_create(tab_imu);
  lv_obj_set_width(imu_tstamp_container, lv_pct(100)); // Full width
  lv_obj_set_height(imu_tstamp_container,
                    LV_SIZE_CONTENT); // Height fits content
  lv_obj_set_flex_flow(imu_tstamp_container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(imu_tstamp_container, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_all(imu_tstamp_container, 8, 0); // Internal padding
  lv_obj_set_style_pad_column(imu_tstamp_container, 10, 0);
  lv_obj_set_style_pad_row(imu_tstamp_container, 5,
                           0); // Space between rows if wrapped

  imuLabels.timestamp = lv_label_create(imu_tstamp_container);

  lv_obj_t *acc_top_container = lv_obj_create(tab_imu);
  lv_obj_set_width(acc_top_container, lv_pct(100));      // Full width
  lv_obj_set_height(acc_top_container, LV_SIZE_CONTENT); // Height fits content
  lv_obj_set_flex_flow(acc_top_container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(acc_top_container, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_set_style_pad_all(acc_top_container, 8, 0); // Internal padding
  lv_obj_set_style_pad_column(acc_top_container, 10, 0);
  lv_obj_set_style_pad_row(acc_top_container, 5,
                           0); // Space between rows if wrapped
  lv_obj_set_scroll_dir(acc_top_container, LV_DIR_NONE);

  imuLabels.acc_top_x = lv_label_create(acc_top_container);
  imuLabels.acc_top_y = lv_label_create(acc_top_container);
  imuLabels.acc_top_z = lv_label_create(acc_top_container);

  lv_obj_t *mag_container = lv_obj_create(tab_imu);
  lv_obj_set_width(mag_container, lv_pct(100));      // Full width
  lv_obj_set_height(mag_container, LV_SIZE_CONTENT); // Height fits content
  lv_obj_set_flex_flow(mag_container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(mag_container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START,
                        LV_FLEX_ALIGN_START);
  lv_obj_set_style_pad_all(mag_container, 8, 0); // Internal padding
  lv_obj_set_style_pad_column(mag_container, 10, 0);
  lv_obj_set_style_pad_row(mag_container, 5,
                           0); // Space between rows if wrapped
  lv_obj_set_scroll_dir(mag_container, LV_DIR_NONE);

  imuLabels.mag_x = lv_label_create(mag_container);
  imuLabels.mag_y = lv_label_create(mag_container);
  imuLabels.mag_z = lv_label_create(mag_container);

  imu_data_default(&imuData);
  lvgl_update_imu_data(&imuData);

  // -------------------------------
  // TAB CMD
  // -------------------------------

  /* 2 columns, 2 rows */
  static lv_coord_t grid_cols[] = {LV_GRID_FR(1), LV_GRID_FR(1),
                                   LV_GRID_TEMPLATE_LAST};
  static lv_coord_t grid_rows[] = {LV_SIZE_CONTENT, LV_SIZE_CONTENT,
                                   LV_GRID_TEMPLATE_LAST};

  lv_obj_t *btn_container = lv_obj_create(tab_cmd);
  lv_obj_set_size(btn_container, LV_PCT(100), LV_SIZE_CONTENT);
  lv_obj_set_layout(btn_container, LV_LAYOUT_GRID);
  lv_obj_set_grid_dsc_array(btn_container, grid_cols, grid_rows);
  lv_obj_set_style_pad_row(btn_container, 20, 0);

  // lv_obj_t *btn_container = lv_obj_create(tab_cmd);
  // lv_obj_set_size(btn_container, lv_pct(100), lv_pct(100));
  // lv_obj_set_flex_flow(btn_container, LV_FLEX_FLOW_COLUMN);
  // lv_obj_set_style_align(btn_container, LV_ALIGN_CENTER, 0);
  // lv_obj_set_style_pad_top(btn_container, 10, 0);    // padding from top
  // lv_obj_set_style_pad_bottom(btn_container, 10, 0); // padding from bottom
  // lv_obj_set_flex_align(btn_container, LV_FLEX_FLOW_COLUMN,
  //                       LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  // BUTTON - START LOG
  lv_obj_t *measure_start_btn = lv_btn_create(btn_container);
  lv_obj_add_event_cb(measure_start_btn, btn_measure_start_handler,
                      LV_EVENT_ALL, NULL);
  lv_obj_set_grid_cell(measure_start_btn, LV_GRID_ALIGN_STRETCH, 0, 1,
                       LV_GRID_ALIGN_CENTER, 0, 1);

  lv_obj_t *measure_start_label;
  measure_start_label = lv_label_create(measure_start_btn);
  lv_label_set_text(measure_start_label, "START\nLOG");
  lv_obj_set_style_text_align(measure_start_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_center(measure_start_label);

  // BUTTON - STOP LOG
  lv_obj_t *measure_stop_btn = lv_btn_create(btn_container);
  lv_obj_add_event_cb(measure_stop_btn, btn_measure_stop_handler, LV_EVENT_ALL,
                      NULL);
  lv_obj_set_grid_cell(measure_stop_btn, LV_GRID_ALIGN_STRETCH, 1, 1,
                       LV_GRID_ALIGN_CENTER, 0, 1);

  lv_obj_t *measure_stop_label;
  measure_stop_label = lv_label_create(measure_stop_btn);
  lv_label_set_text(measure_stop_label, "STOP\nLOG");
  lv_obj_set_style_text_align(measure_stop_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_center(measure_stop_label);

  lv_obj_t *restart_btn = lv_btn_create(btn_container);
  lv_obj_add_event_cb(restart_btn, btn_restart_handler, LV_EVENT_ALL, NULL);
  lv_obj_set_grid_cell(restart_btn, LV_GRID_ALIGN_STRETCH, 0, 1,
                       LV_GRID_ALIGN_CENTER, 1, 1);

  lv_obj_t *reset_label;
  reset_label = lv_label_create(restart_btn);
  lv_label_set_text(reset_label, "RESET");
  lv_obj_set_style_text_align(reset_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_center(reset_label);

  lv_obj_t *power_off_btn = lv_btn_create(btn_container);
  lv_obj_add_event_cb(power_off_btn, btn_power_off_handler, LV_EVENT_ALL, NULL);
  lv_obj_set_grid_cell(power_off_btn, LV_GRID_ALIGN_STRETCH, 1, 1,
                       LV_GRID_ALIGN_CENTER, 1, 1);

  lv_obj_t *power_off_label;
  power_off_label = lv_label_create(power_off_btn);
  lv_label_set_text(power_off_label, "POWER\nOFF");
  lv_obj_set_style_text_align(power_off_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_center(power_off_label);

  // 3 = tab_cmd (0-indexed)
  lv_tabview_set_act(tabview, 3, LV_ANIM_OFF);
}
