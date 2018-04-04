/******************************************************************
 * Copyright    Copyright (c) 2015- Hisilicon Technologies CO., Ltd.
 * File name    isp_metadata_vendor_tag.h
 * Description:
 *
 * Version      1.0
 * Date         2014-05-08 09:49:15
 * Author       chenhongzhi_c00228774
 ********************************************************************/

#ifndef ISP_METADATA_VENDOR_TAG_H_INCLUDED
#define ISP_METADATA_VENDOR_TAG_H_INCLUDED

#include "isp_metadata.h"

typedef enum isp_metadata_vendor_section
{
    ANDROID_HUAWEI_DEVICE_CAPABILITIES = VENDOR_SECTION,
    ANDROID_HUAWEI_DEVICE_PARAMETERS,
    ANDROID_HUAWEI_METADATA,
    ANDROID_HUAWEI_STREAM_INFO,
    ANDROID_HUAWEI_STREAM_PARAMETERS,
    ANDROID_HUAWEI_SECTION_MAX,
} isp_metadata_vendor_section_t;

typedef enum isp_metadata_vendor_section_start
{
    ANDROID_HUAWEI_DEVICE_CAPABILITIES_START = ANDROID_HUAWEI_DEVICE_CAPABILITIES  << 16,
    ANDROID_HUAWEI_DEVICE_PARAMETERS_START   = ANDROID_HUAWEI_DEVICE_PARAMETERS    << 16,
    ANDROID_HUAWEI_METADATA_START            = ANDROID_HUAWEI_METADATA             << 16,
    ANDROID_HUAWEI_STREAM_INFO_START         = ANDROID_HUAWEI_STREAM_INFO          << 16,
    ANDROID_HUAWEI_STREAM_PARAMETERS_START   = ANDROID_HUAWEI_STREAM_PARAMETERS    << 16,
} isp_metadata_vendor_section_start_t;

typedef enum isp_metadata_vendor_tag {
    //Device capability
    ANDROID_HUAWEI_DEVICE_NO_USED_DUMMPY_TAG =
      ANDROID_HUAWEI_DEVICE_CAPABILITIES_START,
    ANDROID_HUAWEI_DEVICE_SENSOR_POSITION,
    ANDROID_HW_VIDEO_FPS_SUPPORTED,
    ANDROID_HW_FRONT_FLASH_ADJUST_SUPPORTED,
    ANDROID_HW_SUPPORTED_ISO_VALUES,
    ANDROID_HW_SUPPORTED_CONTRAST,
    ANDROID_HW_SUPPORTED_SATURATION,
    ANDROID_HW_SUPPORTED_BRIGHTNESS,
    ANDROID_HW_SUPPORTED_SHARPNESS,
    ANDROID_HW_SUPPORTED_METERING,
    ANDROID_HW_BURST_SUPPORTED,
    ANDROID_HW_CAPTURE_BEAUTY_SUPPORTED,
    ANDROID_HW_VIDEO_BEAUTY_SUPPORTED,
    ANDROID_HW_ZSL_SUPPORTED,
    ANDROID_HW_BEST_SHOT_SUPPORTED,
    ANDROID_HW_ALLFOCUS_SHOT_SUPPORTED,
    ANDROID_HW_LCD_COMPENSATE_SUPPORTED,
    ANDROID_HW_HDR_MOVIE_SUPPORTED,
    ANDROID_HW_CAPTURE_MIRROR_SUPPORTED,
    ANDROID_HW_MAGNIFIER_SUPPORTED,
    ANDROID_HW_MANUAL_FOCUS_SUPPORTED,
    ANDROID_HW_MANUAL_VCM_START_VALUE,
    ANDROID_HW_MANUAL_VCM_END_VALUE,
    ANDROID_HW_BURST_SUPPORT_COUNT,
    ANDROID_HW_BURST_JPEG_QUALITY,
    ANDROID_HW_BURST_YUV_BUFFER_COUNT,
    ANDROID_HW_BURST_RAW_BUFFER_COUNT,
    ANDROID_HW_SENSOR_CONFIGURATION_SUPPORTED,
    ANDROID_HW_DUAL_PRIMARY_SUPPORTED,
    ANDROID_HW_EQUIVALENT_FOCUS,
    ANDROID_HW_SMILE_DETECTION_SUPPORTED,
    ANDROID_HW_SCOPE_SUPPORTED,
    ANDROID_HW_CAPTURE_SM_TYPE,
    ANDROID_HW_CAPTURE_SM_THRESHOLD,
    ANDROID_HW_SEEKING_EXTCOUNT_COMBINATION,
    ANDROID_HW_QUICKTHUMBNAIL_RESOLUTION_SUPPORTED,
    ANDROID_HW_GYRO_NV_GYRO_DATA,
    ANDROID_HW_BEAUTY_LEVEL_RANGE,
    ANDROID_HW_POST_PROCESS_SUPPORTED,
    ANDROID_HW_DENOISE_SUPPORTED,
    ANDROID_HW_TARGET_TRACKING_SUPPORTED,
    ANDROID_HW_SUPER_RESOLUTION_SUPPORTED,
    ANDROID_HW_SUPER_RESOLUTION_PICTURE_SIZE,
    ANDROID_HW_BIG_APERTURE_SUPPORTED,
    ANDROID_HW_SUPPORTED_APERTURE_VALUE,
    ANDROID_HW_COLOR_EFFECT_SUPPORTED,
    ANDROID_HW_INNER_STOPSTREAMING_SUPPORTED,
    ANDROID_HW_HFPS_MODE_SUPPORTED,
    ANDROID_HW_COLOR_EFFECT_RANGE,
    ANDROID_HW_VIDEO_CALLBACK_SUPPORTED,
    ANDROID_HW_HIGH_VIDEO_CALLBACK_THRESHOLD_SUPPORTED,
    ANDROID_HW_NICE_FOOD_SUPPORTED,
    ANDROID_HW_MAKE_UP_SUPPORTED,
    ANDROID_HW_MAKE_UP_EFFECT_SUPPORTED,
    ANDROID_HW_BEAUTY_1080P_SUPPORTED,
    ANDROID_HW_RAWBUFFER_ALLOC_OPT_SUPPORTED,
    ANDROID_HW_DM_WATERMARK_SUPPORTED,
    ANDROID_HW_SENSOR_DROP_COUNT,
    ANDROID_HW_FAST_BINDER_SUPPORTED,
    ANDROID_HW_EIS_VIDHANCE_SUPPORTED,
    ANDROID_HW_GYRO_SEEK_SUPPORTED,
    ANDROID_HW_CAMERA_FORK2_SUPPORTED,
    ANDROID_HW_HFBC_SUPPORTED,
    //for mmi af test
    ANDROID_HW_MMI_7CMAF_TEST_SUPPORT,
    ANDROID_HW_MMI_7CMAF_TEST_HLIMMIT,
    ANDROID_HW_MMI_7CMAF_TEST_LLIMMIT,
    ANDROID_HW_SUPPORTED_EXPOSURE_MODES,
    ANDROID_HW_SUPPORTED_RELAYOUT_MODES,
    ANDROID_HW_EXPOSURE_LEVEL_RANGE,
    ANDROID_HW_EXPOWURE_COMP_SUPPORTED,
    ANDROID_HW_EXPOWURE_COMP_STEP,
    ANDROID_HW_AVAILABLE_VIDEO_STABILIZATION_MODES,
    ANDROID_HW_EXPOSURE_MODE_SUPPORTED_ISO,
    ANDROID_HW_EXPOSURE_MODE_SUPPORTED_EXPOSURE_TIME,
    ANDROID_HW_OIS_SUPPORTED,
    ANDROID_HW_MINIISP_STATE,
    ANDROID_HW_RDN_ALWAYS_ON,
    ANDROID_HW_DM_TYPE_SINGLE,
    ANDROID_HW_DM_TYPE_SINGLE_FLASH,
    ANDROID_HW_DM_TYPE_MULTIDNS,
    ANDROID_HW_DM_TYPE_HDR,
    ANDROID_HW_VIDEO_RECORDER_ZSL_MODE,
    ANDROID_HW_HIGH_RES_VIDEO_ZSL_MODE,
    ANDROID_HW_HIGH_RES_VIDEO_PROCESSED_SIZES_AND_DURATION,
    ANDROID_HW_MAX_BUFFER_COUNT,
        ANDROID_HW_MAX_JPEG_BUFFER_COUNT,
        ANDROID_HW_MAX_VIDEO_BUFFER_COUNT,
        ANDROID_HW_MAX_HIGH_VIDEO_BUFFER_COUNT,
    ANDROID_HW_RECORD_FIX_DDRFREQ,
    ANDROID_HW_RECORD_FIX_DDRFREQ_THRESHOLD_FPS,
    ANDROID_HW_DROP_RAW_COUNT,
    ANDROID_HW_INSERT_PREVIEW_COUNT_IN_BURST,
    ANDROID_HW_THIRDAPP_FIX_DDRFREQ,
    ANDROID_HW_ZSL_MAX_SIZE,
    ANDROID_HW_PROFESSIONAL_MODE_SUPPORTED,
    ANDROID_HW_SENSOR_EXPOSURE_RANGE,
    ANDROID_HW_SENSOR_WB_RANGE,
    ANDROID_HW_SENSOR_ISO_RANGE,
    ANDROID_HW_RAW_IMG_SAVE_SUPPORTED,
    ANDROID_HW_EXPOSURE_HINT_SUPPORTED,
    ANDROID_HW_DYNAMIC_ISO_RANGE_SUPPORTED,
        ANDROID_HW_FOCUS_ASSIST_FLASH_SUPPORTED_MODE,
        ANDROID_HW_REQUIRE_VIDEO_STATUS,
        ANDROID_HW_PRE_SHUTTER_SUPPORTED,
        ANDROID_HISTAR_AVAILABLE_VIDEO_STABILIZATION_MODES,
        ANDROID_HW_FRAMEWORK_ZSL_SUPPORTED,
        ANDROID_HW_QUICKTHUMBNAIL_SIZE,
        ANDROID_HW_SLOW_JPEG_MODE_THRESHOLD,
        ANDROID_HW_SENSOR_BINNING_SIZES_SUPPORTED,
        ANDROID_HW_SUB_SENSOR_BINNING_SIZES_SUPPORTED,
        ANDROID_HW_SENSOR_DROP_START_FRAME_COUNT,
        ANDROID_HW_DARK_RAIDER_MODE_SUPPORTED,
        ANDROID_HW_SUB_DARK_RAIDER_MODE_SUPPORTED,
        ANDROID_HW_MONO_MODE_SUPPORTED,
        ANDROID_HW_AVAILABLE_MONO_FILTER_MODES,
        ANDROID_HW_LCD_FLASH_STATUS_SUPPORTED,
        ANDROID_HW_PROFESSIONAL_RAW12_SUPPORTED,
        ANDROID_HW_FBCD_RAW_SUPPORTED,
        ANDROID_HW_FBCD_RAW_FORMAT_SUPPORTED,
        ANDROID_HW_CODEC_WIDE_2_NARROW,
        ANDROID_HW_CAMERA_CARVEOUT_RAM_SUPPORTED,
        ANDROID_HW_CAMERA_API2_SUPPORTED,
        ANDROID_HW_IVP_NOT_ALIGNED,
        ANDROID_HW_IVP_PRO_SUPPORTED,
        ANDROID_HW_NOT_USE_RAWNF_DROP,
        ANDROID_HW_FLASH_RESUME_DROP_COUNT,
        ANDROID_HW_SET_DUAL_FLASH_MMI_ISO_VALUE,
        ANDROID_HW_SET_DUAL_FLASH_MMI_EXPOTIME_VALUE,
        ANDROID_HW_OIS_DEFAULT_GRYO_GAIN,
        ANDROID_HW_ASYNC_ALLOCATION_SUPPORT,
        ANDROID_HW_MULTI_CAMERA_CAP,
        ANDROID_HW_SCALER_AVAILABLE_PREVIEW_DEPTH_SIZES,
        ANDROID_HW_SCALER_AVAILABLE_CAPTURE_DEPTH_SIZES,
        ANDROID_SCALER_SECOND_AVAILABLE_PROCESSED_SIZES,
        ANDROID_SCALER_SECOND_AVAILABLE_JPEG_SIZES,
        ANDROID_HW_SCALER_AVAILABLE_JPEG_SIZES_AND_DURATION,
        ANDROID_HW_SCALER_AVAILABLE_PROCESSED_SIZES_AND_DURATION,
        ANDROID_HW_SUB_SENSOR_AVAILABLE_JPEG_SIZES,
        ANDROID_HW_SUB_SENSOR_INFO_ACTIVE_ARRAY_SIZE,
        ANDROID_HW_SUB_SENSOR_INFO_PIXEL_ARRAY_SIZE,
        ANDROID_HW_MAX_USABLE_RAM,
        ANDROID_HW_PRE_CONNECT_PP,
        ANDROID_HW_CPU_BOOST_SUPPORTED,
        ANDROID_HW_MAX_PREVIEW_RESOLUTION,
        ANDROID_HW_MEMORY_CUT_DOWN,
        ANDROID_HW_MAP_BUFFER_SIZE,
        ANDROID_HW_MAP_BUFFER_SIZE_VENDOR_CAPBILITY_EXTEND,//add for mapbuffer allocate dynamicly according to different sensor caps
        ANDROID_HW_ISP_POOL_MAP_SIZE,
        ANDROID_HW_A7_STACK_BUFFER_SIZE,
        ANDROID_HW_MAP_BUFFER_CUT_SIZE,
        ANDROID_HW_PREPARE_CAPTURE_SUPPORTED,
        ANDROID_HW_AF_TRIGGER_LOCK_SUPPORTED,
        ANDROID_HW_SPEEDUP_CAPTURE_SUPPORTED,
        ANDROID_HW_SEEK_PRO_SUPPORTED,
        ANDROID_HW_DNS_SEEK_PRO_SUPPORTED,
        ANDROID_HW_DIGITALZOOM_SEEK_PRO_SUPPORTED,
        ANDROID_HW_SUPPORTED_COLOR_MODES,
        ANDROID_HW_MMI_TEST_FEATURE_SUPPORTED,
        ANDROID_HW_PORTRAIT_MODE_SUPPORTED,
        ANDROID_HW_FRONT_VIRTUAL_EFFECT_SUPPORTED_LEVEL,

        //iris
        ANDROID_HW_IRIS_RECOGINITION_SUPPORTED,
        ANDROID_HW_IRIS_SENSOR_AVAILABLE,
        ANDROID_HW_IRIS_MAP_BUFFER_SIZE,
        ANDROID_HW_IRIS_ALGO_MAP_BUFFER_SIZE,
        ANDROID_HW_IRIS_RECOMMAND_LEFT_EYE_RECT,
        ANDROID_HW_IRIS_RECOMMAND_RIGHT_EYE_RECT,
        ANDROID_HW_SEC_BUFFER_AVAILABLE,

        // add for capture scale
        ANDROID_HW_ISP_CAPTURE_SCALE_RATIO,
        ANDROID_HW_VIDEO_DIRECT_YUV_SUPPORT,

        ANDROID_HW_SLOWMO_NO_ZOOM,
        ANDROID_HW_THIRDCAM_NO_ZSL,
        ANDROID_HW_JPEG_FILTER_BUFFER_COUNT,
        ANDROID_HW_HISP_YUV_ALIGNMENT,
    ANDROID_HW_DUAL_PRIMARY_SINGLE_REPROCESS,
    ANDROID_HW_AF_SECOND_AVAILABLE_MODES,
    ANDROID_HW_DMAP_ACTIVE_ARRAY_SIZE,
    ANDROID_HW_REDMAP_ACTIVE_ARRAY_SIZE,
    ANDROID_HW_DMAP_DENSE_MAX_ACTIVE_SIZE,
    ANDROID_HW_DMAP_CAMERA_ALGORITHM_ALIGN_LIMITED,
    ANDROID_HW_NEED_UPDATE_FE_RAW_SIZE_SUPPORTED,
    ANDROID_HW_OPTICAL_ZOOM_THRESHOLD,
    ANDROID_HW_COLOR_STAT_DISABLE,
    ANDROID_HUAWEI_DEVICE_CAPABILITIES_END,

    //Device parameters
    ANDROID_HW_DEVICE_PARAMETERS_1 =
        ANDROID_HUAWEI_DEVICE_PARAMETERS_START,
    ANDROID_HUAWEI_DEVICE_PARAMETERS_END,

    //metadata
    ANDROID_HW_TARGET_TRACKING_COORDINATE_OFFSET =
        ANDROID_HUAWEI_METADATA_START,
    ANDROID_HW_FOCUS_VCM_VALUE,
        ANDROID_HW_DUAL_FOCUS_VCM_VALUES,
    ANDROID_HUAWEI_OUTPUT_STREAMS,
    ANDROID_HUAWEI_INPUT_STREAMS,
    ANDROID_HW_BURST_INTERVAL,
    ANDROID_HW_ISO,
    ANDROID_HW_FACE_AREA,
        ANDROID_HW_FACE_INFOS,
    ANDROID_HW_CONTRAST,
    ANDROID_HW_SATURATION,
    ANDROID_HW_BRIGHTNESS,
    ANDROID_HW_SHARPNESS,
    ANDROID_HW_METERING_MODE,
    ANDROID_HW_BURST_COUNT,
    ANDROID_HW_ROTATE_ENABLE,
    ANDROID_HW_STOP_BURST,
    ANDROID_HW_CAPTURE_EXPOSURE_BRACKETING,
    ANDROID_HW_FACE_ORIENTATION,
    ANDROID_HW_FACE_DETECTION,
    ANDROID_HW_SMILE_DETECTION,
    ANDROID_HW_BLINK_DETECTION,
    ANDROID_HW_FACE_BEAUTY,
    ANDROID_HW_FACE_MEIWO,
    ANDROID_HW_ALGO_DEBUG_INFO,
    ANDROID_HW_ALGO_SCENE_INFO,
    ANDROID_HW_ALGO_CAPTURE_RAWNF,
    ANDROID_HW_ALGO_CAPTURE_YUVNF,
    ANDROID_HW_ALGO_CAPTURE_GCD,
    ANDROID_HW_ALGO_CAPTURE_SHARPNESS,
    ANDROID_HW_IMAGE_PROCESS_MODE,
    ANDROID_HW_FACE_BEAUTY_LEVEL,
    ANDROID_HW_IMAGE_DENOISE,
    ANDROID_HW_SCOPE,
    ANDROID_HW_BEST_SHOT_MODE,
    ANDROID_HW_FACE_BEAUTY_PREVIEW_ONLY,
    ANDROID_HW_DEVICE_TARGET_TRACKING,
    ANDROID_HW_DEVICE_TARGET_TRACKING_COORDINATE,
    ANDROID_HW_HDR_MOVIE_MODE,
    ANDROID_HW_CAPTURE_MIRROR_MODE,
    ANDROID_HW_MAGNIFIER_STEP_VALUE,
    ANDROID_HW_PANORAMA,
        ANDROID_HW_PANORAMA2D,
        ANDROID_HW_PANORAMA2D_CAP,
        ANDROID_HW_PANORAMA2D_INDEX,
        ANDROID_HW_PANORAMA2D_TINY_WIDTH,
        ANDROID_HW_PANORAMA2D_TINY_HEIGHT,
    ANDROID_HW_MANUAL_FOCUS_MODE,
    ANDROID_HW_MANUAL_FOCUS_STEP_VALUE,
    ANDROID_HW_FRONT_MIRROR_MODE,
    ANDROID_HW_FRONT_MIRROR_POINT,
    ANDROID_HW_EFFECT,
    ANDROID_HW_COLOR_EFFECT_LEVEL,
    ANDROID_HW_MAKE_UP_MODE,
    ANDROID_HW_MAKE_UP_EFFECT,
    ANDROID_HW_EXPOSURE_TIME,
        ANDROID_HW_VIDEO_STATUS,
    ANDROID_HW_DUAL_PRIMARY_MODE,
    ANDROID_HW_ALTEK_METADATA,
    ANDROID_HW_FRAME_COUNT,
    ANDROID_HW_FRAME_RESOLUTION,
    ANDROID_HW_FRAME_LOSSLESS,
    ANDROID_HW_SCENE_LIGHT_LEVEL,
    ANDROID_HW_SENSOR_RESOLUTION_COMBINE,
    ANDROID_HW_SUB_CAP_RESOLUTION,
    ANDROID_HW_SHUTTER_FRAME_INDEX,
    ANDROID_HW_SAVE_FRAME_INDEX,
    ANDROID_HW_QUICK_THUMBNAIL_FRAME_INDEX,
    ANDROID_HW_NEED_HINT_USER,
    ANDROID_HW_SNAPSHOT_MODE,
    ANDROID_HW_SNAPSHOT_BRACKET_EV,
    ANDROID_HW_SNAPSHOT_EDOF_VCM_CODE,
    ANDROID_HW_SNAPSHOT_ALLFOCUS_VCM_CODE,
    ANDROID_HW_SNAPSHOT_MULTI_DENOISE_STATE,
    ANDROID_HW_SNAPSHOT_MOTION_STATE,
    ANDROID_HW_ZOOM_IN,
    ANDROID_HW_HWCAMERA_FLAG,
        ANDROID_HW_HWCAMERA2_FLAG,
    ANDROID_HW_MANUAL_FLASH,
    ANDROID_HW_EXT_SCENE_MODE,
    ANDROID_HW_DUAL_CAMERA_VERIFICATION_MODE,
    ANDROID_HW_DUAL_CAMERA_VERIFICATION_RESULT,
    ANDROID_HW_MMI_CAMERA_PDAF_VERIFICATION_MODE,
    ANDROID_HW_MMI_CAMERA_PDAF_VERIFICATION_RESULT,
    ANDROID_HW_LCD_FLASH_MODE,
        ANDROID_HW_LCD_FLASH_STATUS,
        ANDROID_HW_AFC_MODE,
        ANDROID_HW_OIS_VCM_CALIB_MODE,
        ANDROID_HW_MMI_TEST_FEATURE_RESULT,
    ANDROID_HW_NICE_FOOD_MODE,
        ANDROID_HW_API_VERSION,
        ANDROID_HW_DM_WATERMARK_MODE,
    //for dual abnormal
    ANDROID_HW_DUAL_ABNORMAL,
    //for aperture to refocus
    ANDROID_HW_APERTURE_REFOCUS,
    //for ae coff
    ANDROID_HW_ALGO_EXPO_MAX,
    ANDROID_HW_ALGO_EXPO_MIN,
    ANDROID_HW_ALGO_GAIN_MAX,
    ANDROID_HW_ALGO_GAIN_MIN,
    ANDROID_HW_ALGO_LUM_MAX,
    ANDROID_HW_ALGO_LUM_MIN,
    ANDROID_HW_ALGO_EXPO,
    ANDROID_HW_ALGO_EXPO_TIME,
    ANDROID_HW_ALGO_GAIN,
    ANDROID_HW_ALGO_ISO,
    //for extra coff
    ANDROID_HW_ALGO_MEAN_Y,
    ANDROID_HW_ALGO_AECSTABE,
    ANDROID_HW_ALGO_G_GAIN,
    ANDROID_HW_ALGO_MOTION_X,
    ANDROID_HW_ALGO_MOTION_Y,
    ANDROID_HW_ALGO_FOCUS_LENGTH,
    ANDROID_HW_ALGO_AF_WINDOW_CHANGE,
    ANDROID_HW_ISO_STATE,
    ANDROID_HW_DUAL_CAMERA,
    ANDROID_HW_IS_NEED_MERGE,
    ANDROID_HW_EIXF_HDR,
    ANDROID_HW_AE_TARGET_LOW,
    ANDROID_HW_AE_TARGET_HIGH,
    ANDROID_HW_CAPTURE_TIMESTAMP,
    ANDROID_HW_ZSL_MODE,
    ANDROID_HW_JPEG_FILENAME,
    ANDROID_HW_FAST_SHOT_MODE,
    ANDROID_HW_APERTURE_MODE,
    ANDROID_HW_APERTURE_VALUE,
    ANDROID_HW_ALLFOCUS_MODE,

    //for iris
    ANDROID_HW_LUM_LEVEL,
    ANDROID_HW_IRIS_MODE,
    ANDROID_HW_IRIS_STATE,
    ANDROID_HW_IRIS_INFO,
    ANDROID_HW_IRIS_AUTH_TOKEN,
    ANDROID_HW_IRIS_GROUP_ID,
    ANDROID_HW_IRIS_LEFT_EYE_RECT,
    ANDROID_HW_IRIS_RIGHT_EYE_RECT,
    ANDROID_HW_IRIS_ENROLL_MODE,

    // add for shake algo result
    ANDROID_HW_SHAKE_ALGO_RESULT,
    ANDROID_HW_FRAME_SHAKE_INFO,
    // add for shake algo result end

    ANDROID_HW_FOCUS_AREAS,
    ANDROID_HW_METERING_AREAS,
    ANDROID_HW_RECORDING_HINT,
    ANDROID_HW_SHARPNESS_STATE,
    ANDROID_HW_LUMA_DENOISE_STATE,
    ANDROID_HW_CHROMA_DENOISE_STATE,
    ANDROID_HW_RAW_DENOISE_STATE,
    ANDROID_HW_YUV_DENOISE_STATE,
    ANDROID_HW_PIC_TAG,
    ANDROID_HW_DROP_FLAG_FOR_JPEG,
    ANDROID_HW_SNAPSHOT_STABLE_PICTURE,
    ANDROID_HW_EXPOSURE_MODE,
    ANDROID_HW_EXPOSURE_MODE_TRY_AE,
    ANDROID_HW_EXPOSURE_MODE_SNAPSHOT,
    ANDROID_HW_EXPOSURE_MESURE_TIME,
    ANDROID_HW_EXPOSURE_MODE_PREVIEW_STATE,
    ANDROID_HW_EXPOSURE_MODE_CAPTURE_EXPO,
    ANDROID_HW_EXPOSURE_MODE_CAPTURE_ISO,
    ANDROID_HW_EXPOSURE_MODE_CAPTURE_GAIN,
    ANDROID_HW_EXPOSURE_MODE_MAX_EXPO,
    ANDROID_HW_ROTATION_VALUE,
    ANDROID_HW_JPEG_THUMBNAIL_SIZE,
    ANDROID_HW_JPEG_QUALITY,
    ANDROID_HW_JPEG_THUMBNAIL_QUALITY,
    ANDROID_HW_MMI_LOWLIGHT_PARAM,
        ANDROID_HW_QUICKTHUMBNAIL_RESOLUTION,
    //for ois module
    ANDROID_HW_OIS_ENABLE,
        ANDROID_HW_OIS_GRYO_OFFSET_CALIB,
        ANDROID_HW_OIS_GRYO_OFFSET_CALIB_RESULT,
        ANDROID_HW_OIS_HALL_CALIB,
        ANDROID_HW_OIS_HALL_CALIB_RESULT,
        ANDROID_HW_OIS_SET_GRYO_GAIN,
        ANDROID_HW_OIS_SAVE_GRYO_GAIN,
        ANDROID_HW_OIS_HALL_CHECK,
        ANDORID_HW_OIS_HALL_CHECK_RESULT,
        ANDROID_HW_OIS_HALL_DATA,
        ANDROID_HW_OIS_GYRO_INFO,
        ANDROID_HW_GYRO_INFO,
    ANDROID_HW_MMI_ENABLE,
    ANDROID_HW_AGINGVCM_ENABLE,
    ANDROID_HW_OIS_RUNNING,
    ANDROID_HW_OIS_CHECK,
    ANDROID_HW_OIS_MAGNETISM_CHECK,
    ANDROID_HW_OIS_ACTUALLY_RUNNING,
    ANDROID_HW_CAPTURE_OIS_MODE,
    //for dual flash mode
    ANDROID_HW_DUAL_FLASH_MODE,
    ANDROID_HW_DUAL_FLASH_CURRENT_INDEX,
    ANDROID_HW_DUAL_FLASH_PICTURE_INDEX,
        ANDROID_HW_DUAL_FLASH_PROCESS_STATE,
    ANDROID_HW_DUAL_FLASH_VCM_STATE,

    ANDROID_HW_SENSOR_WB_VALUE,
    ANDROID_HW_SENSOR_EXPOSURE_TIME,
    ANDROID_HW_SENSOR_ISO,
    ANDROID_HW_SENSOR_GAIN,
    ANDROID_HW_PROFESSIONAL_MODE,
        ANDROID_HW_LASER_CALIBRATE,
        ANDROID_HW_LASER_CALIBRATE_RESULT,
        ANDROID_HW_LASER_INFO,
        ANDROID_HW_LASER_STATUS,
        ANDROID_HW_LASER_CAPTURE_DISABLE,
        ANDROID_HW_PRO_FOCUS_MODE,
        ANDROID_HW_FOCUS_ASSIST_FLASH_MODE,
     //for mmi AF test
    ANDROID_HW_MMI_7CMAF_TEST_ENABLE,
    ANDROID_HW_MMI_7CMAF_TEST_CONTRAST,
        ANDROID_HW_MMI_7CMAF_TEST_RESULT,
     //for equip mmi AF test
    ANDROID_HW_MMI_EQUIP_AF_TEST_ENABLE,
    ANDROID_HW_MMI_CAPTURE_DUAL_MODE,
    ANDROID_HW_MMI_GAMMA_MODE,
    ANDROID_HW_MMI_RAW_REGION,
    ANDROID_HW_MMI_IVP_PRO,
    ANDROID_HW_MMI_SUB_RAW_REGION,

        ANDROID_HW_HDC_CALIBRATE,
    ANDROID_HW_FRAME_RESOLUTION_FIXED,
    ANDROID_HW_OTP_CHECK_RESULT,
    ANDROID_HW_GAMMA_CURVE,
    ANDROID_HW_EXPOSURE_COMP_VALUE,
    ANDROID_HW_MANUAL_ISO,
    ANDROID_HW_MANUAL_EXPOSURE,
    ANDROID_HW_PICTURE_WIDTH,
    ANDROID_HW_PICTURE_HEIGHT,
    ANDROID_HW_OVISP_METADATA,
    ANDROID_HW_ALGO_B_GAIN,
    ANDROID_HW_ALGO_R_GAIN,
    ANDROID_HW_ALGO_AWB_R_GAIN,
    ANDROID_HW_ALGO_AWB_GR_GAIN,
    ANDROID_HW_ALGO_AWB_GB_GAIN,
    ANDROID_HW_ALGO_AWB_B_GAIN,
    ANDROID_HW_ALGO_AWB_COLOR_TEMPERATURE,
    ANDROID_HW_ALGO_BANDING_TYPE,
    ANDROID_HW_ALGO_FPS,
    ANDROID_HW_ALGO_CURRENT_VTS,
    ANDROID_HW_ALGO_BASIC_FI,
    ANDROID_HW_ALGO_BASIC_VTS,
    ANDROID_HW_ALGO_RAW_HEIGHT,
    ANDROID_HW_ALGO_RAW_WIDTH,
    ANDROID_HW_ALGO_SCALE_RATIO,
    ANDROID_HW_ALGO_BV_VALUE,
    ANDROID_HW_ALGO_STABLE,

    ANDROID_HW_VIDEO_SET_ISP_SCALE_RATIO,
    ANDROID_HW_VIDEO_FPS,
    ANDROID_HW_HIGH_RES_VIDEO_MODE,
    ANDROID_HW_CAPTURE_STREAM_SIZE,
    ANDROID_HW_VIDEO_STREAM_SIZE,
    ANDROID_HW_FOCUS_METERING_SEPARATION,
    ANDROID_HW_FRONT_FLASH_LEVEL,
    ANDROID_HW_PREPARE_CAPTURE_FLAG,
    ANDROID_HW_CAPTURE_STREAM_RESOLUTION,
    ANDROID_HW_EARLY_CAPTURE_FLAG,
        ANDROID_HW_AF_TRIGGER_LOCK,
    ANDROID_HW_PP_CANCELLED,
        ANDROID_HW_ALGO_AF_FOCUS_STAGE,
        ANDROID_HW_ALGO_AF_E_REL_TYPE,
        ANDROID_HW_ALGO_AF_IS_SINGLE_FRAME_AF,
        ANDROID_HW_ALGO_AF_BEFORE_LENS_MOVE_TIME,
        ANDROID_HW_ALGO_AF_LENS_MOVING_TIME,
        ANDROID_HW_ALGO_AF_AFTER_LENS_MOVE_TIME,
        ANDROID_HW_ALGO_AF_SENSOR_EFL,
        ANDROID_HW_ALGO_AF_VCM_CODE,
        ANDROID_HW_ALGO_AF_CONTRAST_VALUE,
        ANDROID_HW_LOW_CAPTURE_STATE,
        ANDROID_HW_HINT_USER_VALUE,
        ANDROID_HW_MULTI_CAMERA_MODE,
        ANDROID_HW_LASER_DATA,
        ANDROID_HW_CAMERA_GYRO_DATA,
        ANDROID_HW_CAMERA_ACCELEROMETER_DATA,
        ANDROID_HW_CAMERA_LINEAR_ACCELERATION_DATA,
        ANDROID_HW_DEPTH_CALIB_DATA,
        ANDROID_HW_AE_DEBUG_EXIF_INFO,
        ANDROID_HW_FLASH_DEBUG_EXIF_INFO,
        ANDROID_HW_RDN_DEBUG_EXIF_INFO,
        ANDROID_HW_YUVDN_DEBUG_EXIF_INFO,
        ANDROID_HW_SHARPEN_DEBUG_EXIF_INFO,
        ANDROID_HW_CC_DEBUG_EXIF_INFO,
        ANDROID_HW_CE_DEBUG_EXIF_INFO,
        ANDROID_HW_GAMMA_DEBUG_EXIF_INFO,
        ANDROID_HW_DRC_DEBUG_EXIF_INFO,
        ANDROID_HW_AWB_DEBUG_EXIF_INFO,
        ANDROID_HW_AF_DEBUG_EXIF_INFO,
        ANDROID_HW_CAPTURE_AWB_DEBUG_EXIF_INFO,
        ANDROID_HW_FLASH_CURRENT_DEBUG_EXIF_INFO,
        ANDROID_HW_AF_DEBUG_EXIF_INFO_EX, //no use
        ANDROID_HW_ARSR_EXIF_INFO,
        ANDROID_HW_RAWNF_EXIF_INFO,
        ANDROID_HW_PDAF_EXIF_INFO,
    /****** histarISP metadata add start ******/
    ANDROID_HISTAR_COMMON_SENSOR_INFO,
    ANDROID_HISTAR_COMMON_SENSOR_RESOLUTION,
    ANDROID_HISTAR_COMMON_OTPINFO,
    ANDROID_HISTAR_COMMON_STAT3A,
    ANDROID_HISTAR_AE_STAT_INFO,
    ANDROID_HISTAR_AF_STAT_INFO,
    ANDROID_HISTAR_COMMON_SAVERAW,
    ANDROID_HISTAR_COMMON_SAVEYUV,
    ANDROID_HISTAR_COMMON_FACE_DETECTION,
    //blc
    ANDROID_HISTAR_BLC_VALUE,

    //dgamma
    ANDROID_HISTAR_DGAMMA_ENABLE,
    ANDROID_HISTAR_DGAMMA_SET,
    ANDROID_HISTAR_DGAMMA_GET,
    ANDROID_HISTAR_DGAMMA_SET_LUTSCL_FACTOR,

    //lsc
    ANDROID_HISTAR_LSC_ENABLE,
    ANDROID_HISTAR_LSC_MANUAL,
    ANDROID_HISTAR_LSC_MANUAL_GAIN,
    ANDROID_HISTAR_LSC_MANUAL_AWB,
    ANDROID_HISTAR_LSC_SET_INTENSITY,
    ANDROID_HISTAR_LSC_GET_INTENSITY,
    ANDROID_HISTAR_LSC_SET_CHROMATIX,
    ANDROID_HISTAR_LSC_GET_AUTO_GAIN,
    ANDROID_HISTAR_LSC_GET_AUTO_AWB,

    //dpcc todo
    //rawnf todo

    //awb
    ANDROID_HISTAR_AWB_ENABLE,
    ANDROID_HISTAR_AWB_GET_GAIN,
    //android.control.awbLock
    ANDROID_HISTAR_AWB_SET_LIGHT,
    ANDROID_HISTAR_AWB_SET_GAIN,
    ANDROID_HISTAR_AWB_DAMPING_ENABLE,
    ANDROID_HISTAR_AWB_SET_DAMPING_PARAM,
    ANDROID_HISTAR_AWB_GET_WHITEPOINT,
    ANDROID_HISTAR_AWB_SET_WHITEPOINT,

    //gcd todo
    //cc todo
    //drc todo

    //gamma
    ANDROID_HISTAR_GAMMA_ENABLE,
    ANDROID_HISTAR_GAMMA_MANUAL,
    ANDROID_HISTAR_GAMMA_GET_AUTO_PARAMS,
    ANDROID_HISTAR_GAMMA_SET_MANUAL_PARAMS,
    ANDROID_HISTAR_GAMMA_SET_AUTO_PARAMS,

    //ce todo
    //yuvnf todo
    //sharpness todo

    //AE
    //ISP_CONTROL_AE_ANTIBANDING_MODE, //android tag
    //ISP_CONTROL_AE_EXPOSURE_COMPENSATION, //android tag
    ANDROID_HISTAR_AE_SET_TARGET_Y,
    ANDROID_HISTAR_AE_SET_METER_MODE,
    ANDROID_HISTAR_AE_SET_METER_AREA,
    ANDROID_HISTAR_AE_SET_TOLERANCE,
    //ISP_CONTROL_AE_LOCK, //android tag
    ANDROID_HISTAR_AE_SET_GAIN,
    ANDROID_HISTAR_AE_SET_EXPO_TIME,
    ANDROID_HISTAR_AE_GET_CURRENT_Y,
    ANDROID_HISTAR_AE_GET_GAIN,
    ANDROID_HISTAR_AE_GET_EXPO_TIME,
    ANDROID_HISTAR_AE_GET_STABLE_STATUS,
    ANDROID_HISTAR_AE_GET_LOCK_STATUS,
    ANDROID_HISTAR_AE_MANUAL,
    //android.control.aeCompensationRange //static
    //android.control.aeCompensationStep //static
    ANDROID_HISTAR_AE_GAIN_RANGE, //static
    //android.sensor.info.exposureTimeRange //static
    //android.sensor.info.sensitivityRange //static
        ANDROID_HISTAR_AE_FACE_TOUCH_EVCOMP,
        ANDROID_HISTAR_AE_LOCK_STATE,

    ANDROID_HISTAR_VIDEO_STABILIZATION_MODE,

    //histarISP raw2yuv metadata add start
        // ANDROID_HISTAR_BUFFER_METADATA,
    //histarISP metadata add end

    /****** histarISP metadata add end ******/
        ANDROID_HW_IS_CAPTURE_PREVIEW,
        ANDROID_HW_PREVIEW_STATE,
        ANDROID_HW_RECORDING_ONLY_REQUEST,

        ANDROID_HW_DMAP_IMAGE_BASED_TYPE,
        ANDROID_HW_DMAP_FORMAT,
        ANDROID_HW_DMAP_STATE,
        ANDROID_HW_DMAP_SYNC_MASK,
        ANDROID_HW_DMAP_SIZE,
        ANDROID_HW_CAPTURE_REDMAP_SIZE,
        ANDROID_HW_CAPTURE_ENABLE_DMAP,
        ANDROID_HW_CAPTURE_ENABLE_REDMAP,
        ANDROID_HW_CAPTURE_DMAP_SYNC_MASK,
        ANDROID_HW_CAPTURE_DMAP_IMAGE_BASED,
        ANDROID_HW_CAPTURE_SECONDARY_NOT_NEED_DMAP,

    ANDROID_HW_DMAP_WARP_PARAMS,
    ANDROID_HW_DGEN_CFG_PARAMS,
    ANDROID_HW_DOPT_CFG_PARAMS,

    ANDROID_HW_IS_INNER_RESTART,
    ANDROID_HW_RAW_STREAM_RESOLUTION,

        ANDROID_HW_OIS_DEGREE_MODE,
    ANDROID_HW_RAW_IMG_SAVE_SWITCH,
    ANDROID_HW_RAW_IMG_FILE_PATH_NAME,
    ANDROID_HW_EXPOSURE_HINT_MODE,
    ANDROID_HW_DYNAMIC_ISO_RANGE,
    ANDROID_HW_EXPOSURE_STATE_HINT,
        ANDROID_HW_FRAME_LUMINANCE,
    ANDROID_HW_ALGO_AE_COMP,
        ANDROID_HW_SATURATION_COMP_VALID,
        ANDROID_HW_ALGO_PREFLASH_STATE,
        ANDROID_HW_AE_GAIN,
        ANDROID_HW_TARGET_LUMINANCE,
        ANDROID_HW_AWB_ILLUMINANT,
        ANDROID_HW_AWB_DAMPING_PARAM,
        ANDROID_HW_AWB_MANUAL,
        ANDROID_HW_AWB_CURRENT_WP,
        ANDROID_HW_GAMMA_LOCK,
        ANDROID_HW_GAMMA_MODE,
        ANDROID_HW_GAMMA_DAMPING_PARAM,
        ANDROID_HW_LSC_ENABLE,
        ANDROID_HW_TNR_ENABLE,
        ANDROID_HW_DPCC_ENABLE,
        ANDROID_HW_DRC_ENABLE,
        ANDROID_HW_YUVNF_ENABLE,
        ANDROID_HW_SHARPNESS_ENABLE,
        ANDROID_HW_CC_ENABLE,
        ANDROID_HW_RAWNF_ENABLE,
        ANDROID_HW_DRC_MODE,
        ANDROID_HW_DRC_DAMPING_PARAM,
        ANDROID_HW_CC_MODE,
        ANDROID_HW_CC_PARAM,
        ANDROID_HW_CC_MATRIX,
        ANDROID_HW_LPD_PARAMS,
        ANDROID_HW_DARK_RAIDER_MODE_ENABLE,
        ANDROID_HW_SNAPSHOT_BINNING_RATIO,
        ANDROID_HW_DARK_RAIDER_CAPTURE,
        ANDROID_HW_DARK_RAIDER_PREVIEW,
        ANDROID_HW_MONO_MODE,
        ANDROID_HW_BURST_SNAPSHOT_MODE,
        ANDROID_HW_CAPTURE_FLASH_EFFECT,
        ANDROID_HW_FOCUS_VCM_STABLE,
        ANDROID_HW_RAWNF_MANUAL_FLAG,
        ANDROID_HW_AUXAE_MANUAL_FLAG,
        ANDROID_HW_AE_RESUMING_FLAG,
        ANDROID_HW_AE_CURR_LUMA,
        ANDROID_HW_AE_EXPO_VALUE,
        ANDROID_HW_AWB_PARAMS,
        ANDROID_HW_AF_DISPLAY_INFO,

        // for front camera face virtual effect
        ANDROID_HW_FRONT_VIRTUAL_EFFECT_MODE,
        ANDROID_HW_FRONT_VIRTUAL_EFFECT_LEVEL,

        ANDROID_HW_JPEG_ATTACH_JPEG_NAME,
        ANDROID_HW_JPEG_ATTACH_DEPTH_BUFFER_NAME,

        ANDROID_HW_INTERNAL_RAW,
        ANDROID_HW_DATAFLOW_TYPE,
        ANDROID_HW_INTERNAL_CROP_REGION,
        ANDROID_HW_INTERNAL_AE_REGION,
        ANDROID_HW_INTERNAL_AF_REGION,
        ANDROID_HW_CAPTURE_BUILD,
        ANDROID_HW_CAPTURE_TRIGGER,
        ANDROID_HW_RECORDING_STATUS,
        ANDROID_HW_STREAM_OPT_MODE,
        ANDROID_HW_COLOR_MODE,
        ANDROID_HW_URGENT_PARAM,
        ANDROID_HW_THERMAL_DUAL2SINGLE,

        ANDROID_HW_VIDEO_BOKEH_AF_REGION,

        ANDROID_HW_PRO_EXPOSURE_TIME,
        ANDROID_HISTAR_BUFFER_METADATA,
    ANDROID_HUAWEI_METADATA_END,
    //stream info
    ANDROID_HUAWEI_CAMERA3_STREAM_T =
        ANDROID_HUAWEI_STREAM_INFO_START,
        ANDROID_HUAWEI_CAMERA3_STREAM_WIDTH,
        ANDROID_HUAWEI_CAMERA3_STREAM_HEIGHT,
        ANDROID_HUAWEI_CAMERA3_STREAM_FORMAT,
        ANDROID_HUAWEI_CAMERA3_STREAM_USAGE,
        ANDROID_HUAWEI_CAMERA3_STREAM_MAX_BUFFERS,
        ANDROID_HUAWEI_CAMERA3_STREAM_ROTATION,
    ANDROID_HUAWEI_IS_METADATA,
    ANDROID_HUAWEI_STREAM_INFO_END,

    //stream parameters
    ANDROID_HUAWEI_STREAM_PARAMETERS_1 =
        ANDROID_HUAWEI_STREAM_PARAMETERS_START,
    ANDROID_HUAWEI_STREAM_PARAMETERS_END
} isp_metadata_vendor_tag_t;

//ANDROID_HW_SHARPNESS
typedef enum camera_metadata_enum_android_hw_sharpness_mode {
    ANDROID_HW_SHARPNESS_AUTO,
    ANDROID_HW_SHARPNESS_1,
    ANDROID_HW_SHARPNESS_2,
    ANDROID_HW_SHARPNESS_3,
    ANDROID_HW_SHARPNESS_4,
    ANDROID_HW_SHARPNESS_5,
} camera_metadata_enum_android_hw_sharpness_mode_t;

//ANDROID_HW_CONTRAST
typedef enum camera_metadata_enum_android_hw_contrast_mode {
    ANDROID_HW_CONTRAST_NEGATIVE2,
    ANDROID_HW_CONTRAST_NEGATIVE1,
    ANDROID_HW_CONTRAST_0,
    ANDROID_HW_CONTRAST_POSITIVE1,
    ANDROID_HW_CONTRAST_POSITIVE2,
} camera_metadata_enum_android_hw_contrast_mode_t;

//ANDROID_HW_SATURATION
typedef enum camera_metadata_enum_android_hw_saturation_mode {
    ANDROID_HW_SATURATION_NEGATIVE2,
    ANDROID_HW_SATURATION_NEGATIVE1,
    ANDROID_HW_SATURATION_0,
    ANDROID_HW_SATURATION_POSITIVE1,
    ANDROID_HW_SATURATION_POSITIVE2,
} camera_metadata_enum_android_hw_saturation_mode_t;

// ANDROID_HW_BRIGHTNESS
typedef enum camera_metadata_enum_android_hw_brightness_mode {
    ANDROID_HW_BRIGHTNESS_NEGATIVE2,
    ANDROID_HW_BRIGHTNESS_NEGATIVE1,
    ANDROID_HW_BRIGHTNESS_0,
    ANDROID_HW_BRIGHTNESS_POSITIVE1,
    ANDROID_HW_BRIGHTNESS_POSITIVE2,
} camera_metadata_enum_android_hw_brightness_mode_t;

//ANDROID_HW_SUPPORTED_METERING
typedef enum camera_metadata_enum_android_hw_metering_mode {
    ANDROID_HW_METERING_SPOT,
    ANDROID_HW_METERING_CENTER_WEIGHTED,
    ANDROID_HW_METERING_AVERAGE,
    ANDROID_HW_METERING_MATRIX,
} camera_metadata_enum_android_hw_metering_mode_t;

//ANDROID_HW_BOOL_MODE
typedef enum camera_metadata_enum_android_hw_bool_mode {
    ANDROID_HW_FALSE,
    ANDROID_HW_TRUE,
} camera_metadata_enum_android_hw_bool_mode_t;

const tag_info_t* get_vendor_tag_info(unsigned int tag);

const char* get_vendor_section_name(unsigned int tag);

unsigned int get_vendor_tag_count(void);

void get_all_vendor_tags(unsigned int *tag_array);

int get_section_index_offset_vendor(unsigned int tag, unsigned int* offset);

#endif /*ISP_METADATA_VENDOR_TAG_H_INCLUDED*/

/********************************* END ***********************************************/
