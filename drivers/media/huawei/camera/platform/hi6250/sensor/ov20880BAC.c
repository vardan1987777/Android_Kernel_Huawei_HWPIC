


#include <linux/module.h>
#include <linux/printk.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/rpmsg.h>

#include "hwsensor.h"
#include "sensor_commom.h"
#include "hw_csi.h"

#define I2S(i) container_of(i, sensor_t, intf)
#define POWER_DELAY_0        0//delay 0 ms
#define POWER_DELAY_1        1//delay 1 ms

//lint -save -e846 -e514 -e866 -e30 -e84 -e785 -e64
//lint -save -e826 -e838 -e715 -e747 -e778 -e774 -e732
//lint -save -e650 -e31 -e731 -e528 -e753 -e737

static bool s_ov20880BAC_power_on = false;
extern struct hw_csi_pad hw_csi_pad;


char const* ov20880BAC_get_name(hwsensor_intf_t* si);
int ov20880BAC_config(hwsensor_intf_t* si, void  *argp);
int ov20880BAC_power_up(hwsensor_intf_t* si);
int ov20880BAC_power_down(hwsensor_intf_t* si);
int ov20880BAC_match_id(hwsensor_intf_t* si, void * data);
int ov20880BAC_csi_enable(hwsensor_intf_t* si);
int ov20880BAC_csi_disable(hwsensor_intf_t* si);

static hwsensor_vtbl_t
s_ov20880BAC_vtbl =
{
    .get_name = ov20880BAC_get_name,
    .config = ov20880BAC_config,
    .power_up = ov20880BAC_power_up,
    .power_down = ov20880BAC_power_down,
    .match_id = ov20880BAC_match_id,
    .csi_enable = ov20880BAC_csi_enable,
    .csi_disable = ov20880BAC_csi_disable,
};

static hwsensor_intf_t *s_intf = NULL;
static sensor_t *s_sensor = NULL;

struct sensor_power_setting hw_ov20880BAC_power_up_setting[] = {
    //disable MCAM0 reset
    {
        .seq_type = SENSOR_SUSPEND,
        .config_val = SENSOR_GPIO_LOW,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_1,
    },

    //disable MCAM1 reset
    {
        .seq_type = SENSOR_SUSPEND2,
        .config_val = SENSOR_GPIO_LOW,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_1,
    },

    //MCAM0 AVDD 2.80V
    {
        .seq_type = SENSOR_AVDD2,
        .data = (void*)"main-sensor-avdd",
        .config_val = LDO_VOLTAGE_V2P8V,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_0,
    },

    //MCAM0,MCAM1 DVDD 1.2V
    {
        .seq_type = SENSOR_DVDD2,
        .config_val = LDO_VOLTAGE_1P2V,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_0,
    },

    //SCAM,MCAM1 AVDD 2.8V
    {
        .seq_type = SENSOR_AVDD,
        .data = (void*)"slave-sensor-avdd",
        .config_val = LDO_VOLTAGE_V2P8V,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_0,
    },

    //SCAM DVDD 1.10V
    {
        .seq_type = SENSOR_DVDD,
        .config_val = LDO_VOLTAGE_1P1V, //OV use 1.1V from AFE
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_1,
    },

    //MCAM0,MCAM1,SCAM IOVDD 1.80V
    {
        .seq_type = SENSOR_IOVDD,
        .data = (void*)"main-sensor-iovdd",
        .config_val = LDO_VOLTAGE_1P8V,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_1,
    },

    //Swith MIPI to SubCamera
    {
        .seq_type = SENSOR_MIPI_SW,
        .config_val = SENSOR_GPIO_LOW,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_1,
    },

    //SCAM CLK
    {
        .seq_type = SENSOR_MCLK,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_1,
    },

    //SCAM RST
    {
        .seq_type = SENSOR_RST,
        .config_val = SENSOR_GPIO_HIGH,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_1,
    },
};

struct sensor_power_setting hw_ov20880BAC_power_down_setting[] = {
    //SCAM RST
    {
        .seq_type = SENSOR_RST,
        .config_val = SENSOR_GPIO_LOW,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_1,
    },

    //SCAM CLK
    {
        .seq_type = SENSOR_MCLK,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_1,
    },
    //Swith MIPI to MainCamera1
    {
        .seq_type = SENSOR_MIPI_SW,
        .config_val = SENSOR_GPIO_HIGH,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_1,/* meet sensor spec requirement dealy 1 ms */
    },
    //MCAM0,MCAM1,SCAM IOVDD 1.80V
    {
        .seq_type = SENSOR_IOVDD,
        .data = (void*)"main-sensor-iovdd",
        .config_val = LDO_VOLTAGE_1P8V,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_1,
    },
    //SCAM DVDD 1.1V
    {
        .seq_type = SENSOR_DVDD,
        .config_val = LDO_VOLTAGE_1P1V, //OV use 1.1V from AFE
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_1,
    },
    //SCAM,MCAM1 AVDD 2.8V
    {
        .seq_type = SENSOR_AVDD,
        .data = (void*)"slave-sensor-avdd",
        .config_val = LDO_VOLTAGE_V2P8V,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_0,
    },
    //MCAM0,MCAM1 DVDD 1.2V
    {
        .seq_type = SENSOR_DVDD2,
        .config_val = LDO_VOLTAGE_1P2V,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_0,
    },
    //MCAM0 AVDD 2.80V
    {
        .seq_type = SENSOR_AVDD2,
        .data = (void*)"main-sensor-avdd",
        .config_val = LDO_VOLTAGE_V2P8V,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_0,
    },
    //disable MCAM0 reset
    {
        .seq_type = SENSOR_SUSPEND,
        .config_val = SENSOR_GPIO_LOW,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_1,
    },

    //disable MCAM1 reset
    {
        .seq_type = SENSOR_SUSPEND2,
        .config_val = SENSOR_GPIO_LOW,
        .sensor_index = SENSOR_INDEX_INVALID,
        .delay = POWER_DELAY_1,
    },
};

atomic_t volatile ov20880BAC_powered = ATOMIC_INIT(0);
struct mutex ov20880BAC_power_lock;
sensor_t s_ov20880BAC =
{
    .intf = {
        .vtbl = &s_ov20880BAC_vtbl,
    },
    .power_setting_array = {
        .size = ARRAY_SIZE(hw_ov20880BAC_power_up_setting),
        .power_setting = hw_ov20880BAC_power_up_setting,
    },
    .power_down_setting_array = {
        .size = ARRAY_SIZE(hw_ov20880BAC_power_down_setting),
        .power_setting = hw_ov20880BAC_power_down_setting,
    },

    .p_atpowercnt = &ov20880BAC_powered,
};

const struct of_device_id
s_ov20880BAC_dt_match[] =
{
    {
        .compatible = "huawei,ov20880BAC",
        .data = &s_ov20880BAC.intf,
    },
    {
    },
};

MODULE_DEVICE_TABLE(of, s_ov20880BAC_dt_match);

struct platform_driver
s_ov20880BAC_driver =
{
    .driver =
    {
        .name = "huawei,ov20880BAC",
        .owner = THIS_MODULE,
        .of_match_table = s_ov20880BAC_dt_match,
    },
};

char const *ov20880BAC_get_name(hwsensor_intf_t *si)
{
    sensor_t* sensor = NULL;

    if (NULL == si) {
        cam_err("%s. si is NULL.", __func__);
        return NULL;
    }

    sensor = I2S(si);
    if (NULL == sensor || NULL == sensor->board_info || NULL == sensor->board_info->name) {
        cam_err("%s. sensor or board_info->name is NULL.", __func__);
        return NULL;
    }
    return sensor->board_info->name;
}

int ov20880BAC_power_up(hwsensor_intf_t *si)
{
    int ret = 0;
    sensor_t* sensor = NULL;

    if (NULL == si) {
        cam_err("%s. si is NULL.", __func__);
        return -EINVAL;
    }

    sensor = I2S(si);
    if (NULL == sensor || NULL == sensor->board_info || NULL == sensor->board_info->name) {
        cam_err("%s. sensor or board_info->name is NULL.", __func__);
        return -EINVAL;
    }
    cam_info("enter %s. index = %d name = %s", __func__, sensor->board_info->sensor_index, sensor->board_info->name);
    ret = hw_sensor_power_up_config(sensor->dev, sensor->board_info);
    if (0 == ret ){
        cam_info("%s. power up config success.", __func__);
    }else{
        cam_err("%s. power up config fail.", __func__);
        return ret;
    }
    if (hw_is_fpga_board()) {
        ret = do_sensor_power_on(sensor->board_info->sensor_index, sensor->board_info->name);
    } else {
        ret = hw_sensor_power_up(sensor);
    }
    if (0 == ret )
    {
        cam_info("%s. power up sensor success.", __func__);
    }
    else
    {
        cam_err("%s. power up sensor fail.", __func__);
    }
    return ret;
}

int ov20880BAC_power_down(hwsensor_intf_t *si)
{
    int ret = 0;
    sensor_t* sensor = NULL;

    if (NULL == si) {
        cam_err("%s. si is NULL.", __func__);
        return -EINVAL;
    }

    sensor = I2S(si);
    if (NULL == sensor || NULL == sensor->board_info || NULL == sensor->board_info->name) {
        cam_err("%s. sensor or board_info->name is NULL.", __func__);
        return -EINVAL;
    }
    cam_info("enter %s. index = %d name = %s", __func__, sensor->board_info->sensor_index, sensor->board_info->name);
    if (hw_is_fpga_board()) {
        ret = do_sensor_power_off(sensor->board_info->sensor_index, sensor->board_info->name);
    } else {
        ret = hw_sensor_power_down(sensor);
    }
    if (0 == ret )
    {
        cam_info("%s. power down sensor success.", __func__);
    }
    else
    {
        cam_err("%s. power down sensor fail.", __func__);
    }
    hw_sensor_power_down_config(sensor->board_info);
    return ret;
}

int ov20880BAC_csi_enable(hwsensor_intf_t *si)
{
    return 0;
}

int ov20880BAC_csi_disable(hwsensor_intf_t *si)
{
    return 0;
}

int ov20880BAC_match_id(hwsensor_intf_t *si, void *data)
{
    sensor_t* sensor = NULL;
    struct sensor_cfg_data *cdata = NULL;

    cam_info("%s enter.", __func__);

    if (NULL == si) {
        cam_err("%s. si is NULL.", __func__);
        return -EINVAL;
    }

    sensor = I2S(si);
    if (NULL == sensor || NULL == sensor->board_info || NULL == sensor->board_info->name) {
        cam_err("%s. sensor or board_info is NULL.", __func__);
        return -EINVAL;
    }
    cdata  = (struct sensor_cfg_data *)data;
    if (NULL == cdata) {
        cam_err("%s. cdata is NULL.", __func__);
        return -EINVAL;
    }
    cdata->data = sensor->board_info->sensor_index;
    cam_info("%s name:%s", __func__, sensor->board_info->name);

    return 0;
}

int ov20880BAC_config(hwsensor_intf_t *si, void  *argp)
{
    struct sensor_cfg_data *data = NULL;
    int ret = 0;

    if (NULL == si || NULL == argp) {
        cam_err("%s : si or argp is null", __func__);
        return -EINVAL;
    }

    data = (struct sensor_cfg_data *)argp;
    cam_debug("ov20880BAC cfgtype = %d", data->cfgtype);
    switch (data->cfgtype) {
        case SEN_CONFIG_POWER_ON:
            if (mutex_lock_interruptible(&ov20880BAC_power_lock))
                return -ERESTARTSYS;
            if (!s_ov20880BAC_power_on){
                if(NULL == si->vtbl->power_up){
                    cam_err("%s. si->vtbl->power_up is null.", __func__);
                    ret=-EINVAL;
                }else{
                    ret = si->vtbl->power_up(si);
                    if(0 == ret){
                        s_ov20880BAC_power_on = true;
                    }
                }
            }
            /*lint -e455 -esym(455,*)*/
            mutex_unlock(&ov20880BAC_power_lock);
            /*lint -e455 +esym(455,*)*/
            break;
        case SEN_CONFIG_POWER_OFF:
            if (mutex_lock_interruptible(&ov20880BAC_power_lock))
                return -ERESTARTSYS;
            if (s_ov20880BAC_power_on){
                if(NULL == si->vtbl->power_down){
                    cam_err("%s. si->vtbl->power_down is null.", __func__);
                    ret=-EINVAL;
                }else{
                    ret = si->vtbl->power_down(si);
                    if(0 == ret){
                        s_ov20880BAC_power_on = false;
                    }
                }
            }
            /*lint -e455 -esym(455,*)*/
            mutex_unlock(&ov20880BAC_power_lock);
            /*lint -e455 +esym(455,*)*/
            break;
        case SEN_CONFIG_WRITE_REG:
            break;
        case SEN_CONFIG_READ_REG:
            break;
        case SEN_CONFIG_WRITE_REG_SETTINGS:
            break;
        case SEN_CONFIG_READ_REG_SETTINGS:
            break;
        case SEN_CONFIG_ENABLE_CSI:
            break;
        case SEN_CONFIG_DISABLE_CSI:
            break;
        case SEN_CONFIG_MATCH_ID:
            if(NULL == si->vtbl->match_id){
                cam_err("%s. si->vtbl->match_id is null.", __func__);
                ret=-EINVAL;
            }else{
                ret = si->vtbl->match_id(si,argp);
            }
            break;
        default:
            cam_err("%s cfgtype(%d) is error", __func__, data->cfgtype);
            break;
    }
    return ret;
}

int32_t ov20880BAC_platform_probe(struct platform_device *pdev)
{
    int rc = 0;
    const struct of_device_id *id = NULL;
    hwsensor_intf_t *intf = NULL;
    sensor_t *sensor = NULL;
    struct device_node *np = NULL;
    cam_notice("enter %s",__func__);

    if (NULL == pdev) {
        cam_err("%s pdev is NULL", __func__);
        return -EINVAL;
    }

    np = pdev->dev.of_node;
    if (NULL == np) {
        cam_err("%s of_node is NULL", __func__);
        return -ENODEV;
    }

    id = of_match_node(s_ov20880BAC_dt_match, np);
    if (!id) {
        cam_err("%s none id matched", __func__);
        return -ENODEV;
    }

    intf = (hwsensor_intf_t*)id->data;
    if (NULL == intf) {
        cam_err("%s intf is NULL", __func__);
        return -ENODEV;
    }

    sensor = I2S(intf);
    if(NULL == sensor){
        cam_err("%s sensor is NULL rc %d", __func__, rc);
        return -ENODEV;
    }
    rc = hw_sensor_get_dt_data(pdev, sensor);
    if (rc < 0) {
        cam_err("%s no dt data rc %d", __func__, rc);
        return -ENODEV;
    }

    mutex_init(&ov20880BAC_power_lock);
    sensor->dev = &pdev->dev;
    rc = hwsensor_register(pdev, intf);
    if (rc < 0) {
        cam_err("%s hwsensor_register failed rc %d\n", __func__, rc);
        return -ENODEV;
    }
    s_intf = intf;

    rc = rpmsg_sensor_register(pdev, (void*)sensor);
    if (rc < 0) {
        hwsensor_unregister(intf);
        s_intf = NULL;
        cam_err("%s rpmsg_sensor_register failed rc %d\n", __func__, rc);
        return -ENODEV;
    }
    s_sensor = sensor;

    return rc;
}

int __init
ov20880BAC_init_module(void)
{
    cam_notice("enter %s", __func__);

    return platform_driver_probe(&s_ov20880BAC_driver,
        ov20880BAC_platform_probe);
}

void __exit
ov20880BAC_exit_module(void)
{
    if( NULL != s_sensor)
    {
        rpmsg_sensor_unregister((void*)s_sensor);
        s_sensor = NULL;
    }
    if (NULL != s_intf) {
        hwsensor_unregister(s_intf);
        s_intf = NULL;
    }
    platform_driver_unregister(&s_ov20880BAC_driver);
}
//lint -restore

/*lint -e528 -esym(528,*)*/
module_init(ov20880BAC_init_module);
module_exit(ov20880BAC_exit_module);
/*lint -e528 +esym(528,*)*/
/*lint -e753 -esym(753,*)*/
MODULE_DESCRIPTION("ov20880BAC");
MODULE_LICENSE("GPL v2");
/*lint -e753 +esym(753,*)*/
